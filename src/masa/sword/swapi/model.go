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
	"sync"
	"sync/atomic"
	"time"
)

var (
	ErrInvalidTime = errors.New("invalid time")
)

type ModelHandler func(model *ModelData, msg *SwordMessage, err error) bool

type modelCmd struct {
	done chan int
	cmd  func(model *Model)
}

type modelCond struct {
	id   int
	done chan int
	cond func(model *Model) bool
}

type ModelRequest struct {
	Message *SwordMessage
	Command *modelCmd
	Cond    *modelCond
}

type ModelErrorHandler func(*ModelData, *SwordMessage, error) error

// The model registers a handler to a Client and updates itself with
// incoming messages in a separate goroutine. It supports two kind of
// actions: commands which are just functions applied once on the Model,
// mostly used to implement queries, and conditions which are fired each
// time a message is processed until they return true. Messages, commands and
// conditions are deliberately serialized so we know that commands issued from
// a message handler will have a model update with all previous messages.
//
// Note that accessor will return either immutable entities or copies of
// such entities (mostly collections) so clients can manipulate the
// result freely.
type Model struct {
	// State only touched by the run() goroutine
	conds        []*modelCond
	data         *ModelData
	errorHandler ModelErrorHandler
	handlerId    int32
	handlers     map[int32]ModelHandler
	listeners    *ModelListeners
	timeouts     map[int32]time.Time
	closed       bool

	// State only touched by caller routines
	condId      int32
	WaitTimeout time.Duration

	// Shared state
	requests  chan *ModelRequest
	running   sync.WaitGroup
	listening sync.WaitGroup
}

func NewModel() *Model {
	listeners := NewModelListeners()
	model := Model{
		WaitTimeout: 60 * time.Second,
		handlers:    map[int32]ModelHandler{},
		timeouts:    map[int32]time.Time{},
		listeners:   listeners,
		requests:    make(chan *ModelRequest, 128),
		conds:       []*modelCond{},
		data:        NewModelData(listeners),
	}
	model.setErrorHandler(nil)

	model.running.Add(1)
	go model.run()
	return &model
}

func (model *Model) setErrorHandler(errorHandler ModelErrorHandler) {
	if errorHandler == nil {
		errorHandler = func(data *ModelData, msg *SwordMessage, err error) error {
			panic(err)
		}
	}
	model.errorHandler = errorHandler
}

// Reconfigure the model error handler. A nil argument restore the default
// panicking handler. The handler is called upon update errors with the current
// ModelData, the input message and the error instance. Return nil to tell the
// error was handled and the model correctly updated, an error otherwise. Upon
// error, all the conditions will be terminated on error and model goroutine
// terminated.
func (model *Model) SetErrorHandler(errorHandler ModelErrorHandler) {
	model.waitCommand(func(m *Model) {
		model.setErrorHandler(errorHandler)
	})
}

func (model *Model) remove(handlerId int32) {
	delete(model.handlers, handlerId)
	delete(model.timeouts, handlerId)
}

func (model *Model) run() error {
	defer func() {
		for _, handler := range model.handlers {
			handler(model.data, nil, ErrConnectionClosed)
		}
		model.running.Done()
	}()

	ticker := time.NewTicker(1 * time.Second)
	defer ticker.Stop()
	for {
		select {
		case rq, ok := <-model.requests:
			if !ok {
				return nil
			}
			if rq.Message != nil {
				if err := model.data.update(rq.Message); err != nil {
					err = model.errorHandler(model.data, rq.Message, err)
					if err != nil {
						for _, cond := range model.conds {
							cond.done <- 0
						}
						return err
					}
				}

				// Execute handlers
				for id, handler := range model.handlers {
					if handler(model.data, rq.Message, nil) {
						model.remove(id)
					}
				}

				//Execute conditions
				for i := 0; i != len(model.conds); {
					it := model.conds[i]
					if it.cond(model) {
						it.done <- 1
						model.conds = append(model.conds[:i], model.conds[i+1:]...)
						continue
					}
					i++
				}
			} else if rq.Command != nil {
				rq.Command.cmd(model)
				rq.Command.done <- 1
			} else if rq.Cond != nil {
				if rq.Cond.cond(model) {
					rq.Cond.done <- 1
				} else {
					model.conds = append(model.conds, rq.Cond)
				}
			}
		case now := <-ticker.C:
			for id, timeout := range model.timeouts {
				if now.After(timeout) {
					model.handlers[id](model.data, nil, ErrTimeout)
					model.remove(id)
				}
			}
		}
	}
}

// Terminates model goroutine. After calling Close(), trying to interact with
// the model, like calling WaitCondition, will panic. Calling Close() multiple
// times is a no-op.
func (model *Model) Close() {
	if model.closed {
		return
	}
	model.closed = true
	// Wait for all listeners to be gone
	model.listening.Wait()
	close(model.requests)
	model.running.Wait()
}

// Register a handler on supplied client and start processing messages
func (model *Model) Listen(client *Client) {
	handler := func(msg *SwordMessage, client, context int32, err error) bool {
		if err != nil {
			model.listening.Done()
			return true
		}
		model.requests <- &ModelRequest{Message: msg}
		return false
	}
	model.listening.Add(1)
	client.Register(handler)
}

func (model *Model) waitCommand(cmd func(model *Model)) {
	c := &ModelRequest{
		Command: &modelCmd{
			done: make(chan int, 1),
			cmd:  cmd,
		},
	}
	model.requests <- c
	<-c.Command.done
}

// Registers a handler to be called upon ModelData updates, after the message
// changes have been applied. If timeout is non-nul, the handler will be called
// with ErrTimeout after supplied duration. It will be called with
// ErrConnectionClosed upon Model termination. Returned identifier can be used
// with UnregisterHandler.
func (model *Model) RegisterHandlerTimeout(timeout time.Duration,
	handler ModelHandler) int32 {

	id := int32(0)
	model.waitCommand(func(model *Model) {
		id = model.handlerId
		model.handlerId++
		model.handlers[id] = handler
		if timeout > 0 {
			model.timeouts[id] = time.Now().Add(timeout)
		}
	})
	return id
}

func (model *Model) RegisterHandler(handler ModelHandler) int32 {
	return model.RegisterHandlerTimeout(model.WaitTimeout, handler)
}

func (model *Model) UnregisterHandler(id int32) {
	model.waitCommand(func(model *Model) {
		if handler := model.handlers[id]; handler != nil {
			handler(model.data, nil, ErrConnectionClosed)
			model.remove(id)
		}
	})
}

func (model *Model) RegisterListener(listener ModelListener) int32 {
	id := int32(0)
	model.waitCommand(func(model *Model) {
		id = model.listeners.Register(listener)
	})
	return id
}

func (model *Model) UnregisterListener(listener int32) {
	model.waitCommand(func(model *Model) {
		model.listeners.Unregister(listener)
	})
}

// Post and wait for a condition to be validated. Return false on
// timeout. The condition is validated only once.
func (model *Model) waitCond(timeout time.Duration,
	cond func(model *Model) bool) bool {

	id := atomic.AddInt32(&model.condId, 1)
	c := &ModelRequest{
		Cond: &modelCond{
			id:   int(id),
			done: make(chan int, 1),
			cond: cond,
		},
	}
	model.requests <- c
	timeoutch := make(chan struct{})
	if timeout.Nanoseconds() > 0 {
		go func() {
			time.Sleep(timeout)
			close(timeoutch)
		}()
	}
	removed := false
	select {
	case <-c.Cond.done:
		break
	case <-timeoutch:
		model.waitCommand(func(model *Model) {
			for i := 0; i != len(model.conds); {
				if model.conds[i].id == c.Cond.id {
					model.conds[i].done <- 0
					model.conds = append(model.conds[:i], model.conds[i+1:]...)
					removed = true
					break
				}
				i++
			}
		})
	}
	return !removed
}

// Return true if the model is ready.
func (model *Model) IsReady() bool {
	ready := false
	model.waitCommand(func(model *Model) {
		ready = model.data.Ready
	})
	return ready
}

// Wait for the model to become ready, return false otherwise.
func (model *Model) WaitReady(timeout time.Duration) bool {
	return model.waitCond(timeout, func(model *Model) bool {
		return model.data.Ready
	})
}

func (model *Model) GetData() *ModelData {
	var d *ModelData
	model.waitCommand(func(model *Model) {
		d = &ModelData{}
		DeepCopy(d, model.data)
	})
	return d
}

func (model *Model) GetProfile(login string) *Profile {
	var p *Profile
	model.waitCommand(func(model *Model) {
		profile, ok := model.data.Profiles[login]
		if ok {
			p = &Profile{}
			DeepCopy(p, profile)
		}
	})
	return p
}

func (model *Model) GetTacticalLine(lineId uint32) *TacticalLine {
	var p *TacticalLine
	model.waitCommand(func(model *Model) {
		line, ok := model.data.TacticalLines[lineId]
		if ok {
			p = &TacticalLine{}
			DeepCopy(p, line)
		}
	})
	return p
}

func (model *Model) GetFormation(formationId uint32) *Formation {
	var f *Formation
	model.waitCommand(func(model *Model) {
		formation, ok := model.data.Formations[formationId]
		if ok {
			f = &Formation{}
			DeepCopy(f, formation)
		}
	})
	return f
}

func (model *Model) GetAutomat(automatId uint32) *Automat {
	var a *Automat
	model.waitCommand(func(model *Model) {
		automat, ok := model.data.Automats[automatId]
		if ok {
			a = &Automat{}
			DeepCopy(a, automat)
		}
	})
	return a
}

func (model *Model) GetUnit(unitId uint32) *Unit {
	var u *Unit
	model.waitCommand(func(model *Model) {
		unit, ok := model.data.Units[unitId]
		if ok {
			u = &Unit{}
			DeepCopy(u, unit)
		}
	})
	return u
}

func (model *Model) GetCrowd(crowdId uint32) *Crowd {
	var c *Crowd
	model.waitCommand(func(model *Model) {
		crowd, ok := model.data.Crowds[crowdId]
		if ok {
			c = &Crowd{}
			DeepCopy(c, crowd)
		}
	})
	return c
}

func (model *Model) GetLocalWeather(id uint32) *LocalWeather {
	var w *LocalWeather
	model.waitCommand(func(model *Model) {
		weather, ok := model.data.LocalWeathers[id]
		if ok {
			w = &LocalWeather{}
			DeepCopy(w, weather)
		}
	})
	return w
}

func (model *Model) GetGlobalWeather() *Weather {
	var w *Weather
	model.waitCommand(func(model *Model) {
		weather := model.data.GlobalWeather
		w = &Weather{}
		DeepCopy(w, weather)
	})
	return w
}

func (model *Model) GetOrder(id uint32) *Order {
	var o *Order
	model.waitCommand(func(model *Model) {
		order, ok := model.data.Orders[id]
		if ok {
			o = &Order{}
			DeepCopy(o, order)
		}
	})
	return o
}

func (model *Model) GetObjectKnowledge(id uint32) *ObjectKnowledge {
	var k *ObjectKnowledge
	model.waitCommand(func(model *Model) {
		knowledge, ok := model.data.ObjectKnowledges[id]
		if ok {
			k = &ObjectKnowledge{}
			DeepCopy(k, knowledge)
		}
	})
	return k
}

func (model *Model) GetUnitKnowledge(id uint32) *UnitKnowledge {
	var k *UnitKnowledge
	model.waitCommand(func(model *Model) {
		knowledge, ok := model.data.UnitKnowledges[id]
		if ok {
			k = &UnitKnowledge{}
			DeepCopy(k, knowledge)
		}
	})
	return k
}

func (model *Model) GetCrowdKnowledge(id uint32) *CrowdKnowledge {
	var k *CrowdKnowledge
	model.waitCommand(func(model *Model) {
		knowledge, ok := model.data.CrowdKnowledges[id]
		if ok {
			k = &CrowdKnowledge{}
			DeepCopy(k, knowledge)
		}
	})
	return k
}

func (model *Model) GetTick() int32 {
	tick := int32(0)
	model.waitCommand(func(model *Model) {
		tick = model.data.Tick
	})
	return tick
}

func (model *Model) GetObject(id uint32) *Object {
	var o *Object
	model.waitCommand(func(model *Model) {
		object, ok := model.data.Objects[id]
		if ok {
			o = &Object{}
			DeepCopy(o, object)
		}
	})
	return o
}

func (model *Model) WaitUntilTick(tick int32) bool {
	return model.waitCond(model.WaitTimeout, func(model *Model) bool {
		return tick <= model.data.Tick
	})
}

func (model *Model) WaitUntilTickEnds(tick int32) bool {
	return model.waitCond(model.WaitTimeout, func(model *Model) bool {
		return tick <= model.data.LastTick
	})
}

func (model *Model) WaitTicks(ticks int32) bool {
	endTick := model.GetTick() + ticks
	return model.WaitUntilTick(endTick)
}

// Wait for condition to return true. Return false if the timeout kicks in
// before the condition.
func (model *Model) WaitConditionTimeout(timeout time.Duration,
	condition func(data *ModelData) bool) bool {
	wrapper := func(model *Model) bool {
		return condition(model.data)
	}
	return model.waitCond(timeout, wrapper)
}

func (model *Model) WaitCondition(condition func(data *ModelData) bool) bool {
	wrapper := func(model *Model) bool {
		return condition(model.data)
	}
	return model.waitCond(model.WaitTimeout, wrapper)
}
