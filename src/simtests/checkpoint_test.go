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
	"swapi/simu"
)

func (s *TestSuite) TestCheckpointMessages(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()

	check := func(input, errmsg string) {
		boundaries := []string{}
		ctx := client.Register(func(msg *swapi.SwordMessage, context int32, err error) bool {
			if err != nil {
				return true
			}
			if msg.SimulationToClient == nil || msg.SimulationToClient.GetMessage() == nil {
				return false
			}
			m := msg.SimulationToClient.GetMessage()
			if reply := m.GetControlCheckpointSaveBegin(); reply != nil {
				boundaries = append(boundaries, "+"+reply.GetName())
			} else if reply := m.GetControlCheckpointSaveEnd(); reply != nil {
				boundaries = append(boundaries, "-"+reply.GetName())
			}
			return false
		})

		checkpoint, err := client.CreateCheckpoint(input)
		if len(errmsg) > 0 {
			c.Assert(checkpoint, Equals, "")
			c.Assert(err, ErrorMatches, errmsg)
		} else {
			c.Assert(err, IsNil)
			if len(input) > 0 {
				c.Assert(checkpoint, Equals, input)
			} else {
				c.Assert(checkpoint, Matches, `\d{4}_\d{2}_\d{2}__\d{2}h\d{2}m\d{2}`)
			}
			c.Assert(len(boundaries), Greater, 0)
		}
		if len(boundaries) > 0 {
			c.Assert(len(boundaries), Equals, 2)
			c.Assert(boundaries[0][:1], Equals, "+")
			c.Assert(boundaries[1][:1], Equals, "-")
			c.Assert(boundaries[0][1:], Equals, boundaries[1][1:])
		}
		client.Unregister(ctx)
	}

	// Must have supervisor rights
	check("", ".*rights check failed.*")
	client.Close()
	client = loginAndWaitModel(c, sim, "admin", "")

	// No checkpoint name
	check("", "")

	// Named checkpoint
	validname := "some spacy name with numb3rs and _"
	check(validname, "")

	// Overwrite
	check(validname, "")

	// Invalid UTF-8 sequence
	name := []byte("invalid utf-8")
	name = append(name, byte(140))
	check(string(name), ".*invalid utf-8.*")

	// Invalid file name
	check("some invalid ? name", ".*invalid checkpoint name.*")

	// Invalid very long filename (limited at 255 chars on windows)
	longname := ""
	for i := 0; i < 300; i++ {
		longname += "a"
	}
	check(longname, ".*create_directory.*")
}

func loadCheckpointAndWaitModel(c *C, user, password, exercise, session, checkpoint string) (
	*simu.SimProcess, *swapi.Client) {

	sim := startSimOnCheckpoint(c, exercise, session, checkpoint, 1000, false)
	client := loginAndWaitModel(c, sim, user, password)
	return sim, client
}

// Test SimProcess --checkpoint. This should be with other SimProcess tests but
// it is really difficult to craft checkpoint inputs, so we generate one here
// from a running exercise and reload it.
// There will be other tests to check the checkpoint content.
func (s *TestSuite) TestCheckpointRestart(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallEmpty)
	defer sim.Stop()
	party := client.Model.GetData().FindPartyByName("party1")
	c.Assert(party, NotNil)
	CreateFormation(c, client, party.Id)
	automat := createAutomatForParty(c, client, "party1")
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)
	session := sim.Opts.SessionName
	checkpoint, err := client.CreateCheckpoint("")
	c.Assert(err, IsNil)
	client.Close()
	sim.Stop()

	sim, client = loadCheckpointAndWaitModel(c, "admin", "", ExCrossroadSmallEmpty,
		session, checkpoint)
	defer sim.Stop()
	unit2 := client.Model.GetUnit(unit.Id)
	c.Assert(unit2, NotNil)
	c.Assert(Nearby(unit2.Position, from), Equals, true)
	automat2 := client.Model.GetAutomat(automat.Id)
	c.Assert(automat2, NotNil)
	c.Assert(automat2.FormationId, Equals, automat.FormationId)
	formation2 := client.Model.GetFormation(automat.FormationId)
	c.Assert(formation2, NotNil)
}
