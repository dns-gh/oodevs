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
	"code.google.com/p/goprotobuf/proto"
	"errors"
	"fmt"
	. "launchpad.net/gocheck"
	"swapi"
	"sword"
)

const (
	// Random existing automat identifier, we should parse the physical
	// database instead.
	AutomatType = uint32(117)
)

// For a given tasker type, send a unit magic action which is not
// implemented by target type. It used to return nothing, not even an
// error. This test should be adjusted if actions support more types.
func postInvalidTasker(client *swapi.Client, tasker *sword.Tasker) error {
	actionType := sword.UnitMagicAction_crowd_total_destruction
	if tasker.GetCrowd() != nil {
		actionType = sword.UnitMagicAction_delete_unit
	}

	msg := swapi.SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     tasker,
					Type:       &actionType,
					Parameters: swapi.MakeParameters(),
				},
			},
		},
	}
	return postInvalidUnitMagicAction(client, &msg)
}

func postInvalidUnitMagicAction(client *swapi.Client, msg *swapi.SwordMessage) error {
	quit := make(chan error)
	handler := func(msg *swapi.SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.SimulationToClient == nil || msg.Context != context {
			return false
		}
		m := msg.SimulationToClient.GetMessage()
		if reply := m.GetUnitMagicActionAck(); reply != nil {
			_, err = swapi.GetUnitMagicActionAck(reply)
			if err == nil {
				err = errors.New(fmt.Sprintf("Got unexpected success: %v", m))
			}
		} else {
			err = errors.New(fmt.Sprintf("Got unexpected: %v", m))
		}
		quit <- err
		return true

	}
	client.Post(*msg, handler)
	err := <-quit
	return err
}

func (s *TestSuite) TestNotImplementedUnitMagicAction(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()

	// Get a formation identifier
	formations := data.ListFormations()
	c.Assert(len(formations), Greater, 0)
	formationId := formations[0].Id

	// Get an automat identifier
	automats := data.ListAutomats()
	c.Assert(len(automats), Greater, 0)
	automatId := automats[0].Id

	// Get a unit identifier
	units := data.ListUnits()
	c.Assert(len(units), Greater, 0)
	unitId := units[0].Id

	// Get a crowd identifier
	crowds := data.ListCrowds()
	c.Assert(len(crowds), Greater, 0)
	crowdId := crowds[0].Id

	// Get a population identifier
	populations := data.ListPopulations()
	c.Assert(len(populations), Greater, 0)
	popId := populations[0].Id

	taskers := []*sword.Tasker{
		&sword.Tasker{
			Formation: &sword.FormationId{
				Id: proto.Uint32(formationId),
			},
		},
		&sword.Tasker{
			Automat: &sword.AutomatId{
				Id: proto.Uint32(automatId),
			},
		},
		&sword.Tasker{
			Unit: &sword.UnitId{
				Id: proto.Uint32(unitId),
			},
		},
		&sword.Tasker{
			Crowd: &sword.CrowdId{
				Id: proto.Uint32(crowdId),
			},
		},
		&sword.Tasker{
			Population: &sword.PopulationId{
				Id: proto.Uint32(popId),
			},
		},
	}
	for _, tasker := range taskers {
		err := postInvalidTasker(client, tasker)
		c.Assert(err, ErrorMatches, "error_invalid_unit",
			Commentf("for tasker %v", tasker))
	}
}

func (s *TestSuite) TestCreateFormation(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallEmpty)
	defer sim.Stop()
	model := client.Model

	// Test with invalid tasker
	_, err := client.CreateFormation(0, 0, "invalid-tasker", 1, "")
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Test invalid level
	_, err = client.CreateFormation(1, 0, "invalid-tasker", 42, "")
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Test invalid log level
	_, err = client.CreateFormation(1, 0, "invalid-tasker", 1, "invalid")
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Add formation to party
	f1, err := client.CreateFormation(1, 0, "newformation", 1, "")
	c.Assert(f1, NotNil)

	// Add formation to formation
	_, err = client.CreateFormation(0, f1.Id, "newformation2", 2, "aucun")
	c.Assert(err, IsNil) // failed to create formation
	dump := printParties(&prettyPrinter{HideUnstable: true},
		model.GetData()).GetOutput()
	expected := "" +
		`Party[-]
  Name: party1
    Formation[-]
      Id: -
      Name: newformation
      ParentId: -
      PartyId: -
      Level: b
      LogLevel: none
        Formation[-]
          Id: -
          Name: newformation2
          ParentId: -
          PartyId: -
          Level: o
          LogLevel: none
    KnowledgeGroup[-]
      Id: -
      PartyId: -
      Name: knowledge group [3]
Party[-]
  Name: party2
    KnowledgeGroup[-]
      Id: -
      PartyId: -
      Name: knowledge group[4]
`
	c.Assert(dump, Equals, expected)

	// Invalid formation parameters (empty)
	actionType := sword.UnitMagicAction_formation_creation
	msg := swapi.SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker: &sword.Tasker{
						Formation: &sword.FormationId{
							Id: proto.Uint32(f1.Id),
						},
					},
					Type:       &actionType,
					Parameters: swapi.MakeParameters(),
				},
			},
		},
	}
	err = postInvalidUnitMagicAction(client, &msg)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")
}

func (s *TestSuite) TestCreateUnit(c *C) {
	sim, client := connectAndWaitModel(c, "user", "user", ExCrossroadSmallOrbat)
	defer sim.Stop()
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

	pos := swapi.MakePoint(0, 0)

	// Valid unit type, should be read from physical database instead
	unitType := uint32(1)

	// Invalid automat
	_, err = client.CreateUnit(12345, unitType, pos)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Invalid unit type
	_, err = client.CreateUnit(automat.Id, 12345, pos)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Add unit to automat, automatically becomes PC
	u, err := client.CreateUnit(automat.Id, unitType, pos)
	c.Assert(err, IsNil)
	c.Assert(u, NotNil)
	c.Assert(u.Pc, Equals, true)
	automat = client.Model.GetAutomat(automat.Id)
	c.Assert(automat.Units[u.Id], NotNil)

	// Second unit, not PC
	u, err = client.CreateUnit(automat.Id, unitType, pos)
	c.Assert(err, IsNil)
	c.Assert(u, NotNil)
	c.Assert(u.Pc, Equals, false)

	automat, err = client.CreateAutomat(formation.Id, 0, AutomatType, kg.Id)
	c.Assert(err, IsNil)

	// Add unit with name, becomes a PC even if PC is false (wut?)
	name := "some unit"
	u1, err := client.CreateUnitWithName(automat.Id, unitType, pos, name, false)
	c.Assert(err, IsNil)
	c.Assert(u1, NotNil)
	c.Assert(u1.Name, Equals, name)
	c.Assert(u1.Pc, Equals, true)

	// Try to create another, it creates a not Pc unit and upgrades it to Pc.
	u2, err := client.CreateUnitWithName(automat.Id, unitType, pos, name, true)
	c.Assert(err, IsNil)
	c.Assert(u2, NotNil)
	c.Assert(u2.Pc, Equals, false)
	ok := client.Model.WaitCondition(func(data *swapi.ModelData) bool {
		return data.FindUnit(u2.Id).Pc
	})
	c.Assert(ok, Equals, true)
	ok = client.Model.WaitCondition(func(data *swapi.ModelData) bool {
		return !data.FindUnit(u1.Id).Pc
	})
	c.Assert(ok, Equals, true)

	// Delete the PC, the other unit becomes the Pc eventually
	err = client.DeleteUnit(u2.Id)
	c.Assert(err, IsNil)
	ok = client.Model.WaitCondition(func(data *swapi.ModelData) bool {
		return data.FindUnit(u1.Id).Pc
	})
	c.Assert(ok, Equals, true)

	// Read-only user can create unit (wut?)
	client.Close()
	client = loginAndWaitModel(c, sim, "user-readonly", "user-readonly",
		ExCrossroadSmallOrbat)
	u, err = client.CreateUnit(automat.Id, unitType, pos)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestDeleteUnit(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()

	// Destroy invalid unit
	err := client.DeleteUnit(1234)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Find some unit
	units := data.ListUnits()
	c.Assert(len(units), Greater, 0)
	unit := units[0]

	// Blast it
	err = client.DeleteUnit(unit.Id)
	c.Assert(err, IsNil)
	c.Assert(model.GetData().FindUnit(unit.Id), IsNil)
}

func (s *TestSuite) TestCreateAutomat(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model

	formations := model.GetData().ListFormations()
	c.Assert(len(formations), Greater, 0)
	formation := formations[0]

	// We want two knowledge groups from different parties, one of which
	// matches selected formation.
	knowledgeGroups := model.GetData().ListKnowledgeGroups()
	c.Assert(len(knowledgeGroups), Greater, 1)
	kg0 := knowledgeGroups[0]
	kg1 := knowledgeGroups[1]
	if kg0.PartyId != formation.PartyId {
		kg0 = knowledgeGroups[1]
		kg1 = knowledgeGroups[0]
	}
	c.Assert(kg0.PartyId, Equals, formation.PartyId)
	c.Assert(kg0.PartyId, Not(Equals), kg1.Id)

	automatType := AutomatType

	// No parent formation or automat
	_, err := client.CreateAutomat(0, 0, automatType, kg0.Id)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Invalid formation
	_, err = client.CreateAutomat(1234, 0, automatType, kg0.Id)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Invalid automat
	_, err = client.CreateAutomat(formation.Id, 1234, automatType, kg0.Id)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Invalid automat type
	_, err = client.CreateAutomat(formation.Id, 0, 12345, kg0.Id)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Invalid knowledge group
	_, err = client.CreateAutomat(formation.Id, 0, automatType, 12345)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Knowledge group not belonging to formation party
	_, err = client.CreateAutomat(formation.Id, 0, automatType, kg1.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Create automat in formation
	a, err := client.CreateAutomat(formation.Id, 0, automatType, kg0.Id)
	c.Assert(err, IsNil)
	c.Assert(a, NotNil)

	// Create automat in previous automat
	aa, err := client.CreateAutomat(0, a.Id, automatType, kg0.Id)
	c.Assert(err, IsNil)
	c.Assert(aa, NotNil)
	a = model.GetAutomat(a.Id)
	aa, ok := a.Automats[aa.Id]
	c.Assert(ok, Equals, true)

	// Knowledge group not belonging to parent automat party
	aa, err = client.CreateAutomat(0, a.Id, automatType, kg1.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")
	c.Assert(aa, IsNil)
}
