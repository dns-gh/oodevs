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
	"code.google.com/p/go-uuid/uuid"
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
	"errors"
	"fmt"
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/sword/swtest"
	"masa/timeline/sdk"
	"masa/timeline/services"
	"masa/timeline/swfake"
	"masa/timeline/util"
	"math/rand"
	"net/url"
	"sync"
	"time"
)

type Fixture struct {
	server     *swfake.SwordServer
	sword      *services.Sword
	controller *Controller
	observer   ControllerObserver
	session    string
	begin      time.Time
	log        util.Logger
}

func (f *Fixture) Close() {
	f.controller.DeleteSession(f.session)
	f.server.Close()
}

func (f *Fixture) Tick() {
	f.begin = f.begin.Add(1 * time.Second)
	f.observer.Tick(f.begin)
}

func (f *Fixture) Rewind() {
	f.begin = f.begin.Add(-1 * time.Second)
	f.observer.Tick(f.begin)
}

func (f *Fixture) WaitConnected() {
	for {
		detached := f.server.GetLinks()
		done := len(detached) > 0
		detached.Close()
		if done {
			return
		}
	}
}

type ModuleLogger struct {
	log  util.Logger
	name string
}

func (m *ModuleLogger) Printf(format string, args ...interface{}) {
	m.log.Printf(m.name+": "+format, args...)
}

func (t *TestSuite) MakeFixture(c *C, logins bool) *Fixture {
	local := fmt.Sprintf("localhost:%v", 1+t.port)
	log := swtest.MakeGocheckLogger(c)
	server, err := swfake.NewSwordServer(&ModuleLogger{log: log, name: "swfake"}, local, false, false)
	c.Assert(err, IsNil)
	server.EnableLogins(logins)
	controller := MakeController(log)
	id := uuid.New()
	_, err = controller.CreateSession(id, "some_name", true)
	c.Assert(err, IsNil)
	ctx := controller.sessions[id]
	observer := ControllerObserver{id, controller}
	_, err = controller.AttachSwordService(id, "some_name", true, local)
	c.Assert(err, IsNil)
	sword := ctx.session.services["some_name"].(*services.Sword)
	_, err = controller.StartSession(id, 0)
	c.Assert(err, IsNil)
	f := &Fixture{server, sword, controller, observer, id, time.Now(), log}
	f.Tick()
	return f
}

func (f *Fixture) createFullEvent(uuid, name, protocol, target, parent, metadata string, payload []byte, begin, end time.Time) (*sdk.Event, error) {
	msg := sdk.Event{
		Uuid:     proto.String(uuid),
		Name:     proto.String(name),
		Begin:    proto.String(util.FormatTime(begin)),
		Parent:   proto.String(parent),
		Metadata: proto.String(metadata),
		Action: &sdk.Action{
			Target:  proto.String(protocol + "://" + target),
			Apply:   proto.Bool(true),
			Payload: payload,
		},
	}
	if !end.IsZero() {
		msg.End = proto.String(util.FormatTime(end))
	}
	return f.controller.UpdateEvent(f.session, uuid, &msg)
}

func (f *Fixture) createEvent(uuid, name, protocol, target string, payload []byte) (*sdk.Event, error) {
	return f.createFullEvent(uuid, name, protocol, target, "", "", payload, f.begin, time.Time{})
}

func (f *Fixture) addEvent(c *C, name, protocol, target string, payload []byte) string {
	uuid := uuid.New()
	_, err := f.createEvent(uuid, name, protocol, target, payload)
	c.Assert(err, IsNil)
	return uuid
}

func (f *Fixture) addSwordEvent(c *C, name, target string, payload []byte) string {
	return f.addEvent(c, name, "sword", target, payload)
}

func (f *Fixture) addMarkerEvent(c *C, name, target string, payload []byte) string {
	return f.addEvent(c, name, "marker", target, payload)
}

func (f *Fixture) addTaskEvent(c *C, uuid string, begin, end time.Time, metadata string) *sdk.Event {
	msg, err := f.createFullEvent(uuid, uuid, "sword", "", "", metadata, []byte{}, begin, end)
	c.Assert(err, IsNil)
	c.Assert(msg, NotNil)
	return msg
}

func (f *Fixture) addChildEvent(c *C, uuid, parent string, begin time.Time) *sdk.Event {
	msg, err := f.createFullEvent(uuid, uuid, "sword", "", parent, "", []byte{}, begin, time.Time{})
	c.Assert(err, IsNil)
	c.Assert(msg, NotNil)
	return msg
}

func (t *TestSuite) TestDeadlockOnActionTargetMismatch(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	f.addSwordEvent(c, "some_name", "mismatch", []byte{})
	// after last tick, session will try to run our event
	// which will deadlock due to url mismatch
	f.Tick()
}

func (f *Fixture) getSomeUnitOrder(c *C, id, parentId uint32) []byte {
	bytes, err := proto.Marshal(&sword.ClientToSim{
		Message: &sword.ClientToSim_Content{
			UnitOrder: &sword.UnitOrder{
				Tasker: swapi.MakeId(id),
				Type:   swapi.MakeId(1337),
				Parent: proto.Uint32(parentId),
			},
		},
	})
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeAutomatOrder(c *C, id uint32) []byte {
	bytes, err := proto.Marshal(&sword.ClientToSim{
		Message: &sword.ClientToSim_Content{
			AutomatOrder: &sword.AutomatOrder{
				Tasker: swapi.MakeId(id),
				Type:   swapi.MakeId(1337),
			},
		},
	})
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeCrowdOrder(c *C, id uint32) []byte {
	bytes, err := proto.Marshal(&sword.ClientToSim{
		Message: &sword.ClientToSim_Content{
			CrowdOrder: &sword.CrowdOrder{
				Tasker: swapi.MakeId(id),
				Type:   swapi.MakeId(1337),
			},
		},
	})
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeFragOrder(c *C, tasker *sword.Tasker) []byte {
	bytes, err := proto.Marshal(&sword.ClientToSim{
		Message: &sword.ClientToSim_Content{
			FragOrder: &sword.FragOrder{
				Tasker: tasker,
				Type:   swapi.MakeId(1337),
			},
		},
	})
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeMagicAction(c *C) []byte {
	bytes, err := proto.Marshal(swapi.CreateMagicAction(&sword.MissionParameters{}, sword.MagicAction_global_weather).ClientToSimulation)
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeUnitMagicAction(c *C, tasker *sword.Tasker) []byte {
	bytes, err := proto.Marshal(swapi.CreateUnitMagicAction(tasker, &sword.MissionParameters{}, sword.UnitMagicAction_move_to).ClientToSimulation)
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeClientUnitMagicAction(c *C, tasker *sword.Tasker) []byte {
	bytes, err := proto.Marshal(swapi.CreateUnitMagicAction(tasker, &sword.MissionParameters{}, sword.UnitMagicAction_unit_change_superior).ClientToSimulation)
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeObjectMagicAction(c *C, id uint32) []byte {
	bytes, err := proto.Marshal(swapi.CreateObjectMagicAction(id, &sword.MissionParameters{}, sword.ObjectMagicAction_create).ClientToSimulation)
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeKnowledgeMagicAction(c *C, id uint32) []byte {
	bytes, err := proto.Marshal(swapi.CreateKnowledgeMagicActionMessage(&sword.MissionParameters{}, id, sword.KnowledgeMagicAction_enable).ClientToSimulation)
	c.Assert(err, IsNil)
	return bytes
}

func (f *Fixture) getSomeAutomatModeMagicAction(c *C, id uint32) []byte {
	bytes, err := proto.Marshal(&sword.ClientToSim{
		Message: &sword.ClientToSim_Content{
			SetAutomatMode: &sword.SetAutomatMode{
				Automate: swapi.MakeId(id),
				Mode:     sword.EnumAutomatMode_engaged.Enum(),
			},
		},
	})
	c.Assert(err, IsNil)
	return bytes
}

func isDone(e *sdk.Event) bool {
	return e.GetDone() ||
		e.GetErrorCode() != 0 ||
		len(e.GetErrorText()) > 0
}

func (f *Fixture) waitFor(c *C, operand func(e *sdk.Event) bool, uuids ...string) {
	set := map[string]struct{}{}
	for _, id := range uuids {
		set[id] = struct{}{}
	}
	deadline := time.Now().Add(1 * time.Minute)
	for time.Now().Before(deadline) {
		events, err := f.controller.ReadEvents(f.session, services.EventFilterConfig{})
		c.Assert(err, IsNil)
		for _, evt := range events {
			if operand(evt) {
				delete(set, evt.GetUuid())
			}
		}
		if len(set) == 0 {
			return
		}
		time.Sleep(100 * time.Millisecond)
	}
	c.Fatal("timeout")
}

func (f *Fixture) waitAllDone(c *C, uuids ...string) {
	f.waitFor(c, func(e *sdk.Event) bool {
		return e.GetDone()
	}, uuids...)
}

func (f *Fixture) waitAllDoneOrDead(c *C, uuids ...string) {
	f.waitFor(c, func(e *sdk.Event) bool {
		return isDone(e)
	}, uuids...)
}

func checkDuplicateOrders(c *C, f *Fixture) int {
	unit := f.addSwordEvent(c, "unit", "some_name", f.getSomeUnitOrder(c, 73, 0))
	automat := f.addSwordEvent(c, "automat", "some_name", f.getSomeAutomatOrder(c, 1))
	crowd := f.addSwordEvent(c, "crows", "some_name", f.getSomeCrowdOrder(c, 1))
	frag := f.addSwordEvent(c, "frag", "some_name", f.getSomeFragOrder(c, swapi.MakeUnitTasker(1337)))
	magic := f.addSwordEvent(c, "magic", "some_name", f.getSomeMagicAction(c))
	umagic := f.addSwordEvent(c, "unit magic", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeUnitTasker(1337)))
	omagic := f.addSwordEvent(c, "object magic", "some_name", f.getSomeObjectMagicAction(c, 1))
	kmagic := f.addSwordEvent(c, "knowledge magic", "some_name", f.getSomeKnowledgeMagicAction(c, 2))
	amagic := f.addSwordEvent(c, "automat mode", "some_name", f.getSomeAutomatModeMagicAction(c, 3))
	f.Tick()
	f.waitAllDone(c, unit, automat, crowd, frag, magic, umagic, omagic, kmagic, amagic)
	num := 9
	events, err := f.controller.ReadEvents(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	c.Assert(events, HasLen, num)
	return num
}

func (t *TestSuite) TestDuplicateOrders(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	checkDuplicateOrders(c, f)
}

func (t *TestSuite) TestMissingServerSideOrders(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	f.WaitConnected()
	detached := f.server.GetLinks()
	defer detached.Close()
	for slink := range detached {
		f.server.WriteToClient(slink, 1, 0, &sword.SimToClient_Content{
			OrderAck: &sword.TaskCreationRequestAck{
				Tasker:    swapi.MakeUnitTasker(17),
				ErrorCode: sword.OrderAck_no_error.Enum(),
				Id:        proto.Uint32(13),
			},
		})
		order := &sword.UnitOrder{
			Tasker: swapi.MakeId(17),
			Type:   swapi.MakeId(23),
			Id:     proto.Uint32(13),
		}
		f.server.WriteToClient(slink, 1, 0, &sword.SimToClient_Content{
			UnitOrder: order,
		})
		f.server.WriteToClient(slink, 1, 0, &sword.SimToClient_Content{
			Action: &sword.Action{
				Id:        proto.Uint32(order.GetId() * 1000),
				UnitOrder: order,
			},
		})
	}
	deadline := time.Now().Add(1 * time.Minute)
	for time.Now().Before(deadline) {
		events, err := f.controller.ReadEvents(f.session, services.EventFilterConfig{})
		c.Assert(err, IsNil)
		if len(events) > 0 {
			return
		}
		time.Sleep(100 * time.Millisecond)
	}
	c.Fatal("timeout")
}

// Push "order" multiple times with different context to every connected
// client. Used to test timeline behaviour with duplicates.
func (f *Fixture) spamOrder(c *C, orders ...*sword.SimToClient_Content) {
	detached := f.server.GetLinks()
	defer detached.Close()
	count := int32(len(orders))
	for slink := range detached {
		for i := int32(0); i < 10; i++ {
			for j, order := range orders {
				f.server.WriteToClient(slink, count*i+int32(j), slink.GetId(), order)
			}
		}
	}
}

// Push a custom event, wait for it to be processed by the timeline and
// check the event count in timeline model, with no filtering, equals "n". The
// pushed event is used like a synchronization barrier. It is also created
// in timeline model and must be considered when setting "n".
func (f *Fixture) addEventAndSync(c *C, n int) {
	unit := f.addSwordEvent(c, "unit", "some_name", f.getSomeUnitOrder(c, 77, 0))
	f.Tick()
	f.waitAllDone(c, unit)
	events, err := f.controller.ReadEvents(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	c.Assert(events, HasLen, n)
}

func (f *Fixture) killLinks() {
	detached := f.server.GetLinks()
	defer detached.Close()
	for slink := range detached {
		slink.Fatal(errors.New("die!"))
	}
}

func (t *TestSuite) TestServerSideDuplicateOrders(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	order := sword.SimToClient_Content{
		UnitOrder: &sword.UnitOrder{
			Tasker: swapi.MakeId(17),
			Type:   swapi.MakeId(23),
			Parent: proto.Uint32(31),
			Id:     proto.Uint32(1337),
		},
	}
	action := sword.SimToClient_Content{
		Action: &sword.Action{
			Id:        proto.Uint32(order.UnitOrder.GetId() * 1000),
			UnitOrder: order.UnitOrder,
		},
	}
	magic := sword.SimToClient_Content{
		Action: &sword.Action{
			Id: proto.Uint32(17),
			UnitMagicAction: &sword.UnitMagicAction{
				Tasker:     swapi.MakeUnitTasker(17),
				Type:       proto.Int32(int32(sword.UnitMagicAction_move_to)),
				Parameters: swapi.MakeParameters(),
			},
		},
	}
	f.WaitConnected()
	f.spamOrder(c, &order, &action)
	// Two events are expected: order and the one pushed by addEventAndSync
	f.addEventAndSync(c, 2)
	f.spamOrder(c, &magic)
	f.addEventAndSync(c, 4)
	f.killLinks()
	// order is already in the model, only the event created by addEventAndSync
	// increases the event count.
	f.addEventAndSync(c, 5)
	f.spamOrder(c, &order, &action)
	f.addEventAndSync(c, 6)
	f.spamOrder(c, &magic)
	f.addEventAndSync(c, 7)
}

func compareTime(a, b time.Time) bool {
	duration := a.Sub(b)
	if duration < 0 {
		duration = -duration
	}
	return duration < 5*time.Minute
}

func (t *TestSuite) TestTriggerEvent(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	uuid := uuid.New()
	f.begin = time.Now().Add(24 * time.Hour)
	target := "external_service"
	event, err := f.createEvent(uuid, "some_event", "sword", target, f.getSomeUnitOrder(c, 1, 0))
	c.Assert(err, IsNil)

	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	_, err = f.controller.AttachService(f.session, target, NewObserverService(target, "some_url", link, f.log))
	c.Assert(err, IsNil)
	defer f.controller.DetachService(f.session, target)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()

	event.Done = proto.Bool(true)
	_, err = f.controller.UpdateEvent(f.session, uuid, event)
	c.Assert(err, IsNil)

	f.waitAllDone(c, uuid)
	data, err := f.controller.ReadEvents(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	c.Assert(data, HasLen, 1)
	begin, err := util.ParseTime(data[0].GetBegin())
	c.Assert(err, IsNil)
	c.Assert(compareTime(begin, time.Now()), Equals, true)

	msg := waitBroadcastTag(messages, sdk.MessageTag_trigger_events)
	event.Begin = proto.String(util.FormatTime(begin))
	swtest.DeepEquals(c, msg, &sdk.Message{
		Tag: sdk.MessageTag_trigger_events.Enum(),
		Events: []*sdk.Event{
			event,
		},
	})
}

func waitBroadcastTag(messages <-chan interface{}, tag sdk.MessageTag) *sdk.Message {
	deadline := time.After(2 * time.Second)
	for {
		select {
		case <-deadline:
			return nil
		case value := <-messages:
			msg := value.(*sdk.Message)
			if msg.GetTag() == tag {
				return msg
			}
		}
	}
}

type FakeService struct {
	lock     sync.Mutex
	events   []*sdk.Event
	uuids    []string
	services []*sdk.Service
}

func (f *FakeService) Proto(name string) *sdk.Service {
	return &sdk.Service{
		Name:  proto.String(name),
		Clock: proto.Bool(false),
	}
}

func (f *FakeService) HasClock() bool                            { return false }
func (f *FakeService) IsLocked() bool                            { return false }
func (f *FakeService) AttachObserver(observer services.Observer) {}
func (f *FakeService) Start() error                              { return nil }
func (f *FakeService) Stop() error                               { return nil }
func (f *FakeService) Trigger(url.URL, *sdk.Event) error         { return nil }
func (f *FakeService) CheckEvent(event *sdk.Event) error         { return nil }
func (f *FakeService) CheckDeleteEvent(uuid string) error        { return nil }
func (f *FakeService) UpdateTick(time.Time)                      {}

func (f *FakeService) UpdateEvents(events ...*sdk.Event) {
	f.lock.Lock()
	defer f.lock.Unlock()
	f.events = events
}

func (f *FakeService) DeleteEvents(uuids ...string) {
	f.lock.Lock()
	defer f.lock.Unlock()
	f.uuids = uuids
}

func (f *FakeService) UpdateServices(services ...*sdk.Service) {
	f.lock.Lock()
	defer f.lock.Unlock()
	f.services = services
}

func (f *FakeService) GetAndClear() ([]*sdk.Event, []string) {
	f.lock.Lock()
	defer f.lock.Unlock()
	events := f.events
	f.events = nil
	uuids := f.uuids
	f.uuids = nil
	return events, uuids
}

func (f *Fixture) checkBroadcastEvents(c *C, messages <-chan interface{}, name string) *sdk.Event {
	event, err := f.createEvent(uuid.New(), "something", "sword", name, f.getSomeUnitOrder(c, 1, 0))
	c.Assert(err, IsNil)
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, msg.Events, []*sdk.Event{event})
	return event
}

func checkEmptyUpdate(c *C, messages <-chan interface{}) {
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, msg.Events, []*sdk.Event{})
}

func (t *TestSuite) TestListeners(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	faker := FakeService{}
	_, err := f.controller.apply(f.session, func(session *Session) (interface{}, error) {
		err := session.AttachService("faker", &faker)
		if err != nil {
			return nil, err
		}
		session.AttachListener(&faker)
		return nil, nil
	})
	c.Assert(err, IsNil)

	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	checkEmptyUpdate(c, messages)

	// test create event
	event := f.checkBroadcastEvents(c, messages, "some_name")
	fakerEvents, _ := faker.GetAndClear()
	swtest.DeepEquals(c, fakerEvents, []*sdk.Event{event})

	// test update event
	event.Name = proto.String("new_name")
	uuid := event.GetUuid()
	updated, err := f.controller.UpdateEvent(f.session, uuid, event)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, updated, event)
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	fakerEvents, _ = faker.GetAndClear()
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, msg.Events, []*sdk.Event{event})
	swtest.DeepEquals(c, fakerEvents, []*sdk.Event{event})

	// test delete event
	err = f.controller.DeleteEvent(f.session, uuid)
	c.Assert(err, IsNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_delete_events)
	_, fakerUuids := faker.GetAndClear()
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, msg.Uuids, []string{uuid})
	swtest.DeepEquals(c, fakerUuids, []string{uuid})

	// test detach
	_, err = f.controller.StopSession(f.session)
	c.Assert(err, IsNil)
	_, err = f.controller.DetachService(f.session, "faker")
	c.Assert(err, IsNil)
	f.checkBroadcastEvents(c, messages, "another_name")
	_, fakerUuids = faker.GetAndClear()
	c.Assert(fakerUuids, IsNil)
}

func checkIsReplayEvent(c *C, event *sdk.Event) {
	c.Assert(event.GetAction().GetTarget(), Equals, "replay://some_name")
}

func (t *TestSuite) TestObservers(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.sword.WaitForStatus(services.SwordStatusConnected)
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_services)
	c.Assert(msg, NotNil)
	expected := f.sword.Proto("some_name")
	c.Assert(expected.Sword.GetHasReplay(), Equals, false)
	swtest.DeepEquals(c, msg.Services, []*sdk.Service{expected})

	// Timeline server reads the service description, looking for
	// the replay service, and updates the sword service it exposes
	// to its clients
	detached := f.server.GetLinks()
	defer detached.Close()
	for slink := range detached {
		f.server.WriteDispatcherToClient(slink, 1, 0,
			&sword.DispatcherToClient_Content{
				ServicesDescription: &sword.ServicesDescription{
					Services: []string{"struct replay::Service"},
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_services)
	c.Assert(msg, NotNil)
	expected.Sword.HasReplay = proto.Bool(true)
	swtest.DeepEquals(c, msg.Services, []*sdk.Service{expected})

	// When replayer sends a control replay information message,
	// timeline server updates its session with the begin/end replay time
	expectedStart := time.Now().Truncate(time.Second).UTC()
	expectedEnd := expectedStart.Add(60 * time.Second)
	for slink := range detached {
		f.server.WriteReplayToClient(slink, 1, 0,
			&sword.ReplayToClient_Content{
				ControlReplayInformation: &sword.ControlReplayInformation{
					CurrentTick:     proto.Int32(0),
					InitialDateTime: swapi.MakeDateTime(expectedStart),
					EndDateTime:     swapi.MakeDateTime(expectedEnd),
					DateTime:        swapi.MakeDateTime(expectedStart),
					TickDuration:    proto.Int32(10),
					TimeFactor:      proto.Int32(1),
					Status:          sword.EnumSimulationState_running.Enum(),
					TickCount:       proto.Int32(6),
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_session)
	c.Assert(msg, NotNil)
	start, err := util.ParseTime(msg.Session.GetStartTime())
	c.Assert(err, IsNil)
	c.Assert(start, Equals, expectedStart)
	end, err := util.ParseTime(msg.Session.GetEndTime())
	c.Assert(err, IsNil)
	c.Assert(end, Equals, expectedEnd)
	c.Assert(start.Before(end), Equals, true)
	// Timeline server creates a special range event, with the
	// replay start and end dates. This event is tagged as replay
	// in the action's target (replay://).
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	c.Assert(len(msg.GetEvents()), Equals, 1)
	event := msg.GetEvents()[0]
	checkIsReplayEvent(c, event)
	c.Assert(string(event.Action.GetPayload()), Equals, `{"begin":false,"end":false,"enabled":true}`)
	start, err = util.ParseTime(event.GetBegin())
	c.Assert(err, IsNil)
	c.Assert(start, Equals, expectedStart)
	end, err = util.ParseTime(event.GetEnd())
	c.Assert(err, IsNil)
	c.Assert(end, Equals, expectedEnd)
}

var (
	hierarchyIds uint32 = 0
)

func addHierarchy(d *swapi.ModelData, id, parent uint32) {
	hierarchyIds++
	d.Hierarchies[id] = swapi.Hierarchies{
		swapi.Hierarchy{
			Id:     hierarchyIds,
			Entity: id,
			Parent: parent,
			Tick:   0,
		},
	}
}

func addParty(c *C, d *swapi.ModelData, id uint32) {
	d.Parties[id] = &swapi.Party{
		Id: id,
	}
	addHierarchy(d, id, 0)
}

func addFormation(c *C, d *swapi.ModelData, id, party, parent uint32) {
	d.Formations[id] = &swapi.Formation{
		Id:       id,
		PartyId:  party,
		ParentId: parent,
	}
	if parent == 0 {
		parent = party
	}
	addHierarchy(d, id, parent)
}

func addAutomat(c *C, d *swapi.ModelData, id, formation uint32) {
	d.Automats[id] = &swapi.Automat{
		Id:          id,
		FormationId: formation,
	}
	addHierarchy(d, id, formation)
}

func addUnit(c *C, d *swapi.ModelData, id, automat uint32) {
	d.Units[id] = &swapi.Unit{
		Id:        id,
		AutomatId: automat,
	}
	addHierarchy(d, id, automat)
}

func addCrowd(c *C, d *swapi.ModelData, id, party uint32) {
	d.Crowds[id] = &swapi.Crowd{
		Id:      id,
		PartyId: party,
	}
	addHierarchy(d, id, party)
}

func addPopulation(c *C, d *swapi.ModelData, id, party uint32) {
	d.Populations[id] = &swapi.Population{
		Id:      id,
		PartyId: party,
	}
	addHierarchy(d, id, party)
}

func addObject(c *C, d *swapi.ModelData, id, party uint32) {
	d.Objects[id] = &swapi.Object{
		Id:      id,
		PartyId: party,
	}
	addHierarchy(d, id, party)
}

func addKnowledgeGroup(c *C, d *swapi.ModelData, id, party uint32) {
	d.KnowledgeGroups[id] = &swapi.KnowledgeGroup{
		Id:      id,
		PartyId: party,
	}
	addHierarchy(d, id, party)
}

func (f *Fixture) findEvent(c *C, uuid string) *sdk.Event {
	events, err := f.controller.ReadEvents(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	for _, event := range events {
		if event.GetUuid() == uuid {
			return event
		}
	}
	c.Fatal("missing event " + uuid)
	return nil
}

func (f *Fixture) testAckWith(c *C, err *swfake.Error, id string) {
	f.Tick()
	if err != nil {
		f.waitAllDoneOrDead(c, id)
	} else {
		f.waitAllDone(c, id)
	}
	event := f.findEvent(c, id)
	c.Assert(event, NotNil)
	code, text := event.GetErrorCode(), event.GetErrorText()
	if err == nil {
		c.Assert(code, Equals, int32(0))
		c.Assert(text, Equals, "")
	} else {
		c.Assert(code, Equals, int32(err.Code))
		c.Assert(text, Equals, err.Text)
	}
}

func (f *Fixture) testAck(c *C, err *swfake.Error, order []byte) {
	f.server.SetLastError(err)
	id := f.addSwordEvent(c, "order", "some_name", order)
	f.testAckWith(c, err, id)
}

func (t *TestSuite) TestOrderAckErrors(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	f.WaitConnected()
	f.testAck(c, nil, f.getSomeUnitOrder(c, 91, 0))
	err := swfake.Error{Code: int32(sword.OrderAck_error_invalid_unit), Text: "random junk"}
	f.testAck(c, &err, f.getSomeUnitOrder(c, 92, 0))
	err = swfake.Error{Code: int32(sword.OrderAck_error_invalid_limit), Text: "random frag junk"}
	f.testAck(c, &err, f.getSomeFragOrder(c, swapi.MakeUnitTasker(0)))
	// magic orders
	f.testAck(c, nil, f.getSomeMagicAction(c))
	err = swfake.Error{Code: int32(sword.MagicActionAck_error_invalid_parameter), Text: "random junk"}
	f.testAck(c, &err, f.getSomeMagicAction(c))
}

func (f *Fixture) resetEvents(c *C, valid bool) {
	events, err := f.controller.ReadEvents(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	for _, evt := range events {
		if valid {
			evt.Done = proto.Bool(false)
		} else {
			evt.ErrorCode = nil
			evt.ErrorText = nil
		}
		_, err := f.controller.UpdateEvent(f.session, evt.GetUuid(), evt)
		c.Assert(err, IsNil)
	}
}

func (t *TestSuite) TestBrokenOrdersAreSentForever(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	errors := []sword.OrderAck_ErrorCode{
		sword.OrderAck_error_invalid_unit,
		sword.OrderAck_error_invalid_limit,
		sword.OrderAck_error_invalid_lima,
	}
	id := f.addSwordEvent(c, "order", "some_name", f.getSomeUnitOrder(c, 93, 0))
	for _, v := range errors {
		f.resetEvents(c, false) // ensure previous error is clean
		err := &swfake.Error{Code: int32(v), Text: v.String()}
		f.server.SetLastError(err)
		f.testAckWith(c, err, id)
		f.Rewind()
	}
}

func (t *TestSuite) TestUncheckedOrdersAreResent(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	id := f.addSwordEvent(c, "order", "some_name", f.getSomeUnitOrder(c, 94, 0))
	for i := 0; i < 4; i++ {
		f.resetEvents(c, true)
		f.testAckWith(c, nil, id)
		f.Rewind()
	}
}

func (t *TestSuite) TestSwordReconnection(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	num := checkDuplicateOrders(c, f)
	for i := 0; i < 10; i++ {
		f.killLinks()
		// from an external point of view, we can still send orders
		unit := f.addSwordEvent(c, "unit", "some_name", f.getSomeUnitOrder(c, 95, 0))
		f.Tick()
		f.waitAllDone(c, unit)
		events, err := f.controller.ReadEvents(f.session, services.EventFilterConfig{})
		c.Assert(err, IsNil)
		c.Assert(events, HasLen, num+i+1)
	}
}

func (t *TestSuite) flushPendingActions(c *C, afterClose, afterTick, preStop bool) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	checkDuplicateOrders(c, f)
	// close the server permanently
	f.server.Close()
	if afterClose {
		time.Sleep(250 * time.Millisecond)
	}
	unit := f.addSwordEvent(c, "argh", "some_name", f.getSomeUnitOrder(c, 96, 0))
	f.Tick()
	if afterTick {
		// poor way to wait for restart loops to begin
		time.Sleep(250 * time.Millisecond)
	}
	wgroup := sync.WaitGroup{}
	wgroup.Add(1)
	go func() {
		if preStop {
			time.Sleep(250 * time.Millisecond)
		}
		// eventually we'll stop the session
		defer wgroup.Done()
		f.controller.StopSession(f.session)
	}()
	// and eventually we'll have all events done or dead
	f.waitAllDoneOrDead(c, unit)
	wgroup.Wait()
}

func (t *TestSuite) TestSwordStopReconnectFlushPendingActions(c *C) {
	t.flushPendingActions(c, true, false, false)
	t.flushPendingActions(c, true, true, false)
	t.flushPendingActions(c, true, false, true)
	t.flushPendingActions(c, true, true, true)
	t.flushPendingActions(c, false, false, false)
	t.flushPendingActions(c, false, true, false)
	t.flushPendingActions(c, false, false, true)
	t.flushPendingActions(c, false, true, true)
}

func (t *TestSuite) TestCreateEventUpdatesParent(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	checkEmptyUpdate(c, messages)

	// test create parent
	parent := f.addTaskEvent(c, "parent", f.begin, f.begin.Add(1*time.Hour), "")
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, msg.Events, []*sdk.Event{parent})

	// test create child, no parent update
	child1 := f.addChildEvent(c, "child1", "parent", f.begin.Add(10*time.Minute))
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, msg.Events, []*sdk.Event{child1})

	// test create child, need parent update
	child2 := f.addChildEvent(c, "child2", "parent", f.begin.Add(2*time.Hour))
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	parent.End = proto.String(util.FormatTime(f.begin.Add(2 * time.Hour)))
	swtest.DeepEquals(c, msg.Events, []*sdk.Event{child2, parent})
}

func (t *TestSuite) TestUpdateEventUpdatesParent(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	checkEmptyUpdate(c, messages)

	// create 1 parent and 1 child
	parent := f.addTaskEvent(c, "parent", f.begin, f.begin.Add(1*time.Hour), "")
	waitBroadcastTag(messages, sdk.MessageTag_update_events)
	child := f.addChildEvent(c, "child", "parent", f.begin.Add(1*time.Minute))
	waitBroadcastTag(messages, sdk.MessageTag_update_events)

	// test update child updates parent
	child.Begin = proto.String(util.FormatTime(f.begin.Add(2 * time.Hour)))
	newChild, err := f.controller.UpdateEvent(f.session, "child", child)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, newChild, child)

	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	parent.End = proto.String(util.FormatTime(f.begin.Add(2 * time.Hour)))
	swtest.DeepEquals(c, msg.Events, []*sdk.Event{child, parent})
}

func (t *TestSuite) TestUpdateEventUpdatesChildren(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	checkEmptyUpdate(c, messages)

	// create 1 parent and 2 children
	parent := f.addTaskEvent(c, "parent", f.begin, f.begin.Add(1*time.Hour), "")
	waitBroadcastTag(messages, sdk.MessageTag_update_events)
	child1 := f.addChildEvent(c, "child1", "parent", f.begin.Add(1*time.Minute))
	waitBroadcastTag(messages, sdk.MessageTag_update_events)
	child2 := f.addChildEvent(c, "child2", "parent", f.begin.Add(2*time.Minute))
	waitBroadcastTag(messages, sdk.MessageTag_update_events)

	// test update parent updates children
	parent.Begin = proto.String(util.FormatTime(f.begin.Add(5 * time.Minute)))
	parent.End = proto.String(util.FormatTime(f.begin.Add(1*time.Hour + 5*time.Minute)))
	newParent, err := f.controller.UpdateEvent(f.session, "parent", parent)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, newParent, parent)
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	child1.Begin = proto.String(util.FormatTime(f.begin.Add(1*time.Minute + 5*time.Minute)))
	child2.Begin = proto.String(util.FormatTime(f.begin.Add(2*time.Minute + 5*time.Minute)))
	if len(msg.Events) == 3 && msg.Events[1].GetUuid() != child1.GetUuid() {
		child1, child2 = child2, child1
	}
	swtest.DeepEquals(c, msg.Events, []*sdk.Event{parent, child1, child2})
}

func mapEvents(events ...*sdk.Event) map[string]*sdk.Event {
	dst := map[string]*sdk.Event{}
	for _, e := range events {
		dst[e.GetUuid()] = e
	}
	return dst
}

func (t *TestSuite) TestDeleteEventUpdatesChildren(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	checkEmptyUpdate(c, messages)

	// create 1 parent and 2 children
	f.addTaskEvent(c, "parent", f.begin, f.begin.Add(1*time.Hour), "")
	waitBroadcastTag(messages, sdk.MessageTag_update_events)
	child1 := f.addChildEvent(c, "child1", "parent", f.begin.Add(1*time.Minute))
	waitBroadcastTag(messages, sdk.MessageTag_update_events)
	child2 := f.addChildEvent(c, "child2", "parent", f.begin.Add(2*time.Minute))
	waitBroadcastTag(messages, sdk.MessageTag_update_events)

	// test delete parent updates children
	err = f.controller.DeleteEvent(f.session, "parent")
	c.Assert(err, IsNil)

	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	child1.Parent = proto.String("")
	child2.Parent = proto.String("")
	swtest.DeepEquals(c, mapEvents(msg.Events...), mapEvents(child1, child2))

	msg = waitBroadcastTag(messages, sdk.MessageTag_delete_events)
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, msg.Uuids, []string{"parent"})
}

func (t *TestSuite) TestFirstConnectionFailureReconnects(c *C) {
	f := t.MakeFixture(c, false)
	defer f.Close()
	f.sword.WaitForRetry(2)
	f.server.EnableLogins(true)
	f.sword.WaitForStatus(services.SwordStatusConnected)
}

func (t *TestSuite) TestKnownEventsAreDeletedWhenBeingFiltered(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	event := f.addTaskEvent(c, "event", f.begin, f.begin.Add(1*time.Hour), "")
	faker := FakeFilterer{FakeService: FakeService{}, filter: false}
	_, err := f.controller.apply(f.session, func(session *Session) (interface{}, error) {
		session.AttachFilterer(&faker)
		return nil, nil
	})
	c.Assert(err, IsNil)
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	input := link.Listen()
	msg := (<-input).(*sdk.Message)
	c.Assert(msg.GetTag(), Equals, sdk.MessageTag_update_tick)
	msg = (<-input).(*sdk.Message)
	//c.Assert(msg.GetTag(), Equals, sdk.MessageTag_update_tick)
	//msg = (<-input).(*sdk.Message)
	c.Assert(msg.GetTag(), Equals, sdk.MessageTag_update_session)
	msg = (<-input).(*sdk.Message)
	c.Assert(msg.GetTag(), Equals, sdk.MessageTag_update_services)
	swtest.DeepEquals(c, <-input, &sdk.Message{
		Tag:    sdk.MessageTag_update_events.Enum(),
		Events: []*sdk.Event{event},
	})
	faker.filter = true
	event.Name = proto.String("another_name")
	_, err = f.controller.UpdateEvent(f.session, event.GetUuid(), event)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, <-input, &sdk.Message{
		Tag:   sdk.MessageTag_delete_events.Enum(),
		Uuids: []string{event.GetUuid()},
	})
}

func toStringSet(values ...string) map[string]struct{} {
	set := map[string]struct{}{}
	for _, value := range values {
		set[value] = struct{}{}
	}
	return set
}

func (t *TestSuite) TestSwordModelNotifiesFilteredObservers(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	f.WaitConnected()
	profile := &sword.Profile{
		Login:           proto.String("party_1_only"),
		ReadOnlyParties: swapi.MakeIdList(1),
		Supervisor:      proto.Bool(false),
	}
	f.server.CreateProfile(profile)
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 1
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		addParty(c, d, 1)
		addParty(c, d, 2)
		return true
	})
	t0 := f.addTaskEvent(c, "task_alone", f.begin, f.begin.Add(1*time.Second), "")
	t1 := f.addTaskEvent(c, "task_p1", f.begin, f.begin.Add(1*time.Second), "{\"sword_entity\":1}")
	f.addTaskEvent(c, "invalid", f.begin, f.begin.Add(1*time.Second), "{\"sword_entity\":9999}")
	p1, err := f.createEvent("p1", "p1", "sword", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(1)))
	c.Assert(err, IsNil)
	p2, err := f.createEvent("p2", "p2", "sword", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(2)))
	c.Assert(err, IsNil)
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{
		"sword_profile": "party_1_only",
	})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, mapEvents(msg.Events...), mapEvents(t0, t1, p1))
	profile.ReadOnlyParties = swapi.MakeIdList(2)
	f.server.CreateProfile(profile)
	msg = waitBroadcastTag(messages, sdk.MessageTag_delete_events)
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, toStringSet(msg.Uuids...), toStringSet(t1.GetUuid(), p1.GetUuid()))
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	swtest.DeepEquals(c, mapEvents(msg.Events...), mapEvents(p2))
}

func generateRandomData(size int) []byte {
	src := make([]byte, size)
	for i := range src {
		src[i] = byte(rand.Intn(0xFF))
	}
	return src[:]
}

func (t *TestSuite) TestGarbageOrdersDoesNotAbort(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	id := f.addSwordEvent(c, "garbage", "some_name", generateRandomData(64))
	f.Tick()
	f.controller.StopSession(f.session)
	f.waitAllDoneOrDead(c, id)
}

func checkErrorUpdate(c *C, f *Fixture, messages chan interface{}, id string, event *sdk.Event) {
	_, err := f.controller.UpdateEvent(f.session, id, event)
	c.Assert(err, NotNil)
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, IsNil)
}

func (t *TestSuite) TestChangeReplayRangeDates(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	f.sword.WaitForStatus(services.SwordStatusConnected)

	// Initialize replay informations
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_services)
	c.Assert(msg, NotNil)
	detached := f.server.GetLinks()
	defer detached.Close()
	for slink := range detached {
		f.server.WriteDispatcherToClient(slink, 1, 0,
			&sword.DispatcherToClient_Content{
				ServicesDescription: &sword.ServicesDescription{
					Services: []string{"struct replay::Service"},
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_services)
	c.Assert(msg, NotNil)
	// Replay range with 1mn length
	replayBegin := time.Now().Truncate(time.Second).UTC()
	replayEnd := replayBegin.Add(60 * time.Second)
	for slink := range detached {
		f.server.WriteReplayToClient(slink, 1, 0,
			&sword.ReplayToClient_Content{
				ControlReplayInformation: &sword.ControlReplayInformation{
					CurrentTick:     proto.Int32(0),
					InitialDateTime: swapi.MakeDateTime(replayBegin),
					EndDateTime:     swapi.MakeDateTime(replayEnd),
					DateTime:        swapi.MakeDateTime(replayBegin),
					TickDuration:    proto.Int32(10),
					TimeFactor:      proto.Int32(1),
					Status:          sword.EnumSimulationState_running.Enum(),
					TickCount:       proto.Int32(6),
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	event := msg.GetEvents()[0]
	checkIsReplayEvent(c, event)
	c.Assert(event.GetName(), Equals, "Replay range")
	c.Assert(event.GetInfo(), Equals, "")
	id := event.GetUuid()
	validEvent := services.CloneEvent(event)

	// Modifying replay range event's informations is valid
	event = services.CloneEvent(validEvent)
	event.Name = proto.String("New name")
	event.Info = proto.String("New information")
	_, err = f.controller.UpdateEvent(f.session, id, event)
	c.Assert(err, IsNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	c.Assert(*msg.GetEvents()[0].Name, Equals, *event.Name)
	c.Assert(*msg.GetEvents()[0].Info, Equals, *event.Info)
	validEvent = services.CloneEvent(event)

	// Modifying replay range event out of replay boundaries returns error
	event = services.CloneEvent(validEvent)
	event.Begin = proto.String(util.FormatTime(replayBegin.Add(-time.Hour)))
	checkErrorUpdate(c, f, messages, id, event)
	event = services.CloneEvent(validEvent)
	event.End = proto.String(util.FormatTime(replayEnd.Add(time.Hour)))
	checkErrorUpdate(c, f, messages, id, event)

	// Modifying replay event with erroneous range returns error
	event = services.CloneEvent(validEvent)
	event.Begin, event.End = event.End, event.Begin
	checkErrorUpdate(c, f, messages, id, event)
	event = services.CloneEvent(validEvent)
	event.Begin = event.End
	checkErrorUpdate(c, f, messages, id, event)

	// Modifying first or last replay event and creating a gap between
	// replay boundaries and events will return an error
	event = services.CloneEvent(validEvent)
	event.Begin = proto.String(util.FormatTime(replayBegin.Add(time.Second)))
	checkErrorUpdate(c, f, messages, id, event)
	event = services.CloneEvent(validEvent)
	event.End = proto.String(util.FormatTime(replayEnd.Add(-time.Second)))
	checkErrorUpdate(c, f, messages, id, event)

	// Modifying payload with garbage returns an error
	event = services.CloneEvent(validEvent)
	event.Action.Payload = []byte(`some garbage`)
	checkErrorUpdate(c, f, messages, id, event)

	// Replay range can be disabled
	event = services.CloneEvent(validEvent)
	event.Action.Payload, _ = json.Marshal(&sdk.ReplayPayload{Enabled: proto.Bool(false)})
	_, err = f.controller.UpdateEvent(f.session, id, event)
	c.Assert(err, IsNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	c.Assert(string(msg.GetEvents()[0].Action.GetPayload()), Equals, `{"begin":false,"end":false,"enabled":false}`)

	// And re-enabled
	event = services.CloneEvent(validEvent)
	event.Action.Payload, _ = json.Marshal(&sdk.ReplayPayload{Enabled: proto.Bool(true)})
	_, err = f.controller.UpdateEvent(f.session, id, event)
	c.Assert(err, IsNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	c.Assert(string(msg.GetEvents()[0].Action.GetPayload()), Equals, `{"begin":false,"end":false,"enabled":true}`)

	// Valid replay event creation, split in half the current replay event
	// The new replay event inherits its informations from the split event
	event = services.CloneEvent(validEvent)
	splitUuid := uuid.New()
	event.Uuid = proto.String(splitUuid)
	event.Begin = proto.String(util.FormatTime(replayBegin.Add(30 * time.Second)))
	event.Name = proto.String("")
	event.Info = proto.String("")
	_, err = f.controller.UpdateEvent(f.session, id, event)
	c.Assert(err, IsNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	c.Assert(len(msg.GetEvents()), Equals, 2)
	// Checks old event update
	event = msg.GetEvents()[1]
	begin, _ := util.ParseTime(event.GetBegin())
	end, _ := util.ParseTime(event.GetEnd())
	c.Assert(event.GetUuid(), Equals, id)
	c.Assert(event.GetName(), Equals, "New name")
	c.Assert(event.GetInfo(), Equals, "New information")
	c.Assert(begin, Equals, replayBegin)
	c.Assert(end, Equals, replayBegin.Add(30*time.Second))
	c.Assert(string(event.Action.GetPayload()), Equals, `{"begin":false,"end":true,"enabled":true}`)
	validEvent = services.CloneEvent(event)
	// Checks new event creation
	event = msg.GetEvents()[0]
	c.Assert(event.GetUuid(), Equals, splitUuid)
	c.Assert(event.GetName(), Equals, "New name")
	c.Assert(event.GetInfo(), Equals, "New information")
	c.Assert(string(event.Action.GetPayload()), Equals, `{"begin":true,"end":false,"enabled":true}`)
	begin, _ = util.ParseTime(event.GetBegin())
	end, _ = util.ParseTime(event.GetEnd())
	c.Assert(begin, Equals, replayBegin.Add(30*time.Second))
	c.Assert(end, Equals, replayEnd)
	splitEvent := services.CloneEvent(event)

	// Creating a new replay event cannot overlap more than one existing event
	// and must share one of the existing event boundaries, otherwise it returns an error
	event = services.CloneEvent(validEvent)
	event.Uuid = proto.String(uuid.New())
	event.Begin = proto.String(util.FormatTime(replayBegin))
	event.End = proto.String(util.FormatTime(replayEnd))
	checkErrorUpdate(c, f, messages, id, event)

	event = services.CloneEvent(splitEvent)
	event.Uuid = proto.String(uuid.New())
	event.Begin = proto.String(util.FormatTime(replayBegin.Add(31 * time.Second)))
	event.End = proto.String(util.FormatTime(replayEnd.Add(-1 * time.Second)))
	checkErrorUpdate(c, f, messages, id, event)

	// Modifying a replay event boundary modifies the previous or the next event too
	event = services.CloneEvent(splitEvent)
	event.Begin = proto.String(util.FormatTime(replayBegin.Add(31 * time.Second)))
	_, err = f.controller.UpdateEvent(f.session, id, event)
	c.Assert(err, IsNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	c.Assert(len(msg.GetEvents()), Equals, 2)
	event = msg.GetEvents()[0]
	c.Assert(event.GetUuid(), Equals, splitUuid)
	begin, _ = util.ParseTime(event.GetBegin())
	c.Assert(begin, Equals, replayBegin.Add(31*time.Second))
	splitEvent = services.CloneEvent(event)
	event = msg.GetEvents()[1]
	end, _ = util.ParseTime(event.GetEnd())
	c.Assert(event.GetUuid(), Equals, id)
	c.Assert(end, Equals, replayBegin.Add(31*time.Second))
	validEvent = services.CloneEvent(event)

	// Removing a replay event fills the gap with the previous event
	err = f.controller.DeleteEvent(f.session, splitUuid)
	c.Assert(err, IsNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_delete_events)
	c.Assert(msg, NotNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	event = msg.GetEvents()[0]
	end, _ = util.ParseTime(event.GetEnd())
	c.Assert(event.GetUuid(), Equals, id)
	c.Assert(end, Equals, replayEnd)
	c.Assert(string(event.Action.GetPayload()), Equals, `{"begin":false,"end":false,"enabled":true}`)

	// Cannot remove the last event
	err = f.controller.DeleteEvent(f.session, id)
	c.Assert(err, NotNil)
}

func (t *TestSuite) TestReplayRangeSendSkipToDate(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	f.sword.WaitForStatus(services.SwordStatusConnected)

	// Initialize replay informations
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	messages := make(chan interface{})
	go func() {
		for msg := range link.Listen() {
			messages <- msg
		}
	}()
	msg := waitBroadcastTag(messages, sdk.MessageTag_update_services)
	c.Assert(msg, NotNil)
	detached := f.server.GetLinks()
	defer detached.Close()
	for slink := range detached {
		f.server.WriteDispatcherToClient(slink, 1, 0,
			&sword.DispatcherToClient_Content{
				ServicesDescription: &sword.ServicesDescription{
					Services: []string{"struct replay::Service"},
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_services)
	c.Assert(msg, NotNil)
	// Replay range with 1mn length
	replayBegin := time.Now().Truncate(time.Second).UTC()
	replayEnd := replayBegin.Add(60 * time.Second)
	for slink := range detached {
		f.server.WriteReplayToClient(slink, 1, 0,
			&sword.ReplayToClient_Content{
				ControlReplayInformation: &sword.ControlReplayInformation{
					CurrentTick:     proto.Int32(0),
					InitialDateTime: swapi.MakeDateTime(replayBegin),
					EndDateTime:     swapi.MakeDateTime(replayEnd),
					DateTime:        swapi.MakeDateTime(replayBegin),
					TickDuration:    proto.Int32(1),
					TimeFactor:      proto.Int32(1),
					Status:          sword.EnumSimulationState_running.Enum(),
					TickCount:       proto.Int32(60),
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	event := msg.GetEvents()[0]
	checkIsReplayEvent(c, event)
	c.Assert(event.GetName(), Equals, "Replay range")
	c.Assert(event.GetInfo(), Equals, "")
	id := event.GetUuid()
	validEvent := services.CloneEvent(event)

	// Split and disable first display range
	event = services.CloneEvent(validEvent)
	splitUuid := uuid.New()
	event.Uuid = proto.String(splitUuid)
	event.Begin = proto.String(util.FormatTime(replayBegin.Add(30 * time.Second)))
	f.controller.UpdateEvent(f.session, id, event)
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	c.Assert(len(msg.GetEvents()), Equals, 2)
	event = services.CloneEvent(validEvent)
	event.End = proto.String(util.FormatTime(replayBegin.Add(30 * time.Second)))
	event.Action.Payload, _ = json.Marshal(&sdk.ReplayPayload{Enabled: proto.Bool(false)})
	_, err = f.controller.UpdateEvent(f.session, id, event)
	c.Assert(err, IsNil)
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_events)
	c.Assert(msg, NotNil)
	c.Assert(string(msg.GetEvents()[0].Action.GetPayload()), Equals, `{"begin":false,"end":true,"enabled":false}`)

	// ticking in a deactivated sends skip to date to replay server
	current := replayBegin.Add(time.Second)
	for slink := range detached {
		f.server.WriteToClient(slink, 1, 0,
			&sword.SimToClient_Content{
				ControlInformation: &sword.ControlInformation{
					CurrentTick:         proto.Int32(1),
					InitialDateTime:     swapi.MakeDateTime(replayBegin),
					DateTime:            swapi.MakeDateTime(current),
					TickDuration:        proto.Int32(1),
					TimeFactor:          proto.Int32(1),
					CheckpointFrequency: proto.Int32(0),
					Status:              sword.EnumSimulationState_running.Enum(),
					SendVisionCones:     proto.Bool(false),
					ProfilingEnabled:    proto.Bool(false),
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_session)

	// ticking is disabled until it reaches an activated replay range
	current = replayBegin.Add(29 * time.Second)
	for slink := range detached {
		f.server.WriteToClient(slink, 1, 0,
			&sword.SimToClient_Content{
				ControlInformation: &sword.ControlInformation{
					CurrentTick:         proto.Int32(29),
					InitialDateTime:     swapi.MakeDateTime(replayBegin),
					DateTime:            swapi.MakeDateTime(current),
					TickDuration:        proto.Int32(1),
					TimeFactor:          proto.Int32(1),
					CheckpointFrequency: proto.Int32(0),
					Status:              sword.EnumSimulationState_running.Enum(),
					SendVisionCones:     proto.Bool(false),
					ProfilingEnabled:    proto.Bool(false),
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_session)
	c.Assert(msg, NotNil)
	c.Assert(msg.Tick, IsNil)

	current = replayBegin.Add(35 * time.Second)
	for slink := range detached {
		f.server.WriteToClient(slink, 1, 0,
			&sword.SimToClient_Content{
				ControlInformation: &sword.ControlInformation{
					CurrentTick:         proto.Int32(35),
					InitialDateTime:     swapi.MakeDateTime(replayBegin),
					DateTime:            swapi.MakeDateTime(current),
					TickDuration:        proto.Int32(1),
					TimeFactor:          proto.Int32(1),
					CheckpointFrequency: proto.Int32(0),
					Status:              sword.EnumSimulationState_running.Enum(),
					SendVisionCones:     proto.Bool(false),
					ProfilingEnabled:    proto.Bool(false),
				},
			})
	}
	msg = waitBroadcastTag(messages, sdk.MessageTag_update_session)
	c.Assert(msg, NotNil)
	currentTime, _ := util.ParseTime(msg.Session.GetTime())
	c.Assert(currentTime, Equals, current)
}
