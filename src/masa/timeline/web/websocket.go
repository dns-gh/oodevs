// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package web

import (
	"code.google.com/p/go.net/websocket"
	"io"
	"net"
	"regexp"
	"server"
	"sync/atomic"
	"time"
	"util"
)

func isTimeout(err error) bool {
	next, ok := err.(net.Error)
	if !ok {
		return false
	}
	return next.Timeout()
}

type WsContext struct {
	log    util.Logger
	link   *websocket.Conn
	input  server.SdkObserver
	output chan interface{}
	quit   chan bool
	done   uint32
}

func (w *WsContext) Quit() bool {
	first := atomic.AddUint32(&w.done, 1) == 1
	if first {
		close(w.quit)
	}
	return first
}

func (w *WsContext) SaveBroadcasts() {
	for message := range w.input.Listen() {
		if len(w.output) == cap(w.output) {
			first := w.Quit()
			if first {
				w.log.Printf("[ws] Unable to send messages fast enough to %s\n", w.link.RemoteAddr())
			}
			continue
		}
		w.output <- message
	}
}

func (w *WsContext) IgnoreReceives() {
	buf := []byte{}
	for {
		w.link.SetReadDeadline(time.Now().Add(1 * time.Second))
		err := websocket.Message.Receive(w.link, buf)
		if err != nil && !isTimeout(err) {
			if err != io.EOF {
				w.log.Printf("[ws] Unable to read from %s: %s\n", w.link.RemoteAddr(), err)
			}
			w.Quit()
			return
		}
		select {
		case <-w.quit:
			return
		default:
		}
	}
}

func (w *WsContext) SendBroadcasts() {
	for {
		select {
		case message := <-w.output:
			w.link.SetWriteDeadline(time.Now().Add(1 * time.Hour))
			err := websocket.JSON.Send(w.link, message)
			if err != nil {
				w.log.Printf("[ws] Unable to send message to %s: %s\n", w.link.RemoteAddr(), err)
				return
			}
		case <-w.quit:
			return
		}
	}
}

func (s *Server) socketHandler(log util.Logger, ws *websocket.Conn) {
	defer ws.Close()
	req := ws.Request()
	socketMatcher := regexp.MustCompile("/socket/(.+)")
	m := socketMatcher.FindStringSubmatch(req.URL.Path)
	if m == nil {
		return
	}
	uuid := m[1]
	config, err := server.ParseEventFilterConfig(req)
	if err != nil {
		log.Printf("[ws] Invalid event filter %s\n", err)
		return
	}
	observer, err := s.controller.RegisterObserver(uuid, config)
	if err != nil {
		log.Printf("[ws] Unable to observe session %s: %s\n", uuid, err)
		return
	}
	defer s.controller.UnregisterObserver(uuid, observer)
	ctx := WsContext{
		log:    log,
		link:   ws,
		input:  observer,
		output: make(chan interface{}, 256),
		quit:   make(chan bool),
	}
	defer ctx.Quit()
	go ctx.SaveBroadcasts()
	go ctx.IgnoreReceives()
	ctx.SendBroadcasts()
}
