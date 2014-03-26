// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swapi

import (
	"errors"
	"io"
	"net"
	"sync"
	"sync/atomic"
	"time"
)

var (
	MaxPostMessages = 32
	// By default, wait indefinitely for handlers to terminate
	ClientTimeout       = 0 * time.Second
	TickPeriod          = 1 * time.Second
	ErrTimeout          = errors.New("handler timeout")
	ErrConnectionClosed = ConnectionError{errors.New("connection closed")}
	InvalidContext      = int32(0)
)

type ConnectionError struct {
	error
}

func IsConnectionError(err error) bool {
	_, ok := err.(ConnectionError)
	return ok
}

type MessageHandler func(msg *SwordMessage, client, context int32, err error) bool
type MessageLogger func(in bool, size int, msg *SwordMessage)

type MessagePost struct {
	message SwordMessage
	handler MessageHandler
	Context int32
}

type HandlerRegister struct {
	handler MessageHandler
	context chan int32
	timeout time.Duration
}

func MakeHandlerRegister(handler MessageHandler, timeout time.Duration) HandlerRegister {
	return HandlerRegister{
		handler: handler,
		// Set capacity to 1 so serve() goroutine does not have to wait for
		// Register() / PostWithTimeout() to be ready before processing more
		// events.
		context: make(chan int32, 1),
		timeout: timeout,
	}
}

type HandlerUnregister struct {
	context int32
	done    chan struct{}
}

type HandlerError struct {
	context int32
	err     error
}

// A command to execute in the serving goroutine
type commandRequest struct {
	cmd  func(c *Client)
	done chan int
}

type Client struct {
	Address string
	Model   *Model
	// Control whether the model should be activated and updated on incoming
	// events. Set if before running the client to have any effect.
	EnableModel bool
	PostTimeout time.Duration
	// Receive input binary payloads, must be registered before calling Run()
	RawMessageHandler RawMessageHandler
	// Called on every inbound or outbound message. The handler will be
	// from different goroutines, and must be set before calling Run().
	// size argument includes the message header.
	Logger MessageLogger

	// context and clientId are only read and set by the serve goroutine
	context     int32
	clientId    int32
	link        net.Conn
	handlers    map[int32]MessageHandler
	timeouts    map[int32]time.Time
	ticker      *time.Ticker
	posts       chan MessagePost
	registers   chan HandlerRegister
	unregisters chan HandlerUnregister
	events      chan SwordMessage
	commands    chan commandRequest
	errors      chan HandlerError
	quit        chan bool
	eof         int32
	listening   sync.WaitGroup
	running     sync.WaitGroup
	serving     sync.WaitGroup
	writing     sync.WaitGroup
	closed      bool
}

func NewClient(address string) (*Client, error) {
	link, err := net.Dial("tcp", address)
	if err != nil {
		return nil, err
	}
	client := &Client{
		Address:     address,
		Model:       NewModel(),
		EnableModel: true,
		PostTimeout: ClientTimeout,
		link:        link,
		handlers:    make(map[int32]MessageHandler),
		timeouts:    make(map[int32]time.Time),
		ticker:      time.NewTicker(TickPeriod),
		posts:       make(chan MessagePost, MaxPostMessages),
		registers:   make(chan HandlerRegister, MaxPostMessages),
		unregisters: make(chan HandlerUnregister, MaxPostMessages),
		events:      make(chan SwordMessage, MaxPostMessages),
		commands:    make(chan commandRequest, MaxPostMessages),
		errors:      make(chan HandlerError, MaxPostMessages),
		quit:        make(chan bool),
	}
	return client, nil
}

func (c *Client) Run() error {
	errors := make(chan error)
	c.serving.Add(1)
	go c.serve()
	c.writing.Add(1)
	go c.write()
	c.listening.Add(1)
	go c.listen(errors)
	if c.EnableModel {
		c.Model.Listen(c)
	}
	return <-errors
}

func ConnectClient(client *Client) {
	client.running.Add(1)
	go func() {
		defer client.running.Done()
		client.Run()
	}()
}

func Connect(host string) (*Client, error) {
	client, err := NewClient(host)
	if err != nil {
		return nil, err
	}
	ConnectClient(client)
	return client, nil
}

func (c *Client) Close() {
	if c.closed {
		return
	}
	c.closed = true
	// Connection might closed remotely while we set this flag.
	atomic.StoreInt32(&c.eof, 1)
	// Terminate the input connection
	c.link.Close()
	c.listening.Wait()
	// Close all serve() inputs then ask it to terminate
	c.ticker.Stop()
	close(c.registers)
	close(c.unregisters)
	close(c.commands)
	c.quit <- true
	c.serving.Wait()
	// Close write() inputs and close the connection
	close(c.posts)
	c.writing.Wait()
	c.Model.Close()
	c.running.Wait()
}

func (c *Client) getContext() int32 {
	for {
		context := c.context
		c.context++
		if context == InvalidContext {
			continue
		}
		// better be safe than sorry
		_, ok := c.handlers[context]
		if !ok {
			return context
		}
	}
}

func (c *Client) register(data HandlerRegister) {
	context := c.getContext()
	data.context <- context
	c.handlers[context] = data.handler
	if data.timeout == 0 {
		return
	}
	c.timeouts[context] = time.Now().Add(data.timeout)
}

func (c *Client) remove(context int32) {
	delete(c.handlers, context)
	delete(c.timeouts, context)
}

func (c *Client) apply(msg *SwordMessage) {
	for context, handler := range c.handlers {
		if handler(msg, c.clientId, context, nil) {
			c.remove(context)
		}
	}
}

func (c *Client) abort(context int32, err error) {
	handler, ok := c.handlers[context]
	if !ok {
		return
	}
	handler(nil, c.clientId, context, err)
	c.remove(context)
}

func (c *Client) timeout(now time.Time) {
	removals := []int32{}
	for context, timeout := range c.timeouts {
		if now.After(timeout) {
			removals = append(removals, context)
		}
	}
	for _, context := range removals {
		c.abort(context, ErrTimeout)
	}
}

func (c *Client) serve() {
	defer c.serving.Done()

	closed := false
	for {
		select {
		case quit := <-c.quit:
			closed = true
			if quit {
				// We can only flush the queue of pending handlers once the
				// input channel is closed, which can only be done by the
				// caller (and not by liste()).
				for cmd := range c.commands {
					cmd.cmd(c)
					cmd.done <- 1
				}
				for data := range c.registers {
					data.handler(nil, 0, 0, ErrConnectionClosed)
				}
				for data := range c.unregisters {
					close(data.done)
				}
			}
			for context, handler := range c.handlers {
				handler(nil, c.clientId, context, ErrConnectionClosed)
			}
			c.handlers = nil
			if quit {
				return
			}
		case data, ok := <-c.registers:
			if ok {
				if closed {
					data.handler(nil, 0, 0, ErrConnectionClosed)
				} else {
					c.register(data)
				}
			}
		case data, ok := <-c.unregisters:
			if ok {
				c.remove(data.context)
				close(data.done)
			}
		case event := <-c.events:
			c.apply(&event)
		case data := <-c.errors:
			c.abort(data.context, data.err)
		case cmd, ok := <-c.commands:
			if ok {
				cmd.cmd(c)
				cmd.done <- 1
			}
		case now := <-c.ticker.C:
			c.timeout(now)
		}
	}
}

func (c *Client) listen(errors chan<- error) {
	defer c.listening.Done()

	reader := NewReader(c.link)
	reader.SetHandler(c.RawMessageHandler)
	for {
		msg := SwordMessage{}
		read, err := reader.Decode(&msg)
		if err != nil {
			if atomic.LoadInt32(&c.eof) != 0 {
				err = io.EOF
			}
			errors <- ConnectionError{err}
			// Tell serve() to invalidate existing and future handlers, but not
			// to stop yet as the caller can still interact with the client.
			c.quit <- false
			return
		}
		if c.Logger != nil {
			c.Logger(true, read, &msg)
		}
		c.events <- msg
	}
}

func (c *Client) write() {
	defer c.writing.Done()

	writer := NewWriter(c.link)
	for post := range c.posts {
		written, err := writer.Encode(post.message.tag, post.message.GetMessage())
		if err != nil {
			c.errors <- HandlerError{post.Context, ConnectionError{err}}
		} else {
			if c.Logger != nil {
				c.Logger(false, written, &post.message)
			}
		}
	}
	Disconnect(c.link, c.PostTimeout)
}

// Supplied handler will be called in two cases:
// - Whenever a message is received, with the message and the context bound
//   to the handler and returned by Register(). The error is nil.
// - During client termination in Close() or when request is aborted,
//   with the context and an error. The message is nil.
func (c *Client) Register(handler MessageHandler) int32 {
	next := MakeHandlerRegister(handler, 0)
	c.registers <- next
	return <-next.context
}

func (c *Client) Unregister(context int32) {
	data := HandlerUnregister{
		context: context,
		done:    make(chan struct{}),
	}
	c.unregisters <- data
	<-data.done
}

func (c *Client) MakeMessage(msg SwordMessage, handler MessageHandler, timeout time.Duration) MessagePost {
	next := MakeHandlerRegister(handler, timeout)
	c.registers <- next
	context := <-next.context
	msg.SetContext(context)
	return MessagePost{msg, handler, context}
}

func (c *Client) PostMessage(msg MessagePost) {
	c.posts <- msg
}

func (c *Client) PostWithTimeout(msg SwordMessage, handler MessageHandler, timeout time.Duration) int32 {
	m := c.MakeMessage(msg, handler, timeout)
	c.PostMessage(m)
	return m.Context
}

func (c *Client) Post(msg SwordMessage, handler MessageHandler) int32 {
	return c.PostWithTimeout(msg, handler, c.PostTimeout)
}

func (c *Client) runCommand(cmd func(c *Client)) {
	rq := commandRequest{
		cmd:  cmd,
		done: make(chan int, 1),
	}
	c.commands <- rq
	<-rq.done
	return
}

func (c *Client) GetClientId() int32 {
	clientId := int32(0)
	c.runCommand(func(client *Client) {
		clientId = c.clientId
	})
	return clientId
}
