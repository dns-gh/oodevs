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

func parseFiltersWith(c *C, filter *FakeSwordFilter) services.EventFilterConfig {
	cfg, err := ParseEventFilterConfig(filter)
	c.Assert(err, IsNil)
	return cfg
}

func parseFilters(c *C, key, value string) services.EventFilterConfig {
	return parseFiltersWith(c, &FakeSwordFilter{
		data: map[string]string{
			key: value,
		},
	})
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
	if len(events) != count {
		c.Fatal(count, " != len of ", swtest.Stringify(events))
	}
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
	f.server.CreateProfile(&sword.Profile{
		Login:               proto.String("f1_read_f3_write"),
		ReadOnlyFormations:  swapi.MakeIdList(1),
		ReadWriteFormations: swapi.MakeIdList(3),
		Supervisor:          proto.Bool(false),
	})
	f.sword.WaitFor(func(d *swapi.ModelData) bool {
		return len(d.Profiles) == 6
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
	f.addSwordEvent(c, "u1", "some_name", f.getSomeUnitOrder(c, 1, 0)) // event without automat
	f.addSwordEvent(c, "u2", "some_name", f.getSomeUnitOrder(c, 2, 2))
	f.addSwordEvent(c, "u3", "some_name", f.getSomeUnitOrder(c, 3, 3))
	f.addSwordEvent(c, "u4", "some_name", f.getSomeUnitOrder(c, 4, 4))
	f.addSwordEvent(c, "u5", "some_name", f.getSomeUnitOrder(c, 5, 5))
	f.addSwordEvent(c, "a1", "some_name", f.getSomeAutomatOrder(c, 1))
	f.addSwordEvent(c, "a2", "some_name", f.getSomeAutomatOrder(c, 2))
	f.addSwordEvent(c, "a3", "some_name", f.getSomeAutomatOrder(c, 3))
	f.addSwordEvent(c, "a4", "some_name", f.getSomeAutomatOrder(c, 4))
	f.addSwordEvent(c, "f1", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(1)))
	f.addSwordEvent(c, "f2", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(2)))
	f.addSwordEvent(c, "f3", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(3)))
	f.addSwordEvent(c, "f4", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(4)))
	f.addSwordEvent(c, "c1", "some_name", f.getSomeCrowdOrder(c, 1))
	f.addSwordEvent(c, "c2", "some_name", f.getSomeCrowdOrder(c, 2))
	f.addSwordEvent(c, "i1", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(1)))
	f.addSwordEvent(c, "i2", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(2)))
	f.addSwordEvent(c, "p1", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(1)))
	f.addSwordEvent(c, "p2", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(2)))
	f.addSwordEvent(c, "uf1", "some_name", f.getSomeFragOrder(c, swapi.MakeUnitTasker(1)))
	f.addSwordEvent(c, "af1", "some_name", f.getSomeFragOrder(c, swapi.MakeAutomatTasker(1)))
	f.addSwordEvent(c, "cf1", "some_name", f.getSomeFragOrder(c, swapi.MakeCrowdTasker(1)))
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
		"sword_profile": "f1_read_f3_write",
	}, 11+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile": "crowd_1_only",
	}, 2+1)

	// test sword_write_only
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile":    "party_1_only",
		"sword_write_only": true,
	}, 0+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile":    "f1_read_f3_write",
		"sword_write_only": true,
	}, 3+1)
	f.applyFilters(c, services.EventFilterConfig{
		"sword_profile":    "f1_read_f3_write",
		"sword_write_only": false,
	}, 11+1)

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
	f.applyFilters(c, parseFilters(c,
		"sword_filter", "u:0"), 1)

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
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:0"), 1)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:0"), 2)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:0"), 1+2)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:0"), 0)

	// all markers allowed
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:*"), 1+3+4)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:*"), 2+3+4)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:*"), 1+2+3+4)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:*"), 0+3+4)

	// filter markers with uuid1
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:uuid1"), 1+3)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:uuid1"), 2+3)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:uuid1"), 1+2+3)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:uuid1"), 0+3)

	// filter markers with uuid2
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:uuid2"), 1+4)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:uuid2"), 2+4)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:uuid2"), 1+2+4)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:uuid2"), 0+4)

	// filter markers with other uuid
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:0,marker:other_uuid"), 1)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:*,marker:other_uuid"), 2)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:*,sword:*,marker:other_uuid"), 1+2)
	f.applyFilters(c, parseFilters(c,
		"filter_service", "none:0,sword:0,marker:other_uuid"), 0)
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

	f.addSwordEvent(c, "u1", "some_name", f.getSomeUnitOrder(c, 0, 0))
	f.addSwordEvent(c, "a1", "some_name", f.getSomeAutomatOrder(c, 0))
	f.addSwordEvent(c, "c1", "some_name", f.getSomeCrowdOrder(c, 0))
	f.addSwordEvent(c, "u2", "some_name", f.getSomeFragOrder(c, swapi.MakeUnitTasker(0)))
	f.addSwordEvent(c, "a2", "some_name", f.getSomeFragOrder(c, swapi.MakeAutomatTasker(0)))
	f.addSwordEvent(c, "f1", "some_name", f.getSomeFragOrder(c, swapi.MakeFormationTasker(0)))
	f.addSwordEvent(c, "p1", "some_name", f.getSomeFragOrder(c, swapi.MakePartyTasker(0)))
	f.addSwordEvent(c, "c2", "some_name", f.getSomeFragOrder(c, swapi.MakeCrowdTasker(0)))
	f.addSwordEvent(c, "i1", "some_name", f.getSomeFragOrder(c, swapi.MakePopulationTasker(0)))

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

	f.addSwordEvent(c, "ma1", "some_name", f.getSomeMagicAction(c))
	f.addSwordEvent(c, "uma1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeUnitTasker(1)))
	f.addSwordEvent(c, "ama1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeAutomatTasker(1)))
	f.addSwordEvent(c, "cma1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakeCrowdTasker(1)))
	f.addSwordEvent(c, "ima1", "some_name", f.getSomeUnitMagicAction(c, swapi.MakePopulationTasker(1)))
	f.addSwordEvent(c, "cuma1", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(1)))
	f.addSwordEvent(c, "cuma2", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(2)))
	f.addSwordEvent(c, "oma1", "some_name", f.getSomeObjectMagicAction(c, 1))
	f.addSwordEvent(c, "kma1", "some_name", f.getSomeKnowledgeMagicAction(c, 1))
	f.addSwordEvent(c, "amma1", "some_name", f.getSomeAutomatModeMagicAction(c, 1))

	f.addSwordEvent(c, "i0", "some_name", f.getSomeClientUnitMagicAction(c, swapi.MakeUnitTasker(0)))

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
	// combine both profile & custom profile
	f.applyFilters(c, parseFiltersWith(c, &FakeSwordFilter{
		data: map[string]string{
			"sword_profile": "party_1_supervisor",
			"sword_filter":  "i:1",
		},
	}), 1+2) // ima1 + ma1 + i0
	f.applyFilters(c, parseFiltersWith(c, &FakeSwordFilter{
		data: map[string]string{
			"sword_profile": "party_1_only",
			"sword_filter":  "i:1",
		},
	}), 0+1) // i0
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
