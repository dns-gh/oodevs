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
	"swapi"
)

const (
	// Random existing automat identifier, we should parse the physical
	// database instead.
	AutomatType = uint32(123)
)

func createAutomat(c *C, client *swapi.Client) *swapi.Automat {
	data := client.Model.GetData()

	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	c.Assert(len(party.Formations), Greater, 0)
	var formation *swapi.Formation
	for _, f := range party.Formations {
		formation = f
		break
	}
	// Find a suitable knowledge group matching the formation, this should be
	// simpler...
	var kg *swapi.KnowledgeGroup
	for _, g := range data.ListKnowledgeGroups() {
		if g.PartyId == formation.PartyId {
			kg = g
			break
		}
	}
	c.Assert(kg, NotNil)

	automat, err := client.CreateAutomat(formation.Id, 0, AutomatType, kg.Id)
	c.Assert(err, IsNil)
	return automat
}

func (s *TestSuite) TestSetAutomatMode(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	c.Assert(automat.Engaged, Equals, true)

	// Invalid automat identifier
	err := client.SetAutomatMode(12456, false)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// This one should work
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	ok := client.Model.WaitCondition(func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})
	c.Assert(ok, Equals, true)

	// Engage it again
	err = client.SetAutomatMode(automat.Id, true)
	c.Assert(err, IsNil)
	ok = client.Model.WaitCondition(func(data *swapi.ModelData) bool {
		return data.FindAutomat(automat.Id).Engaged
	})
	c.Assert(ok, Equals, true)
}
