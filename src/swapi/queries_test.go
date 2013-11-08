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
	flag.String("projectRoot", "", "")
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
	c.Assert(model.IsUnitInProfile(0, "noname"), Equals, false)

	// read-only automat
	u := &Unit{
		Id:        1,
		AutomatId: 1,
	}
	d.Units[u.Id] = u
	fill(&p.ReadOnlyAutomats, 1)
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, true)
	fill(&p.ReadOnlyAutomats)

	// read-write automat
	fill(&p.ReadWriteAutomats, 1)
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, true)
	fill(&p.ReadWriteAutomats)

	// unknown automat
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, false)

	// read-only formation
	a := &Automat{
		Id:          1,
		FormationId: 1,
	}
	d.Automats[a.Id] = a
	fill(&p.ReadOnlyFormations, 1)
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, true)
	fill(&p.ReadOnlyFormations)

	// read-write formation
	fill(&p.ReadWriteFormations, 1)
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, true)
	fill(&p.ReadWriteFormations)

	// unknown formation
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, false)

	// read-only parent formation
	f1 := &Formation{
		Id:       1,
		ParentId: 2,
	}
	d.Formations[f1.Id] = f1
	fill(&p.ReadOnlyFormations, 2)
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(2, "noname"), Equals, true)
	fill(&p.ReadOnlyFormations)

	// read-write parent formation
	fill(&p.ReadWriteFormations, 2)
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(2, "noname"), Equals, true)
	fill(&p.ReadWriteFormations)

	// unknown parent formation
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsFormationInProfile(2, "noname"), Equals, false)

	// unknown population
	c.Assert(model.IsPopulationInProfile(1, "noname"), Equals, false)

	// read-only crowd
	fill(&p.ReadOnlyCrowds, 1)
	c.Assert(model.IsCrowdInProfile(1, "noname"), Equals, true)
	fill(&p.ReadOnlyCrowds)

	// read-write crowd
	fill(&p.ReadWriteCrowds, 1)
	c.Assert(model.IsCrowdInProfile(1, "noname"), Equals, true)
	fill(&p.ReadWriteCrowds)

	// unknown crowd
	c.Assert(model.IsCrowdInProfile(1, "noname"), Equals, false)

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
	fill(&p.ReadOnlyParties, 1)
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(2, "noname"), Equals, true)
	c.Assert(model.IsPopulationInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsCrowdInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsPartyInProfile(1, "noname"), Equals, true)
	fill(&p.ReadOnlyParties)

	// read-write party
	fill(&p.ReadWriteParties, 1)
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsFormationInProfile(2, "noname"), Equals, true)
	c.Assert(model.IsPopulationInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsCrowdInProfile(1, "noname"), Equals, true)
	c.Assert(model.IsPartyInProfile(1, "noname"), Equals, true)
	fill(&p.ReadWriteParties)

	// unknown profiles
	fill(&p.ReadWriteParties, 1)
	c.Assert(model.IsUnitInProfile(1, "bad"), Equals, false)
	c.Assert(model.IsAutomatInProfile(1, "bad"), Equals, false)
	c.Assert(model.IsFormationInProfile(1, "bad"), Equals, false)
	c.Assert(model.IsFormationInProfile(2, "bad"), Equals, false)
	c.Assert(model.IsPopulationInProfile(1, "bad"), Equals, false)
	c.Assert(model.IsCrowdInProfile(1, "bad"), Equals, false)
	c.Assert(model.IsPartyInProfile(1, "bad"), Equals, false)
	fill(&p.ReadWriteParties)

	// unknown party
	c.Assert(model.IsUnitInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsAutomatInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsFormationInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsFormationInProfile(2, "noname"), Equals, false)
	c.Assert(model.IsPopulationInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsCrowdInProfile(1, "noname"), Equals, false)
	c.Assert(model.IsPartyInProfile(1, "noname"), Equals, false)
}
