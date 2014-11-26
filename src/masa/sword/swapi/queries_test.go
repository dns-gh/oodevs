// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swapi

import (
	"flag"
	. "launchpad.net/gocheck"
	"testing"
)

func init() {
	flag.String("application", "", "")
	flag.String("root-dir", "", "")
	flag.String("run-dir", "", "")
	flag.String("exercises-dir", "", "")
	flag.Int("test-port", 0, "")
	flag.Bool("show-log", false, "")
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func fill(dst *map[uint32]struct{}, ids ...uint32) {
	*dst = map[uint32]struct{}{}
	for _, id := range ids {
		(*dst)[id] = struct{}{}
	}
}

var (
	hids uint32 = 0
)

func addHierarchy(d *ModelData, id, parent uint32) {
	hids++
	d.Hierarchies[id] = Hierarchies{
		Hierarchy{
			Id:     hids,
			Entity: id,
			Parent: parent,
		},
	}
}

func (s *TestSuite) TestIsSomethingInProfile(c *C) {
	model := NewModel()
	defer model.Close()

	// unknown unit
	d := model.data
	p := &Profile{
		Login: "noname",
	}
	fill(&p.ReadOnlyAutomats)
	fill(&p.ReadWriteAutomats)
	fill(&p.ReadOnlyFormations)
	fill(&p.ReadWriteFormations)
	fill(&p.ReadOnlyParties)
	fill(&p.ReadWriteParties)
	fill(&p.ReadOnlyCrowds)
	fill(&p.ReadWriteCrowds)
	d.Profiles[p.Login] = p
	c.Assert(d.IsUnitInProfile(0, p), Equals, false)

	// add hierarchy
	const (
		party1 = iota + 1
		formation1
		formation2
		automat1
		unit1
		population1
		crowd1
		object1
		object2
		knowledge1
	)
	addHierarchy(d, party1, 0)
	addHierarchy(d, formation1, formation2)
	addHierarchy(d, formation2, party1)
	addHierarchy(d, automat1, formation1)
	addHierarchy(d, unit1, automat1)
	addHierarchy(d, population1, party1)
	addHierarchy(d, crowd1, party1)
	addHierarchy(d, object1, party1)
	addHierarchy(d, object2, 0)
	addHierarchy(d, knowledge1, party1)

	// read-only automat
	fill(&p.ReadOnlyAutomats, automat1)
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, true)
	fill(&p.ReadOnlyAutomats)

	// read-write automat
	fill(&p.ReadWriteAutomats, automat1)
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, true)
	fill(&p.ReadWriteAutomats)

	// unknown automat
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, false)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, false)

	// read-only formation

	fill(&p.ReadOnlyFormations, formation1)
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, true)
	fill(&p.ReadOnlyFormations)

	// read-write formation
	fill(&p.ReadWriteFormations, formation1)
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, true)
	fill(&p.ReadWriteFormations)

	// unknown formation
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, false)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, false)

	// read-only parent formation
	fill(&p.ReadOnlyFormations, formation2)
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation2, p), Equals, true)
	fill(&p.ReadOnlyFormations)

	// read-write parent formation
	fill(&p.ReadWriteFormations, formation2)
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation2, p), Equals, true)
	fill(&p.ReadWriteFormations)

	// unknown parent formation
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, false)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(formation2, p), Equals, false)

	// unknown population
	c.Assert(d.IsPopulationInProfile(population1, p), Equals, false)

	// unknown object
	c.Assert(d.IsObjectInProfile(object1, p), Equals, false)

	// unknown knowledge group
	c.Assert(d.IsKnowledgeGroupInProfile(knowledge1, p), Equals, false)

	// read-only crowd
	fill(&p.ReadOnlyCrowds, crowd1)
	c.Assert(d.IsCrowdInProfile(crowd1, p), Equals, true)
	fill(&p.ReadOnlyCrowds)

	// read-write crowd
	fill(&p.ReadWriteCrowds, crowd1)
	c.Assert(d.IsCrowdInProfile(crowd1, p), Equals, true)
	fill(&p.ReadWriteCrowds)

	// unknown crowd
	c.Assert(d.IsCrowdInProfile(crowd1, p), Equals, false)

	// read-only party
	fill(&p.ReadOnlyParties, party1)
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation2, p), Equals, true)
	c.Assert(d.IsPopulationInProfile(population1, p), Equals, true)
	c.Assert(d.IsCrowdInProfile(crowd1, p), Equals, true)
	c.Assert(d.IsPartyInProfile(party1, p), Equals, true)
	c.Assert(d.IsKnowledgeGroupInProfile(knowledge1, p), Equals, true)
	c.Assert(d.IsObjectInProfile(object1, p), Equals, true)
	c.Assert(d.IsObjectInProfile(object2, p), Equals, true)
	fill(&p.ReadOnlyParties)

	// read-write party
	fill(&p.ReadWriteParties, party1)
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(formation2, p), Equals, true)
	c.Assert(d.IsPopulationInProfile(population1, p), Equals, true)
	c.Assert(d.IsCrowdInProfile(crowd1, p), Equals, true)
	c.Assert(d.IsPartyInProfile(party1, p), Equals, true)
	c.Assert(d.IsKnowledgeGroupInProfile(knowledge1, p), Equals, true)
	c.Assert(d.IsObjectInProfile(object1, p), Equals, true)
	c.Assert(d.IsObjectInProfile(object2, p), Equals, true)
	fill(&p.ReadWriteParties)

	// unknown party
	c.Assert(d.IsUnitInProfile(unit1, p), Equals, false)
	c.Assert(d.IsAutomatInProfile(automat1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(formation1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(formation2, p), Equals, false)
	c.Assert(d.IsPopulationInProfile(population1, p), Equals, false)
	c.Assert(d.IsCrowdInProfile(crowd1, p), Equals, false)
	c.Assert(d.IsPartyInProfile(party1, p), Equals, false)
	c.Assert(d.IsKnowledgeGroupInProfile(knowledge1, p), Equals, false)
	c.Assert(d.IsObjectInProfile(object1, p), Equals, false)
	c.Assert(d.IsObjectInProfile(object2, p), Equals, true) // object without party is available for any profile
}
