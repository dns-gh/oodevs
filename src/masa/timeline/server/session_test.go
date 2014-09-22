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
	f := &Fixture{server, sword, controller, observer, id, time.Now()}
	f.Tick()
	return f
}

func (f *Fixture) createFullEvent(uuid, name, target, parent, metadata string, payload []byte, begin, end time.Time) (*sdk.Event, error) {
	msg := sdk.Event{
		Uuid:     proto.String(uuid),
		Name:     proto.String(name),
		Begin:    proto.String(util.FormatTime(begin)),
		Parent:   proto.String(parent),
		Metadata: proto.String(metadata),
		Action: &sdk.Action{
			Target:  proto.String("sword://" + target),
			Apply:   proto.Bool(true),
			Payload: payload,
		},
	}
	if !end.IsZero() {
		msg.End = proto.String(util.FormatTime(end))
	}
	return f.controller.CreateEvent(f.session, &msg)
}

func (f *Fixture) createEvent(uuid, name, target string, payload []byte) (*sdk.Event, error) {
	return f.createFullEvent(uuid, name, target, "", "", payload, f.begin, time.Time{})
}

func (f *Fixture) addEvent(c *C, name, target string, payload []byte) string {
	uuid := uuid.New()
	_, err := f.createEvent(uuid, name, target, payload)
	c.Assert(err, IsNil)
	return uuid
}

func (f *Fixture) addTaskEvent(c *C, uuid string, begin, end time.Time, metadata string) *sdk.Event {
	msg, err := f.createFullEvent(uuid, uuid, "", "", metadata, []byte{}, begin, end)
	c.Assert(err, IsNil)
	c.Assert(msg, NotNil)
	return msg
}

func (f *Fixture) addChildEvent(c *C, uuid, parent string, begin time.Time) *sdk.Event {
	msg, err := f.createFullEvent(uuid, uuid, "", parent, "", []byte{}, begin, time.Time{})
	c.Assert(err, IsNil)
	c.Assert(msg, NotNil)
	return msg
}

func (t *TestSuite) TestDeadlockOnActionTargetMismatch(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()
	f.addEvent(c, "some_name", "mismatch", []byte{})
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
	unit := f.addEvent(c, "unit", "some_name", f.getSomeUnitOrder(c, 73, 0))
	automat := f.addEvent(c, "automat", "some_name", f.getSomeAutomatOrder(c, 1))
	crowd := f.addEvent(c, "crows", "some_name", f.getSomeCrowdOrder(c, 1))
	frag := f.addEvent(c, "frag", "some_name", f.getSomeFragOrder(c, swapi.MakeUnitTasker(1337)))
	magic := f.addEvent(c, "magic", "some_name", f.getSomeMagicAction(c))
	umagic := f.addEvent(c, "unit magic", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeUnitTasker(1337)))
	omagic := f.addEvent(c, "object magic", "some_name", f.getSomeObjectMagicAction(c, 1))
	kmagic := f.addEvent(c, "knowledge magic", "some_name", f.getSomeKnowledgeMagicAction(c, 2))
	amagic := f.addEvent(c, "automat mode", "some_name", f.getSomeAutomatModeMagicAction(c, 3))
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
	unit := f.addEvent(c, "unit", "some_name", f.getSomeUnitOrder(c, 77, 0))
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
	event, err := f.createEvent(uuid, "some_event", "some_name", f.getSomeUnitOrder(c, 1, 0))
	c.Assert(err, IsNil)

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
	lock   sync.Mutex
	events []*sdk.Event
	uuids  []string
}

func (f *FakeService) Proto(name string) *sdk.Service {
	return &sdk.Service{
		Name:  proto.String(name),
		Clock: proto.Bool(false),
	}
}

func (f *FakeService) HasClock() bool                      { return false }
func (f *FakeService) IsLocked() bool                      { return false }
func (f *FakeService) Start() error                        { return nil }
func (f *FakeService) Stop() error                         { return nil }
func (f *FakeService) Apply(string, url.URL, []byte) error { return nil }

func (f *FakeService) Update(events ...*sdk.Event) {
	f.lock.Lock()
	defer f.lock.Unlock()
	f.events = events
}

func (f *FakeService) Delete(uuids ...string) {
	f.lock.Lock()
	defer f.lock.Unlock()
	f.uuids = uuids
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
	event, err := f.createEvent(uuid.New(), "something", name, f.getSomeUnitOrder(c, 1, 0))
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
		return nil, session.Attach("faker", &faker)
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

type FakeFilterer struct {
	FakeService
	filter bool
	count  int
}

func (f *FakeFilterer) GetFilters(config services.EventFilterConfig) []services.EventFilter {
	return []services.EventFilter{func(event *sdk.Event) bool {
		f.count++
		return f.filter
	}}
}

func (f *FakeFilterer) Filter(event *sdk.Event, config services.EventFilterConfig) bool {
	return f.GetFilters(config)[0](event)
}

func checkUpdateEvents(c *C, sgroup *sync.WaitGroup, link SdkObserver, filter bool) {
	seen := false
	sgroup.Add(1)
	go func() {
		defer sgroup.Done()
		for msg := range link.Listen() {
			data, ok := msg.(*sdk.Message)
			seen = seen || ok &&
				data.GetTag() == sdk.MessageTag_update_events &&
				len(data.GetEvents()) > 0
		}
		if seen && filter || !seen && !filter {
			c.Error("filtering error")
		}
	}()
}

func (t *TestSuite) TestFiltering(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	faker := FakeFilterer{FakeService: FakeService{}, filter: false}
	_ = services.EventFilterer(&faker)
	_, err := f.controller.apply(f.session, func(session *Session) (interface{}, error) {
		return nil, session.Attach("faker", &faker)
	})
	c.Assert(err, IsNil)

	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	faker.filter = true
	sgroup := sync.WaitGroup{}
	checkUpdateEvents(c, &sgroup, link, faker.filter)
	f.createEvent(uuid.New(), "something", "some_name", f.getSomeUnitOrder(c, 1, 0))
	f.controller.UnregisterObserver(f.session, link)
	sgroup.Wait()
	c.Assert(faker.count, Equals, 1)

	faker.count = 0
	faker.filter = false
	link, err = f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	checkUpdateEvents(c, &sgroup, link, faker.filter)
	f.createEvent(uuid.New(), "something", "some_name", f.getSomeUnitOrder(c, 1, 0))
	f.controller.UnregisterObserver(f.session, link)
	sgroup.Wait()
	c.Assert(faker.count, Equals, 2)
}

func addParty(c *C, d *swapi.ModelData, id uint32) {
	d.Parties[id] = &swapi.Party{
		Id: id,
	}
}

func addFormation(c *C, d *swapi.ModelData, id, party, parent uint32) {
	d.Formations[id] = &swapi.Formation{
		Id:       id,
		PartyId:  party,
		ParentId: parent,
	}
}

func addAutomat(c *C, d *swapi.ModelData, id, formation uint32) {
	d.Automats[id] = &swapi.Automat{
		Id:          id,
		FormationId: formation,
	}
}

func addUnit(c *C, d *swapi.ModelData, id, automat uint32) {
	d.Units[id] = &swapi.Unit{
		Id:        id,
		AutomatId: automat,
	}
}

func addCrowd(c *C, d *swapi.ModelData, id, party uint32) {
	d.Crowds[id] = &swapi.Crowd{
		Id:      id,
		PartyId: party,
	}
}

func addPopulation(c *C, d *swapi.ModelData, id, party uint32) {
	d.Populations[id] = &swapi.Population{
		Id:      id,
		PartyId: party,
	}
}

func addObject(c *C, d *swapi.ModelData, id, party uint32) {
	d.Objects[id] = &swapi.Object{
		Id:      id,
		PartyId: party,
	}
}

func addKnowledgeGroup(c *C, d *swapi.ModelData, id, party uint32) {
	d.KnowledgeGroups[id] = &swapi.KnowledgeGroup{
		Id:      id,
		PartyId: party,
	}
}

type FakeSwordFilter struct {
	key   string
	value string
}

func (f *FakeSwordFilter) FormValue(value string) string {
	if value == f.key {
		return f.value
	}
	return ""
}

func parseFilters(c *C, key, value string) services.EventFilterConfig {
	cfg, err := ParseEventFilterConfig(&FakeSwordFilter{key, value})
	c.Assert(err, IsNil)
	return cfg
}

func countEvents(counter chan<- int, link <-chan interface{}) {
	for msg := range link {
		data, ok := msg.(*sdk.Message)
		if ok && data.GetTag() == sdk.MessageTag_update_events {
			counter <- len(data.GetEvents())
		}
	}
	close(counter)
}

func (f *Fixture) applyFilters(c *C, cfg services.EventFilterConfig, count int) {
	// test filters on ReadEvents
	events, err := f.controller.ReadEvents(f.session, cfg)
	c.Assert(err, IsNil)
	c.Assert(events, HasLen, count)
	// test filters on observers
	link, err := f.controller.RegisterObserver(f.session, cfg)
	c.Assert(err, IsNil)
	counter := make(chan int)
	go countEvents(counter, link.Listen())
	f.controller.UnregisterObserver(f.session, link)
	sum := 0
	for n := range counter {
		sum += n
	}
	c.Assert(sum, Equals, count)
}

func (t *TestSuite) TestFilters(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.WaitConnected()
	f.server.CreateProfile(&sword.Profile{
		Login:             proto.String("automat_1_only"),
		ReadOnlyAutomates: swapi.MakeIdList(1),
		Supervisor:        proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:              proto.String("formation_1_only"),
		ReadOnlyFormations: swapi.MakeIdList(1),
		Supervisor:         proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:              proto.String("formation_2_only"),
		ReadOnlyFormations: swapi.MakeIdList(2),
		Supervisor:         proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_1_only"),
		ReadOnlyParties: swapi.MakeIdList(1),
		Supervisor:      proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:          proto.String("crowd_1_only"),
		ReadOnlyCrowds: swapi.MakeIdList(1),
		Supervisor:     proto.Bool(false),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 5
	})

	// abuse wait command to create hierarchy
	// u1 u2 u3 u4 u5
	// | /  /   /  |
	// a1 a2  a3   a4
	// | /    /    |
	// f1   f3     f4
	//  \  /       |
	//   f2        |
	//   |         |
	//   p1        p2
	//  / \       / \
	// c1 i1    c2   i2
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		addParty(c, d, 1)
		addParty(c, d, 2)
		addFormation(c, d, 2, 1, 0)
		addFormation(c, d, 1, 0, 2)
		addFormation(c, d, 3, 0, 2)
		addFormation(c, d, 4, 2, 0)
		addAutomat(c, d, 1, 1)
		addAutomat(c, d, 2, 1)
		addAutomat(c, d, 3, 3)
		addAutomat(c, d, 4, 4)
		addUnit(c, d, 1, 1)
		addUnit(c, d, 2, 1)
		addUnit(c, d, 3, 2)
		addUnit(c, d, 4, 3)
		addUnit(c, d, 5, 4)
		addCrowd(c, d, 1, 1)
		addCrowd(c, d, 2, 2)
		addPopulation(c, d, 1, 1)
		addPopulation(c, d, 2, 2)
		return true
	})
	f.addEvent(c, "u1", "some_name", f.getSomeUnitOrder(c, 1, 0)) // event without automat
	f.addEvent(c, "u2", "some_name", f.getSomeUnitOrder(c, 2, 2))
	f.addEvent(c, "u3", "some_name", f.getSomeUnitOrder(c, 3, 3))
	f.addEvent(c, "u4", "some_name", f.getSomeUnitOrder(c, 4, 4))
	f.addEvent(c, "u5", "some_name", f.getSomeUnitOrder(c, 5, 5))
	f.addEvent(c, "a1", "some_name", f.getSomeAutomatOrder(c, 1))
	f.addEvent(c, "a2", "some_name", f.getSomeAutomatOrder(c, 2))
	f.addEvent(c, "a3", "some_name", f.getSomeAutomatOrder(c, 3))
	f.addEvent(c, "a4", "some_name", f.getSomeAutomatOrder(c, 4))
	f.addEvent(c, "f1", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(1)))
	f.addEvent(c, "f2", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(2)))
	f.addEvent(c, "f3", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(3)))
	f.addEvent(c, "f4", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(4)))
	f.addEvent(c, "c1", "some_name", f.getSomeCrowdOrder(c, 1))
	f.addEvent(c, "c2", "some_name", f.getSomeCrowdOrder(c, 2))
	f.addEvent(c, "i1", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(1)))
	f.addEvent(c, "i2", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(2)))
	f.addEvent(c, "p1", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(1)))
	f.addEvent(c, "p2", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(2)))
	f.addEvent(c, "uf1", "some_name", f.getSomeFragOrder(c, swapi.MakeUnitTasker(1)))
	f.addEvent(c, "af1", "some_name", f.getSomeFragOrder(c, swapi.MakeAutomatTasker(1)))
	f.addEvent(c, "cf1", "some_name", f.getSomeFragOrder(c, swapi.MakeCrowdTasker(1)))
	id := uuid.New()
	_, err := f.controller.CreateEvent(f.session, &sdk.Event{
		Uuid:  proto.String(id),
		Name:  proto.String("some_event"),
		Begin: proto.String(util.FormatTime(f.begin)),
	})
	c.Assert(err, IsNil)
	// ensure we do get all our events
	f.applyFilters(c, services.EventFilterConfig{}, 23)

	// test sword_profile
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "automat_1_only",
	}, 5+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "formation_1_only",
	}, 8+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "formation_2_only",
	}, 12+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "party_1_only",
	}, 16+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "crowd_1_only",
	}, 2+1)

	// test sword_filter
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "u:1,u:2"), 3+1)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "a:1"), 5+1)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "f:3"), 3+1)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "p:2"), 6+1)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "c:1"), 2+1)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "i:1"), 1+1)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "a:1,f:3,p:2,c:1"), 16+1)

	// test filter_service
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:true,sword:false"), 1)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:false,sword:true"), 22)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:true,sword:true"), 22+1)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:false,sword:false"), 0)

	// test sword_filter_engaged: if activated, orders that were given to units by automata are not displayed
	f.applyFilters(c, parseFilters(c,
		"sword_filter_engaged", "false"), 23)
	f.applyFilters(c, parseFilters(c,
		"sword_filter_engaged", "true"), 23-4)

	// test filter_keyword
	f.applyFilters(c, parseFilters(c,
		"filter_keyword", "u"), 6)
	f.applyFilters(c, parseFilters(c,
		"filter_keyword", " U "), 6)
	f.applyFilters(c, parseFilters(c,
		"filter_keyword", "p2"), 1)
	f.applyFilters(c, parseFilters(c,
		"filter_keyword", "u\\d"), 0)
}

func (t *TestSuite) TestIncompleteMissionsAreVisibleByAnyProfile(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.WaitConnected()
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_1_only"),
		ReadOnlyParties: swapi.MakeIdList(1),
		Supervisor:      proto.Bool(false),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 1
	})

	f.addEvent(c, "u1", "some_name", f.getSomeUnitOrder(c, 0, 0))
	f.addEvent(c, "a1", "some_name", f.getSomeAutomatOrder(c, 0))
	f.addEvent(c, "c1", "some_name", f.getSomeCrowdOrder(c, 0))
	f.addEvent(c, "u2", "some_name", f.getSomeFragOrder(c, swapi.MakeUnitTasker(0)))
	f.addEvent(c, "a2", "some_name", f.getSomeFragOrder(c, swapi.MakeAutomatTasker(0)))
	f.addEvent(c, "f1", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(0)))
	f.addEvent(c, "p1", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(0)))
	f.addEvent(c, "c2", "some_name", f.getSomeFragOrder(c, swapi.MakeCrowdTasker(0)))
	f.addEvent(c, "i1", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(0)))

	f.applyFilters(c, services.EventFilterConfig{}, 9)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "party_1_only",
	}, 9)
}

func (t *TestSuite) TestFiltersOnMagicActions(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.WaitConnected()
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_1_only"),
		ReadOnlyParties: swapi.MakeIdList(1),
		Supervisor:      proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_2_only"),
		ReadOnlyParties: swapi.MakeIdList(2),
		Supervisor:      proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:            proto.String("party_1_supervisor"),
		ReadWriteParties: swapi.MakeIdList(1),
		Supervisor:       proto.Bool(true),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:            proto.String("supervisor"),
		ReadWriteParties: swapi.MakeIdList(1, 2),
		Supervisor:       proto.Bool(true),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 4
	})

	// abuse wait command to create hierarchy
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		addParty(c, d, 1)
		addParty(c, d, 2)
		addFormation(c, d, 1, 1, 0)
		addFormation(c, d, 2, 2, 0)
		addAutomat(c, d, 1, 1)
		addAutomat(c, d, 2, 2)
		addUnit(c, d, 1, 1)
		addUnit(c, d, 2, 2)
		addCrowd(c, d, 1, 1)
		addCrowd(c, d, 2, 2)
		addPopulation(c, d, 1, 1)
		addPopulation(c, d, 2, 2)
		addObject(c, d, 1, 1)
		addObject(c, d, 2, 2)
		addKnowledgeGroup(c, d, 1, 1)
		addKnowledgeGroup(c, d, 2, 2)
		return true
	})

	f.addEvent(c, "ma1", "some_name", f.getSomeMagicAction(c))
	f.addEvent(c, "uma1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeUnitTasker(1)))
	f.addEvent(c, "ama1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeAutomatTasker(1)))
	f.addEvent(c, "cma1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeCrowdTasker(1)))
	f.addEvent(c, "ima1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakePopulationTasker(1)))
	f.addEvent(c, "cuma1", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(1)))
	f.addEvent(c, "cuma2", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(2)))
	f.addEvent(c, "oma1", "some_name", f.getSomeObjectMagicAction(c, 1))
	f.addEvent(c, "kma1", "some_name", f.getSomeKnowledgeMagicAction(c, 1))
	f.addEvent(c, "amma1", "some_name", f.getSomeAutomatModeMagicAction(c, 1))

	f.addEvent(c, "i0", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(0)))

	f.applyFilters(c, services.EventFilterConfig{}, 10+1)

	// test sword_profile
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "party_1_only",
	}, 2+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "party_2_only",
	}, 1+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "party_1_supervisor",
	}, 9+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "supervisor",
	}, 10+1)
}

func (t *TestSuite) TestFiltersHierarchy(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.addTaskEvent(c, "parent1", f.begin, f.begin.Add(1*time.Hour), "")
	f.addChildEvent(c, "child1", "parent1", f.begin.Add(1*time.Minute))

	f.addTaskEvent(c, "parent2", f.begin, f.begin.Add(1*time.Hour), "")
	f.addChildEvent(c, "child2", "parent2", f.begin.Add(1*time.Minute))
	f.addChildEvent(c, "child3", "parent2", f.begin.Add(2*time.Minute))

	f.addTaskEvent(c, "parent3", f.begin, f.begin.Add(1*time.Hour), "")
	f.addChildEvent(c, "child4", "parent3", f.begin.Add(1*time.Minute))
	f.addChildEvent(c, "child5", "parent3", f.begin.Add(2*time.Minute))
	f.addChildEvent(c, "child6", "parent3", f.begin.Add(2*time.Minute))

	// ensure we do get all our events
	f.applyFilters(c, services.EventFilterConfig{}, 9)

	// test filter_show_only
	f.applyFilters(c, parseFilters(c,
		"filter_show_only", ""), 9)
	f.applyFilters(c, parseFilters(c,
		"filter_show_only", "unexisting parent"), 0)
	f.applyFilters(c, parseFilters(c,
		"filter_show_only", "parent1"), 2)
	f.applyFilters(c, parseFilters(c,
		"filter_show_only", "parent2"), 3)
	f.applyFilters(c, parseFilters(c,
		"filter_show_only", "parent3"), 4)
	// parent1 is hidden so child1 is too
	f.applyFilters(c, parseFilters(c,
		"filter_show_only", "child1"), 0)

	// test filter_hide_hierarchies
	f.applyFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent1"), 9-1)
	f.applyFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent2"), 9-2)
	f.applyFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent3"), 9-3)
	f.applyFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent1,parent2"), 9-1-2)
	f.applyFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent1,parent2,parent3"), 9-1-2-3)
}

func (t *TestSuite) TestFiltersMetadata(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.WaitConnected()
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_1_only"),
		ReadOnlyParties: swapi.MakeIdList(1),
		Supervisor:      proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:              proto.String("formation_2_only"),
		ReadOnlyFormations: swapi.MakeIdList(2),
		Supervisor:         proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:              proto.String("formation_3_only"),
		ReadOnlyFormations: swapi.MakeIdList(3),
		Supervisor:         proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:             proto.String("automat_4_only"),
		ReadOnlyAutomates: swapi.MakeIdList(4),
		Supervisor:        proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:          proto.String("crowd_6_only"),
		ReadOnlyCrowds: swapi.MakeIdList(6),
		Supervisor:     proto.Bool(false),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 5
	})

	// abuse wait command to create hierarchy
	//     p1
	//  /  |  \
	// f2  c6  i7
	// |
	// f3
	// |
	// a4
	// |
	// u5
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		addParty(c, d, 1)
		addFormation(c, d, 2, 1, 0)
		addFormation(c, d, 3, 0, 2)
		addAutomat(c, d, 4, 3)
		addUnit(c, d, 5, 4)
		addCrowd(c, d, 6, 1)
		addPopulation(c, d, 7, 1)
		return true
	})

	f.addTaskEvent(c, "task_alone", f.begin, f.begin.Add(1*time.Hour), "")
	f.addTaskEvent(c, "task_invalid_unit", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":0}")
	f.addTaskEvent(c, "task_p1", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":1}")
	f.addTaskEvent(c, "task_f2", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":2}")
	f.addTaskEvent(c, "task_f3", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":3}")
	f.addTaskEvent(c, "task_a4", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":4}")
	f.addTaskEvent(c, "task_u5", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":5}")
	f.addTaskEvent(c, "task_c6", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":6}")
	event := f.addTaskEvent(c, "task_i7", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":7}")

	// ensure we do get all our events
	f.applyFilters(c, services.EventFilterConfig{}, 9)
	// test sword_profile
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "party_1_only",
	}, 7+2)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "formation_2_only",
	}, 4+2)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "formation_3_only",
	}, 3+2)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "automat_4_only",
	}, 2+2)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "crowd_6_only",
	}, 1+2)

	// test sword_filter
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "p:1"), 7+2)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "f:2"), 4+2)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "f:3"), 3+2)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "a:4"), 2+2)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "u:5"), 1+2)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "c:6"), 1+2)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "i:7"), 1+2)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "u:5,i:7,c:6"), 3+2)

	// remove event metadata and try again
	event.Metadata = nil
	_, err := f.controller.UpdateEvent(f.session, event.GetUuid(), event)
	c.Assert(err, IsNil)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "automat_4_only",
	}, 2+3)
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "a:4"), 2+3)
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
	id := f.addEvent(c, "order", "some_name", order)
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
	id := f.addEvent(c, "order", "some_name", f.getSomeUnitOrder(c, 93, 0))
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
	id := f.addEvent(c, "order", "some_name", f.getSomeUnitOrder(c, 94, 0))
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
		unit := f.addEvent(c, "unit", "some_name", f.getSomeUnitOrder(c, 95, 0))
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
	unit := f.addEvent(c, "argh", "some_name", f.getSomeUnitOrder(c, 96, 0))
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
	_ = services.EventFilterer(&faker)
	_, err := f.controller.apply(f.session, func(session *Session) (interface{}, error) {
		return nil, session.Attach("faker", &faker)
	})
	c.Assert(err, IsNil)
	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	defer f.controller.UnregisterObserver(f.session, link)
	input := link.Listen()
	msg := (<-input).(*sdk.Message)
	c.Assert(msg.GetTag(), Equals, sdk.MessageTag_update_tick)
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
	p1, err := f.createEvent("p1", "p1", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(1)))
	c.Assert(err, IsNil)
	p2, err := f.createEvent("p2", "p2", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(2)))
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
