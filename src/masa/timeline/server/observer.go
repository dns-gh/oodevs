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
	"masa/timeline/services"
	"masa/timeline/util"
	"net/url"
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

func makeTriggerEvents(events []*sdk.Event) *sdk.Message {
	return &sdk.Message{
		Tag:    sdk.MessageTag_trigger_events.Enum(),
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

func (o *Observer) TriggerEvents(events ...*sdk.Event) {
	o.output <- makeTriggerEvents(events)
}

func (o *Observer) DeleteEvents(events ...string) {
	o.output <- makeDeleteEvents(events)
}

func (o *Observer) UpdateServices(services ...*sdk.Service) {
	o.output <- makeUpdateServices(services)
}

type ObserverService struct {
	uuid     string
	source   string
	observer SdkObserver
	log      util.Logger
}

func NewObserverService(uuid, source string, observer SdkObserver, log util.Logger) *ObserverService {
	return &ObserverService{
		uuid:     uuid,
		source:   source,
		observer: observer,
		log:      log,
	}
}

func (os *ObserverService) Proto(name string) *sdk.Service {
	return &sdk.Service{
		Name:  proto.String(name),
		Clock: proto.Bool(false),
		External: &sdk.External{
			Source: proto.String(os.source),
		},
	}
}

func (*ObserverService) HasClock() bool                   { return false }
func (*ObserverService) IsLocked() bool                   { return false }
func (*ObserverService) Start() error                     { return nil }
func (*ObserverService) Stop() error                      { return nil }
func (*ObserverService) AttachObserver(services.Observer) {}

func (os *ObserverService) Trigger(url url.URL, event *sdk.Event) error {
	os.log.Printf("external[%v] -> action %v\n", os.uuid, event.GetUuid())
	os.observer.TriggerEvents(event)
	return nil
}
