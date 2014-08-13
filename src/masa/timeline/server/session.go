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
	"sync"
	"time"
)

type SessionObserver interface {
	SdkObserver
	services.EventListener
	UpdateTick(time.Time)
	Close()
}

type ObserverContext struct {
	observer SessionObserver
	// This WaitGroup is used to make sure all updates have been seen when
	// someone call UnregisterObserver on this observer
	group sync.WaitGroup
}

// This WaitGroup is used to make sure all updates have been seen when someone
// detach the service which implement an EventListener
type EventListeners map[services.EventListener]*sync.WaitGroup
type EventFilterers map[services.EventFilterer]struct{}
type Observers map[SdkObserver]*ObserverContext
type Services map[string]services.Service

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

type SessionData struct {
	status    sdk.Session_Status
	tick      time.Time     // current tick time
	locked    bool          // true if the clock is read-only
	offset    time.Duration // initial time offset
	services  Services
	listeners EventListeners
	observers Observers
	events    EventSlice
}

// A Session groups services and manages their running states, while providing
// a unified view over their events.
type Session struct {
	log       util.Logger
	uuid      string
	name      string
	autostart bool       // automatically start events
	mutex     sync.Mutex // protects SessionData
	d         SessionData
	fmutex    sync.Mutex     // protects filterers
	filterers EventFilterers // always modified sequentially
	posts     chan func()    // serializes model updates
	runner    sync.WaitGroup // wait for model updates
}

func NewSession(log util.Logger, uuid, name string, autostart bool) *Session {
	filterers := EventFilterers{
		&ServiceFilter{}:         struct{}{},
		&KeywordFilter{}:         struct{}{},
		&ShowOnlyFilter{}:        struct{}{},
		&HideHierarchiesFilter{}: struct{}{},
	}
	s := &Session{
		log:       log,
		uuid:      uuid,
		name:      name,
		autostart: autostart,
		filterers: filterers,
		posts:     make(chan func(), 16),
		d: SessionData{
			status:    sdk.Session_IDLE,
			services:  Services{},
			listeners: EventListeners{},
			observers: Observers{},
		},
	}
	s.runner.Add(1)
	go s.run()
	return s
}

func (s *Session) run() {
	defer s.runner.Done()
	for post := range s.posts {
		post()
	}
}

func (s *Session) waitUpdates() {
	s.mutex.Lock()
	observers := []SdkObserver{}
	for k := range s.d.observers {
		observers = append(observers, k)
	}
	services := []string{}
	for k := range s.d.services {
		services = append(services, k)
	}
	s.mutex.Unlock()
	for _, k := range observers {
		s.UnregisterObserver(k)
	}
	for _, k := range services {
		s.Detach(k)
	}
}

func (s *Session) Close() {
	s.Stop()
	// This part synchronizes all observers & listeners events and make sure
	// they're dispatched before we can effectively close the session
	s.waitUpdates()
	close(s.posts)
	s.runner.Wait()
}

type Logger func(fmt string, args ...interface{})

func (s *Session) Log(format string, args ...interface{}) {
	msg := fmt.Sprintf(format, args...)
	s.log.Printf("session[%v] %v\n", s.name, msg)
}

func (s *Session) Proto() *sdk.Session {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	session := &sdk.Session{
		Uuid:        proto.String(s.uuid),
		Status:      s.d.status.Enum(),
		Name:        proto.String(s.name),
		NumServices: proto.Int32(int32(len(s.d.services))),
		NumEvents:   proto.Int32(int32(len(s.d.events))),
	}
	if !s.d.tick.IsZero() {
		session.Time = proto.String(util.FormatTime(s.d.tick))
		session.Locked = proto.Bool(s.d.locked)
	}
	if s.d.offset != 0 {
		session.Offset = proto.Int64(int64(s.d.offset))
	}
	return session
}

type SessionList []*Session

func (s SessionList) Proto() []*sdk.Session {
	sessions := make([]*sdk.Session, len(s))
	for i, value := range s {
		sessions[i] = value.Proto()
	}
	return sessions
}

// Returns the number of attached services with clocks and true if any of them
// is read-only.
func (s *SessionData) numClocks() (int, bool) {
	num := int(0)
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
	s.mutex.Lock()
	defer s.mutex.Unlock()
	_, ok := s.d.services[name]
	if ok {
		return ErrServiceNameTaken
	}
	if service.HasClock() {
		if num, _ := s.d.numClocks(); num > 1 {
			return ErrTooManyClocks
		}
	}
	s.d.services[name] = service
	if listener, ok := service.(services.EventListener); ok {
		s.d.listeners[listener] = &sync.WaitGroup{}
	}
	s.fmutex.Lock()
	if filterer, ok := service.(services.EventFilterer); ok {
		s.filterers[filterer] = struct{}{}
	}
	s.fmutex.Unlock()
	return nil
}

func (s *Session) detach(name string) (services.Service, error) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	if s.d.status == sdk.Session_LIVE {
		return nil, ErrInProgress
	}
	service, ok := s.d.services[name]
	if !ok {
		return nil, ErrServiceNotFound
	}
	delete(s.d.services, name)
	var group *sync.WaitGroup
	if listener, ok := service.(services.EventListener); ok {
		group = s.d.listeners[listener]
		delete(s.d.listeners, listener)
	}
	s.fmutex.Lock()
	if filterer, ok := service.(services.EventFilterer); ok {
		delete(s.filterers, filterer)
	}
	s.fmutex.Unlock()
	if group != nil {
		group.Wait()
	}
	return service, nil
}

func (s *Session) Detach(name string) error {
	service, err := s.detach(name)
	if err != nil {
		return err
	}
	service.Stop()
	return nil
}

func (s *SessionData) getServices() Services {
	services := Services{}
	for k, v := range s.services {
		services[k] = v
	}
	return services
}

func (s *SessionData) getEvents() EventSlice {
	events := make(EventSlice, len(s.events))
	copy(events, s.events)
	return events
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
	s.mutex.Lock()
	defer s.mutex.Unlock()
	if s.d.locked {
		return util.NewError(http.StatusBadRequest, "unable to change locked time")
	}
	s.d.offset += begin.Sub(s.d.tick)
	s.d.setTick(s.postObserver, begin)
	return nil
}

func (s *Session) tryStart(offset time.Duration) (Services, error) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	num, locked := s.d.numClocks()
	if num < 1 {
		return nil, ErrMissingClock
	}
	switch s.d.status {
	case sdk.Session_LIVE:
		return nil, nil
	case sdk.Session_STARTING, sdk.Session_STOPPING:
		return nil, ErrInProgress
	}
	s.d.status = sdk.Session_STARTING
	s.d.locked = locked
	s.d.offset = offset
	return s.d.getServices(), nil
}

func (s *Session) Start(offset time.Duration) error {
	services, err := s.tryStart(offset)
	if err != nil || services == nil {
		return err
	}
	return s.startServices(services)
}

func (s *Session) applyServices(src Services, name string, operand func(service services.Service) error) error {
	errors := make(chan error, len(src))
	waiter := sync.WaitGroup{}
	waiter.Add(len(src))
	for _key, _service := range src {
		go func(key string, service services.Service) {
			defer waiter.Done()
			err := operand(service)
			if err != nil {
				errors <- err
				s.Log("unable to %v service '%v': %v", name, key, err)
			}
		}(_key, _service)
	}
	waiter.Wait()
	close(errors) // close channel or next range won't ever quit
	for err := range errors {
		if err != nil {
			return err
		}
	}
	return nil
}

func (s *Session) startServices(src Services) error {
	err := s.applyServices(src, "start", func(service services.Service) error {
		return service.Start()
	})
	next := sdk.Session_LIVE
	if err != nil {
		next = sdk.Session_IDLE
	}
	s.mutex.Lock()
	s.d.status = next
	s.mutex.Unlock()
	return err
}

func (s *Session) tryStop() (Services, error) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	switch s.d.status {
	case sdk.Session_IDLE:
		return nil, nil
	case sdk.Session_STARTING, sdk.Session_STOPPING:
		return nil, ErrInProgress
	}
	s.d.status = sdk.Session_STOPPING
	return s.d.getServices(), nil
}

func (s *Session) Stop() error {
	services, err := s.tryStop()
	if err != nil || services == nil {
		return nil
	}
	return s.stopServices(services)
}

func (s *Session) stopServices(src Services) error {
	err := s.applyServices(src, "stop", func(service services.Service) error {
		return service.Stop()
	})
	next := sdk.Session_IDLE
	if err != nil {
		next = sdk.Session_LIVE
	}
	s.mutex.Lock()
	s.d.status = next
	s.mutex.Unlock()
	return err
}

func (s *Session) Abort(err error) {
	s.Log("error: %v", err)
	go s.Stop()
}

func post(posts chan<- func(), group *sync.WaitGroup, operand func()) {
	group.Add(1)
	posts <- func() {
		defer group.Done()
		operand()
	}
}

func (s EventListeners) Update(posts chan<- func(), events ...*sdk.Event) {
	for k, group := range s {
		listener := k
		post(posts, group, func() {
			listener.Update(events...)
		})
	}
}

func (s EventListeners) Delete(posts chan<- func(), events ...string) {
	for k, group := range s {
		listener := k
		post(posts, group, func() {
			listener.Delete(events...)
		})
	}
}

func (s *Session) postObserver(ctx *ObserverContext, apply func(so SessionObserver)) {
	post(s.posts, &ctx.group, func() {
		apply(ctx.observer)
	})
}

type Poster func(ctx *ObserverContext, apply func(so SessionObserver))

func (s *SessionData) setTick(post Poster, tick time.Time) {
	modified := s.tick != tick
	s.tick = tick
	if !modified {
		return
	}
	for _, observer := range s.observers {
		post(observer, func(so SessionObserver) {
			so.UpdateTick(tick)
		})
	}
}

func (s *Session) Tick(tick time.Time) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	if tick.IsZero() {
		s.d.setTick(s.postObserver, tick)
		return
	}
	last := s.d.tick
	tick = tick.Add(s.d.offset)
	s.d.setTick(s.postObserver, tick)
	if last.IsZero() {
		last = tick
	}
	if s.autostart {
		s.d.runEvents(s.Log, s.d.events, last, tick)
	}
}

func (s *SessionData) runEvent(log Logger, event *Event) {
	if !event.action.apply {
		return
	}
	service, ok := s.services[event.action.url.Host]
	if !ok {
		log("unable to find service '%s' for event '%s'", event.action.url.Host, event.uuid)
		return
	}
	go service.Apply(event.uuid, event.action.url, event.action.payload)
}

func (s *SessionData) runEvents(log Logger, events EventSlice, begin, end time.Time) {
	for _, event := range events {
		if event.done {
			continue
		}
		if event.begin.Before(begin) {
			continue
		}
		if event.begin.After(end) {
			continue
		}
		s.runEvent(log, event)
	}
}

func (s *Session) ReadServices() []*sdk.Service {
	s.mutex.Lock()
	services := s.d.getServices()
	s.mutex.Unlock()
	rpy := make([]*sdk.Service, 0, len(services))
	for k, v := range services {
		rpy = append(rpy, v.Proto(k))
	}
	return rpy
}

func filterEvents(filters []services.EventFilter, events []*sdk.Event) []*sdk.Event {
	if len(filters) == 0 {
		return events
	}
	// more complex than needed, but avoid copies
	count := len(events)
	for i := 0; i < count; i++ {
		for _, filter := range filters {
			if filter(events[i]) {
				events[i] = events[count-1]
				i--
				count--
				break
			}
		}
	}
	return events[:count]
}

func (s *Session) getFilterers() EventFilterers {
	s.fmutex.Lock()
	defer s.fmutex.Unlock()
	filterers := EventFilterers{}
	for k, v := range s.filterers {
		filterers[k] = v
	}
	return filterers
}

func (s *Session) ReadEvents(config services.EventFilterConfig) []*sdk.Event {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	filters := []services.EventFilter{}
	events := s.d.events.Proto()
	for f := range s.getFilterers() {
		filters = append(filters, f.GetFilters(config)...)
	}
	return filterEvents(filters, events)
}

func (s *Session) OnApply(uuid string, err error, lock bool) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	event := s.d.events.Find(uuid)
	if event == nil {
		return
	}
	modified := event.OnApply(err, lock)
	if !modified {
		return
	}
	s.d.listeners.Update(s.posts, event.Proto())
}

func appendParentUpdate(event *Event, updates *[]*sdk.Event) {
	if event.parent != nil && event.parent.SetChild(event) {
		*updates = append(*updates, event.parent.Proto())
	}
}

func (s *Session) CreateEvent(uuid string, msg *sdk.Event) (*sdk.Event, error) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	event, err := NewEvent(msg, s.d.events)
	if err != nil {
		return nil, err
	}
	if event := s.d.events.Find(event.uuid); event != nil {
		return nil, ErrEventUuidTaken
	}
	s.d.events.Append(event)
	proto := event.Proto()
	updates := []*sdk.Event{proto}
	appendParentUpdate(event, &updates)
	s.d.listeners.Update(s.posts, updates...)
	return proto, nil
}

func (s *Session) UpdateEvent(uuid string, msg *sdk.Event) (*sdk.Event, error) {
	s.mutex.Lock()
	event := s.d.events.Find(uuid)
	if event == nil {
		s.mutex.Unlock()
		return s.CreateEvent(uuid, msg)
	}
	modified, triggered, updateChildren, err := event.Update(msg, s.d.tick, s.d.events)
	if err != nil {
		s.mutex.Unlock()
		return nil, err
	}
	proto := event.Proto()
	if modified {
		updates := []*sdk.Event{proto}
		appendParentUpdate(event, &updates)
		if updateChildren {
			for child := range event.children {
				updates = append(updates, child.Proto())
			}
		}
		s.d.listeners.Update(s.posts, updates...)
		if triggered {
			s.d.runEvent(s.Log, event)
		}
	}
	s.mutex.Unlock()
	return proto, err
}

func (s *Session) DeleteEvent(uuid string) error {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	event := s.d.events.Find(uuid)
	updates := []*sdk.Event{}
	parent := &Event{}
	if event != nil {
		parent = event.parent
		for child := range event.children {
			child.parent = nil
			updates = append(updates, child.Proto())
		}
	}
	if !s.d.events.Remove(uuid) {
		return ErrUnknownEvent
	}
	if parent != nil {
		delete(parent.children, event)
	}
	if len(updates) > 0 {
		s.d.listeners.Update(s.posts, updates...)
	}
	s.d.listeners.Delete(s.posts, uuid)
	return nil
}

type FilteredObserver struct {
	SessionObserver
	config  services.EventFilterConfig
	session *Session
}

func (f *FilteredObserver) Update(events ...*sdk.Event) {
	filters := []services.EventFilter{func(event *sdk.Event) bool {
		for it := range f.session.getFilterers() {
			if it.Filter(event, f.config) {
				return true
			}
		}
		return false
	}}
	f.SessionObserver.Update(filterEvents(filters, events)...)
}

func (s *Session) RegisterObserver(config services.EventFilterConfig) SdkObserver {
	observer := &FilteredObserver{
		SessionObserver: NewObserver(),
		config:          config,
		session:         s,
	}
	s.mutex.Lock()
	defer s.mutex.Unlock()
	ctx := &ObserverContext{observer: observer}
	s.d.observers[observer] = ctx
	s.d.listeners[observer] = &ctx.group
	tick := s.d.tick
	events := s.d.events.Proto()
	// we cannot call Update and UpdateTick here directly
	// * We are locked
	// * We haven't returned the observer channel yet so nobody can pop events
	// Instead we post it to the s.posts goroutine
	s.postObserver(ctx, func(so SessionObserver) {
		so.UpdateTick(tick)
		so.Update(events...)
	})
	return observer
}

func (s *Session) UnregisterObserver(observer SdkObserver) {
	s.mutex.Lock()
	ctx := s.d.observers[observer]
	delete(s.d.listeners, ctx.observer)
	delete(s.d.observers, ctx.observer)
	s.mutex.Unlock()
	// Make sure our observer has received all its events before closing it
	ctx.group.Wait()
	ctx.observer.Close()
}
