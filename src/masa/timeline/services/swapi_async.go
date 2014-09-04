// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package services

import (
	"container/list"
	"masa/sword/swapi"
	"sync"
)

// Allows reacting on swapi events without
// running our code under swapi locks
type SwapiAsync struct {
	events *list.List
	mutex  sync.Mutex
	cond   *sync.Cond
	quit   bool
}

type SwapiEvent struct {
	msg      *swapi.SwordMessage
	clientId int32
	ctx      int32
	err      error
}

func NewSwapiAsync() *SwapiAsync {
	a := &SwapiAsync{
		events: list.New(),
	}
	a.cond = sync.NewCond(&a.mutex)
	return a
}

func (a *SwapiAsync) Write(msg *swapi.SwordMessage, clientId, ctx int32, err error) bool {
	a.mutex.Lock()
	quit := a.quit
	if !quit {
		a.events.PushBack(SwapiEvent{msg, clientId, ctx, err})
	}
	a.mutex.Unlock()
	a.cond.Signal()
	return quit
}

func (a *SwapiAsync) pop() (SwapiEvent, bool) {
	a.mutex.Lock()
	defer a.mutex.Unlock()
	for !a.quit && a.events.Len() == 0 {
		a.cond.Wait()
	}
	if a.quit {
		return SwapiEvent{}, false
	}
	return a.events.Remove(a.events.Front()).(SwapiEvent), true
}

type SwapiOperand func(msg *swapi.SwordMessage, clientId, ctx int32, err error) bool

func (a *SwapiAsync) read(operand SwapiOperand) bool {
	event, ok := a.pop()
	if !ok {
		return true
	}
	return operand(event.msg, event.clientId, event.ctx, event.err)
}

func (a *SwapiAsync) run(operand SwapiOperand) {
	for {
		if a.read(operand) {
			a.Close()
			return
		}
	}
}

func (a *SwapiAsync) Start(operand SwapiOperand) {
	go a.run(operand)
}

func (a *SwapiAsync) Close() {
	a.mutex.Lock()
	a.quit = true
	a.mutex.Unlock()
	a.cond.Signal()
}
