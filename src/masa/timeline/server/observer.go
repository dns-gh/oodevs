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
	"masa/timeline/sdk"
	"masa/timeline/util"
	"time"
)

type Observer struct {
	output chan interface{}
}

func NewObserver() *Observer {
	return &Observer{
		output: make(chan interface{}, 16),
	}
}

func (o *Observer) Listen() <-chan interface{} {
	return o.output
}

func (o *Observer) Close() {
	close(o.output)
}

func makeUpdateSession(session *sdk.Session) *sdk.Message {
	return &sdk.Message{
		Tag:     sdk.MessageTag_update_session.Enum(),
		Session: session,
	}
}

func makeUpdateTick(tick time.Time) *sdk.Message {
	return &sdk.Message{
		Tag:  sdk.MessageTag_update_tick.Enum(),
		Tick: proto.String(util.FormatTime(tick)),
	}
}

func makeUpdateEvents(events []*sdk.Event) *sdk.Message {
	return &sdk.Message{
		Tag:    sdk.MessageTag_update_events.Enum(),
		Events: events,
	}
}

func makeDeleteEvents(events []string) *sdk.Message {
	return &sdk.Message{
		Tag:   sdk.MessageTag_delete_events.Enum(),
		Uuids: events,
	}
}

func makeUpdateServices(services []*sdk.Service) *sdk.Message {
	return &sdk.Message{
		Tag:      sdk.MessageTag_update_services.Enum(),
		Services: services,
	}
}

func (o *Observer) UpdateTick(tick time.Time) {
	o.output <- makeUpdateTick(tick)
}

func (o *Observer) UpdateSession(session *sdk.Session) {
	o.output <- makeUpdateSession(session)
}

func (o *Observer) UpdateEvents(events ...*sdk.Event) {
	o.output <- makeUpdateEvents(events)
}

func (o *Observer) DeleteEvents(events ...string) {
	o.output <- makeDeleteEvents(events)
}

func (o *Observer) UpdateServices(services ...*sdk.Service) {
	o.output <- makeUpdateServices(services)
}
