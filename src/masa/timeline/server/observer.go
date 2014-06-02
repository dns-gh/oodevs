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

func (o *Observer) UpdateTick(tick time.Time) {
	o.output <- makeUpdateTick(tick)
}

func (o *Observer) Update(events ...*sdk.Event) {
	o.output <- makeUpdateEvents(events)
}

func (o *Observer) Delete(events ...string) {
	o.output <- makeDeleteEvents(events)
}
