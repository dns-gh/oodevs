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
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
	"errors"
	"fmt"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/timeline/sdk"
	"masa/timeline/util"
	"net/url"
	"time"
)

type SwordStatus int

const (
	SwordStatusDisconnected = iota
	SwordStatusConnecting
	SwordStatusConnected
)

var (
	SwordTimeLayout = "20060102T150405"
	ErrDisconnected = errors.New("disconnected")
	ErrInProgress   = errors.New("in progress")
	ErrSkipped      = errors.New("skipped")
	ErrUnknown      = errors.New("unknown")
)

type Action struct {
	id      string
	url     url.URL
	payload []byte
	msg     swapi.SwordMessage
	lock    bool
}

func NewAction(id string, url url.URL, payload []byte,
	msg swapi.SwordMessage, lock bool) *Action {
	return &Action{
		id:      id,
		url:     url,
		payload: payload,
		msg:     msg,
		lock:    lock,
	}
}

type Ids map[uint32]struct{}

type Sword struct {
	log      util.Logger
	observer Observer // attached observer
	name     string   // service name
	address  string   // sword address
	clock    bool     // whether we use sword clock
	// mutable
	events    map[string]swapi.SwordMessage // decoded messages
	pending   map[string]*Action            // pending actions
	metadata  map[string]*sdk.Metadata      // decoded metadata
	autostart bool                          // auto connect when disconnected
	retry     uint                          // retry count
	link      *SwordLink                    // current sword link
	status    SwordStatus                   // current status
	orders    Ids                           // known order ids
	actions   Ids                           // known action ids
}

func NewSword(log util.Logger, observer Observer, clock bool, name, address string) *Sword {
	return &Sword{
		log:      log,
		observer: observer,
		name:     name,
		address:  address,
		clock:    clock,
		events:   map[string]swapi.SwordMessage{},
		metadata: map[string]*sdk.Metadata{},
		pending:  map[string]*Action{},
		orders:   Ids{},
		actions:  Ids{},
	}
}

func (s *Sword) Proto(name string) *sdk.Service {
	return &sdk.Service{
		Name:  proto.String(name),
		Clock: proto.Bool(s.clock),
		Sword: &sdk.Sword{
			Address: proto.String(s.address),
		},
	}
}

func (s *Sword) HasClock() bool {
	return s.clock
}

func (*Sword) IsLocked() bool {
	return true
}

func (s *Sword) Log(format string, args ...interface{}) {
	msg := fmt.Sprintf(format, args...)
	s.log.Printf("sword[%v] %v\n", s.address, msg)
}

// Sword is managed as a state machine with three states as in SwordStatus
// There are four possible transitions, Start, Stop, Attach

type SwordHandler interface {
	PostAttach(link *SwordLink)
	PostLog(link *SwordLink, format string, args ...interface{})
	PostRestart(link *SwordLink, err error)
	PostCloseAction(link *SwordLink, action string, err error)
	PostCloseWriter(link *SwordLink, writer *SwordWriter)
	PostInvalidateFilters(link *SwordLink)
}

func (s *Sword) PostRestart(link *SwordLink, err error) {
	s.observer.Post(func() {
		if err != nil {
			s.Log("network error: %v", err)
		}
		s.restart(link)
	})
}

func (s *Sword) PostAttach(link *SwordLink) {
	s.observer.Post(func() {
		s.attach(link)
	})
}

func (s *Sword) PostLog(link *SwordLink, format string, args ...interface{}) {
	s.observer.Post(func() {
		if link == s.link {
			s.Log(format, args...)
		}
	})
}

func (s *Sword) PostCloseAction(link *SwordLink, action string, err error) {
	s.observer.Post(func() {
		s.closeAction(link, action, err)
	})
}

func (s *Sword) PostCloseWriter(link *SwordLink, writer *SwordWriter) {
	s.observer.Post(func() {
		link.CloseWriter(writer)
	})
}

func (s *Sword) PostInvalidateFilters(link *SwordLink) {
	s.observer.Post(func() {
		s.invalidateFilters(link)
	})
}

func (s *Sword) Start() error {
	s.autostart = true
	return s.start()
}

func (s *Sword) setStatus(status SwordStatus, restart bool) {
	s.status = status
	if status == SwordStatusDisconnected {
		if !restart {
			// abort all pending actions
			for _, action := range s.pending {
				s.observer.CloseEvent(action.id, ErrDisconnected, action.lock)
			}
			s.pending = map[string]*Action{}
		}
		if s.autostart {
			s.start()
		}
	} else if status == SwordStatusConnected {
		for _, action := range s.pending {
			s.link.PostAction(action)
		}
	}
}

func (s *Sword) getRetryTimeout() time.Duration {
	s.retry++
	// 1 -> first try, no timeout
	// 2 -> first reconnect, no timeout
	// 3 -> begin at 1 second and double each time
	if s.retry <= 2 {
		return 0 * time.Second
	}
	sleep := 1 << (s.retry - 3) * time.Second
	if sleep > time.Minute {
		s.retry-- // prevent cycles
		sleep = time.Minute
	}
	return sleep
}

func (s *Sword) start() error {
	if s.status == SwordStatusConnecting {
		return ErrInProgress
	} else if s.status == SwordStatusConnected {
		return ErrSkipped
	}
	if !s.autostart {
		return ErrSkipped
	}
	timeout := s.getRetryTimeout()
	prefix := ""
	if s.link != nil {
		prefix = "re"
	}
	suffix := ""
	if d := timeout.Seconds(); d > 0 {
		suffix = fmt.Sprintf(" in %vs", d)
	}
	s.Log("%sconnecting%s", prefix, suffix)
	s.setStatus(SwordStatusConnecting, false)
	go func() {
		time.Sleep(timeout)
		asyncConnect(s, s.address, s.name, s.clock)
	}()
	return nil
}

func logConnect(handler SwordHandler, err error) {
	if err != nil {
		handler.PostLog(nil, "connection failed: %s", err)
	} else {
		handler.PostLog(nil, "connected")
	}
}

func asyncConnect(handler SwordHandler, address, name string, clock bool) {
	link, err := NewSwordLink(handler, address, name, clock)
	if err != nil {
		logConnect(handler, err)
		link = nil
	}
	handler.PostAttach(link)
}

func (s *Sword) attach(link *SwordLink) {
	if s.status != SwordStatusConnecting {
		if link != nil {
			link.Kill()
		}
		return
	}
	if link == nil {
		s.setStatus(SwordStatusDisconnected, true)
		return
	}
	if s.link != nil {
		// now that we have something new
		// we can kill the old model
		s.link.Kill()
	}
	s.link = link
	// Known orders & actions are preserved across connections
	link.Attach(s.observer, s.orders, s.actions)
	s.retry = 0
	s.Log("connected")
	s.setStatus(SwordStatusConnected, false)
}

func (s *Sword) Stop() error {
	s.autostart = false
	if s.status == SwordStatusDisconnected {
		return ErrSkipped
	}
	// SwordStatusConnecting:
	// s.link == nil from start
	// s.link == xxx from restart
	// SwordStatusConnected:
	// s.link = xxx
	if s.link == nil {
		s.setStatus(SwordStatusDisconnected, false)
		return nil
	}
	link := s.link
	s.link = nil
	link.Detach()
	s.Log("disconnected")
	s.setStatus(SwordStatusDisconnected, false)
	return nil
}

func (s *Sword) restart(link *SwordLink) {
	if s.status != SwordStatusConnected &&
		s.link != link {
		// ignore any restart in flowing state
		return
	}
	// lie and tell we are disconnected
	// do not touch the current link, as
	// we need to keep the model alive
	// until we have a new one
	s.setStatus(SwordStatusDisconnected, true)
}

func isOrder(msg *sword.ClientToSim) bool {
	m := msg.GetMessage()
	switch {
	case m.UnitOrder != nil:
		return true
	case m.AutomatOrder != nil:
		return true
	case m.CrowdOrder != nil:
		return true
	case m.FragOrder != nil:
		return true
	}
	return false
}

func (s *Sword) Apply(uuid string, url url.URL, payload []byte) error {
	msg, ok := s.events[uuid]
	if !ok {
		return ErrUnknown
	}
	if _, ok := s.pending[uuid]; ok {
		return ErrInProgress
	}
	data, err := json.Marshal(msg.ClientToSimulation.GetMessage())
	if err != nil {
		data = []byte(err.Error())
	}
	s.Log("-> action %v %s", uuid, data)
	action := NewAction(uuid, url, payload, msg, isOrder(msg.ClientToSimulation))
	s.pending[uuid] = action
	if s.status == SwordStatusConnected {
		s.link.PostAction(action)
	}
	return nil
}

func (s *Sword) closeAction(link *SwordLink, id string, err error) {
	// Discard actions when we're not in connected state
	// It should not happen and would not fix potential errors
	// when our order has been applied by the server but the ack
	// has not been received
	if s.status != SwordStatusConnected || s.link != link {
		return
	}
	action, ok := s.pending[id]
	if !ok {
		return
	}
	delete(s.pending, id)
	s.observer.CloseEvent(id, err, action.lock)
}

func isSwordEvent(event *sdk.Event) bool {
	action := event.GetAction()
	url, err := url.Parse(action.GetTarget())
	if err != nil {
		return false
	}
	if url.Scheme != "sword" {
		return false
	}
	return len(action.GetPayload()) > 0
}

func (s *Sword) cacheEvent(event *sdk.Event, overwrite bool) *swapi.SwordMessage {
	id := event.GetUuid()
	if !overwrite {
		if msg, ok := s.events[id]; ok {
			return &msg
		}
	}
	if !isSwordEvent(event) {
		return nil
	}
	msg := swapi.SwordMessage{}
	payload := event.GetAction().GetPayload()
	err := swapi.DecodeMessage(&msg, swapi.ClientToSimulationTag, payload)
	if err != nil {
		s.Log("unable to decode payload on event %v: payload:%v error:%v",
			id, string(payload), err)
		msg = swapi.SwordMessage{ClientToSimulation: &sword.ClientToSim{}}
	}
	s.events[id] = msg
	return &msg
}

func (s *Sword) Update(events ...*sdk.Event) {
	for _, event := range events {
		s.cacheEvent(event, true)
		s.cacheMetadata(event, true)
	}
}

func (s *Sword) Delete(events ...string) {
	for _, uuid := range events {
		delete(s.events, uuid)
		delete(s.metadata, uuid)
	}
}

func (s *Sword) invalidateFilters(link *SwordLink) {
	if link == s.link {
		s.observer.InvalidateFilters()
	}
}

// Custom test functions
func (s *Sword) waitFor(operand func() bool) {
	reply := make(chan bool, 1)
	for {
		s.observer.Post(func() {
			reply <- operand()
		})
		if <-reply {
			return
		}
		time.Sleep(100 * time.Millisecond)
	}
}

func (s *Sword) WaitFor(operand func(d *swapi.ModelData) bool) {
	// a bit convoluted because we want to block, but only this
	// function and not gosword model, and we must run inside
	// the observer event loop to use session data
	s.waitFor(func() bool {
		if s.link == nil {
			return false
		}
		return s.link.client.Model.Query(operand)
	})
}

func (s *Sword) WaitForRetry(retry uint) {
	s.waitFor(func() bool {
		return s.retry >= retry
	})
}

func (s *Sword) WaitForStatus(status SwordStatus) {
	s.waitFor(func() bool {
		return s.status == status
	})
}
