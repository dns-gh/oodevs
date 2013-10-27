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
	"strings"
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

		checkpoint, snapshot, err := client.CreateCheckpoint(input, false)
		c.Assert(snapshot, IsNil)
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
			c.Assert(boundaries, HasLen, 2)
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
	check(strings.Repeat("a", 300), ".*create_directory.*")
}

func (s *TestSuite) TestCheckpointSendState(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	check := func(sendState bool) {
		boundaries := ""
		ctx := client.Register(func(msg *swapi.SwordMessage, context int32, err error) bool {
			if err != nil {
				return true
			}
			if msg.SimulationToClient == nil || msg.SimulationToClient.GetMessage() == nil {
				return false
			}
			m := msg.SimulationToClient.GetMessage()
			if reply := m.GetControlCheckpointSaveBegin(); reply != nil {
				boundaries += "<"
			} else if reply := m.GetControlCheckpointSaveEnd(); reply != nil {
				boundaries += ">"
			} else if reply := m.GetControlSendCurrentStateBegin(); reply != nil {
				boundaries += "("
			} else if reply := m.GetControlSendCurrentStateEnd(); reply != nil {
				boundaries += ")"
			} else if len(boundaries) > 0 && boundaries[len(boundaries)-1] != '>' {
				boundaries += "o"
			}
			return false
		})

		_, snapshot, err := client.CreateCheckpoint("checkpoint", sendState)
		c.Assert(err, IsNil)
		if sendState {
			c.Assert(boundaries, Matches, `o*<\(o+\)>o*`)
			c.Assert(snapshot, NotNil)
		} else {
			c.Assert(boundaries, Matches, `o*<>o*`)
			c.Assert(snapshot, IsNil)
		}
		client.Unregister(ctx)
	}

	check(false)
	check(true)
}

func loadCheckpointAndWaitModel(c *C, user, password, exercise, session, checkpoint string) (
	*simu.SimProcess, *swapi.Client) {
	sim := startSimOnCheckpoint(c, exercise, session, checkpoint, 1000, true)
	client := loginAndWaitModel(c, sim, user, password)
	return sim, client
}

func checkpointAndRestart(c *C, sim *simu.SimProcess, client *swapi.Client) (
	*simu.SimProcess, *swapi.Client) {
	session := sim.Opts.SessionName
	exercise := sim.Opts.ExerciseName
	checkpoint, _, err := client.CreateCheckpoint("", false)
	c.Assert(err, IsNil)
	client.Close()
	sim.Stop()

	sim, client = loadCheckpointAndWaitModel(c, "admin", "", exercise,
		session, checkpoint)
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

	sim, client = checkpointAndRestart(c, sim, client)
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

func (s *TestSuite) TestCheckpointCrowd(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallEmpty)
	defer sim.Stop()
	party := client.Model.GetData().FindPartyByName("party1")
	c.Assert(party, NotNil)

	// Create a crowd and give it a mission
	pos := swapi.Point{X: -15.9219, Y: 28.3456}
	crowd, err := client.CreateCrowd(party.Id, 0, CrowdType, pos, 100, 0, 0, "crowd")
	c.Assert(err, IsNil)
	to := swapi.Point{X: -15.9219, Y: 28.346}
	params := swapi.MakeParameters(swapi.MakePointParam(to))
	order, err := client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, params)
	c.Assert(err, IsNil)

	// Restart and check the crowd exists and has a mission
	sim, client = checkpointAndRestart(c, sim, client)
	defer sim.Stop()
	crowd2 := client.Model.GetCrowd(crowd.Id)
	c.Assert(crowd2, NotNil)
	data := client.Model.GetData()
	found := false
	for _, o := range data.Orders {
		if o.TaskerId == crowd.Id {
			c.Assert(o.MissionType, Equals, order.MissionType)
			found = true
			break
		}
	}
	c.Assert(found, Equals, true)
}

func (s *TestSuite) TestCheckpointUnit(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallEmpty)
	defer sim.Stop()
	party := client.Model.GetData().FindPartyByName("party1")
	c.Assert(party, NotNil)
	CreateFormation(c, client, party.Id)
	automat := createAutomatForParty(c, client, "party1")
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnitWithName(automat.Id, UnitType, from,
		"some unit name", false)
	c.Assert(err, IsNil)
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Give a mission
	heading := swapi.MakeHeading(0)
	dest := swapi.MakePointParam(swapi.Point{X: -15.8193, Y: 28.3456})
	params := swapi.MakeParameters(heading, nil, nil, nil, dest)
	order, err := client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)

	sim, client = checkpointAndRestart(c, sim, client)
	defer sim.Stop()
	data := client.Model.GetData()
	unit2 := data.Units[unit.Id]
	c.Assert(unit2, NotNil)
	c.Assert(unit2.Name, Equals, unit.Name)
	c.Assert(unit2.Pc, Equals, unit.Pc)
	automat2 := data.Automats[automat.Id]
	c.Assert(automat2, NotNil)
	c.Assert(automat2.Engaged, Equals, false)
	c.Assert(automat2.FormationId, Equals, automat.FormationId)
	formation2 := data.Formations[automat.FormationId]
	c.Assert(formation2, NotNil)

	order2 := data.Orders[order.Id]
	c.Assert(order2, NotNil)
	c.Assert(order2, DeepEquals, order)
}

func (s *TestSuite) TestCheckpointLogConvoy(c *C) {
	sim, client := connectAndWaitModelWithStep(c, "admin", "", ExCrossroadSmallLog, 300)
	defer sim.Stop()

	// Find the supply base
	model := client.Model.GetData()
	var supplyAutomat *swapi.Automat
	for _, a := range model.Automats {
		if strings.Contains(a.Name, "LOG.Supply logistic area") {
			supplyAutomat = a
		}
	}
	c.Assert(supplyAutomat, NotNil)

	// Deploy it
	MissionLogDeploy := uint32(44584)
	heading := swapi.MakeHeading(0)
	limit1 := swapi.MakeLimit(
		swapi.Point{X: -15.8302, Y: 28.3765},
		swapi.Point{X: -15.825, Y: 28.3413})
	limit2 := swapi.MakeLimit(
		swapi.Point{X: -15.7983, Y: 28.3765},
		swapi.Point{X: -15.7991, Y: 28.3413})
	params := swapi.MakeParameters(heading, nil, limit1, limit2, nil)
	_, err := client.SendAutomatOrder(supplyAutomat.Id, MissionLogDeploy, params)
	c.Assert(err, IsNil)

	// Once scout ammunitions are depleted, a convoy should be generated, with
	// a pathfind, eventually.
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, u := range data.Units {
			if strings.Contains(u.Name, "LOG.Convoy") && u.PathPoints > 0 {
				return true
			}
		}
		return false
	})

	sim, client = checkpointAndRestart(c, sim, client)
	defer sim.Stop()
	model = client.Model.GetData()

	// Is the convoy still there?
	getSomeUnitByName(c, model, "LOG.Convoy")
}
