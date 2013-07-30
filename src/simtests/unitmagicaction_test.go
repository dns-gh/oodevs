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
	"fmt"
	. "launchpad.net/gocheck"
	"math"
	"strings"
	"swapi"
	"sword"
	"time"
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
				err = fmt.Errorf("Got unexpected success: %v", m)
			}
		} else {
			err = fmt.Errorf("Got unexpected: %v", m)
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
		c.Assert(err, IsSwordError, "error_invalid_unit",
			Commentf("for tasker %v", tasker))
	}
}

func (s *TestSuite) TestCreateFormation(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallEmpty)
	defer sim.Stop()
	model := client.Model

	// Test with invalid tasker
	_, err := client.CreateFormation(0, 0, "invalid-tasker", 1, "")
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Test invalid level
	_, err = client.CreateFormation(1, 0, "invalid-tasker", 42, "")
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Test invalid log level
	_, err = client.CreateFormation(1, 0, "invalid-tasker", 1, "invalid")
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Add formation to party
	f1, err := client.CreateFormation(1, 0, "newformation", 1, "")
	c.Assert(f1, NotNil)
	c.Assert(f1.PartyId, Equals, uint32(1))
	c.Assert(f1.ParentId, Equals, uint32(0))
	c.Assert(f1.Level, Equals, "b")
	c.Assert(f1.LogLevel, Equals, "none")

	// Add formation to formation
	f2, err := client.CreateFormation(0, f1.Id, "newformation2", 2, "logistic-base")
	c.Assert(err, IsNil)
	c.Assert(f2.PartyId, Equals, uint32(1))
	c.Assert(f2.ParentId, Equals, f1.Id)
	c.Assert(f2.Level, Equals, "o")
	c.Assert(f2.LogLevel, Equals, "logistic_base")
	f1 = model.GetFormation(f1.Id)
	c.Assert(f1.Formations[f2.Id].Id, Equals, f2.Id)

	// Invalid formation parameters (empty)
	_, err = client.CreateFormationTest(0, f1.Id, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")
}

func Nearby(pointA, pointB swapi.Point) bool {
	return math.Abs(pointA.X-pointB.X) < 1e-6 &&
		math.Abs(pointA.Y-pointB.Y) < 1e-6
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

	pos := swapi.Point{X: -15.9219, Y: 28.3456}

	// Valid unit type, should be read from physical database instead
	unitType := uint32(1)

	// No tasker
	_, err = client.CreateUnit(0, unitType, pos)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid automat
	_, err = client.CreateUnit(InvalidIdentifier, unitType, pos)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid unit type
	_, err = client.CreateUnit(automat.Id, InvalidIdentifier, pos)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Add unit to automat, automatically becomes PC
	u, err := client.CreateUnit(automat.Id, unitType, pos)
	c.Assert(err, IsNil)
	c.Assert(u, NotNil)
	c.Assert(u.Pc, Equals, true)

	// Check unit position
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return Nearby(data.FindUnit(u.Id).Position, pos)
	})

	automat = client.Model.GetAutomat(automat.Id)
	c.Assert(automat.Units[u.Id], NotNil)

	pos = swapi.Point{X: -15.8219, Y: 28.2456}

	// Second unit, not PC
	u, err = client.CreateUnit(automat.Id, unitType, pos)
	c.Assert(err, IsNil)
	c.Assert(u, NotNil)
	c.Assert(u.Pc, Equals, false)

	// Check unit position
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return Nearby(data.FindUnit(u.Id).Position, pos)
	})

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
	c.Assert(err, IsSwordError, "error_invalid_unit")

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
	var kg0 *swapi.KnowledgeGroup
	var kg1 *swapi.KnowledgeGroup
	for _, kg := range knowledgeGroups {
		if kg0 == nil && kg.PartyId == formation.PartyId {
			kg0 = kg
		}
		if kg1 == nil && kg.PartyId != formation.PartyId {
			kg1 = kg
		}
	}
	c.Assert(kg0, NotNil)
	c.Assert(kg1, NotNil)

	automatType := AutomatType

	// No parent formation or automat
	_, err := client.CreateAutomat(0, 0, automatType, kg0.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid formation
	_, err = client.CreateAutomat(InvalidIdentifier, 0, automatType, kg0.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid automat
	_, err = client.CreateAutomat(formation.Id, InvalidIdentifier, automatType, kg0.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid automat type
	_, err = client.CreateAutomat(formation.Id, 0, InvalidIdentifier, kg0.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid knowledge group
	_, err = client.CreateAutomat(formation.Id, 0, automatType, InvalidIdentifier)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Knowledge group not belonging to formation party
	_, err = client.CreateAutomat(formation.Id, 0, automatType, kg1.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Create automat in formation
	a, err := client.CreateAutomat(formation.Id, 0, automatType, kg0.Id)
	c.Assert(err, IsNil)
	c.Assert(a, NotNil)
	c.Assert(a.KnowledgeGroupId, Equals, kg0.Id)

	// Create automat in previous automat
	aa, err := client.CreateAutomat(0, a.Id, automatType, kg0.Id)
	c.Assert(err, IsNil)
	c.Assert(aa, NotNil)
	a = model.GetAutomat(a.Id)
	aa, ok := a.Automats[aa.Id]
	c.Assert(ok, Equals, true)

	// Knowledge group not belonging to parent automat party
	aa, err = client.CreateAutomat(0, a.Id, automatType, kg1.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	c.Assert(aa, IsNil)
}

func (s *TestSuite) TestCreateCrowd(c *C) {
	checkError := func(crowd *swapi.Crowd, err error, expected string) {
		c.Assert(crowd, IsNil)
		c.Assert(err, IsSwordError, expected)
	}

	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	data := client.Model.GetData()
	pos := swapi.Point{X: 0, Y: 0}
	model := client.Model
	crowdName := "crowd"
	healthy, wounded, dead := int32(10), int32(11), int32(12)

	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Invalid tasker
	crowd, err := client.CreateCrowd(0, 0, CrowdType, pos, healthy, wounded, dead, crowdName)
	checkError(crowd, err, "error_invalid_unit")

	// Invalid party identifier
	crowd, err = client.CreateCrowd(InvalidIdentifier, 0, CrowdType, pos,
		healthy, wounded, dead, crowdName)
	checkError(crowd, err, "error_invalid_unit")

	// Invalid party identifier
	crowd, err = client.CreateCrowd(0, InvalidIdentifier, CrowdType, pos,
		healthy, wounded, dead, crowdName)
	checkError(crowd, err, "error_invalid_unit")

	// We can't create a crowd with 0 humans
	crowd, err = client.CreateCrowd(party.Id, 0, CrowdType, pos, 0, 0, 0, crowdName)
	checkError(crowd, err, "error_invalid_parameter")

	// Invalid crowd type
	crowd, err = client.CreateCrowd(party.Id, 0, "invalidType", pos, healthy,
		wounded, dead, crowdName)
	checkError(crowd, err, "error_invalid_parameter")

	// Create crowd in party
	crowd, err = client.CreateCrowd(party.Id, 0, CrowdType, pos, healthy,
		wounded, dead, crowdName)
	c.Assert(err, IsNil)
	c.Assert(crowd, NotNil)

	formations := model.GetData().ListFormations()
	c.Assert(len(formations), Greater, 0)
	formation := formations[0]

	// Create crowd in party with formation identifier
	crowd, err = client.CreateCrowd(formation.Id, 0, CrowdType, pos, healthy,
		wounded, dead, crowdName)
	c.Assert(err, IsNil)
	c.Assert(crowd, NotNil)
}

func (s *TestSuite) TestTeleportUnit(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	pos := swapi.Point{X: -15.8219, Y: 28.2456}

	// No tasker
	err = client.TeleportUnit(0, pos)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot teleport unit if its automat is engaged
	err = client.TeleportUnit(unit.Id, pos)
	c.Assert(err, IsSwordError, "error_automat_engaged")

	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	// Teleport unit
	err = client.TeleportUnit(unit.Id, pos)
	c.Assert(err, IsNil)

	// Check unit position
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return Nearby(data.FindUnit(unit.Id).Position, pos)
	})
}

func (s *TestSuite) TestLogisticsChangeLinks(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid automat id
	err := client.LogisticsChangeLinks(10, []uint32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// valid automat id with no link
	err = client.LogisticsChangeLinks(9, []uint32{})
	c.Assert(err, IsNil)

	// error : 42 is an invalid superior id
	newSuperiors := []uint32{23, 42}
	err = client.LogisticsChangeLinks(9, newSuperiors)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// valid superiors id
	newSuperiors = []uint32{25, 31}
	err = client.LogisticsChangeLinks(9, newSuperiors)
	c.Assert(err, IsNil)

	// logistics links model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		a := data.FindAutomat(9)
		return (a != nil) && (len(a.LogSuperiors) > 1)
	})
	automat := client.Model.GetData().FindAutomat(9)
	c.Assert(automat.LogSuperiors, DeepEquals, newSuperiors)
}

func (s *TestSuite) TestLogisticsSupplyChangeQuotas(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid supplied id parameter
	err := client.LogisticsSupplyChangeQuotas(25, 42, map[uint32]int32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid supplier id parameter
	err = client.LogisticsSupplyChangeQuotas(42, 23, map[uint32]int32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error : valid units id but no dotation
	err = client.LogisticsSupplyChangeQuotas(25, 23, map[uint32]int32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// quotas model updated and no error with valid ids
	newQuotas := map[uint32]int32{1: 100, 2: 200}
	err = client.LogisticsSupplyChangeQuotas(25, 23, newQuotas)
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		a := data.FindAutomat(23)
		return (a != nil) && (len(a.SuperiorQuotas) > 0)
	})
	automat := client.Model.GetData().FindAutomat(23)
	c.Assert(automat.SuperiorQuotas, DeepEquals, newQuotas)
}

func (s *TestSuite) TestLogisticsSupplyPushFlow(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid supplier parameter
	err := client.LogisticsSupplyPushFlow(42, 9)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid receiver parameter
	param := swapi.MakeParameter(&sword.MissionParameter_Value{})
	err = client.LogisticsSupplyPushFlowTest(23, swapi.MakeParameters(param))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// valid supplier parameter
	err = client.LogisticsSupplyPushFlow(23, 9)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestLogisticsSupplyPullFlow(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid supplier parameter
	err := client.LogisticsSupplyPullFlow(23, 42)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid receiver parameter
	err = client.LogisticsSupplyPullFlow(42, 9)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// valid supplier parameter
	err = client.LogisticsSupplyPullFlow(23, 9)
	c.Assert(err, IsNil)
}

func CheckSuperior(model *swapi.Model, c *C,
	unitId, newAutomatId, oldAutomatId uint32) {
	// Check AutomatId attribute in Unit
	unit := model.GetData().FindUnit(unitId)
	c.Assert(unit, NotNil)
	c.Assert(unit.AutomatId, Equals, newAutomatId)

	// Check oldAutomat is empty
	oldAutomat := model.GetData().FindAutomat(oldAutomatId)
	c.Assert(oldAutomat, NotNil)
	c.Assert(oldAutomat.Units, HasLen, 0)

	// Check newAutomat contains one element
	newAutomat := model.GetData().FindAutomat(newAutomatId)
	c.Assert(newAutomat, NotNil)
	c.Assert(newAutomat.Units, HasLen, 1)
}

func CreateFormation(c *C, client *swapi.Client, partyId uint32) *swapi.Formation {
	formation, err := client.CreateFormation(partyId, 0, "newformation", 1, "")
	c.Assert(err, IsNil)
	return formation
}

func CreateAutomat(c *C, client *swapi.Client, formationId, knowledgeGroup uint32) *swapi.Automat {
	knowledgeGroups := client.Model.GetData().ListKnowledgeGroups()
	kg0 := knowledgeGroups[knowledgeGroup]

	automat, err := client.CreateAutomat(formationId, 0, AutomatType, kg0.Id)
	c.Assert(err, IsNil)
	c.Assert(automat.KnowledgeGroupId, Equals, kg0.Id)
	return automat
}

func CreateUnit(c *C, client *swapi.Client, automatId uint32) *swapi.Unit {
	unit, err := client.CreateUnit(automatId, UnitType, swapi.Point{X: -15.8219, Y: 28.2456})
	c.Assert(err, IsNil)
	return unit
}

func (s *TestSuite) TestUnitChangeSuperior(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	f1 := CreateFormation(c, client, 1)

	// Create 2 automats
	a1 := CreateAutomat(c, client, f1.Id, 0)
	a2 := CreateAutomat(c, client, f1.Id, 0)

	// One unit in a2 automat
	u2 := CreateUnit(c, client, a2.Id)

	// error: no tasker
	err := client.ChangeSuperior(0, a1.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// error: invalid tasker
	err = client.ChangeSuperior(12345, a1.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// error: missing parameter
	err = client.ChangeSuperior(u2.Id, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid automat identifier
	err = client.ChangeSuperior(u2.Id, 12345)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: automat parameter isn't in the same side
	f2 := CreateFormation(c, client, 2)
	a3 := CreateAutomat(c, client, f2.Id, 2)

	err = client.ChangeSuperior(u2.Id, a3.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change superior u2 -> a1
	err = client.ChangeSuperior(u2.Id, a1.Id)
	c.Assert(err, IsNil)

	// Wait unit update
	client.Model.WaitTicks(1)
	CheckSuperior(client.Model, c, u2.Id, a1.Id, a2.Id)

	// Change superior u2 -> a1 again, does nothing
	err = client.ChangeSuperior(u2.Id, a1.Id)
	c.Assert(err, IsNil)

	// Wait unit update
	client.Model.WaitTicks(1)
	CheckSuperior(client.Model, c, u2.Id, a1.Id, a2.Id)
}

func (s *TestSuite) TestPcChangeSuperior(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	f1 := CreateFormation(c, client, 1)
	// Create 2 automats
	a1 := CreateAutomat(c, client, f1.Id, 0)
	a2 := CreateAutomat(c, client, f1.Id, 0)
	// Create 2 units in a1 automat
	u1 := CreateUnit(c, client, a1.Id)
	u2 := CreateUnit(c, client, a1.Id)

	// Check u1 is PC
	c.Assert(u1.Pc, Equals, true)
	c.Assert(u2.Pc, Equals, false)

	// Change superior u2 -> a2
	err := client.ChangeSuperior(u2.Id, a2.Id)
	c.Assert(err, IsNil)

	// Check u2 is PC
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindUnit(u2.Id).Pc
	})

	// Change superior u1 -> a2
	err = client.ChangeSuperior(u1.Id, a2.Id)
	c.Assert(err, IsNil)

	// Check u1 is not PC
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindUnit(u1.Id).Pc
	})
}

func (s *TestSuite) TestDebugBrain(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	// Missing boolean parameter
	params := swapi.MakeParameters()
	err = client.DebugBrainTest(unit.Id, params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid tasker
	err = client.DebugBrain(123456, true)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// This one should work
	err = client.DebugBrain(unit.Id, true)
	c.Assert(err, IsNil)
	client.Model.WaitCondition(func(model *swapi.ModelData) bool {
		return model.FindUnit(unit.Id).DebugBrain
	})

	// Enable again
	err = client.DebugBrain(unit.Id, true)
	c.Assert(err, IsNil)

	// Give a mission and wait for debug reports to be emitted
	seen := make(chan error)
	client.Register(func(msg *swapi.SwordMessage, context int32, err error) bool {
		if err != nil {
			seen <- err
			return true
		}
		if msg.SimulationToClient == nil ||
			msg.SimulationToClient.GetMessage() == nil {
			return false
		}
		trace := msg.SimulationToClient.GetMessage().GetTrace()
		if trace == nil {
			return false
		}
		if trace.GetSource() == nil || trace.GetSource().GetUnit() == nil ||
			trace.GetSource().GetUnit().GetId() != unit.Id ||
			strings.Index(trace.GetMessage(), "digraph") == -1 {
			return false
		}
		seen <- nil
		return true
	})

	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	params = swapi.MakeParameters(
		swapi.MakeHeading(0),
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakePointParam(to))
	err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)

	select {
	case err := <-seen:
		c.Assert(err, IsNil)
	case <-time.After(20 * time.Second):
		c.Fatal("timed out while waiting for brain debug information")
	}

	// Disable debugging
	// It is hard to test that no debug reports are being emitted any longer,
	// and this kind of test usually requires a timeout which makes the tests
	// slower. Skip it for now.
	err = client.DebugBrain(unit.Id, false)
	c.Assert(err, IsNil)
	client.Model.WaitCondition(func(model *swapi.ModelData) bool {
		return !model.FindUnit(unit.Id).DebugBrain
	})
}

func MakeTransferCondition(remaining int32, lent int32) func(*swapi.ModelData) bool {
	return func(data *swapi.ModelData) bool {
		unit := data.FindUnit(11)
		target := data.FindUnit(12)
		return unit.EquipmentDotations[11].Available == remaining &&
			len(unit.LentEquipments) != 0 && unit.LentEquipments[0].Borrower == 12 && unit.LentEquipments[0].TypeId == 11 && unit.LentEquipments[0].Quantity == lent &&
			len(target.BorrowedEquipments) != 0 && target.BorrowedEquipments[0].Owner == 11 && target.BorrowedEquipments[0].TypeId == 11 && target.BorrowedEquipments[0].Quantity == lent
	}
}

func (s *TestSuite) TestTransferEquipment(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid parameters count
	err := client.TransferEquipment(11, 12, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid empty equipment list")

	// error: invalid unit identifier
	err = client.TransferEquipment(1000, 12, []swapi.Equipment{{11, 1}})
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// error: target does not have equipment
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{1000, 1}})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: equipment #0 not found in source unit")

	// error: invalid unit identifier
	err = client.TransferEquipment(11, 1000, []swapi.Equipment{{11, 1}})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid target identifier")

	// error: cannot transfer from a unit to itself
	err = client.TransferEquipment(11, 11, []swapi.Equipment{{11, 1}})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: source and target are identical")

	// valid: transfer equipment
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{11, 1}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, MakeTransferCondition(3, 1))

	// valid: transfer twice the same equipment sums the two quantities
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{11, 1}, {11, 1}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, MakeTransferCondition(1, 3))

	// valid: transfering more caps to the available quantity
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{11, 1000}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, MakeTransferCondition(0, 4))

	// valid: transfering back
	err = client.TransferEquipment(12, 11, []swapi.Equipment{{11, 3}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, MakeTransferCondition(3, 1))
}
