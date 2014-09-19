// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package server

import (
	"code.google.com/p/goprotobuf/proto"
	"fmt"
	"masa/timeline/sdk"
	"masa/timeline/services"
	"masa/timeline/util"
	"net/http"
	"time"
)

var (
	ErrServiceNameTaken = util.NewError(http.StatusConflict, "service name taken")
	ErrInProgress       = util.NewError(http.StatusServiceUnavailable, "in progress")
	ErrMissingClock     = util.NewError(http.StatusPreconditionFailed, "missing clock")
	ErrServiceNotFound  = util.NewError(http.StatusNotFound, "service not found")
	ErrTooManyClocks    = util.NewError(http.StatusConflict, "too many clocks")
	ErrEventUuidTaken   = util.NewError(http.StatusConflict, "event uuid taken")
	ErrUnknownSession   = util.NewError(http.StatusNotFound, "unknown session")
	ErrUnknownEvent     = util.NewError(http.StatusNotFound, "unknown event")
)

type EventListener interface {
	Update(events EventSlice, encoded []*sdk.Event)
	Delete(events ...string)
}

type DefaultEventListener struct {
	services.EventListener
}

func (d DefaultEventListener) Update(events EventSlice, encoded []*sdk.Event) {
	d.EventListener.Update(encoded...)
}

type EventListeners map[interface{}]EventListener
type EventFilterers map[services.EventFilterer]struct{}
type Observers map[interface{}]*FilteredObserver
type Services map[string]services.Service

// A Session groups services and manages their running states, while providing
// a unified view over their events.
type Session struct {
	log       util.Logger
	uuid      string
	name      string
	autostart bool // automatically start events
	observer  services.Observer
	// mutable data
	status    sdk.Session_Status
	tick      time.Time     // current tick time
	locked    bool          // true if the clock is read-only
	offset    time.Duration // initial time offset
	services  Services
	listeners EventListeners
	observers Observers
	filterers EventFilterers
	events    EventSlice
}

func NewSession(log util.Logger, observer services.Observer, uuid, name string, autostart bool) *Session {
	return &Session{
		log:       log,
		uuid:      uuid,
		name:      name,
		autostart: autostart,
		status:    sdk.Session_IDLE,
		services:  Services{},
		listeners: EventListeners{},
		observers: Observers{},
		filterers: EventFilterers{
			&ServiceFilter{}:         struct{}{},
			&KeywordFilter{}:         struct{}{},
			&ShowOnlyFilter{}:        struct{}{},
			&HideHierarchiesFilter{}: struct{}{},
		},
	}
}

func (s *Session) Close() {
	s.Stop()
	for _, k := range s.observers {
		s.UnregisterObserver(k)
	}
	for k := range s.services {
		s.Detach(k)
	}
}

type Logger func(fmt string, args ...interface{})

func (s *Session) Log(format string, args ...interface{}) {
	msg := fmt.Sprintf(format, args...)
	s.log.Printf("session[%v] %v\n", s.name, msg)
}

func (s *Session) Proto() *sdk.Session {
	session := &sdk.Session{
		Uuid:        proto.String(s.uuid),
		Status:      s.status.Enum(),
		Name:        proto.String(s.name),
		NumServices: proto.Int32(int32(len(s.services))),
		NumEvents:   proto.Int32(int32(len(s.events))),
	}
	if !s.tick.IsZero() {
		session.Time = proto.String(util.FormatTime(s.tick))
		session.Locked = proto.Bool(s.locked)
	}
	if s.offset != 0 {
		session.Offset = proto.Int64(int64(s.offset))
	}
	return session
}

// Returns the number of attached services with clocks and true if any of them
// is read-only.
func (s *Session) numClocks() (int, bool) {
	num := 0
	locked := false
	for _, service := range s.services {
		if service.HasClock() {
			num++
			locked = locked || service.IsLocked()
		}
	}
	return num, locked
}

func (s *Session) Attach(name string, service services.Service) error {
	_, ok := s.services[name]
	if ok {
		return ErrServiceNameTaken
	}
	if service.HasClock() {
		if num, _ := s.numClocks(); num > 1 {
			return ErrTooManyClocks
		}
	}
	s.services[name] = service
	if listener, ok := service.(services.EventListener); ok {
		s.listeners[listener] = DefaultEventListener{listener}
	}
	if filterer, ok := service.(services.EventFilterer); ok {
		s.filterers[filterer] = struct{}{}
	}
	return nil
}

func (s *Session) Detach(name string) error {
	if s.status == sdk.Session_LIVE {
		return ErrInProgress
	}
	service, ok := s.services[name]
	if !ok {
		return ErrServiceNotFound
	}
	delete(s.services, name)
	if listener, ok := service.(services.EventListener); ok {
		delete(s.listeners, listener)
	}
	if filterer, ok := service.(services.EventFilterer); ok {
		delete(s.filterers, filterer)
	}
	service.Stop()
	return nil
}

func (s *Session) Update(next *sdk.Session) error {
	tick := next.GetTime()
	if len(tick) == 0 {
		return nil
	}
	begin, err := util.ParseTime(tick)
	if err != nil {
		return util.NewError(http.StatusBadRequest, "invalid time parameter", err.Error())
	}
	if s.locked {
		return util.NewError(http.StatusBadRequest, "unable to change locked time")
	}
	s.offset += begin.Sub(s.tick)
	s.setTick(begin)
	return nil
}

func (s *Session) Start(offset time.Duration) error {
	num, locked := s.numClocks()
	if num < 1 {
		return ErrMissingClock
	}
	switch s.status {
	case sdk.Session_LIVE:
		return nil
	case sdk.Session_STARTING, sdk.Session_STOPPING:
		return ErrInProgress
	}
	s.status = sdk.Session_STARTING
	s.locked = locked
	s.offset = offset
	return s.startServices()
}

func (s *Session) applyServices(src Services, name string, operand func(service services.Service) error) error {
	// add better error management when one service fails
	var last error
	for key, service := range src {
		err := operand(service)
		if err != nil {
			s.Log("unable to %v service '%v': %v", name, key, err)
			last = err
		}
	}
	return last
}

func (s *Session) startServices() error {
	err := s.applyServices(s.services, "start", func(service services.Service) error {
		return service.Start()
	})
	if err != nil {
		return err
	}
	s.status = sdk.Session_LIVE
	return nil
}

func (s *Session) Stop() error {
	switch s.status {
	case sdk.Session_IDLE:
		return nil
	case sdk.Session_STARTING, sdk.Session_STOPPING:
		return ErrInProgress
	}
	s.status = sdk.Session_STOPPING
	return s.stopServices()
}

func (s *Session) stopServices() error {
	err := s.applyServices(s.services, "stop", func(service services.Service) error {
		return service.Stop()
	})
	if err != nil {
		return err
	}
	s.status = sdk.Session_IDLE
	return nil
}

func (s EventListeners) Update(events EventSlice, encoded []*sdk.Event) {
	for _, listener := range s {
		listener.Update(events, encoded)
	}
}

func (s EventListeners) Delete(events ...string) {
	for _, listener := range s {
		listener.Delete(events...)
	}
}

func (s *Session) setTick(tick time.Time) {
	modified := s.tick != tick
	s.tick = tick
	if !modified {
		return
	}
	for _, observer := range s.observers {
		observer.UpdateTick(tick)
	}
}

func (s *Session) runEvent(event *Event) {
	if !event.action.apply {
		return
	}
	service, ok := s.services[event.action.url.Host]
	if !ok {
		s.Log("unable to find service '%s' for event '%s'", event.action.url.Host, event.uuid)
		return
	}
	err := service.Apply(event.uuid, event.action.url, event.action.payload)
	if err != nil {
		s.Log("unable to apply event '%s': %v", event.uuid, err)
	}
}

func (s *Session) runEvents(begin, end time.Time) {
	for _, event := range s.events {
		if event.done {
			continue
		}
		if event.begin.Before(begin) {
			continue
		}
		if event.begin.After(end) {
			continue
		}
		s.runEvent(event)
	}
}

func (s *Session) ReadServices() []*sdk.Service {
	rpy := make([]*sdk.Service, 0, len(s.services))
	for k, v := range s.services {
		rpy = append(rpy, v.Proto(k))
	}
	return rpy
}

type Filters []services.EventFilter

func (f Filters) Filter(event *sdk.Event) bool {
	for _, filter := range f {
		if filter(event) {
			return true
		}
	}
	return false
}

func filterEvent(filters Filters, event *Event, encoded *sdk.Event,
	cache map[*Event]bool) bool {
	filtered, ok := cache[event]
	if ok {
		return filtered
	}
	if event.parent != nil {
		if filterEvent(filters, event.parent, event.parent.Proto(), cache) {
			return true
		}
	}
	filtered = filters.Filter(encoded)
	cache[event] = filtered
	return filtered
}

func filterEvents(events EventSlice, encoded []*sdk.Event, filters Filters) []*sdk.Event {
	if len(filters) == 0 {
		return encoded
	}
	// keep a cache of filtered events
	cache := map[*Event]bool{}
	// it's more complex than needed, but avoid copies
	// while we swap encoded, we cannot modify events!
	// instead we keep a tracking index
	count := len(events)
	idx := 0
	for i := 0; i < count; i++ {
		event := events[idx]
		idx = i + 1
		if filterEvent(filters, event, encoded[i], cache) {
			encoded[i] = encoded[count-1]
			idx = count - 1
			i--
			count--
		}
	}
	return encoded[:count]
}

func (s *Session) ReadEvents(config services.EventFilterConfig) []*sdk.Event {
	filters := []services.EventFilter{}
	for f := range s.filterers {
		filters = append(filters, f.GetFilters(config)...)
	}
	return filterEvents(s.events, s.events.Proto(), filters)
}

func makeUpdate(event *Event) (EventSlice, []*sdk.Event) {
	updates := EventSlice{event}
	encoded := []*sdk.Event{event.Proto()}
	if event.parent != nil && event.parent.SetChild(event) {
		updates = append(updates, event.parent)
		encoded = append(encoded, event.parent.Proto())
	}
	return updates, encoded
}

func (s *Session) CreateEvent(uuid string, msg *sdk.Event) (*sdk.Event, error) {
	event, err := NewEvent(msg, s.events)
	if err != nil {
		return nil, err
	}
	if event := s.events.Find(event.uuid); event != nil {
		return nil, ErrEventUuidTaken
	}
	s.events.Append(event)
	updates, encoded := makeUpdate(event)
	s.listeners.Update(updates, encoded)
	return encoded[0], nil
}

func (s *Session) UpdateEvent(uuid string, msg *sdk.Event) (*sdk.Event, error) {
	event := s.events.Find(uuid)
	if event == nil {
		return s.CreateEvent(uuid, msg)
	}
	modified, triggered, children, err := event.Update(msg, s.tick, s.events)
	if err != nil {
		return nil, err
	}
	if !modified {
		return event.Proto(), nil
	}
	updates, encoded := makeUpdate(event)
	if children {
		for child := range event.children {
			updates = append(updates, child)
			encoded = append(encoded, child.Proto())
		}
	}
	s.listeners.Update(updates, encoded)
	if triggered {
		s.runEvent(event)
	}
	return encoded[0], nil
}

func (s *Session) DeleteEvent(uuid string) error {
	event := s.events.Find(uuid)
	updates, encoded := EventSlice{}, []*sdk.Event{}
	parent := &Event{}
	if event != nil {
		parent = event.parent
		for child := range event.children {
			child.parent = nil
			updates = append(updates, child)
			encoded = append(encoded, child.Proto())
		}
	}
	if !s.events.Remove(uuid) {
		return ErrUnknownEvent
	}
	if parent != nil {
		delete(parent.children, event)
	}
	if len(updates) > 0 {
		s.listeners.Update(updates, encoded)
	}
	s.listeners.Delete(uuid)
	return nil
}

type FilteredObserver struct {
	SdkObserver
	observer *Observer
	config   services.EventFilterConfig
	session  *Session
	known    map[string]struct{}
}

func (f *FilteredObserver) update(events EventSlice, encoded []*sdk.Event, reset bool) {
	// maintain a set of known events so we can send
	// delete notifications on events that become filtered
	// but were known previously
	deleted := make([]string, 0, len(encoded))
	added := make([]*sdk.Event, 0, len(encoded))
	filters := []services.EventFilter{func(event *sdk.Event) bool {
		id := event.GetUuid()
		for it := range f.session.filterers {
			if it.Filter(event, f.config) {
				deleted = append(deleted, id)
				return true
			}
		}
		prev := len(f.known)
		f.known[id] = struct{}{}
		if prev != len(f.known) {
			added = append(added, event)
		}
		return false
	}}
	// filterEvents modify encoded in-place
	// we cannot change encoded here so we clone it before
	if len(filters) > 0 {
		clone := make([]*sdk.Event, len(encoded))
		copy(clone, encoded)
		encoded = clone
	}
	updated := filterEvents(events, encoded, filters)
	f.Delete(deleted...)
	if reset {
		updated = added
	}
	f.observer.Update(updated...)
}

func (f *FilteredObserver) Update(events EventSlice, encoded []*sdk.Event) {
	f.update(events, encoded, false)
}

func (f *FilteredObserver) UpdateTick(tick time.Time) {
	f.observer.UpdateTick(tick)
}

func (s *Session) InvalidateFilters() {
	encoded := s.events.Proto()
	for _, observer := range s.observers {
		observer.update(s.events, encoded, true)
	}
}

func (f *FilteredObserver) Delete(events ...string) {
	deleted := []string{}
	for _, event := range events {
		prev := len(f.known)
		delete(f.known, event)
		if prev != len(f.known) {
			deleted = append(deleted, event)
		}
	}
	if len(deleted) > 0 {
		f.observer.Delete(deleted...)
	}
}

func (s *Session) RegisterObserver(config services.EventFilterConfig) SdkObserver {
	observer := NewObserver()
	filtered := &FilteredObserver{
		SdkObserver: observer,
		observer:    observer,
		config:      config,
		session:     s,
		known:       map[string]struct{}{},
	}
	s.observers[filtered] = filtered
	s.listeners[filtered] = filtered
	// beware, as we haven't returned the output channel yet,
	// nobody listens on output messages. Make sure the output
	// channel contain at least 2 buffers or it will deadlock
	// fix the api so it doesn't use channels later
	observer.UpdateTick(s.tick)
	filtered.Update(s.events, s.events.Proto())
	return filtered
}

func (s *Session) UnregisterObserver(filtered SdkObserver) {
	delete(s.listeners, filtered)
	prev := len(s.observers)
	delete(s.observers, filtered)
	if prev != len(s.observers) {
		filtered.(*FilteredObserver).observer.Close()
	}
}

func (s *Session) Abort(err error) {
	s.Log("error: %v", err)
	s.Stop()
}

func (s *Session) Tick(tick time.Time) {
	if tick.IsZero() {
		s.setTick(tick)
		return
	}
	last := s.tick
	tick = tick.Add(s.offset)
	s.setTick(tick)
	if last.IsZero() {
		last = tick
	}
	if s.autostart {
		s.runEvents(last, tick)
	}
}

func (s *Session) CloseEvent(uuid string, err error, lock bool) {
	event := s.events.Find(uuid)
	if event == nil {
		return
	}
	modified := event.OnApply(err, lock)
	if !modified {
		return
	}
	s.listeners.Update(EventSlice{event}, []*sdk.Event{event.Proto()})
}
