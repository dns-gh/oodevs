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
	"time"
)

var (
	MaxPostMessages     = 32
	ClientTimeout       = 10 * time.Minute
	TickPeriod          = 1 * time.Second
	ErrTimeout          = errors.New("handler timeout")
	ErrConnectionClosed = errors.New("connection closed")
	InvalidContext      = int32(0)
)

type MessageHandler func(msg *SwordMessage, context int32, err error) bool

type MessagePost struct {
	message SwordMessage
	handler MessageHandler
	context int32
}

type HandlerRegister struct {
	handler MessageHandler
	context chan int32
	timeout time.Duration
}

func MakeHandlerRegister(handler MessageHandler, timeout time.Duration) HandlerRegister {
	return HandlerRegister{
		handler: handler,
		context: make(chan int32),
		timeout: timeout,
	}
}

type HandlerError struct {
	context int32
	err     error
}

type Client struct {
	Address string
	Model   *Model

	context     int32
	link        net.Conn
	handlers    map[int32]MessageHandler
	timeouts    map[int32]time.Time
	ticker      *time.Ticker
	posts       chan MessagePost
	registers   chan HandlerRegister
	unregisters chan int32
	events      chan SwordMessage
	errors      chan HandlerError
	quit        chan bool
	eof         bool
}

func NewClient(address string) (*Client, error) {
	link, err := net.Dial("tcp", address)
	if err != nil {
		return nil, err
	}
	client := &Client{
		Address:     address,
		Model:       NewModel(),
		link:        link,
		handlers:    make(map[int32]MessageHandler),
		timeouts:    make(map[int32]time.Time),
		ticker:      time.NewTicker(TickPeriod),
		posts:       make(chan MessagePost, MaxPostMessages),
		registers:   make(chan HandlerRegister, MaxPostMessages),
		unregisters: make(chan int32, MaxPostMessages),
		events:      make(chan SwordMessage, MaxPostMessages),
		errors:      make(chan HandlerError, MaxPostMessages),
		quit:        make(chan bool),
	}
	return client, nil
}

func (c *Client) Run() error {
	errors := make(chan error)
	go c.serve()
	go c.write()
	go c.listen(errors)
	c.Model.Listen(c)
	return <-errors
}

func Connect(host string) (*Client, error) {
	client, err := NewClient(host)
	if err != nil {
		return nil, err
	}
	go client.Run()
	return client, nil
}

func (c *Client) Close() {
	c.eof = true
	c.quit <- true
	close(c.posts)
	c.ticker.Stop()
	c.link.Close()
	for context, handler := range c.handlers {
		handler(nil, context, ErrConnectionClosed)
	}
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
	panic("unreachable")
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
	removals := []int32{}
	for context, handler := range c.handlers {
		remove := handler(msg, context, nil)
		if remove {
			removals = append(removals, context)
		}
	}
	for _, context := range removals {
		c.remove(context)
	}
}

func (c *Client) abort(context int32, err error) {
	handler, ok := c.handlers[context]
	if !ok {
		return
	}
	handler(nil, context, err)
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
	for {
		select {
		case <-c.quit:
			return
		case data := <-c.registers:
			c.register(data)
		case context := <-c.unregisters:
			c.remove(context)
		case event := <-c.events:
			c.apply(&event)
		case data := <-c.errors:
			c.abort(data.context, data.err)
		case now := <-c.ticker.C:
			c.timeout(now)
		}
	}
}

func (c *Client) listen(errors chan<- error) {
	reader := NewReader(c.link)
	for {
		msg := SwordMessage{}
		err := reader.Decode(&msg)
		if err != nil {
			if c.eof {
				err = io.EOF
			}
			errors <- err
			return
		}
		c.events <- msg
	}
}

func (c *Client) write() {
	writer := NewWriter(c.link)
	for post := range c.posts {
		err := writer.Encode(post.message.tag, post.message.GetMessage())
		if err != nil {
			c.errors <- HandlerError{post.context, err}
		}
	}
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
	c.unregisters <- context
}

func (c *Client) PostWithTimeout(msg SwordMessage, handler MessageHandler, timeout time.Duration) int32 {
	next := MakeHandlerRegister(handler, timeout)
	c.registers <- next
	context := <-next.context
	msg.SetContext(context)
	c.posts <- MessagePost{msg, handler, context}
	return context
}

func (c *Client) Post(msg SwordMessage, handler MessageHandler) int32 {
	return c.PostWithTimeout(msg, handler, 0)
}
