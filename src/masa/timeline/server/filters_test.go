// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package server

import (
	"code.google.com/p/go-uuid/uuid"
	"code.google.com/p/goprotobuf/proto"
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/sword/swtest"
	"masa/timeline/sdk"
	"masa/timeline/services"
	"masa/timeline/util"
	"sync"
	"time"
)

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
	_, err := f.controller.apply(f.session, func(session *Session) (interface{}, error) {
		session.AttachFilterer(&faker)
		return nil, nil
	})
	c.Assert(err, IsNil)

	link, err := f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	faker.filter = true
	sgroup := sync.WaitGroup{}
	checkUpdateEvents(c, &sgroup, link, faker.filter)
	f.createEvent(uuid.New(), "something", "sword", "some_name", f.getSomeUnitOrder(c, 1, 0))
	f.controller.UnregisterObserver(f.session, link)
	sgroup.Wait()
	c.Assert(faker.count, Equals, 1)

	faker.count = 0
	faker.filter = false
	link, err = f.controller.RegisterObserver(f.session, services.EventFilterConfig{})
	c.Assert(err, IsNil)
	checkUpdateEvents(c, &sgroup, link, faker.filter)
	f.createEvent(uuid.New(), "something", "sword", "some_name", f.getSomeUnitOrder(c, 1, 0))
	f.controller.UnregisterObserver(f.session, link)
	sgroup.Wait()
	c.Assert(faker.count, Equals, 2)
}

type FakeSwordFilter struct {
	data map[string]string
}

func (f *FakeSwordFilter) FormValue(key string) string {
	return f.data[key]
}

func parseFilters(c *C, values ...string) services.EventFilterConfig {
	c.Assert(len(values)&1, Equals, 0)
	data := map[string]string{}
	for i := 0; i < len(values); i += 2 {
		data[values[i]] = values[i+1]
	}
	cfg, err := ParseEventFilterConfig(&FakeSwordFilter{data})
	c.Assert(err, IsNil)
	return cfg
}

func checkEvents(counter chan<- string, link <-chan interface{}) {
	for msg := range link {
		data, ok := msg.(*sdk.Message)
		if ok && data.GetTag() == sdk.MessageTag_update_events {
			for _, event := range data.GetEvents() {
				counter <- event.GetName()
			}
		}
	}
	close(counter)
}

func (f *Fixture) checkFilters(c *C, cfg services.EventFilterConfig, names ...string) {
	expected := map[string]struct{}{}
	for _, name := range names {
		expected[name] = struct{}{}
	}
	// test filters on ReadEvents
	events, err := f.controller.ReadEvents(f.session, cfg)
	c.Assert(err, IsNil)
	actual := map[string]struct{}{}
	for _, event := range events {
		actual[event.GetName()] = struct{}{}
	}
	swtest.DeepEquals(c, actual, expected)
	// test filters on observers
	link, err := f.controller.RegisterObserver(f.session, cfg)
	c.Assert(err, IsNil)
	counter := make(chan string)
	go checkEvents(counter, link.Listen())
	f.controller.UnregisterObserver(f.session, link)
	actual = map[string]struct{}{}
	for name := range counter {
		actual[name] = struct{}{}
	}
	swtest.DeepEquals(c, actual, expected)
}

func (t *TestSuite) TestFilters(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.WaitConnected()
	f.server.CreateProfile(&sword.Profile{
		Login:             proto.String("automat_11_only"),
		ReadOnlyAutomates: swapi.MakeIdList(11),
		Supervisor:        proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:              proto.String("formation_21_only"),
		ReadOnlyFormations: swapi.MakeIdList(21),
		Supervisor:         proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:              proto.String("formation_22_only"),
		ReadOnlyFormations: swapi.MakeIdList(22),
		Supervisor:         proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_31_only"),
		ReadOnlyParties: swapi.MakeIdList(31),
		Supervisor:      proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:          proto.String("crowd_41_only"),
		ReadOnlyCrowds: swapi.MakeIdList(41),
		Supervisor:     proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:               proto.String("f21_read_f23_write"),
		ReadOnlyFormations:  swapi.MakeIdList(21),
		ReadWriteFormations: swapi.MakeIdList(23),
		Supervisor:          proto.Bool(false),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 6
	})

	// abuse wait command to create hierarchy
	// u1 u2 u3 u4 u5
	// | /  /   /  |
	// a11 a12 a13 a14
	// | /    /    |
	// f21  f23    f24
	//  \  /       |
	//   f22       |
	//   |         |
	//   p31       p32
	//  / \       / \
	// c41 i51   c42 i52
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		addParty(c, d, 31)
		addParty(c, d, 32)
		addFormation(c, d, 22, 31, 0)
		addFormation(c, d, 21, 0, 22)
		addFormation(c, d, 23, 0, 22)
		addFormation(c, d, 24, 32, 0)
		addAutomat(c, d, 11, 21)
		addAutomat(c, d, 12, 21)
		addAutomat(c, d, 13, 23)
		addAutomat(c, d, 14, 24)
		addUnit(c, d, 1, 11)
		addUnit(c, d, 2, 11)
		addUnit(c, d, 3, 12)
		addUnit(c, d, 4, 13)
		addUnit(c, d, 5, 14)
		addCrowd(c, d, 41, 31)
		addCrowd(c, d, 42, 32)
		addPopulation(c, d, 51, 31)
		addPopulation(c, d, 52, 32)
		return true
	})
	f.addSwordEvent(c, "u1", "some_name", f.getSomeUnitOrder(c, 1, 0)) // event without automat
	f.addSwordEvent(c, "u2", "some_name", f.getSomeUnitOrder(c, 2, 12))
	f.addSwordEvent(c, "u3", "some_name", f.getSomeUnitOrder(c, 3, 13))
	f.addSwordEvent(c, "u4", "some_name", f.getSomeUnitOrder(c, 4, 14))
	f.addSwordEvent(c, "u5", "some_name", f.getSomeUnitOrder(c, 5, 15))
	f.addSwordEvent(c, "a11", "some_name", f.getSomeAutomatOrder(c, 11))
	f.addSwordEvent(c, "a12", "some_name", f.getSomeAutomatOrder(c, 12))
	f.addSwordEvent(c, "a13", "some_name", f.getSomeAutomatOrder(c, 13))
	f.addSwordEvent(c, "a14", "some_name", f.getSomeAutomatOrder(c, 14))
	f.addSwordEvent(c, "f21", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(21)))
	f.addSwordEvent(c, "f22", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(22)))
	f.addSwordEvent(c, "f23", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(23)))
	f.addSwordEvent(c, "f24", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(24)))
	f.addSwordEvent(c, "c41", "some_name", f.getSomeCrowdOrder(c, 41))
	f.addSwordEvent(c, "c42", "some_name", f.getSomeCrowdOrder(c, 42))
	f.addSwordEvent(c, "i51", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(51)))
	f.addSwordEvent(c, "i52", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(52)))
	f.addSwordEvent(c, "p31", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(31)))
	f.addSwordEvent(c, "p32", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(32)))
	f.addSwordEvent(c, "uf1", "some_name", f.getSomeFragOrder(c, swapi.MakeUnitTasker(1)))
	f.addSwordEvent(c, "af11", "some_name", f.getSomeFragOrder(c, swapi.MakeAutomatTasker(11)))
	f.addSwordEvent(c, "cf41", "some_name", f.getSomeFragOrder(c, swapi.MakeCrowdTasker(41)))
	id := uuid.New()
	_, err := f.controller.CreateEvent(f.session, &sdk.Event{
		Uuid:  proto.String(id),
		Name:  proto.String("some_event"),
		Begin: proto.String(util.FormatTime(f.begin)),
	})
	c.Assert(err, IsNil)
	// ensure we do get all our events
	f.checkFilters(c, services.EventFilterConfig{},
		"a11", "a12", "a13", "a14", "af11", "c41",
		"c42", "cf41", "f21", "f22", "f23", "f24",
		"i51", "i52", "p31", "p32", "some_event",
		"u1", "u2", "u3", "u4", "u5", "uf1")

	// test sword_profile
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "automat_11_only",
	), "a11", "af11", "some_event", "u1", "u2", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "formation_21_only",
	), "a11", "a12", "af11", "f21", "some_event",
		"u1", "u2", "u3", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "formation_22_only",
	), "a11", "a12", "a13", "af11", "f21", "f22", "f23",
		"some_event", "u1", "u2", "u3", "u4", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_only",
	), "a11", "a12", "a13", "af11", "c41", "cf41",
		"f21", "f22", "f23", "i51", "p31",
		"some_event", "u1", "u2", "u3", "u4", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "f21_read_f23_write",
	), "a11", "a12", "a13", "af11", "f21", "f23",
		"some_event", "u1", "u2", "u3", "u4", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "crowd_41_only",
	), "c41", "cf41", "some_event")

	// test sword_read_only
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_only",
		"sword_read_only", "false",
	), "some_event")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "f21_read_f23_write",
		"sword_read_only", "false",
	), "a13", "f23", "some_event", "u4")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "f21_read_f23_write",
		"sword_read_only", "true",
	), "a11", "a12", "a13", "af11", "f21", "f23",
		"some_event", "u1", "u2", "u3", "u4", "uf1")

	// test sword_filter
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "u:1,u:2",
	), "some_event", "u1", "u2", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "a:11",
	), "a11", "af11", "some_event", "u1", "u2", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "f:23",
	), "a13", "f23", "some_event", "u4")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "p:32",
	), "a14", "c42", "f24", "i52", "p32", "some_event", "u5")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "c:41",
	), "c41", "cf41", "some_event")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "i:51",
	), "i51", "some_event")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "a:11,f:23,p:32,c:41",
	), "a11", "a13", "a14", "af11", "c41", "c42",
		"cf41", "f23", "f24", "i52", "p32",
		"some_event", "u1", "u2", "u4", "u5", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "u:0",
	), "some_event")

	// test sword_filter_engaged: if activated, orders that were given to units by automata are not displayed
	f.checkFilters(c, parseFilters(c,
		"sword_filter_engaged", "false",
	), "a11", "a12", "a13", "a14", "af11", "c41",
		"c42", "cf41", "f21", "f22", "f23", "f24",
		"i51", "i52", "p31", "p32", "some_event",
		"u1", "u2", "u3", "u4", "u5", "uf1")
	f.checkFilters(c, parseFilters(c,
		"sword_filter_engaged", "true",
	), "a11", "a12", "a13", "a14", "af11", "c41",
		"c42", "cf41", "f21", "f22", "f23", "f24",
		"i51", "i52", "p31", "p32", "some_event",
		"u1", "uf1")

	// test filter_keyword
	f.checkFilters(c, parseFilters(c,
		"filter_keyword", "u",
	), "u1", "u2", "u3", "u4", "u5", "uf1")
	f.checkFilters(c, parseFilters(c,
		"filter_keyword", " U ",
	), "u1", "u2", "u3", "u4", "u5", "uf1")
	f.checkFilters(c, parseFilters(c,
		"filter_keyword", "p32",
	), "p32")
	f.checkFilters(c, parseFilters(c,
		"filter_keyword", "u\\d",
	))
}

func (t *TestSuite) TestServiceFilters(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	id := uuid.New()
	_, err := f.controller.CreateEvent(f.session, &sdk.Event{
		Uuid:  proto.String(id),
		Name:  proto.String("some_event"),
		Begin: proto.String(util.FormatTime(f.begin)),
	})
	c.Assert(err, IsNil)

	f.addSwordEvent(c, "event1", "some_name", []byte("some_payload"))
	f.addSwordEvent(c, "event2", "some_name", []byte("some_payload"))

	f.addMarkerEvent(c, "marker1", "uuid1", []byte("some_payload"))
	f.addMarkerEvent(c, "marker2", "uuid1", []byte("some_payload"))
	f.addMarkerEvent(c, "marker3", "uuid1", []byte("some_payload"))

	f.addMarkerEvent(c, "marker4", "uuid2", []byte("some_payload"))
	f.addMarkerEvent(c, "marker5", "uuid2", []byte("some_payload"))
	f.addMarkerEvent(c, "marker6", "uuid2", []byte("some_payload"))
	f.addMarkerEvent(c, "marker7", "uuid2", []byte("some_payload"))

	// no markers allowed
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:0",
	), "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:0",
	), "event1", "event2")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:0",
	), "event1", "event2", "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:0",
	))

	// all markers allowed
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:*",
	), "marker1", "marker2", "marker3", "marker4",
		"marker5", "marker6", "marker7", "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:*",
	), "event1", "event2", "marker1", "marker2",
		"marker3", "marker4", "marker5", "marker6", "marker7")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:*",
	), "event1", "event2", "marker1", "marker2", "marker3",
		"marker4", "marker5", "marker6", "marker7", "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:*",
	), "marker1", "marker2", "marker3", "marker4",
		"marker5", "marker6", "marker7")

	// filter markers with uuid1
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:uuid1",
	), "marker1", "marker2", "marker3", "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:uuid1",
	), "event1", "event2", "marker1", "marker2", "marker3")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:uuid1",
	), "event1", "event2", "marker1", "marker2", "marker3", "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:uuid1",
	), "marker1", "marker2", "marker3")

	// filter markers with uuid2
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:uuid2",
	), "marker4", "marker5", "marker6", "marker7", "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:uuid2",
	), "event1", "event2", "marker4", "marker5", "marker6", "marker7")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:uuid2",
	), "event1", "event2", "marker4", "marker5", "marker6",
		"marker7", "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:uuid2",
	), "marker4", "marker5", "marker6", "marker7")

	// filter markers with other uuid
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:other_uuid",
	), "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:other_uuid",
	), "event1", "event2")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:other_uuid",
	), "event1", "event2", "some_event")
	f.checkFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:other_uuid",
	))
}

func (t *TestSuite) TestIncompleteMissionsAreVisibleByAnyProfile(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.WaitConnected()
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_31_only"),
		ReadOnlyParties: swapi.MakeIdList(31),
		Supervisor:      proto.Bool(false),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 1
	})

	f.addSwordEvent(c, "u1", "some_name", f.getSomeUnitOrder(c, 0, 0))
	f.addSwordEvent(c, "a11", "some_name", f.getSomeAutomatOrder(c, 0))
	f.addSwordEvent(c, "c41", "some_name", f.getSomeCrowdOrder(c, 0))
	f.addSwordEvent(c, "u2", "some_name", f.getSomeFragOrder(c, swapi.MakeUnitTasker(0)))
	f.addSwordEvent(c, "a12", "some_name", f.getSomeFragOrder(c, swapi.MakeAutomatTasker(0)))
	f.addSwordEvent(c, "f21", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(0)))
	f.addSwordEvent(c, "p31", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(0)))
	f.addSwordEvent(c, "c42", "some_name", f.getSomeFragOrder(c, swapi.MakeCrowdTasker(0)))
	f.addSwordEvent(c, "i51", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(0)))

	f.checkFilters(c, services.EventFilterConfig{},
		"a11", "a12", "c41", "c42", "f21",
		"i51", "p31", "u1", "u2")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_only",
	), "a11", "a12", "c41", "c42", "f21",
		"i51", "p31", "u1", "u2")
}

func (t *TestSuite) TestFiltersOnMagicActions(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.WaitConnected()
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_31_only"),
		ReadOnlyParties: swapi.MakeIdList(31),
		Supervisor:      proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_32_only"),
		ReadOnlyParties: swapi.MakeIdList(32),
		Supervisor:      proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:            proto.String("party_31_supervisor"),
		ReadWriteParties: swapi.MakeIdList(31),
		Supervisor:       proto.Bool(true),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:            proto.String("supervisor"),
		ReadWriteParties: swapi.MakeIdList(31, 32),
		Supervisor:       proto.Bool(true),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 4
	})

	// abuse wait command to create hierarchy
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		addParty(c, d, 31)
		addParty(c, d, 32)
		addFormation(c, d, 21, 31, 0)
		addFormation(c, d, 22, 32, 0)
		addAutomat(c, d, 11, 21)
		addAutomat(c, d, 12, 22)
		addUnit(c, d, 1, 11)
		addUnit(c, d, 2, 12)
		addCrowd(c, d, 41, 31)
		addCrowd(c, d, 42, 32)
		addPopulation(c, d, 51, 31)
		addPopulation(c, d, 52, 32)
		addObject(c, d, 61, 31)
		addObject(c, d, 62, 32)
		addKnowledgeGroup(c, d, 71, 31)
		addKnowledgeGroup(c, d, 72, 32)
		return true
	})

	f.addSwordEvent(c, "ma1", "some_name", f.getSomeMagicAction(c))
	f.addSwordEvent(c, "uma1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeUnitTasker(1)))
	f.addSwordEvent(c, "ama11", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeAutomatTasker(11)))
	f.addSwordEvent(c, "cma41", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeCrowdTasker(41)))
	f.addSwordEvent(c, "ima51", "some_name", f.getSomeUnitMagicAction(c, swapi.MakePopulationTasker(51)))
	f.addSwordEvent(c, "cuma1", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(1)))
	f.addSwordEvent(c, "cuma2", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(2)))
	f.addSwordEvent(c, "oma61", "some_name", f.getSomeObjectMagicAction(c, 61))
	f.addSwordEvent(c, "kma71", "some_name", f.getSomeKnowledgeMagicAction(c, 71))
	f.addSwordEvent(c, "amma11", "some_name", f.getSomeAutomatModeMagicAction(c, 11))

	f.addSwordEvent(c, "i0", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(0)))

	f.checkFilters(c, services.EventFilterConfig{},
		"ama11", "amma11", "cma41", "cuma1", "cuma2",
		"i0", "ima51", "kma71", "ma1", "oma61", "uma1")

	// test sword_profile
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_only",
	), "amma11", "cuma1", "i0")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_32_only",
	), "cuma2", "i0")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_supervisor",
	), "ama11", "amma11", "cma41", "cuma1", "i0",
		"ima51", "kma71", "ma1", "oma61", "uma1")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "supervisor",
	), "ama11", "amma11", "cma41", "cuma1", "cuma2",
		"i0", "ima51", "kma71", "ma1", "oma61", "uma1")
	// combine profile & read_only
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_only",
		"sword_read_only", "false",
	), "i0")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_32_only",
		"sword_read_only", "false",
	), "i0")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_supervisor",
		"sword_read_only", "false",
	), "ama11", "amma11", "cma41", "cuma1", "i0",
		"ima51", "kma71", "ma1", "oma61", "uma1")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "supervisor",
		"sword_read_only", "false",
	), "ama11", "amma11", "cma41", "cuma1", "cuma2",
		"i0", "ima51", "kma71", "ma1", "oma61", "uma1")
	// combine both profile & custom profile
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_supervisor",
		"sword_filter", "i:51",
	), "i0", "ima51", "ma1")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_only",
		"sword_filter", "i:51",
	), "i0")
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
	f.checkFilters(c, services.EventFilterConfig{},
		"child1", "child2", "child3", "child4", "child5",
		"child6", "parent1", "parent2", "parent3")

	// test filter_show_only
	f.checkFilters(c, parseFilters(c,
		"filter_show_only", "",
	), "child1", "child2", "child3", "child4", "child5",
		"child6", "parent1", "parent2", "parent3")
	f.checkFilters(c, parseFilters(c,
		"filter_show_only", "unexisting parent",
	))
	f.checkFilters(c, parseFilters(c,
		"filter_show_only", "parent1",
	), "child1", "parent1")
	f.checkFilters(c, parseFilters(c,
		"filter_show_only", "parent2",
	), "child2", "child3", "parent2")
	f.checkFilters(c, parseFilters(c,
		"filter_show_only", "parent3",
	), "child4", "child5", "child6", "parent3")
	// parent1 is hidden so child1 is too
	f.checkFilters(c, parseFilters(c,
		"filter_show_only", "child1",
	))

	// test filter_hide_hierarchies
	f.checkFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent1",
	), "child2", "child3", "child4", "child5",
		"child6", "parent1", "parent2", "parent3")
	f.checkFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent2",
	), "child1", "child4", "child5", "child6",
		"parent1", "parent2", "parent3")
	f.checkFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent3",
	), "child1", "child2", "child3", "parent1",
		"parent2", "parent3")
	f.checkFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent1,parent2",
	), "child4", "child5", "child6", "parent1",
		"parent2", "parent3")
	f.checkFilters(c, parseFilters(c,
		"filter_hide_hierarchies", "parent1,parent2,parent3",
	), "parent1", "parent2", "parent3")
}

func (t *TestSuite) TestFiltersMetadata(c *C) {
	f := t.MakeFixture(c, true)
	defer f.Close()

	f.WaitConnected()
	f.server.CreateProfile(&sword.Profile{
		Login:           proto.String("party_31_only"),
		ReadOnlyParties: swapi.MakeIdList(31),
		Supervisor:      proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:              proto.String("formation_22_only"),
		ReadOnlyFormations: swapi.MakeIdList(22),
		Supervisor:         proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:              proto.String("formation_23_only"),
		ReadOnlyFormations: swapi.MakeIdList(23),
		Supervisor:         proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:             proto.String("automat_14_only"),
		ReadOnlyAutomates: swapi.MakeIdList(14),
		Supervisor:        proto.Bool(false),
	})
	f.server.CreateProfile(&sword.Profile{
		Login:          proto.String("crowd_46_only"),
		ReadOnlyCrowds: swapi.MakeIdList(46),
		Supervisor:     proto.Bool(false),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 5
	})

	// abuse wait command to create hierarchy
	//    p31
	//  /  |  \
	// f22 c46 i57
	// |
	// f23
	// |
	// a14
	// |
	// u5
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		addParty(c, d, 31)
		addFormation(c, d, 22, 31, 0)
		addFormation(c, d, 23, 0, 22)
		addAutomat(c, d, 14, 23)
		addUnit(c, d, 5, 14)
		addCrowd(c, d, 46, 31)
		addPopulation(c, d, 57, 31)
		return true
	})

	f.addTaskEvent(c, "task_alone", f.begin, f.begin.Add(1*time.Hour), "")
	f.addTaskEvent(c, "task_invalid_unit", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":0}")
	f.addTaskEvent(c, "task_p31", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":31}")
	f.addTaskEvent(c, "task_f22", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":22}")
	f.addTaskEvent(c, "task_f23", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":23}")
	f.addTaskEvent(c, "task_a14", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":14}")
	f.addTaskEvent(c, "task_u5", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":5}")
	f.addTaskEvent(c, "task_c46", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":46}")
	event := f.addTaskEvent(c, "task_i57", f.begin, f.begin.Add(1*time.Hour), "{\"sword_entity\":57}")

	// ensure we do get all our events
	f.checkFilters(c, services.EventFilterConfig{},
		"task_a14", "task_alone", "task_c46",
		"task_f22", "task_f23", "task_i57",
		"task_invalid_unit", "task_p31", "task_u5")
	// test sword_profile
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "party_31_only",
	), "task_a14", "task_alone", "task_c46",
		"task_f22", "task_f23", "task_i57",
		"task_invalid_unit", "task_p31", "task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "formation_22_only",
	), "task_a14", "task_alone", "task_f22",
		"task_f23", "task_invalid_unit", "task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "formation_23_only",
	), "task_a14", "task_alone", "task_f23",
		"task_invalid_unit", "task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "automat_14_only",
	), "task_a14", "task_alone", "task_invalid_unit",
		"task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "crowd_46_only",
	), "task_alone", "task_c46", "task_invalid_unit")

	// test sword_filter
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "p:31",
	), "task_a14", "task_alone", "task_c46",
		"task_f22", "task_f23", "task_i57",
		"task_invalid_unit", "task_p31", "task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "f:22",
	), "task_a14", "task_alone", "task_f22",
		"task_f23", "task_invalid_unit", "task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "f:23",
	), "task_a14", "task_alone", "task_f23",
		"task_invalid_unit", "task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "a:14",
	), "task_a14", "task_alone", "task_invalid_unit",
		"task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "u:5",
	), "task_alone", "task_invalid_unit", "task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "c:46",
	), "task_alone", "task_c46", "task_invalid_unit")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "i:57",
	), "task_alone", "task_i57", "task_invalid_unit")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "u:5,i:57,c:46",
	), "task_alone", "task_c46", "task_i57",
		"task_invalid_unit", "task_u5")

	// remove event metadata and try again
	event.Metadata = nil
	_, err := f.controller.UpdateEvent(f.session, event.GetUuid(), event)
	c.Assert(err, IsNil)
	f.checkFilters(c, parseFilters(c,
		"sword_profile", "automat_14_only",
	), "task_a14", "task_alone", "task_i57",
		"task_invalid_unit", "task_u5")
	f.checkFilters(c, parseFilters(c,
		"sword_filter", "a:14",
	), "task_a14", "task_alone", "task_i57",
		"task_invalid_unit", "task_u5")
}
