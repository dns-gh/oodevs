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
	"masa/timeline/sdk"
	"net/url"
	"time"
)

type Observer interface {
	// abort service with error <err>
	Abort(err error)
	// update current time with <tick>
	Tick(tick time.Time)
	// close action <uuid> with error <err> and set as read-only if <lock>
	CloseEvent(uuid string, err error, lock bool)
	// update event <uuid> with <event> data
	UpdateEvent(uuid string, event *sdk.Event)
	// invalid all active filters
	InvalidateFilters()
	// post command to observer loop
	Post(operand func())
}

type Service interface {
	Proto(name string) *sdk.Service // convert service to protobuf
	HasClock() bool                 // whether we use the service clock
	IsLocked() bool                 // whether service clock is immutable
	Start() error
	Stop() error
	Apply(uuid string, url url.URL, payload []byte) error
}

type EventListener interface {
	Update(events ...*sdk.Event) // create & update are merged
	Delete(events ...string)
}

type EventFilter func(event *sdk.Event) bool
type EventFilterConfig map[string]interface{}

type EventFilterer interface {
	GetFilters(config EventFilterConfig) []EventFilter
	// for single event filtering only
	Filter(event *sdk.Event, config EventFilterConfig) bool
}
