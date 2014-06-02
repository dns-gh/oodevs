// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package client

import (
	"code.google.com/p/goprotobuf/proto"
	. "launchpad.net/gocheck"
	"masa/timeline/sdk"
	"masa/timeline/server"
	"masa/timeline/services"
	"masa/timeline/util"
)

var (
	DummySession  = &sdk.Session{}
	DummySessions = []*sdk.Session{DummySession}
	DummyError    = &util.Error{Code: 1234}
	DummyEvent    = &sdk.Event{}
	DummyEvents   = []*sdk.Event{DummyEvent}
	DummyService  = &sdk.Service{}
	DummyServices = []*sdk.Service{DummyService}
)

type FakeHandler struct {
	name    string
	uuid    string
	clock   bool
	address string
	begin   string
	offset  int64
	event   *sdk.Event
}

func (f *FakeHandler) CreateSession(uuid, name string) (*sdk.Session, error) {
	f.uuid = uuid
	f.name = name
	return DummySession, DummyError
}

func (f *FakeHandler) ReadSession(uuid string) (*sdk.Session, error) {
	f.uuid = uuid
	return DummySession, DummyError
}

func (f *FakeHandler) UpdateSession(uuid string, next *sdk.Session) (*sdk.Session, error) {
	f.uuid = uuid
	return DummySession, DummyError
}

func (f *FakeHandler) DeleteSession(uuid string) error {
	f.uuid = uuid
	return DummyError
}

func (f *FakeHandler) ListSessions() []*sdk.Session {
	return DummySessions
}

func (f *FakeHandler) AttachTimerService(uuid, name string, begin string) (*sdk.Session, error) {
	f.uuid = uuid
	f.name = name
	f.begin = begin
	return DummySession, DummyError
}

func (f *FakeHandler) AttachSwordService(uuid, name string, clock bool, address string) (*sdk.Session, error) {
	f.uuid = uuid
	f.name = name
	f.clock = clock
	f.address = address
	return DummySession, DummyError
}

func (f *FakeHandler) DetachService(uuid, name string) (*sdk.Session, error) {
	f.uuid = uuid
	f.name = name
	return DummySession, DummyError
}

func (f *FakeHandler) StartSession(uuid string, offset int64) (*sdk.Session, error) {
	f.uuid = uuid
	f.offset = offset
	return DummySession, DummyError
}

func (f *FakeHandler) StopSession(uuid string) (*sdk.Session, error) {
	f.uuid = uuid
	return DummySession, DummyError
}

func (f *FakeHandler) CreateEvent(uuid string, event *sdk.Event) (*sdk.Event, error) {
	f.uuid = uuid
	f.event = event
	return DummyEvent, DummyError
}

func (f *FakeHandler) ReadServices(uuid string) ([]*sdk.Service, error) {
	f.uuid = uuid
	return DummyServices, DummyError
}

func (f *FakeHandler) ReadEvents(uuid string, config services.EventFilterConfig) ([]*sdk.Event, error) {
	f.uuid = uuid
	return DummyEvents, DummyError
}

func (f *FakeHandler) UpdateEvent(session, event string, msg *sdk.Event) (*sdk.Event, error) {
	f.event = msg
	return DummyEvent, DummyError
}

func (f *FakeHandler) DeleteEvent(session, event string) error {
	f.uuid = event
	return DummyError
}

func (f *FakeHandler) RegisterObserver(session string, config services.EventFilterConfig) (server.SdkObserver, error) {
	return nil, DummyError
}

func (f *FakeHandler) UnregisterObserver(session string, observer server.SdkObserver) {
}

func process(c *C, msg *sdk.ClientRequest) *FakeHandler {
	dummy := FakeHandler{}
	handler := MakeHandler(&dummy)
	err := handler.Process(msg)
	c.Assert(err, DeepEquals, DummyError)
	return &dummy
}

func (TestSuite) TestCreateSession(c *C) {
	msg := sdk.ClientRequest{
		Type: sdk.Type_SESSION_CREATE.Enum(),
		Session: &sdk.SessionRequests{
			Create: &sdk.SessionCreate{
				Uuid: proto.String("uuid"),
				Name: proto.String("name"),
			},
		},
	}
	dummy := process(c, &msg)
	c.Assert(dummy.uuid, Equals, "uuid")
	c.Assert(dummy.name, Equals, "name")
}

func (TestSuite) TestStartSession(c *C) {
	msg := sdk.ClientRequest{
		Type: sdk.Type_SESSION_START.Enum(),
		Session: &sdk.SessionRequests{
			Start: &sdk.SessionStart{
				Uuid:   proto.String("uuid"),
				Offset: proto.Int64(13),
			},
		},
	}
	dummy := process(c, &msg)
	c.Assert(dummy.uuid, Equals, "uuid")
	c.Assert(dummy.offset, Equals, int64(13))
}
