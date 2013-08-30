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
	"reflect"
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
	client = loginAndWaitModel(c, sim, "user-readonly", "user-readonly")
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

func CheckUnitSuperior(model *swapi.Model, c *C,
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

func CheckAutomatSuperior(model *swapi.Model, c *C,
	automatId, newFormationId, oldFormationId uint32) {

	d := model.GetData()
	// Check AutomatId attribute in Unit
	automat := d.FindAutomat(automatId)
	c.Assert(automat, NotNil)
	c.Assert(automat.FormationId, Equals, newFormationId)

	// Check oldFormation is empty
	oldFormation := d.FindFormation(oldFormationId)
	c.Assert(oldFormation, NotNil)
	c.Assert(oldFormation.Automats, HasLen, 0)

	// Check newFormation contains one element
	newFormation := d.FindFormation(newFormationId)
	c.Assert(newFormation, NotNil)
	c.Assert(newFormation.Automats, HasLen, 1)
}

func CheckFormationSuperior(model *swapi.Model, c *C,
	formationId, newParentId uint32) {

	d := model.GetData()
	// Check AutomatId attribute in Unit
	formation := d.FindFormation(formationId)
	c.Assert(formation, NotNil)

	// Check newFormation or newParty contains one element
	newFormation := d.FindFormation(newParentId)
	if newFormation == nil {
		newParty := d.Parties[newParentId]
		c.Assert(newParty, NotNil)
		c.Assert(formation.ParentId, Equals, uint32(0))
	} else {
		c.Assert(newFormation, NotNil)
		c.Assert(newFormation.Formations, HasLen, 1)
		c.Assert(formation.ParentId, Equals, newParentId)
	}
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
	err := client.ChangeUnitSuperior(0, a1.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// error: invalid tasker
	err = client.ChangeUnitSuperior(12345, a1.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// error: missing parameter
	err = client.ChangeUnitSuperior(u2.Id, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid automat identifier
	err = client.ChangeUnitSuperior(u2.Id, 12345)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: automat parameter isn't in the same side
	f2 := CreateFormation(c, client, 2)
	a3 := CreateAutomat(c, client, f2.Id, 2)

	err = client.ChangeUnitSuperior(u2.Id, a3.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change superior u2 -> a1
	err = client.ChangeUnitSuperior(u2.Id, a1.Id)
	c.Assert(err, IsNil)

	// Wait unit update
	client.Model.WaitTicks(1)
	CheckUnitSuperior(client.Model, c, u2.Id, a1.Id, a2.Id)

	// Change superior u2 -> a1 again, does nothing
	err = client.ChangeUnitSuperior(u2.Id, a1.Id)
	c.Assert(err, IsNil)

	// Wait unit update
	client.Model.WaitTicks(1)
	CheckUnitSuperior(client.Model, c, u2.Id, a1.Id, a2.Id)
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
	err := client.ChangeUnitSuperior(u2.Id, a2.Id)
	c.Assert(err, IsNil)

	// Check u2 is PC
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindUnit(u2.Id).Pc
	})

	// Change superior u1 -> a2
	err = client.ChangeUnitSuperior(u1.Id, a2.Id)
	c.Assert(err, IsNil)

	// Check u1 is not PC
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindUnit(u1.Id).Pc
	})
}

func (s *TestSuite) TestAutomatChangeSuperior(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	f1 := CreateFormation(c, client, 1)
	f2 := CreateFormation(c, client, 1)

	// Create 1 automat
	a1 := CreateAutomat(c, client, f1.Id, 0)

	// error: invalid tasker
	err := client.ChangeAutomatSuperior(12345, f2.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: missing parameter
	err = client.ChangeAutomatSuperior(a1.Id, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid formation identifier
	err = client.ChangeAutomatSuperior(a1.Id, 12345)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: formation parameter isn't in the same side
	f3 := CreateFormation(c, client, 2)
	err = client.ChangeAutomatSuperior(a1.Id, f3.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change superior u2 -> a1
	err = client.ChangeAutomatSuperior(a1.Id, f2.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindAutomat(a1.Id).FormationId == f2.Id
	})
	CheckAutomatSuperior(client.Model, c, a1.Id, f2.Id, f1.Id)

	// Change superior u2 -> a1 again, does nothing
	err = client.ChangeAutomatSuperior(a1.Id, f1.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindAutomat(a1.Id).FormationId == f1.Id
	})
	CheckAutomatSuperior(client.Model, c, a1.Id, f1.Id, f2.Id)
}

func (s *TestSuite) TestFormationChangeSuperior(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	//create 2 formations
	f1 := CreateFormation(c, client, 1)
	f2 := CreateFormation(c, client, 1)

	// error: invalid tasker
	err := client.ChangeFormationSuperior(12345, f2.Id, false)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: missing parameter
	err = client.ChangeFormationSuperior(f1.Id, 0, false)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid formation identifier
	err = client.ChangeFormationSuperior(f1.Id, 12345, false)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: formation parameter isn't in the same side
	f3 := CreateFormation(c, client, 2)

	err = client.ChangeFormationSuperior(f1.Id, f3.Id, false)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change superior f1 -> f2
	err = client.ChangeFormationSuperior(f1.Id, f2.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindFormation(f1.Id).ParentId == f2.Id
	})
	CheckFormationSuperior(client.Model, c, f1.Id, f2.Id)

	// Change superior f1 -> party1
	err = client.ChangeFormationSuperior(f1.Id, 1, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindFormation(f1.Id).ParentId == 0
	})
	CheckFormationSuperior(client.Model, c, f1.Id, 1)
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

func CheckLentEquipment(data *swapi.ModelData, from uint32, to uint32, available int32, lent int32) bool {
	unit := data.FindUnit(from)
	return unit.EquipmentDotations[11].Available == available &&
		(lent == 0 && len(unit.LentEquipments) == 0 ||
			len(unit.LentEquipments) != 0 && unit.LentEquipments[0].Borrower == to && unit.LentEquipments[0].TypeId == 11 && unit.LentEquipments[0].Quantity == lent)
}

func CheckBorrowedEquipment(data *swapi.ModelData, from uint32, to uint32, available int32, borrowed int32) bool {
	target := data.FindUnit(to)
	return target.EquipmentDotations[11].Available == available &&
		(borrowed == 0 && len(target.BorrowedEquipments) == 0 ||
			len(target.BorrowedEquipments) != 0 && target.BorrowedEquipments[0].Owner == from && target.BorrowedEquipments[0].TypeId == 11 && target.BorrowedEquipments[0].Quantity == borrowed)
}

func MakeTransferCondition(from uint32, to uint32, lent int32) func(*swapi.ModelData) bool {
	return func(data *swapi.ModelData) bool {
		return CheckLentEquipment(data, from, to, 4-lent, lent) && CheckBorrowedEquipment(data, from, to, 4+lent, lent)
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
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no equipment type of parameter #0 available to lend in source unit")

	// error: invalid unit identifier
	err = client.TransferEquipment(11, 1000, []swapi.Equipment{{11, 1}})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid target identifier")

	// error: cannot transfer from a unit to itself
	err = client.TransferEquipment(11, 11, []swapi.Equipment{{11, 1}})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: source and target are identical")

	// error: negative equipment count
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{11, -1}})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid negative equipment count #0")

	// valid: transfer zero available equipments
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{11, 0}})
	c.Assert(err, IsNil)

	// valid: transfer zero unavailable equipments
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{1000, 0}})
	c.Assert(err, IsNil)

	// valid: transfer equipment
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{11, 1}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, MakeTransferCondition(11, 12, 1))

	// valid: transfer twice the same equipment sums the two quantities
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{11, 1}, {11, 1}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, MakeTransferCondition(11, 12, 3))

	// valid: transfering more caps to the available quantity
	err = client.TransferEquipment(11, 12, []swapi.Equipment{{11, 1000}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, MakeTransferCondition(11, 12, 4))

	// valid: transfering back
	err = client.TransferEquipment(12, 11, []swapi.Equipment{{11, 3}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, MakeTransferCondition(11, 12, 1))

	// valid: transfering to a third unit
	err = client.TransferEquipment(12, 13, []swapi.Equipment{{11, 5}})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckLentEquipment(data, 11, 12, 3, 1) && CheckBorrowedEquipment(data, 11, 12, 0, 1) &&
			CheckLentEquipment(data, 12, 13, 0, 5) && CheckBorrowedEquipment(data, 12, 13, 9, 5)
	})

	//valid: destroying a unit cancels its lendings and borrowings
	client.DeleteUnit(12)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckLentEquipment(data, 11, 13, 4, 0) && CheckBorrowedEquipment(data, 11, 13, 4, 0)
	})
}

func (s *TestSuite) TestSurrender(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	isSurrendered := func(data *swapi.ModelData, automatId, partyId uint32) bool {
		automat := data.FindAutomat(automatId)
		for _, unit := range automat.Units {
			if unit.PartySurrenderedTo != partyId {
				return false
			}
		}
		return true
	}

	data := client.Model.GetData()
	automats := data.ListAutomats()
	c.Assert(len(automats), Greater, 0)
	automat := automats[0]

	// find another party Id
	armies := data.Parties
	c.Assert(len(armies), Greater, 1)
	var otherPartyId, otherPartyId2 uint32
	for p := range armies {
		if p != automat.PartyId {
			if otherPartyId == 0 {
				otherPartyId = p
			} else {
				otherPartyId2 = p
				break
			}
		}
	}
	c.Assert(otherPartyId, Greater, uint32(0))
	c.Assert(otherPartyId2, Greater, uint32(0))

	// Cancelling a surrender while free is a no-op
	err := client.CancelSurrender(automat.Id)
	c.Assert(err, IsNil)

	// error: no parameters
	err = client.SurrenderTest(automat.Id, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: parameter not a party
	err = client.SurrenderTest(automat.Id, swapi.MakeParameters(swapi.MakeNullValue()))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: bad number of parameters
	err = client.SurrenderTest(automat.Id, swapi.MakeParameters(swapi.MakeNullValue(), swapi.MakeNullValue()))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: surrender to own party
	err = client.Surrender(automat.Id, automat.PartyId)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// surrender to other party
	err = client.Surrender(automat.Id, otherPartyId)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return isSurrendered(data, automat.Id, otherPartyId)
	})

	// Surrendering again to the same party is a no-op
	err = client.Surrender(automat.Id, otherPartyId)
	c.Assert(err, IsNil)

	// error: surrender again to another party
	err = client.Surrender(automat.Id, otherPartyId2)
	c.Assert(err, IsSwordError, "error_unit_surrendered")

	// cancel surrender
	err = client.CancelSurrender(automat.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !isSurrendered(data, automat.Id, 0)
	})

	// Move a normal unit into a surrendered automat
	err = client.Surrender(automat.Id, otherPartyId)
	c.Assert(err, IsNil)
	a2 := CreateAutomat(c, client, automat.FormationId, 0)
	u2 := CreateUnit(c, client, a2.Id)
	// Wait for the unit to be fully generated, there is an update race
	// otherwise masking the previous failure.
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FindUnit(u2.Id).EquipmentDotations) > 0
	})
	err = client.ChangeUnitSuperior(u2.Id, automat.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindUnit(u2.Id).AutomatId == automat.Id &&
			isSurrendered(data, automat.Id, otherPartyId)
	})

	// Move it back to a free automat
	err = client.ChangeUnitSuperior(u2.Id, a2.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindUnit(u2.Id).AutomatId == a2.Id &&
			isSurrendered(data, a2.Id, 0)
	})
}

func CheckHumanQuantity(actual []*swapi.HumanDotation, expected map[int32]int32) bool {
	for _, human := range actual {
		if human.Quantity != expected[human.Rank] {
			return false
		}
	}
	return true
}

func CheckHumanTotalState(actual []*swapi.HumanDotation, rank, state, quantity,
	injury int32, psyop, contaminated bool) bool {
	expected := swapi.HumanDotation{
		Quantity:     quantity,
		Rank:         rank,
		State:        state,
		Injury:       injury,
		Psyop:        psyop,
		Contaminated: contaminated,
	}
	for _, human := range actual {
		if *human == expected {
			return true
		}
	}
	return false
}

func CheckHumanState(actual []*swapi.HumanDotation, rank, state, quantity,
	injury int32) bool {
	return CheckHumanTotalState(actual, rank, state, quantity,
		injury, false, false)
}

const (
	// HumanRank
	eOfficer        = 0
	eWarrantOfficer = 1
	eTrooper        = 2
	// HumanState
	eHealthy = 0
	eInjured = 1
	eDead    = 2
	// InjuriesSeriousness
	eInjuryU1 = 0
	eInjuryU2 = 1
	eInjuryU3 = 2
	eInjuryUe = 3
)

// Deprecated
func (s *TestSuite) TestUnitCreateWounds(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)

	// Check initial humans state
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanQuantity(data.FindUnit(u1.Id).HumanDotations,
			map[int32]int32{eOfficer: 1, eWarrantOfficer: 4, eTrooper: 7})
	})

	// Error: Invalid parameters count
	err := client.CreateWounds(u1.Id, map[int32]int32{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters count, 1 parameter expected")

	// Error: Quantity must be non-zero positive
	err = client.CreateWounds(u1.Id, map[int32]int32{0: 0})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[0\] must be positive a non-zero positive number`)
	err = client.CreateWounds(u1.Id, map[int32]int32{0: -10})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: Invalid wound
	err = client.CreateWounds(u1.Id, map[int32]int32{123: 10})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[1\] invalid, bad wound enumeration`)

	// Unit create wounds
	// Magical action apply primarily to Tropper then WarrantOfficer and Officer
	quantity := int32(3)
	notWounded := int32(0)
	woundedU1 := int32(2)
	woundedU2 := int32(3)
	dead := int32(5)

	err = client.CreateWounds(u1.Id, map[int32]int32{woundedU2: quantity})
	err = client.CreateWounds(u1.Id, map[int32]int32{woundedU1: quantity})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eOfficer, eHealthy, 1, notWounded) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eWarrantOfficer, eHealthy, 4, notWounded) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eHealthy, 1, notWounded) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eInjured, quantity, eInjuryU2) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eInjured, quantity, eInjuryU1)
	})

	// Kill all humans
	err = client.CreateWounds(u1.Id, map[int32]int32{dead: 1234})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eOfficer, eDead, 1, notWounded) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eWarrantOfficer, eDead, 4, notWounded) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eDead, 1, notWounded) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eInjured, quantity, eInjuryU2) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eInjured, quantity, eInjuryU1)
	})

	// Heal humans
	err = client.CreateWounds(u1.Id, map[int32]int32{notWounded: 7})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eOfficer, eDead, 1, notWounded) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eWarrantOfficer, eDead, 4, notWounded) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eHealthy, 7, notWounded)
	})
}

func (s *TestSuite) TestUnitChangeHumanState(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)

	// Check initial humans state
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanQuantity(data.FindUnit(u1.Id).HumanDotations,
			map[int32]int32{eOfficer: 1, eWarrantOfficer: 4, eTrooper: 7})
	})

	// Error: Invalid parameters count
	err := client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters count, 1 parameter expected")

	human := swapi.HumanDotation{
		Quantity:     -10,
		Rank:         eOfficer,
		State:        eHealthy,
		Injury:       eInjuryU2,
		Psyop:        false,
		Contaminated: false,
	}

	// Error: Quantity must be non-zero positive
	err = client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{&human})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[0\] must be positive a non-zero positive number`)
	human.Quantity = 0
	err = client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{&human})
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	human.Quantity = 1

	// Error: Invalid rank enumeration
	human.Rank = 123
	err = client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{&human})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[1\] invalid, bad human rank enumeration`)
	human.Rank = eTrooper

	// Error: Invalid state enumeration
	human.State = 123
	err = client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{&human})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[2\] invalid, bad human state enumeration`)

	// Change human state
	human.State = eInjured
	human.Injury = eInjuryU3
	human.Psyop = true
	human.Contaminated = true
	err = client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{&human})
	c.Assert(err, IsNil)

	// Check human composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eOfficer, eHealthy, 1, 0) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eWarrantOfficer, eHealthy, 4, 0) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eHealthy, 6, 0) &&
			CheckHumanTotalState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eInjured, 1, eInjuryU3, true, true)
	})

	// Heal tropper, kill officer and wound 1245 warrant officers
	human.State = eHealthy
	human.Injury = 0
	human.Psyop = false
	human.Contaminated = false

	officer := swapi.HumanDotation{
		Quantity:     1,
		Rank:         eOfficer,
		State:        eDead,
		Injury:       0,
		Psyop:        false,
		Contaminated: false,
	}

	warrantOfficer := swapi.HumanDotation{
		Quantity:     1245,
		Rank:         eWarrantOfficer,
		State:        eInjured,
		Injury:       eInjuryUe,
		Psyop:        false,
		Contaminated: false,
	}

	err = client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{&human, &officer, &warrantOfficer})
	c.Assert(err, IsNil)

	// Check human composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eOfficer, eDead, 1, 0) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eWarrantOfficer, eInjured, 4, eInjuryUe) &&
			CheckHumanState(data.FindUnit(u1.Id).HumanDotations, eTrooper, eHealthy, 7, 0)
	})
}

func (s *TestSuite) TestUnitChangeDotation(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)

	// Check initial state
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FindUnit(u1.Id).ResourceDotations) > 1
	})

	u1 = client.Model.GetData().FindUnit(u1.Id)
	firstDotation := u1.ResourceDotations[0]
	secondDotation := u1.ResourceDotations[1]

	c.Assert(firstDotation, DeepEquals, &swapi.ResourceDotation{Type: 1, Quantity: 3200, Threshold: 10})
	c.Assert(secondDotation, DeepEquals, &swapi.ResourceDotation{Type: 3, Quantity: 8000, Threshold: 10})

	// Error: Invalid parameters count
	err := client.ChangeDotation(u1.Id, []*swapi.ResourceDotation{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters count, 1 parameter expected")

	resource := swapi.ResourceDotation{
		Type:      1245,
		Quantity:  0,
		Threshold: 0,
	}

	// Error: Invalid dotation category
	err = client.ChangeDotation(u1.Id, []*swapi.ResourceDotation{&resource})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[0\] must be a valid dotation category identifier`)
	resource.Type = 1

	// Error: Quantity must be positive
	resource.Quantity = -123
	err = client.ChangeDotation(u1.Id, []*swapi.ResourceDotation{&resource})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[1\] must be a positive number`)
	resource.Quantity = 1000

	// Error: Threshold must be a number between 0 and 100
	resource.Threshold = 123
	err = client.ChangeDotation(u1.Id, []*swapi.ResourceDotation{&resource})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[2\] must be a number between 0 and 100`)
	resource.Threshold = 50

	// Change dotation
	resource2 := swapi.ResourceDotation{
		Type:      3,
		Quantity:  0,
		Threshold: 100,
	}

	err = client.ChangeDotation(u1.Id, []*swapi.ResourceDotation{&resource, &resource2})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return *data.FindUnit(u1.Id).ResourceDotations[0] == resource &&
			*data.FindUnit(u1.Id).ResourceDotations[1] == resource2
	})

	// Change dotation with a huge quantity
	resource.Quantity = 123456789
	err = client.ChangeDotation(u1.Id, []*swapi.ResourceDotation{&resource})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		dotation := *data.FindUnit(u1.Id).ResourceDotations[0]
		return dotation.Quantity == int32(1000)
	})
}

func (s *TestSuite) TestUnitChangeEquipmentState(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)
	equipmentId := uint32(11)
	equipment := swapi.EquipmentDotation{
		Available:     4,
		Unavailable:   0,
		Repairable:    0,
		OnSiteFixable: 0,
		Repairing:     0,
		Captured:      0,
		Breakdowns:    nil,
	}

	// Check initial state
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FindUnit(u1.Id).EquipmentDotations) != 0
	})

	c.Assert(*client.Model.GetData().FindUnit(u1.Id).EquipmentDotations[equipmentId], DeepEquals, equipment)

	// Error: Invalid parameters count
	err := client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters count, 1 parameter expected")

	// Error: Invalid negative parameters
	equipment.Available = -1
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[1\] must be a positive number`)

	// Can't exceed the initial quantity
	equipment = swapi.EquipmentDotation{
		Available:     0,
		Unavailable:   10,
		Repairable:    0,
		OnSiteFixable: 0,
		Repairing:     0,
		Captured:      0,
		Breakdowns:    nil,
	}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	equipment.Unavailable = 4
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.FindUnit(u1.Id).EquipmentDotations[equipmentId], &equipment)
	})

	// Change equipments
	equipment = swapi.EquipmentDotation{
		Available:     1,
		Unavailable:   1,
		Repairable:    0,
		OnSiteFixable: 0,
		Repairing:     1,
		Captured:      1,
		Breakdowns:    nil,
	}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.FindUnit(u1.Id).EquipmentDotations[equipmentId], &equipment)
	})

	// Error: breakdown missing
	equipment = swapi.EquipmentDotation{
		Available:     1,
		Unavailable:   1,
		Repairable:    1,
		OnSiteFixable: 0,
		Repairing:     1,
		Captured:      0,
		Breakdowns:    nil,
	}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[7\] size must be equal to parameters\[0\]\[0\]\[3\]`)

	// Error: invalid breakdown
	equipment.Breakdowns = []int32{1523}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[7\] must be a breakdown identifier`)

	// Error: invalid breakdown for the composante
	equipment.Breakdowns = []int32{1}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[7\] invalid breakdown identifier for the composante`)

	// Change equipments
	equipment.Breakdowns = []int32{82}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.FindUnit(u1.Id).EquipmentDotations[equipmentId], &equipment)
	})
}

func (s *TestSuite) TestUnitCreateBreakdowns(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)
	equipmentId := uint32(11)

	initial := swapi.EquipmentDotation{
		Available:     4,
		Unavailable:   0,
		Repairable:    0,
		OnSiteFixable: 0,
		Repairing:     0,
		Captured:      0,
		Breakdowns:    nil,
	}

	// Error: Invalid parameters count
	err := client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters count, 1 parameter expected")

	equipment := swapi.EquipmentDotation{
		Available:     0,
		Unavailable:   0,
		Repairable:    1,
		OnSiteFixable: 0,
		Repairing:     0,
		Captured:      0,
		Breakdowns:    []int32{82},
	}

	// Error: Invalid equipment type
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{123: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[0\] must be an composante type which can have breakdown`)

	// Error: Quantity must be non-zero positive
	equipment.Repairable = -1
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[1\] must be positive a non-zero positive number`)
	equipment.Repairable = 0
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[1\] must be positive a non-zero positive number`)

	equipment.Repairable = 1
	// Error: invalid breakdown
	equipment.Breakdowns = []int32{1523}
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[2\] must be a breakdown type identifier`)

	// Create breakdown
	equipment.Breakdowns = []int32{82}
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	initial.Repairable = 1
	initial.Available = 3
	initial.Breakdowns = []int32{82}
	// Check breakdown
	initial.Breakdowns = equipment.Breakdowns
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.FindUnit(u1.Id).EquipmentDotations[equipmentId], &initial)
	})

	// Create 5 breakdowns but 3 available
	equipment = swapi.EquipmentDotation{
		Available:     0,
		Unavailable:   0,
		Repairable:    5,
		OnSiteFixable: 0,
		Repairing:     0,
		Captured:      0,
		Breakdowns:    []int32{82},
	}
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	// Check breakdown
	initial.Repairable = 4
	initial.Available = 0
	initial.Breakdowns = []int32{82, 82, 82, 82}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.FindUnit(u1.Id).EquipmentDotations[equipmentId], &initial)
	})
}
