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

	// read-only automat
	u := &Unit{
		Id:        1,
		AutomatId: 1,
	}
	d.Units[u.Id] = u
	fill(&p.ReadOnlyAutomats, 1)
	c.Assert(d.IsUnitInProfile(1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, true)
	fill(&p.ReadOnlyAutomats)

	// read-write automat
	fill(&p.ReadWriteAutomats, 1)
	c.Assert(d.IsUnitInProfile(1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, true)
	fill(&p.ReadWriteAutomats)

	// unknown automat
	c.Assert(d.IsUnitInProfile(1, p), Equals, false)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, false)

	// read-only formation
	a := &Automat{
		Id:          1,
		FormationId: 1,
	}
	d.Automats[a.Id] = a
	fill(&p.ReadOnlyFormations, 1)
	c.Assert(d.IsUnitInProfile(1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(1, p), Equals, true)
	fill(&p.ReadOnlyFormations)

	// read-write formation
	fill(&p.ReadWriteFormations, 1)
	c.Assert(d.IsUnitInProfile(1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(1, p), Equals, true)
	fill(&p.ReadWriteFormations)

	// unknown formation
	c.Assert(d.IsUnitInProfile(1, p), Equals, false)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(1, p), Equals, false)

	// read-only parent formation
	f1 := &Formation{
		Id:       1,
		ParentId: 2,
	}
	d.Formations[f1.Id] = f1
	fill(&p.ReadOnlyFormations, 2)
	c.Assert(d.IsUnitInProfile(1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(2, p), Equals, true)
	fill(&p.ReadOnlyFormations)

	// read-write parent formation
	fill(&p.ReadWriteFormations, 2)
	c.Assert(d.IsUnitInProfile(1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(2, p), Equals, true)
	fill(&p.ReadWriteFormations)

	// unknown parent formation
	c.Assert(d.IsUnitInProfile(1, p), Equals, false)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(2, p), Equals, false)

	// unknown population
	c.Assert(d.IsPopulationInProfile(1, p), Equals, false)

	// unknown object
	c.Assert(d.IsObjectInProfile(1, p), Equals, false)

	// unknown knowledge group
	c.Assert(d.IsKnowledgeGroupInProfile(1, p), Equals, false)

	// read-only crowd
	fill(&p.ReadOnlyCrowds, 1)
	c.Assert(d.IsCrowdInProfile(1, p), Equals, true)
	fill(&p.ReadOnlyCrowds)

	// read-write crowd
	fill(&p.ReadWriteCrowds, 1)
	c.Assert(d.IsCrowdInProfile(1, p), Equals, true)
	fill(&p.ReadWriteCrowds)

	// unknown crowd
	c.Assert(d.IsCrowdInProfile(1, p), Equals, false)

	// read-only party
	f2 := &Formation{
		Id:      2,
		PartyId: 1,
	}
	d.Formations[f2.Id] = f2
	p0 := &Population{
		Id:      1,
		PartyId: 1,
	}
	d.Populations[p0.Id] = p0
	c0 := &Crowd{
		Id:      1,
		PartyId: 1,
	}
	d.Crowds[c0.Id] = c0
	kg0 := &KnowledgeGroup{
		Id:      1,
		PartyId: 1,
	}
	d.KnowledgeGroups[kg0.Id] = kg0
	o1 := &Object{
		Id:      1,
		PartyId: 1,
	}
	d.Objects[o1.Id] = o1
	o2 := &Object{
		Id:      2,
		PartyId: 0,
	}
	d.Objects[o2.Id] = o2

	fill(&p.ReadOnlyParties, 1)
	c.Assert(d.IsUnitInProfile(1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(2, p), Equals, true)
	c.Assert(d.IsPopulationInProfile(1, p), Equals, true)
	c.Assert(d.IsCrowdInProfile(1, p), Equals, true)
	c.Assert(d.IsPartyInProfile(1, p), Equals, true)
	c.Assert(d.IsKnowledgeGroupInProfile(1, p), Equals, true)
	c.Assert(d.IsObjectInProfile(1, p), Equals, true)
	c.Assert(d.IsObjectInProfile(2, p), Equals, true)
	fill(&p.ReadOnlyParties)

	// read-write party
	fill(&p.ReadWriteParties, 1)
	c.Assert(d.IsUnitInProfile(1, p), Equals, true)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(1, p), Equals, true)
	c.Assert(d.IsFormationInProfile(2, p), Equals, true)
	c.Assert(d.IsPopulationInProfile(1, p), Equals, true)
	c.Assert(d.IsCrowdInProfile(1, p), Equals, true)
	c.Assert(d.IsPartyInProfile(1, p), Equals, true)
	c.Assert(d.IsKnowledgeGroupInProfile(1, p), Equals, true)
	c.Assert(d.IsObjectInProfile(1, p), Equals, true)
	c.Assert(d.IsObjectInProfile(2, p), Equals, true)
	fill(&p.ReadWriteParties)

	// unknown party
	c.Assert(d.IsUnitInProfile(1, p), Equals, false)
	c.Assert(d.IsAutomatInProfile(1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(1, p), Equals, false)
	c.Assert(d.IsFormationInProfile(2, p), Equals, false)
	c.Assert(d.IsPopulationInProfile(1, p), Equals, false)
	c.Assert(d.IsCrowdInProfile(1, p), Equals, false)
	c.Assert(d.IsPartyInProfile(1, p), Equals, false)
	c.Assert(d.IsKnowledgeGroupInProfile(1, p), Equals, false)
	c.Assert(d.IsObjectInProfile(1, p), Equals, false)
	c.Assert(d.IsObjectInProfile(2, p), Equals, true) // object without party is available for any profile
}

func (s *TestSuite) TestIsUrbanInProfile(c *C) {
	model := NewModel()
	defer model.Close()
	d := model.data
	p := &Profile{Login: "noname"}
	d.Profiles[p.Login] = p
	urban := NewUrban(1, "noname", nil)
	d.Objects[urban.Id] = urban
	c.Assert(d.IsObjectInProfile(1, p), Equals, true)
}
