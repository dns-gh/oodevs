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
	gouuid "code.google.com/p/go-uuid/uuid"
	"masa/timeline/sdk"
	"masa/timeline/services"
	"masa/timeline/util"
	"net/http"
	"sync"
	"time"
)

type SessionContext struct {
	session *Session
	posts   *util.EventLoop
}

// Controller maps Sessions to identifiers, which can be accessed concurrently.
// It enforces session command sequentiality by posting every command to the
// target session event loop.
type Controller struct {
	log      util.Logger
	mutex    sync.Mutex
	sessions map[string]*SessionContext
}

func MakeController(log util.Logger) *Controller {
	c := &Controller{
		log:      log,
		sessions: make(map[string]*SessionContext),
	}
	return c
}

func checkUuid(src string) bool {
	return gouuid.Parse(src).Variant() != gouuid.Invalid
}

func (c *Controller) CreateSession(uuid, name string, autostart bool) (*sdk.Session, error) {
	if len(uuid) == 0 {
		uuid = gouuid.New()
	} else if !checkUuid(uuid) {
		return nil, util.NewError(http.StatusBadRequest, "invalid uuid parameter")
	}
	ctx := &SessionContext{
		session: NewSession(c.log, &ControllerObserver{uuid, c}, uuid, name, autostart),
		posts:   util.NewEventLoop(),
	}
	c.mutex.Lock()
	_, ok := c.sessions[ctx.session.uuid]
	if !ok {
		c.sessions[ctx.session.uuid] = ctx
	}
	if ok {
		c.mutex.Unlock()
		return nil, util.NewErrorCode(http.StatusConflict)
	}
	ctx.posts.Start()
	reply := make(chan *sdk.Session, 1)
	ctx.posts.Post(func() {
		reply <- ctx.session.Proto()
	})
	c.mutex.Unlock()
	return <-reply, nil
}

func (c *Controller) ReadSession(uuid string) (*sdk.Session, error) {
	return c.applySession(uuid, func(session *Session) error {
		return nil
	})
}

func (c *Controller) UpdateSession(uuid string, msg *sdk.Session) (*sdk.Session, error) {
	return c.applySession(uuid, func(session *Session) error {
		return session.Update(msg)
	})
}

func (c *Controller) DeleteSession(uuid string) error {
	// avoid calling session commands inside mutex
	c.mutex.Lock()
	ctx, ok := c.sessions[uuid]
	delete(c.sessions, uuid)
	c.mutex.Unlock()
	if !ok {
		return util.NewErrorCode(http.StatusNotFound)
	}
	ctx.posts.Close()
	ctx.session.Close()
	return nil
}

func (c *Controller) ListSessions() []*sdk.Session {
	// avoid calling session commands inside mutex
	c.mutex.Lock()
	uuids := make([]string, 0, len(c.sessions))
	for k := range c.sessions {
		uuids = append(uuids, k)
	}
	c.mutex.Unlock()
	sessions := make([]*sdk.Session, 0, len(uuids))
	for _, uuid := range uuids {
		value, err := c.ReadSession(uuid)
		if err != nil {
			continue
		}
		sessions = append(sessions, value)
	}
	return sessions
}

func (c *Controller) apply(uuid string, operand func(*Session) (interface{}, error)) (
	interface{}, error) {
	c.mutex.Lock()
	ctx, ok := c.sessions[uuid]
	if !ok {
		c.mutex.Unlock()
		return nil, util.NewErrorCode(http.StatusNotFound)
	}
	type Reply struct {
		value interface{}
		err   error
	}
	reply := make(chan Reply, 1)
	ctx.posts.Post(func() {
		value, err := operand(ctx.session)
		reply <- Reply{value, err}
	})
	c.mutex.Unlock()
	r := <-reply
	if r.err != nil {
		return nil, util.NewError(http.StatusInternalServerError, r.err.Error())
	}
	return r.value, nil
}

func (c *Controller) post(uuid string, operand func(*Session)) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	ctx, ok := c.sessions[uuid]
	if !ok {
		return
	}
	ctx.posts.Post(func() {
		operand(ctx.session)
	})
}

// Executes "operand" on target session, in session event loop
func (c *Controller) applySession(uuid string, operand func(*Session) error) (
	*sdk.Session, error) {
	value, err := c.apply(uuid, func(session *Session) (interface{}, error) {
		err := operand(session)
		return session.Proto(), err
	})
	if err != nil {
		return nil, err
	}
	return value.(*sdk.Session), err
}

func (c *Controller) AttachTimerService(uuid, name string, base string) (*sdk.Session, error) {
	begin := time.Now()
	if len(base) > 0 {
		var err error
		begin, err = util.ParseTime(base)
		if err != nil {
			return nil, util.NewError(http.StatusBadRequest, "invalid time parameter", err.Error())
		}
	}
	return c.applySession(uuid, func(session *Session) error {
		timer := services.NewTimer(&ControllerObserver{uuid, c}, begin)
		return session.AttachService(name, timer)
	})
}

func (c *Controller) AttachSwordService(uuid, name string, clock bool, address string) (*sdk.Session, error) {
	return c.applySession(uuid, func(session *Session) error {
		sword := services.NewSword(c.log, &ControllerObserver{uuid, c}, clock, name, address)
		err := session.AttachService(name, sword)
		if err != nil {
			return err
		}
		session.AttachListener(sword)
		session.AttachFilterer(sword)
		return nil
	})
}

func (c *Controller) DetachService(uuid, name string) (*sdk.Session, error) {
	return c.applySession(uuid, func(session *Session) error {
		return session.Detach(name)
	})
}

func (c *Controller) StartSession(uuid string, offset int64) (*sdk.Session, error) {
	return c.applySession(uuid, func(session *Session) error {
		return session.Start(time.Duration(offset) * time.Millisecond)
	})
}

func (c *Controller) StopSession(uuid string) (*sdk.Session, error) {
	return c.applySession(uuid, func(session *Session) error {
		return session.Stop()
	})
}

func (c *Controller) CreateEvent(uuid string, msg *sdk.Event) (*sdk.Event, error) {
	value, err := c.apply(uuid, func(session *Session) (interface{}, error) {
		return session.CreateEvent(uuid, msg)
	})
	if err != nil {
		return nil, err
	}
	return value.(*sdk.Event), err
}

func (c *Controller) ReadServices(uuid string) ([]*sdk.Service, error) {
	value, err := c.apply(uuid, func(session *Session) (interface{}, error) {
		return session.ReadServices(), nil
	})
	if err != nil {
		return nil, err
	}
	return value.([]*sdk.Service), err
}

func (c *Controller) ReadEvents(uuid string, config services.EventFilterConfig) ([]*sdk.Event, error) {
	value, err := c.apply(uuid, func(session *Session) (interface{}, error) {
		return session.ReadEvents(config), nil
	})
	if err != nil {
		return nil, err
	}
	return value.([]*sdk.Event), err
}

func (c *Controller) UpdateEvent(uuid, event string, msg *sdk.Event) (*sdk.Event, error) {
	value, err := c.apply(uuid, func(session *Session) (interface{}, error) {
		return session.UpdateEvent(event, msg)
	})
	if err != nil {
		return nil, err
	}
	return value.(*sdk.Event), err
}

func (c *Controller) DeleteEvent(uuid, event string) error {
	_, err := c.apply(uuid, func(session *Session) (interface{}, error) {
		return nil, session.DeleteEvent(event)
	})
	return err
}

func (c *Controller) RegisterObserver(uuid string, config services.EventFilterConfig) (SdkObserver, error) {
	value, err := c.apply(uuid, func(session *Session) (interface{}, error) {
		return session.RegisterObserver(config), nil
	})
	if err != nil {
		return nil, err
	}
	return value.(SdkObserver), err
}

func (c *Controller) UnregisterObserver(uuid string, observer SdkObserver) {
	// maybe we should not ignore errors here...
	c.apply(uuid, func(session *Session) (interface{}, error) {
		session.UnregisterObserver(observer)
		return nil, nil
	})
}

// Implements services.Observer
// This is not implemented directly by a controller
// in order to enforce boundaries. Make sure no
// function is blocking or we will suffer deadlocks
type ControllerObserver struct {
	session    string
	controller *Controller
}

func (c *ControllerObserver) Abort(err error) {
	c.controller.post(c.session, func(session *Session) {
		session.Abort(err)
	})
}

func (c *ControllerObserver) Tick(tick time.Time) {
	c.controller.post(c.session, func(session *Session) {
		session.Tick(tick)
	})
}

func (c *ControllerObserver) CloseEvent(uuid string, err error, lock bool) {
	c.controller.post(c.session, func(session *Session) {
		session.CloseEvent(uuid, err, lock)
	})
}

func (c *ControllerObserver) UpdateEvent(uuid string, event *sdk.Event) {
	c.controller.post(c.session, func(session *Session) {
		session.UpdateEvent(uuid, event)
	})
}

func (c *ControllerObserver) Post(operand func()) {
	c.controller.post(c.session, func(session *Session) {
		operand()
	})
}

func (c *ControllerObserver) InvalidateFilters() {
	c.controller.post(c.session, func(session *Session) {
		session.InvalidateFilters()
	})
}

func (c *ControllerObserver) UpdateServices() {
	c.controller.post(c.session, func(session *Session) {
		session.UpdateServices()
	})
}

func (c *ControllerObserver) UpdateRangeDates(start, end time.Time) {
	c.controller.post(c.session, func(session *Session) {
		session.UpdateRangeDates(start, end)
	})
}
