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
	"net/http"
	"sdk"
	"services"
	"sync"
	"time"
	"util"
)

// Controller maps Sessions to identifiers, and can be accessed concurrently.
type Controller struct {
	log      util.Logger
	mutex    sync.Mutex
	sessions map[string]*Session // sessions mapping
}

func MakeController(log util.Logger) *Controller {
	c := &Controller{
		log:      log,
		sessions: make(map[string]*Session),
	}
	return c
}

func (c *Controller) addSession(session *Session) bool {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	prev := len(c.sessions)
	c.sessions[session.uuid] = session
	return prev != len(c.sessions)
}

func (c *Controller) listSessions() SessionList {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	sessions := make(SessionList, 0, len(c.sessions))
	for _, value := range c.sessions {
		sessions = append(sessions, value)
	}
	return sessions
}

func CheckUuid(src string) bool {
	return gouuid.Parse(src).Variant() != gouuid.Invalid
}

func (c *Controller) CreateSession(uuid, name string) (*sdk.Session, error) {
	if len(uuid) == 0 {
		uuid = gouuid.New()
	} else if !CheckUuid(uuid) {
		return nil, util.NewError(http.StatusBadRequest, "invalid uuid parameter")
	}
	session := NewSession(c.log, uuid, name)
	ok := c.addSession(session)
	if !ok {
		return nil, util.NewErrorCode(http.StatusConflict)
	}
	return session.Proto(), nil
}

func (c *Controller) ReadSession(uuid string) (*sdk.Session, error) {
	session := c.FindSession(uuid)
	if session == nil {
		return nil, util.NewErrorCode(http.StatusNotFound)
	}
	return session.Proto(), nil
}

func (c *Controller) UpdateSession(uuid string, msg *sdk.Session) (*sdk.Session, error) {
	return c.ApplySession(uuid, func(session *Session) error {
		return session.Update(msg)
	})
}

func (c *Controller) DeleteSession(uuid string) error {
	c.mutex.Lock()
	session, ok := c.sessions[uuid]
	delete(c.sessions, uuid)
	c.mutex.Unlock()
	if !ok {
		return util.NewErrorCode(http.StatusNotFound)
	}
	session.Close()
	return nil
}

func (c *Controller) ListSessions() []*sdk.Session {
	return c.listSessions().Proto()
}

func (c *Controller) FindSession(uuid string) *Session {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	return c.sessions[uuid]
}

// Executes "operand" on target session, in caller context.
func (c *Controller) ApplySession(uuid string, operand func(*Session) error) (
	*sdk.Session, error) {

	session := c.FindSession(uuid)
	if session == nil {
		return nil, util.NewErrorCode(http.StatusNotFound)
	}
	err := operand(session)
	if err != nil {
		return nil, util.NewError(http.StatusInternalServerError, err.Error())
	}
	return session.Proto(), nil
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
	return c.ApplySession(uuid, func(session *Session) error {
		timer := services.NewTimer(session, begin)
		return session.Attach(name, timer)
	})
}

func (c *Controller) AttachSwordService(uuid, name string, clock bool, address string) (*sdk.Session, error) {
	return c.ApplySession(uuid, func(session *Session) error {
		sword := services.NewSword(c.log, session, clock, name, address)
		return session.Attach(name, sword)
	})
}

func (c *Controller) DetachService(uuid, name string) (*sdk.Session, error) {
	return c.ApplySession(uuid, func(session *Session) error {
		return session.Detach(name)
	})
}

func (c *Controller) StartSession(uuid string, offset int64) (*sdk.Session, error) {
	return c.ApplySession(uuid, func(session *Session) error {
		return session.Start(time.Duration(offset) * time.Millisecond)
	})
}

func (c *Controller) StopSession(uuid string) (*sdk.Session, error) {
	return c.ApplySession(uuid, func(session *Session) error {
		return session.Stop()
	})
}

func (c *Controller) CreateEvent(uuid string, msg *sdk.Event) (*sdk.Event, error) {
	session := c.FindSession(uuid)
	if session == nil {
		return nil, util.NewErrorCode(http.StatusNotFound)
	}
	return session.CreateEvent(uuid, msg)
}

func (c *Controller) ReadServices(uuid string) ([]*sdk.Service, error) {
	session := c.FindSession(uuid)
	if session == nil {
		return nil, util.NewErrorCode(http.StatusNotFound)
	}
	return session.ReadServices(), nil
}

func (c *Controller) ReadEvents(uuid string, config services.EventFilterConfig) ([]*sdk.Event, error) {
	session := c.FindSession(uuid)
	if session == nil {
		return nil, util.NewErrorCode(http.StatusNotFound)
	}
	return session.ReadEvents(config), nil
}

func (c *Controller) UpdateEvent(uuid, event string, msg *sdk.Event) (*sdk.Event, error) {
	session := c.FindSession(uuid)
	if session == nil {
		return nil, ErrUnknownSession
	}
	return session.UpdateEvent(event, msg)
}

func (c *Controller) DeleteEvent(uuid, event string) error {
	session := c.FindSession(uuid)
	if session == nil {
		return ErrUnknownSession
	}
	return session.DeleteEvent(event)
}

func (c *Controller) RegisterObserver(uuid string, config services.EventFilterConfig) (SdkObserver, error) {
	session := c.FindSession(uuid)
	if session == nil {
		return nil, ErrUnknownSession
	}
	return session.RegisterObserver(config), nil
}

func (c *Controller) UnregisterObserver(uuid string, observer SdkObserver) {
	session := c.FindSession(uuid)
	if session == nil {
		return
	}
	session.UnregisterObserver(observer)
}
