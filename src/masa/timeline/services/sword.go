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
	gouuid "code.google.com/p/go-uuid/uuid"
	"code.google.com/p/goprotobuf/proto"
	"encoding/base64"
	"encoding/json"
	"errors"
	"fmt"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/timeline/sdk"
	"masa/timeline/util"
	"net/http"
	"net/url"
	"time"
)

var (
	ErrInvalidReplayEventParameter = util.NewError(http.StatusBadRequest, "invalid replay parameter")
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
	ReplayRangeUuid = gouuid.New()
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
	reconnect bool                          // auto connect when disconnected
	retry     uint                          // retry count
	link      *SwordLink                    // current sword link
	status    SwordStatus                   // current status
	orders    Ids                           // known order ids
	actions   Ids                           // known action ids
	services  SwordServices                 // published sword services (like replay/simulation/aar)
	startTime time.Time                     // exercise start date
	endTime   time.Time                     // exercise end date
	replays   []*sdk.Event                  // replay events list
}

func NewSword(log util.Logger, clock bool, name, address string) *Sword {
	return &Sword{
		log:      log,
		name:     name,
		address:  address,
		clock:    clock,
		events:   map[string]swapi.SwordMessage{},
		metadata: map[string]*sdk.Metadata{},
		pending:  map[string]*Action{},
		orders:   Ids{},
		actions:  Ids{},
		services: SwordServices{},
		replays:  []*sdk.Event{},
	}
}

func (s *Sword) HasReplay() bool {
	_, ok := s.services[sword.EnumService_service_replay]
	return ok
}

func (s *Sword) Proto(name string) *sdk.Service {
	return &sdk.Service{
		Name:  proto.String(name),
		Clock: proto.Bool(s.clock),
		Sword: &sdk.Sword{
			Address:   proto.String(s.address),
			HasReplay: proto.Bool(s.HasReplay()),
		},
	}
}

func (s *Sword) HasClock() bool {
	return s.clock
}

func (*Sword) IsLocked() bool {
	return true
}

func (s *Sword) AttachObserver(observer Observer) {
	s.observer = observer
}

func (s *Sword) Log(format string, args ...interface{}) {
	msg := fmt.Sprintf(format, args...)
	s.log.Printf("sword[%v] %v\n", s.address, msg)
}

// Sword is managed as a state machine with three states as in SwordStatus
// There are four possible transitions, Start, Stop, Attach

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

func (s *Sword) PostServices(link *SwordLink, services SwordServices) {
	s.observer.Post(func() {
		s.setServices(link, services)
	})
}

func (s *Sword) PostReplayRangeDates(link *SwordLink, start, end time.Time) {
	s.observer.Post(func() {
		s.setReplayRangeDates(link, start, end)
	})
}

func (s *Sword) Start() error {
	s.reconnect = true
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
		if s.reconnect {
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
	if !s.reconnect {
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

func logConnect(handler SwordLinkObserver, err error) {
	if err != nil {
		handler.PostLog(nil, "connection failed: %s", err)
	} else {
		handler.PostLog(nil, "connected")
	}
}

func asyncConnect(handler SwordLinkObserver, address, name string, clock bool) {
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
	s.reconnect = false
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
	case m == nil:
		return false
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

func (s *Sword) Trigger(url url.URL, event *sdk.Event) error {
	uuid := event.GetUuid()
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
	action := NewAction(uuid, url, event.GetAction().GetPayload(),
		msg, isOrder(msg.ClientToSimulation))
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

func isSwordEvent(event *sdk.Event, scheme string, checkPayload bool) bool {
	action := event.GetAction()
	url, err := url.Parse(action.GetTarget())
	if err != nil {
		return false
	}
	if url.Scheme != scheme {
		return false
	}
	return !checkPayload || checkPayload && len(action.GetPayload()) > 0
}

func (s *Sword) cacheEvent(event *sdk.Event, overwrite bool) *swapi.SwordMessage {
	id := event.GetUuid()
	if !overwrite {
		if msg, ok := s.events[id]; ok {
			return &msg
		}
	}
	if !isSwordEvent(event, "sword", true) {
		return nil
	}
	msg := swapi.SwordMessage{}
	payload := event.GetAction().GetPayload()
	err := swapi.DecodeMessage(&msg, swapi.ClientToSimulationTag, payload)
	if err != nil {
		s.Log("unable to decode payload on event %v: payload:%v error:%v",
			id, base64.StdEncoding.EncodeToString(payload), err)
		msg = swapi.SwordMessage{ClientToSimulation: &sword.ClientToSim{}}
	}
	s.events[id] = msg
	return &msg
}

func checkTime(parameter string, replayBegin, replayEnd time.Time) (eventTime time.Time, err error) {
	eventTime, err = util.ParseTime(parameter)
	if err != nil || eventTime.Before(replayBegin) || eventTime.After(replayEnd) {
		err = ErrInvalidReplayEventParameter
	}
	return
}

func checkBoundaries(event *sdk.Event, replayBegin, replayEnd time.Time) (begin, end time.Time, err error) {
	begin, err = checkTime(event.GetBegin(), replayBegin, replayEnd)
	if err != nil {
		return
	}
	end, err = checkTime(event.GetEnd(), replayBegin, replayEnd)
	return
}

func marshal(begin, end bool) []byte {
	buffer, _ := json.Marshal(&sdk.ReplayPayload{Begin: proto.Bool(begin), End: proto.Bool(end)})
	return buffer
}

func updatePayload(index, size int, event *sdk.Event) {
	begin := size > 1 && index != 0
	end := size > 1 && index+1 < size
	event.Action.Payload = marshal(begin, end)
}

func (s *Sword) updateReplay(event *sdk.Event) {
	// check if the event has replay protocol
	if !isSwordEvent(event, "replay", false) {
		return
	}
	eventBegin, eventEnd, err := checkBoundaries(event, s.startTime, s.endTime)
	if err != nil {
		return
	}
	// add first replay event
	if len(s.replays) == 0 {
		s.replays = append(s.replays, CloneEvent(event))
		return
	}
	// modifying replay event, filling all gaps
	for index, replay := range s.replays {
		if event.GetUuid() == replay.GetUuid() {
			if event.GetBegin() != replay.GetBegin() && index > 0 {
				s.replays[index-1].End = event.Begin
				s.observer.UpdateEvent(s.replays[index-1].GetUuid(), s.replays[index-1])
			} else if event.GetEnd() != replay.GetEnd() && index < len(s.replays)-1 {
				s.replays[index+1].Begin = event.End
				s.observer.UpdateEvent(s.replays[index+1].GetUuid(), s.replays[index+1])
			}
			s.replays[index] = CloneEvent(event)
			return
		}
	}
	// splitting and creating a new replay event, filling all gaps
	replaysCopy := []*sdk.Event{}
	size := len(s.replays) + 1 // one element is added
	for index, replay := range s.replays {
		replayBegin, _ := util.ParseTime(replay.GetBegin())
		replayEnd, _ := util.ParseTime(replay.GetEnd())
		if replayEnd == eventEnd && replayBegin.Before(eventBegin) {
			// the old event takes the first part of the split, the new event the second
			replay.End = event.Begin
			event.Name = replay.Name
			event.Info = replay.Info
			updatePayload(index, size, replay)
			updatePayload(index+1, size, event)
			replaysCopy = append(replaysCopy, replay)
			replaysCopy = append(replaysCopy, CloneEvent(event))
			s.observer.UpdateEvent(replay.GetUuid(), replay)
		} else if replayBegin == eventBegin && replayEnd.After(eventEnd) {
			// the old event takes the second part of the split, the new event the first
			replay.Begin = event.End
			event.Name = replay.Name
			event.Info = replay.Info
			updatePayload(index, size, event)
			updatePayload(index+1, size, replay)
			replaysCopy = append(replaysCopy, event)
			replaysCopy = append(replaysCopy, replay)
			s.observer.UpdateEvent(replay.GetUuid(), replay)
		} else {
			replaysCopy = append(replaysCopy, replay)
		}
	}
	s.replays = replaysCopy
	return
}

func (s *Sword) deleteReplay(uuid string) {
	size := len(s.replays) - 1 // one element is deleted
	for index, replay := range s.replays {
		if replay.GetUuid() == uuid {
			if index == 0 {
				// Removing the first replay event fills the gap with the next one
				s.replays[1].Begin = replay.Begin
				updatePayload(0, size, s.replays[1])
				s.observer.UpdateEvent(s.replays[1].GetUuid(), s.replays[1])
			} else {
				// Removing a replay event fills the gap with the previous one
				s.replays[index-1].End = replay.End
				updatePayload(index-1, size, s.replays[index-1])
				s.observer.UpdateEvent(s.replays[index-1].GetUuid(), s.replays[index-1])
			}
			s.replays = append(s.replays[:index], s.replays[index+1:]...)
		}
	}
}

func (s *Sword) CheckEvent(event *sdk.Event) error {
	// First event replay creation requires no check
	if len(s.replays) == 0 {
		return nil
	}
	// Checks if it is a replay event creation
	if !isSwordEvent(event, "replay", false) {
		return nil
	}
	// Cannot modify event outside replay boundaries
	eventBegin, eventEnd, err := checkBoundaries(event, s.startTime, s.endTime)
	if err != nil {
		return err
	}
	// Cannot modify event with invalid range
	if eventBegin == eventEnd || eventBegin.After(eventEnd) {
		return ErrInvalidReplayEventParameter
	}
	// Checks event modifier
	for index, replay := range s.replays {
		if event.GetUuid() == replay.GetUuid() {
			// Cannot modify both begin and end boundaries
			if replay.GetBegin() != event.GetBegin() && replay.GetEnd() != event.GetEnd() {
				return ErrInvalidReplayEventParameter
			}
			// Cannot create gap between events and replay boundaries
			if index == 0 && eventBegin.After(s.startTime) || index == len(s.replays)-1 && eventEnd.Before(s.endTime) {
				return ErrInvalidReplayEventParameter
			}
			// Cannot overlap more than one event
			if index > 0 {
				previousReplay, _ := util.ParseTime(s.replays[index-1].GetBegin())
				if previousReplay.After(eventBegin) {
					return ErrInvalidReplayEventParameter
				}
			}
			if index < len(s.replays)-1 {
				nextEnd, _ := util.ParseTime(s.replays[index+1].GetEnd())
				if nextEnd.Before(eventEnd) {
					return ErrInvalidReplayEventParameter
				}
			}
			return nil
		}
	}
	// Checks new replay event (see it like a split of an existing replay event)
	for _, replay := range s.replays {
		replayBegin, _ := util.ParseTime(replay.GetBegin())
		replayEnd, _ := util.ParseTime(replay.GetEnd())
		// Checks the split, the new event takes a part and the old event the other
		if replayBegin == eventBegin && replayEnd.After(eventEnd) || replayEnd == eventEnd && replayBegin.Before(eventBegin) {
			return nil
		}
	}
	// If there is no valid modification or no valid creation, it's an error
	return ErrInvalidReplayEventParameter
}

func (s *Sword) CheckDeleteEvent(uuid string) (err error) {
	err = nil
	for _, replay := range s.replays {
		if uuid == replay.GetUuid() {
			// Cannot remove the last replay event
			if len(s.replays) < 2 {
				err = ErrInvalidReplayEventParameter
			}
			return
		}
	}
	return
}

func (s *Sword) UpdateEvents(events ...*sdk.Event) {
	for _, event := range events {
		s.updateReplay(event)
		s.cacheEvent(event, true)
		s.cacheMetadata(event, true)
	}
}

func (s *Sword) DeleteEvents(events ...string) {
	for _, uuid := range events {
		s.deleteReplay(uuid)
		delete(s.events, uuid)
		delete(s.metadata, uuid)
	}
}

func (s *Sword) invalidateFilters(link *SwordLink) {
	if link == s.link {
		s.observer.InvalidateFilters()
	}
}

func (s *Sword) setServices(link *SwordLink, services SwordServices) {
	if link == s.link {
		s.services = services
		s.observer.UpdateServices()
	}
}

func (s *Sword) setReplayRangeDates(link *SwordLink, start, end time.Time) {
	if link == s.link {
		s.startTime = start
		s.endTime = end
		event := &sdk.Event{
			Uuid:     proto.String(ReplayRangeUuid),
			Name:     proto.String("Replay range"),
			Begin:    proto.String(util.FormatTime(start)),
			End:      proto.String(util.FormatTime(end)),
			ReadOnly: proto.Bool(false),
			Action: &sdk.Action{
				Target:  proto.String("replay://" + s.name),
				Payload: marshal(false, false),
			},
		}
		s.observer.UpdateEvent(ReplayRangeUuid, event)
		s.observer.UpdateRangeDates(start, end)
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

func CloneEvent(event *sdk.Event) *sdk.Event {
	clone := &sdk.Event{}
	swapi.DeepCopy(clone, event)
	return clone
}
