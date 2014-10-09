// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package simtests

import (
	. "launchpad.net/gocheck"
)

func (s *TestSuite) TestCreateOrbat(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallTest))
	defer stopSimAndClient(c, sim, client)
	phydb := loadPhysicalData(c, "test")

	party := &Party{
		Name: "party1",
		Formations: []*Formation{
			&Formation{
				Name: "formation-1",
				Formations: []*Formation{
					&Formation{
						Name: "formation-1.1",
						Automats: []*Automat{
							&Automat{
								Name: "automat",
								Type: "Maintenance Log Automat 2",
								Units: []*Unit{
									&Unit{
										Name: "unit",
										Type: "Maintenance Log Unit 3",
									},
								},
							},
						},
					},
				},
			},
		},
	}
	// First run should create everything which can be created
	err := FindOrCreateEntities(client, phydb, party)
	c.Assert(err, IsNil)
	c.Assert(party.Entity, NotNil)
	c.Assert(len(party.Formations), Greater, 0)
	c.Assert(party.Formations[0].Entity, NotNil)
	c.Assert(len(party.Formations[0].Formations), Greater, 0)
	c.Assert(party.Formations[0].Formations[0].Entity, NotNil)
	c.Assert(party.Formations[0].Formations[0].Created, Equals, true)
	c.Assert(len(party.Formations[0].Formations[0].Automats), Greater, 0)
	c.Assert(party.Formations[0].Formations[0].Automats[0].Entity, NotNil)
	c.Assert(party.Formations[0].Formations[0].Automats[0].Created, Equals, true)
	c.Assert(len(party.Formations[0].Formations[0].Automats[0].Units), Greater, 0)
	c.Assert(party.Formations[0].Formations[0].Automats[0].Units[0].Entity, NotNil)
	c.Assert(party.Formations[0].Formations[0].Automats[0].Units[0].Created, Equals, true)

	// Second run should not create anything
	err = FindOrCreateEntities(client, phydb, party)
	c.Assert(err, IsNil)
	c.Assert(party.Entity, NotNil)
	c.Assert(len(party.Formations), Greater, 0)
	c.Assert(party.Formations[0].Entity, NotNil)
	c.Assert(len(party.Formations[0].Formations), Greater, 0)
	c.Assert(party.Formations[0].Formations[0].Entity, NotNil)
	c.Assert(party.Formations[0].Formations[0].Created, Equals, false)
	c.Assert(len(party.Formations[0].Formations[0].Automats), Greater, 0)
	c.Assert(party.Formations[0].Formations[0].Automats[0].Entity, NotNil)
	c.Assert(party.Formations[0].Formations[0].Automats[0].Created, Equals, false)
	c.Assert(len(party.Formations[0].Formations[0].Automats[0].Units), Greater, 0)
	c.Assert(party.Formations[0].Formations[0].Automats[0].Units[0].Entity, NotNil)
	c.Assert(party.Formations[0].Formations[0].Automats[0].Units[0].Created, Equals, false)
}
