// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package util

import (
	"sync"
)

// Implements an unbounded event loop
type EventLoop struct {
	posts []func()
	quit  bool
	mutex sync.Mutex
	cond  *sync.Cond
	group sync.WaitGroup
}

func NewEventLoop() *EventLoop {
	e := &EventLoop{}
	e.cond = sync.NewCond(&e.mutex)
	return e
}

func (e *EventLoop) run() {
	for {
		e.mutex.Lock()
		for !e.quit && len(e.posts) == 0 {
			e.cond.Wait()
		}
		if e.quit {
			e.mutex.Unlock()
			break
		}
		post := e.posts[0]
		e.posts = e.posts[1:]
		e.mutex.Unlock()
		post()
	}
	for _, post := range e.posts {
		post()
	}
	e.group.Done()
}

func (e *EventLoop) Start() {
	e.group.Add(1)
	go e.run()
}

func (e *EventLoop) Close() {
	e.mutex.Lock()
	e.quit = true
	e.mutex.Unlock()
	e.cond.Signal()
	e.group.Wait()
}

func (e *EventLoop) Post(post func()) {
	e.mutex.Lock()
	e.posts = append(e.posts, post)
	e.mutex.Unlock()
	e.cond.Signal()
}
