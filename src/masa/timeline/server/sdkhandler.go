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
	"masa/timeline/sdk"
	"masa/timeline/services"
)

type SdkObserver interface {
	Listen() <-chan interface{}
}

type SdkController interface {
	// sessions
	CreateSession(uuid, name string, autostart bool) (*sdk.Session, error)
	ReadSession(uuid string) (*sdk.Session, error)
	UpdateSession(uuid string, msg *sdk.Session) (*sdk.Session, error)
	DeleteSession(uuid string) error
	ListSessions() []*sdk.Session
	StartSession(uuid string, offset int64) (*sdk.Session, error)
	StopSession(uuid string) (*sdk.Session, error)
	// services
	AttachTimerService(uuid, name string, base string) (*sdk.Session, error)
	AttachSwordService(uuid, name string, clock bool, address string) (*sdk.Session, error)
	DetachService(uuid, name string) (*sdk.Session, error)
	ReadServices(uuid string) ([]*sdk.Service, error)
	// events
	CreateEvent(uuid string, msg *sdk.Event) (*sdk.Event, error)
	ReadEvents(uuid string, config services.EventFilterConfig) ([]*sdk.Event, error)
	UpdateEvent(session, event string, msg *sdk.Event) (*sdk.Event, error)
	DeleteEvent(session, event string) error
	// observer
	RegisterObserver(session string, config services.EventFilterConfig) (SdkObserver, error)
	UnregisterObserver(session string, observer SdkObserver)
}
