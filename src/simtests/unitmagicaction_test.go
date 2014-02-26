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
	handler := func(msg *swapi.SwordMessage, id, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.SimulationToClient == nil || msg.Context != context {
			return false
		}
		m := msg.SimulationToClient.GetMessage()
		if reply := m.GetUnitMagicActionAck(); reply != nil {
			code := reply.GetErrorCode()
			if code != sword.UnitActionAck_error_invalid_unit {
				name := sword.UnitActionAck_ErrorCode_name[int32(code)]
				err = fmt.Errorf("Got unexpected error: %v", name)
			} else {
				err = fmt.Errorf("error_invalid_unit")
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
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	model := client.Model
	data := model.GetData()

	formationId := getSomeFormation(c, data).Id
	automatId := getSomeAutomat(c, data).Id
	unitId := getSomeUnit(c, data).Id
	crowdId := getSomeCrowd(c, data).Id
	popId := getSomePopulation(c, data).Id

	taskers := []*sword.Tasker{
		swapi.MakeFormationTasker(formationId),
		swapi.MakeAutomatTasker(automatId),
		swapi.MakeUnitTasker(unitId),
		swapi.MakeCrowdTasker(crowdId),
		swapi.MakePopulationTasker(popId),
	}
	for _, tasker := range taskers {
		err := postInvalidTasker(client, tasker)
		c.Assert(err, ErrorMatches, "error_invalid_unit.*",
			Commentf("for tasker %v", tasker))
	}
}

func (s *TestSuite) TestCreateFormation(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)

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

	// Invalid formation parameters (empty)
	_, err = client.CreateFormationTest(0, f1.Id, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Logistic level is optional or null
	tests := []*sword.MissionParameters{
		swapi.MakeParameters(float32(1), "some name"),
		swapi.MakeParameters(float32(1), "some name", nil),
	}
	for _, params := range tests {
		f, err := client.CreateFormationTest(0, f1.Id, params)
		c.Assert(err, IsNil)
		c.Assert(f, NotNil)
	}
}

func (s *TestSuite) TestCreateUnit(c *C) {
	opts := ClientOpts{
		User:     "user",
		Password: "user",
		Exercise: ExCrossroadSmallOrbat,
	}
	sim, client := connectAndWaitModel(c, &opts)
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()

	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	formation := getSomeFormation(c, data)

	// Find a suitable knowledge group matching the formation, this should be
	// simpler...
	var kg *swapi.KnowledgeGroup
	for _, g := range data.KnowledgeGroups {
		if g.PartyId == formation.PartyId {
			kg = g
			break
		}
	}
	c.Assert(kg, NotNil)

	automat, err := client.CreateAutomat(formation.Id, AutomatType, kg.Id)
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
	c.Assert(u.AutomatId, Equals, automat.Id)
	c.Assert(u.Pc, Equals, true)

	// Check unit position
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return isNearby(data.Units[u.Id].Position, pos)
	})

	pos = swapi.Point{X: -15.8219, Y: 28.2456}

	// Second unit, not PC
	u, err = client.CreateUnit(automat.Id, unitType, pos)
	c.Assert(err, IsNil)
	c.Assert(u, NotNil)
	c.Assert(u.Pc, Equals, false)

	// Check unit position
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return isNearby(data.Units[u.Id].Position, pos)
	})

	automat, err = client.CreateAutomat(formation.Id, AutomatType, kg.Id)
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
		return data.Units[u2.Id].Pc
	})
	c.Assert(ok, Equals, true)
	ok = client.Model.WaitCondition(func(data *swapi.ModelData) bool {
		return !data.Units[u1.Id].Pc
	})
	c.Assert(ok, Equals, true)

	// Delete the PC, the other unit becomes the Pc eventually
	err = client.DeleteUnit(u2.Id)
	c.Assert(err, IsNil)
	ok = client.Model.WaitCondition(func(data *swapi.ModelData) bool {
		return data.Units[u1.Id].Pc
	})
	c.Assert(ok, Equals, true)

	// Read-only user can create unit (wut?)
	client.Close()
	opts = ClientOpts{
		User:     "user-readonly",
		Password: "user-readonly",
	}
	client = loginAndWaitModel(c, sim, &opts)
	u, err = client.CreateUnit(automat.Id, unitType, pos)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestDeleteUnit(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	model := client.Model
	data := model.GetData()

	// Destroy invalid unit
	err := client.DeleteUnit(1234)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Find some unit and make it move
	unit := getSomeUnitByName(c, data, "ARMOR.MBT")
	parent := model.GetAutomat(unit.AutomatId)
	c.Assert(parent, NotNil)
	err = client.SetAutomatMode(parent.Id, false)
	c.Assert(err, IsNil)

	heading := swapi.MakeHeading(0)
	dest := swapi.MakePointParam(swapi.Point{X: -15.8193, Y: 28.3456})
	params := swapi.MakeParameters(heading, nil, nil, nil, dest)
	_, err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !isNearby(data.Units[unit.Id].Position, unit.Position)
	})

	// Blast it
	err = client.DeleteUnit(unit.Id)
	c.Assert(err, IsNil)
	c.Assert(model.GetUnit(unit.Id), IsNil)
}

func (s *TestSuite) TestCreateCrowd(c *C) {
	checkError := func(crowd *swapi.Crowd, err error, expected string) {
		c.Assert(crowd, IsNil)
		c.Assert(err, IsSwordError, expected)
	}

	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
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

	formation := getSomeFormation(c, model.GetData())

	// Create crowd in party with formation identifier
	crowd, err = client.CreateCrowd(formation.Id, 0, CrowdType, pos, healthy,
		wounded, dead, crowdName)
	c.Assert(err, IsNil)
	c.Assert(crowd, NotNil)
}

func (s *TestSuite) TestTeleport(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	pos := swapi.Point{X: -15.8219, Y: 28.2456}

	// No tasker
	err = client.Teleport(swapi.MakeUnitTasker(0), pos)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot teleport unit if its automat is engaged
	err = client.Teleport(swapi.MakeUnitTasker(unit.Id), pos)
	c.Assert(err, IsSwordError, "error_automat_engaged")

	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Teleport unit
	err = client.Teleport(swapi.MakeUnitTasker(unit.Id), pos)
	c.Assert(err, IsNil)

	// Check unit position
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return isNearby(data.Units[unit.Id].Position, pos)
	})
}

func (s *TestSuite) TestLogisticsChangeLinks(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

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
		a, ok := data.Automats[9]
		return ok && len(a.LogSuperiors) > 1
	})
	automat := client.Model.GetData().Automats[9]
	c.Assert(automat.LogSuperiors, DeepEquals, newSuperiors)
}

func (s *TestSuite) TestLogisticsSupplyChangeQuotas(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: invalid supplied id parameter
	err := client.LogisticsSupplyChangeQuotas(25, swapi.MakeAutomatTasker(42), map[uint32]int32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid supplier id parameter
	err = client.LogisticsSupplyChangeQuotas(42, swapi.MakeAutomatTasker(23), map[uint32]int32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error : valid units id but no dotation
	err = client.LogisticsSupplyChangeQuotas(25, swapi.MakeAutomatTasker(23), map[uint32]int32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// quotas model updated and no error with valid ids
	newQuotas := map[uint32]int32{1: 100, 2: 200}
	err = client.LogisticsSupplyChangeQuotas(25, swapi.MakeAutomatTasker(23), newQuotas)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		a, ok := data.Automats[23]
		return ok && len(a.SuperiorQuotas) > 0
	})
	automat := client.Model.GetData().Automats[23]
	c.Assert(automat.SuperiorQuotas, DeepEquals, newQuotas)

	quotas := map[uint32]int32{7: 400}
	formationId := uint32(25)
	err = client.LogisticsSupplyChangeQuotas(6, swapi.MakeFormationTasker(formationId), quotas)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		a, ok := data.Formations[formationId]
		return ok && len(a.SuperiorQuotas) > 0
	})
	formation := client.Model.GetData().Formations[formationId]
	c.Assert(formation.SuperiorQuotas, DeepEquals, quotas)
}

func (s *TestSuite) TestLogisticsSupplyPushFlow(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)

	data := client.Model.GetData()
	supplier := getSomeAutomatByName(c, data, "Supply 1").Id
	unit := getSomeUnitByName(c, data, "Log Unit 4 1").Id
	receiver := getSomeAutomatByName(c, data, "Supply 2").Id
	const resource = 96
	const transporter = 12
	c.Assert(data.Units[unit].Installation, Not(Equals), 100)

	// deploy supplier
	MissionLogDeploy := uint32(8)
	heading := swapi.MakeHeading(0)
	_, err := client.SendAutomatOrder(supplier, MissionLogDeploy,
		swapi.MakeParameters(heading, nil,
			swapi.MakeLimit(
				swapi.Point{X: -15.7594, Y: 28.2861},
				swapi.Point{X: -15.7647, Y: 28.2761}),
			swapi.MakeLimit(
				swapi.Point{X: -15.7538, Y: 28.2819},
				swapi.Point{X: -15.7570, Y: 28.2744}),
			nil))
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)

	// Waiting for deployment
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit].Installation == 100
	})

	// error: invalid supplier
	result, err := client.LogisticsSupplyPushFlow(1000, receiver, nil, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid supplier")
	c.Assert(result, HasLen, 0)

	// error: invalid parameters
	param := swapi.MakeParameter(&sword.MissionParameter_Value{})
	result, err = client.LogisticsSupplyPushFlowTest(supplier, swapi.MakeParameters(param))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters")
	c.Assert(result, HasLen, 0)

	// error: invalid recipients
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, nil, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: at least one resource expected")
	c.Assert(result, HasLen, 0)

	// error: invalid resource
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{1000: 1}, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid resource")
	c.Assert(result, HasLen, 0)

	// error: invalid resource quantity
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{resource: 0}, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: resource quantity must be positive")
	c.Assert(result, HasLen, 0)

	// valid no transporter
	transporters := client.Model.GetUnit(unit).EquipmentDotations[transporter].Available
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{resource: 1}, nil)
	c.Assert(err, IsNil)
	c.Assert(result, HasLen, 0)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit].EquipmentDotations[transporter].Available == transporters-1
	})

	// error: invalid transporter
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{resource: 1}, map[uint32]uint32{1000: 1})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid transporter")
	c.Assert(result, HasLen, 0)

	// error: invalid transporter quantity
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{resource: 1}, map[uint32]uint32{transporter: 0})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: transporter quantity must be positive")
	c.Assert(result, HasLen, 0)

	// error: transporter cannot load supply nature
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{resource: 1}, map[uint32]uint32{13: 1})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: not all supplies can be loaded by transporters")
	c.Assert(result, HasLen, 0)

	// error: transporter overloaded
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{resource: 10000000}, map[uint32]uint32{transporter: 1})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: transporter capacity mass overloaded")
	c.Assert(result, DeepEquals, []bool{false, true, false, true})

	// valid underloaded transporters
	transporters = client.Model.GetUnit(unit).EquipmentDotations[transporter].Available
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{resource: 1}, map[uint32]uint32{transporter: 2})
	c.Assert(err, IsNil)
	c.Assert(result, DeepEquals, []bool{true, false, true, false})
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit].EquipmentDotations[transporter].Available == transporters-2
	})

	// valid transporters
	transporters = client.Model.GetUnit(unit).EquipmentDotations[transporter].Available
	result, err = client.LogisticsSupplyPushFlow(supplier, receiver, map[uint32]uint32{resource: 5}, map[uint32]uint32{transporter: 1})
	c.Assert(err, IsNil)
	c.Assert(result, DeepEquals, []bool{false, false, false, false})
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit].EquipmentDotations[transporter].Available == transporters-1
	})
}

func (s *TestSuite) TestLogisticsSupplyPullFlow(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)

	data := client.Model.GetData()
	supplier := getSomeAutomatByName(c, data, "Supply 1").Id
	unit := getSomeUnitByName(c, data, "Log Unit 4 2").Id
	receiver := getSomeAutomatByName(c, data, "Supply 2").Id
	const resource = 96
	const transporter = 12

	// deploy receiver
	MissionLogDeploy := uint32(8)
	heading := swapi.MakeHeading(0)
	_, err := client.SendAutomatOrder(receiver, MissionLogDeploy,
		swapi.MakeParameters(heading, nil,
			swapi.MakeLimit(
				swapi.Point{X: -15.7594, Y: 28.2861},
				swapi.Point{X: -15.7647, Y: 28.2761}),
			swapi.MakeLimit(
				swapi.Point{X: -15.7538, Y: 28.2819},
				swapi.Point{X: -15.7570, Y: 28.2744}),
			nil))
	c.Assert(err, IsNil)

	// Waiting for deployment
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit].Installation == 100
	})

	// error: invalid supplier parameter
	result, err := client.LogisticsSupplyPullFlow(receiver, 1000, nil, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid supplier")
	c.Assert(result, HasLen, 0)

	// error: invalid receiver parameter
	result, err = client.LogisticsSupplyPullFlow(1000, supplier, nil, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid receiver")
	c.Assert(result, HasLen, 0)

	// error: invalid parameters
	param := swapi.MakeParameter(&sword.MissionParameter_Value{})
	result, err = client.LogisticsSupplyPullFlowTest(receiver, swapi.MakeParameters(param))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters")
	c.Assert(result, HasLen, 0)

	// error: invalid resource
	result, err = client.LogisticsSupplyPullFlow(receiver, supplier, map[uint32]uint32{1000: 1}, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid resource")
	c.Assert(result, HasLen, 0)

	// valid no transporter
	transporters := client.Model.GetUnit(unit).EquipmentDotations[transporter].Available
	result, err = client.LogisticsSupplyPullFlow(receiver, supplier, map[uint32]uint32{resource: 1}, nil)
	c.Assert(err, IsNil)
	c.Assert(result, HasLen, 0)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit].EquipmentDotations[transporter].Available == transporters-1
	})

	// error: invalid transporter
	result, err = client.LogisticsSupplyPullFlow(receiver, supplier, map[uint32]uint32{resource: 1}, map[uint32]uint32{1000: 1})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid transporter")
	c.Assert(result, HasLen, 0)

	// error: invalid transporter quantity
	result, err = client.LogisticsSupplyPullFlow(receiver, supplier, map[uint32]uint32{resource: 1}, map[uint32]uint32{transporter: 0})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: transporter quantity must be positive")
	c.Assert(result, HasLen, 0)

	// error: transporter cannot load supply nature
	result, err = client.LogisticsSupplyPullFlow(supplier, receiver, map[uint32]uint32{resource: 1}, map[uint32]uint32{13: 1})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: not all supplies can be loaded by transporters")
	c.Assert(result, HasLen, 0)

	// error: transporter overloaded
	result, err = client.LogisticsSupplyPullFlow(receiver, supplier, map[uint32]uint32{resource: 10000000}, map[uint32]uint32{transporter: 1})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: transporter capacity mass overloaded")
	c.Assert(result, DeepEquals, []bool{false, true, false, true})

	// valid underloaded transporter
	transporters = client.Model.GetUnit(unit).EquipmentDotations[transporter].Available
	result, err = client.LogisticsSupplyPullFlow(receiver, supplier, map[uint32]uint32{resource: 1}, map[uint32]uint32{transporter: 2})
	c.Assert(err, IsNil)
	c.Assert(result, DeepEquals, []bool{true, false, true, false})
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit].EquipmentDotations[transporter].Available == transporters-2
	})

	// valid transporter
	transporters = client.Model.GetUnit(unit).EquipmentDotations[transporter].Available
	result, err = client.LogisticsSupplyPullFlow(receiver, supplier, map[uint32]uint32{resource: 5}, map[uint32]uint32{transporter: 1})
	c.Assert(err, IsNil)
	c.Assert(result, DeepEquals, []bool{false, false, false, false})
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit].EquipmentDotations[transporter].Available == transporters-1
	})
}

func CheckUnitSuperior(model *swapi.Model, c *C,
	unitId, newAutomatId, oldAutomatId uint32) {
	// Check AutomatId attribute in Unit
	unit := model.GetUnit(unitId)
	c.Assert(unit, NotNil)
	c.Assert(unit.AutomatId, Equals, newAutomatId)

	data := model.GetData()
	// Check oldAutomat is empty
	oldAutomat := data.Automats[oldAutomatId]
	c.Assert(oldAutomat, NotNil)
	c.Assert(getAutomatUnits(data, oldAutomat.Id), HasLen, 0)

	// Check newAutomat contains one element
	newAutomat := data.Automats[newAutomatId]
	c.Assert(newAutomat, NotNil)
	c.Assert(getAutomatUnits(data, newAutomat.Id), HasLen, 1)
}

func CheckAutomatSuperior(model *swapi.Model, c *C,
	automatId, newFormationId, oldFormationId uint32) {

	d := model.GetData()
	// Check AutomatId attribute in Unit
	automat := d.Automats[automatId]
	c.Assert(automat, NotNil)
	c.Assert(automat.FormationId, Equals, newFormationId)

	// Check oldFormation is empty
	oldFormation := d.Formations[oldFormationId]
	c.Assert(oldFormation, NotNil)
	c.Assert(getFormationAutomats(d, oldFormation.Id), HasLen, 0)

	// Check newFormation contains one element
	newFormation := d.Formations[newFormationId]
	c.Assert(newFormation, NotNil)
	c.Assert(getFormationAutomats(d, newFormation.Id), HasLen, 1)
}

func CheckFormationSuperior(model *swapi.Model, c *C,
	formationId, newParentId uint32) {

	d := model.GetData()
	// Check AutomatId attribute in Unit
	formation := d.Formations[formationId]
	c.Assert(formation, NotNil)

	// Check newFormation or newParty contains one element
	newFormation := d.Formations[newParentId]
	if newFormation == nil {
		newParty := d.Parties[newParentId]
		c.Assert(newParty, NotNil)
		c.Assert(formation.ParentId, Equals, uint32(0))
	} else {
		c.Assert(newFormation, NotNil)
		c.Assert(getChildFormations(d, newFormation.Id), HasLen, 1)
		c.Assert(formation.ParentId, Equals, newParentId)
	}
}

func CreateFormation(c *C, client *swapi.Client, partyId uint32) *swapi.Formation {
	formation, err := client.CreateFormation(partyId, 0, "newformation", 1, "")
	c.Assert(err, IsNil)
	return formation
}

func CreateAutomat(c *C, client *swapi.Client, formationId, groupId uint32) *swapi.Automat {
	data := client.Model.GetData()
	if groupId == 0 {
		groupId = getSomeKnowledgeGroup(c, data).Id
	}
	kg0, ok := data.KnowledgeGroups[groupId]
	c.Assert(ok, Equals, true)
	c.Assert(kg0, NotNil)
	automat, err := client.CreateAutomat(formationId, AutomatType, kg0.Id)
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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	f1 := CreateFormation(c, client, 1)

	// Create 2 automats
	data := client.Model.GetData()
	kg0 := getAnyKnowledgeGroupIdWithPartyIndex(c, data, 0)
	kg1 := getAnyKnowledgeGroupIdWithPartyIndex(c, data, 1)
	a1 := CreateAutomat(c, client, f1.Id, kg0)
	a2 := CreateAutomat(c, client, f1.Id, kg0)

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
	a3 := CreateAutomat(c, client, f2.Id, kg1)

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

func checkUnitFireDamages(msg *sword.UnitsFireDamages) error {
	if msg == nil {
		return nil
	}
	for _, d := range msg.Elem {
		for _, eq := range d.Equipments.Elem {
			// Damages cannot recreate/repair units
			if *eq.Available > 0 || *eq.Unavailable < 0 || *eq.Repairable < 0 {
				return fmt.Errorf("invalid damage message: %v", msg)
			}
		}
	}
	return nil
}

func checkUnitsFireDamages(msg *swapi.SwordMessage) error {
	if msg.SimulationToClient == nil {
		return nil
	}
	m := msg.SimulationToClient.GetMessage()
	switch {
	case m.Explosion != nil:
		return checkUnitFireDamages(m.Explosion.UnitsDamages)
	case m.StopCrowdFire != nil:
		return checkUnitFireDamages(m.StopCrowdFire.UnitsDamages)
	case m.StopUnitFire != nil:
		return checkUnitFireDamages(m.StopUnitFire.UnitsDamages)
	}
	return nil
}

func (s *TestSuite) TestFireOrderCreationOnUnit(c *C) {
	opts := NewAdminOpts(ExCrossroadSmallOrbat)
	opts = FixSeed(opts) // help with mortar shots
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)

	// Check unit damages messages
	var damagesErrors []error
	handlerId := client.Register(func(msg *swapi.SwordMessage, id, ctx int32, err error) bool {
		if err != nil {
			return true
		}
		if err := checkUnitsFireDamages(msg); err != nil {
			damagesErrors = append(damagesErrors, err)
		}
		return false
	})

	f1 := CreateFormation(c, client, 1)
	f2 := CreateFormation(c, client, 2)

	// Create 2 automats
	data := client.Model.GetData()
	kg0 := getAnyKnowledgeGroupIdWithPartyIndex(c, data, 0)
	kg1 := getAnyKnowledgeGroupIdWithPartyIndex(c, data, 1)
	a1 := CreateAutomat(c, client, f1.Id, kg0)
	a2 := CreateAutomat(c, client, f2.Id, kg1)

	// Create 2 mortar units
	const infMortarTroopType = 31
	reporter, err := client.CreateUnit(a1.Id, infMortarTroopType, swapi.Point{X: -15.8219, Y: 28.2456})
	c.Assert(err, IsNil)
	target, err := client.CreateUnit(a2.Id, infMortarTroopType, swapi.Point{X: -15.8219, Y: 28.2456})
	c.Assert(err, IsNil)

	const fullyOperational = 100
	// Waiting for the target initialization
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Units[target.Id]
		return !unit.Neutralized && unit.RawOperationalState == fullyOperational
	})

	// Adding the target in reporter's knowledges
	const identifiedLevel = 0
	targetKnowledge, err := client.CreateUnitKnowledge(a1.KnowledgeGroupId, target.Id, identifiedLevel)
	c.Assert(err, IsNil)

	// Launching a magic strike with good parameters
	const dotation81mmHighExplosiveShell = 9
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, dotation81mmHighExplosiveShell, 1)
	c.Assert(err, IsNil)

	// testing strike effect: neutralization and attrition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Units[target.Id]
		return unit.Neutralized && unit.RawOperationalState < fullyOperational
	})

	// Testing wrong reporter identifier
	err = client.CreateFireOrderOnUnit(0, targetKnowledge.Id, dotation81mmHighExplosiveShell, 1)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Testing wrong knowledge identifier
	err = client.CreateFireOrderOnUnit(reporter.Id, 0, dotation81mmHighExplosiveShell, 1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Testing wrong dotation identifier
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, 0, 1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Testing direct fire dotation
	const directFireDotation9mmBullet = 17
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, directFireDotation9mmBullet, 1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Testing negative or empty iterations
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, dotation81mmHighExplosiveShell, -1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, dotation81mmHighExplosiveShell, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Testing strike on non illuminated target with guided dotation
	const dotation120mmHightExplosiveShellGuided = 34
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, dotation120mmHightExplosiveShellGuided, 1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	client.Unregister(handlerId)
	c.Assert(damagesErrors, IsNil)
}

func (s *TestSuite) TestPcChangeSuperior(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

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
		return data.Units[u2.Id].Pc
	})

	// Change superior u1 -> a2
	err = client.ChangeUnitSuperior(u1.Id, a2.Id)
	c.Assert(err, IsNil)

	// Check u1 is not PC
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.Units[u1.Id].Pc
	})
}

func (s *TestSuite) TestAutomatChangeSuperior(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

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
		return data.Automats[a1.Id].FormationId == f2.Id
	})
	CheckAutomatSuperior(client.Model, c, a1.Id, f2.Id, f1.Id)

	// Change superior u2 -> a1 again, does nothing
	err = client.ChangeAutomatSuperior(a1.Id, f1.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Automats[a1.Id].FormationId == f1.Id
	})
	CheckAutomatSuperior(client.Model, c, a1.Id, f1.Id, f2.Id)
}

func (s *TestSuite) TestFormationChangeSuperior(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

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
		return data.Formations[f1.Id].ParentId == f2.Id
	})
	CheckFormationSuperior(client.Model, c, f1.Id, f2.Id)

	// Change superior f1 -> party1
	err = client.ChangeFormationSuperior(f1.Id, 1, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Formations[f1.Id].ParentId == 0
	})
	CheckFormationSuperior(client.Model, c, f1.Id, 1)
}

func (s *TestSuite) TestDebugBrain(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
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
		return model.Units[unit.Id].DebugBrain
	})

	// Enable again
	err = client.DebugBrain(unit.Id, true)
	c.Assert(err, IsNil)

	// Give a mission and wait for debug reports to be emitted
	seen := make(chan error)
	client.Register(func(msg *swapi.SwordMessage, id, context int32, err error) bool {
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
		swapi.MakeHeading(0), nil, nil, nil, swapi.MakePointParam(to))
	_, err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
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
		return !model.Units[unit.Id].DebugBrain
	})
}

func CheckLentEquipment(data *swapi.ModelData, from uint32, to uint32, available int32, lent int32) bool {
	unit := data.Units[from]
	return unit.EquipmentDotations[11].Available == available &&
		(lent == 0 && len(unit.LentEquipments) == 0 ||
			len(unit.LentEquipments) != 0 && unit.LentEquipments[0].Borrower == to && unit.LentEquipments[0].TypeId == 11 && unit.LentEquipments[0].Quantity == lent)
}

func CheckBorrowedEquipment(data *swapi.ModelData, from uint32, to uint32, available int32, borrowed int32) bool {
	target := data.Units[to]
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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: invalid parameters count
	err := client.TransferEquipment(11, 12, nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid empty equipment list")

	// error: invalid unit identifier
	err = client.TransferEquipment(1000, 12, []swapi.Equipment{{11, 1}})
	c.Assert(err, ErrorMatches, "error_invalid_unit.*")

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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	isSurrendered := func(data *swapi.ModelData, automatId, partyId uint32) bool {
		for _, k := range getAutomatUnits(data, automatId) {
			if data.Units[uint32(k)].PartySurrenderedTo != partyId {
				return false
			}
		}
		return true
	}

	data := client.Model.GetData()
	automat := getSomeAutomat(c, data)

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
	err = client.SurrenderTest(automat.Id, swapi.MakeParameters(nil))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: bad number of parameters
	err = client.SurrenderTest(automat.Id, swapi.MakeParameters(nil, nil))
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
	a2 := CreateAutomat(c, client, automat.FormationId, 3)
	u2 := CreateUnit(c, client, a2.Id)
	// Wait for the unit to be fully generated, there is an update race
	// otherwise masking the previous failure.
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Units[u2.Id].EquipmentDotations) > 0
	})
	err = client.ChangeUnitSuperior(u2.Id, automat.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[u2.Id].AutomatId == automat.Id &&
			isSurrendered(data, automat.Id, otherPartyId)
	})

	// Move it back to a free automat
	err = client.ChangeUnitSuperior(u2.Id, a2.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[u2.Id].AutomatId == a2.Id &&
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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)

	// Check initial humans state
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanQuantity(data.Units[u1.Id].HumanDotations,
			map[int32]int32{eOfficer: 1, eWarrantOfficer: 4, eTrooper: 7})
	})

	// Error: Invalid parameters count
	err := client.CreateWounds(u1.Id, map[int32]int32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: Quantity must be non-zero positive
	err = client.CreateWounds(u1.Id, map[int32]int32{0: 0})
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	err = client.CreateWounds(u1.Id, map[int32]int32{0: -10})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: Invalid wound
	err = client.CreateWounds(u1.Id, map[int32]int32{123: 10})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

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
		return CheckHumanState(data.Units[u1.Id].HumanDotations, eOfficer, eHealthy, 1, notWounded) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eWarrantOfficer, eHealthy, 4, notWounded) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eHealthy, 1, notWounded) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eInjured, quantity, eInjuryU2) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eInjured, quantity, eInjuryU1)
	})

	// Kill all humans
	err = client.CreateWounds(u1.Id, map[int32]int32{dead: 1234})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanState(data.Units[u1.Id].HumanDotations, eOfficer, eDead, 1, notWounded) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eWarrantOfficer, eDead, 4, notWounded) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eDead, 1, notWounded) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eInjured, quantity, eInjuryU2) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eInjured, quantity, eInjuryU1)
	})

	// Heal humans
	err = client.CreateWounds(u1.Id, map[int32]int32{notWounded: 7})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanState(data.Units[u1.Id].HumanDotations, eOfficer, eDead, 1, notWounded) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eWarrantOfficer, eDead, 4, notWounded) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eHealthy, 7, notWounded)
	})
}

func (s *TestSuite) TestUnitChangeHumanState(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)

	// Check initial humans state
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanQuantity(data.Units[u1.Id].HumanDotations,
			map[int32]int32{eOfficer: 1, eWarrantOfficer: 4, eTrooper: 7})
	})

	// Error: Invalid parameters count
	err := client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters count, 1 parameter expected")

	human := swapi.HumanDotation{
		Quantity: -10,
		Rank:     eOfficer,
		State:    eHealthy,
		Injury:   eInjuryU2,
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
		return CheckHumanState(data.Units[u1.Id].HumanDotations, eOfficer, eHealthy, 1, 0) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eWarrantOfficer, eHealthy, 4, 0) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eHealthy, 6, 0) &&
			CheckHumanTotalState(data.Units[u1.Id].HumanDotations, eTrooper, eInjured, 1, eInjuryU3, true, true)
	})

	// Heal trooper, kill officer and wound 1245 warrant officers
	human.State = eHealthy
	human.Injury = 0
	human.Psyop = false
	human.Contaminated = false
	human.Quantity = 7

	officer := swapi.HumanDotation{
		Quantity: 1,
		Rank:     eOfficer,
		State:    eDead,
	}

	warrantOfficer := swapi.HumanDotation{
		Quantity: 1245,
		Rank:     eWarrantOfficer,
		State:    eInjured,
		Injury:   eInjuryUe,
	}

	err = client.ChangeHumanState(u1.Id, []*swapi.HumanDotation{&human, &officer, &warrantOfficer})
	c.Assert(err, IsNil)

	// Check human composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumanState(data.Units[u1.Id].HumanDotations, eOfficer, eDead, 1, 0) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eWarrantOfficer, eInjured, 4, eInjuryUe) &&
			CheckHumanState(data.Units[u1.Id].HumanDotations, eTrooper, eHealthy, 7, 0)
	})
}

func (s *TestSuite) TestUnitChangeDotation(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)

	// Check initial state
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Units[u1.Id].ResourceDotations) > 1
	})

	u1 = client.Model.GetUnit(u1.Id)
	firstDotationId := uint32(1)
	secondDotationId := uint32(3)

	firstDotation := u1.ResourceDotations[firstDotationId]
	secondDotation := u1.ResourceDotations[secondDotationId]

	c.Assert(firstDotation, DeepEquals,
		swapi.ResourceDotation{
			Quantity:  3200,
			Threshold: 10,
		})
	c.Assert(secondDotation, DeepEquals,
		swapi.ResourceDotation{
			Quantity:  8000,
			Threshold: 10,
		})

	// Error: Invalid parameters count
	err := client.ChangeDotation(u1.Id, map[uint32]*swapi.ResourceDotation{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters count, 1 parameter expected")

	resource := swapi.ResourceDotation{}

	// Error: Invalid dotation category
	err = client.ChangeDotation(u1.Id, map[uint32]*swapi.ResourceDotation{1245: &resource})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[0\] must be a valid dotation category identifier`)

	// Error: Quantity must be positive
	resource.Quantity = -123
	err = client.ChangeDotation(u1.Id, map[uint32]*swapi.ResourceDotation{firstDotationId: &resource})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[1\] must be a positive number`)
	resource.Quantity = 1000

	// Error: Threshold must be a number between 0 and 100
	resource.Threshold = 123
	err = client.ChangeDotation(u1.Id, map[uint32]*swapi.ResourceDotation{firstDotationId: &resource})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[2\] must be a number between 0 and 100`)
	resource.Threshold = 50

	// Change 2 dotations
	resource2 := swapi.ResourceDotation{
		Threshold: 100,
	}
	err = client.ChangeDotation(u1.Id, map[uint32]*swapi.ResourceDotation{firstDotationId: &resource, secondDotationId: &resource2})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[u1.Id].ResourceDotations[firstDotationId] == resource &&
			data.Units[u1.Id].ResourceDotations[secondDotationId] == resource2
	})

	// Change 1 dotation
	resource.Threshold = 42
	err = client.ChangeDotation(u1.Id, map[uint32]*swapi.ResourceDotation{firstDotationId: &resource})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[u1.Id].ResourceDotations[firstDotationId] == resource &&
			data.Units[u1.Id].ResourceDotations[secondDotationId] == resource2
	})

	// Change dotation with a huge quantity
	resource.Quantity = 123456789
	err = client.ChangeDotation(u1.Id, map[uint32]*swapi.ResourceDotation{firstDotationId: &resource})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		dotation := data.Units[u1.Id].ResourceDotations[firstDotationId]
		return dotation.Quantity == int32(1000)
	})
}

func (s *TestSuite) TestUnitChangeEquipmentState(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)
	equipmentId := uint32(11)
	equipment := swapi.EquipmentDotation{
		Available: 4,
	}

	// Check initial state
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Units[u1.Id].EquipmentDotations) != 0
	})

	c.Assert(*client.Model.GetUnit(u1.Id).EquipmentDotations[equipmentId], DeepEquals, equipment)

	// Error: Invalid parameters count
	err := client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid parameters count, 1 parameter expected")

	// Error: Invalid negative parameters
	equipment.Available = -1
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[1\] must be a positive number`)

	// Error: Cannot exceed the initial quantity
	equipment = swapi.EquipmentDotation{
		Unavailable: 10,
	}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: number of equipment states \\(10\\) different from number of existing equipments \\(4\\)")

	// Error: Cannot change to repairing
	equipment = swapi.EquipmentDotation{
		Available:   1,
		Unavailable: 1,
		Repairing:   1,
		Captured:    1,
	}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, ErrorMatches, "error_invalid_parameter: cannot change an equipment state to in maintenance directly")

	// Change equipments
	equipment = swapi.EquipmentDotation{
		Available:     1,
		Unavailable:   1,
		OnSiteFixable: 1,
		Captured:      1,
	}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[u1.Id].EquipmentDotations[equipmentId], &equipment)
	})

	// Error: breakdown missing
	equipment = swapi.EquipmentDotation{
		Available:     1,
		Unavailable:   1,
		Repairable:    1,
		OnSiteFixable: 1,
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
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\]\[0\]\[7\] invalid breakdown identifier for the equipment`)

	// Change equipments
	equipment.Breakdowns = []int32{82}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[u1.Id].EquipmentDotations[equipmentId], &equipment)
	})

	// Change breakdown
	equipment.Breakdowns = []int32{83}
	err = client.ChangeEquipmentState(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[u1.Id].EquipmentDotations[equipmentId], &equipment)
	})
}

func (s *TestSuite) TestUnitCreateBreakdowns(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)
	equipmentId := uint32(11)

	initial := swapi.EquipmentDotation{
		Available: 4,
	}

	// Error: Invalid parameters count
	err := client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	equipment := swapi.EquipmentDotation{
		Repairable: 1,
		Breakdowns: []int32{82},
	}

	// Error: Invalid equipment type
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{123: &equipment})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: Quantity must be non-zero positive
	equipment.Repairable = -1
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	equipment.Repairable = 0
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	equipment.Repairable = 1
	// Error: invalid breakdown
	equipment.Breakdowns = []int32{1523}
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

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
		return reflect.DeepEqual(data.Units[u1.Id].EquipmentDotations[equipmentId], &initial)
	})

	// Create 5 breakdowns but 3 available
	equipment = swapi.EquipmentDotation{
		Repairable: 5,
		Breakdowns: []int32{82},
	}
	err = client.CreateBreakdowns(u1.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)

	// Check breakdown
	initial.Repairable = 4
	initial.Available = 0
	initial.Breakdowns = []int32{82, 82, 82, 82}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[u1.Id].EquipmentDotations[equipmentId], &initial)
	})
}

func (s *TestSuite) TestUnitChangePosture(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// Error: invalid parameter count
	err := client.ChangePostureTest(11, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: invalid parameter type
	err = client.ChangePostureTest(11, swapi.MakeParameters(swapi.MakeString("invalid")))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: invalid enumeration value
	err = client.ChangePostureTest(11, swapi.MakeParameters(swapi.MakeEnumeration(1000)))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Valid: change posture
	unit := client.Model.GetUnit(11)
	c.Assert(unit.Posture.Old, Not(Equals), sword.UnitAttributes_parked)
	c.Assert(unit.Posture.New, Not(Equals), sword.UnitAttributes_parked_on_self_prepared_area)
	err = client.ChangePosture(unit.Id, sword.UnitAttributes_parked)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		u := data.Units[unit.Id]
		return u.Posture.Old == sword.UnitAttributes_parked &&
			u.Posture.New == sword.UnitAttributes_parked_on_self_prepared_area &&
			u.Posture.Transition > 0
	})
}

func (s *TestSuite) TestUnitChangeAdhesions(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)

	// By default, adhesions are empty
	c.Assert(len(u1.Adhesions), Equals, 0)

	// Error : missing parameter
	err := client.ChangeUnitAdhesions(u1.Id, map[uint32]float32{})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: invalid parameters count, one parameter expected`)

	// Error : adhesion must be between -1 and 1
	adhesions := map[uint32]float32{0: 1.1, 1: 5.2}
	err = client.ChangeUnitAdhesions(u1.Id, adhesions)
	c.Assert(err, ErrorMatches, `error_invalid_parameter: adhesion must be between -1 and 1`)

	// Change unit adhesions
	adhesions = map[uint32]float32{0: 0.7, 1: -0.5}
	err = client.ChangeUnitAdhesions(u1.Id, adhesions)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Units[u1.Id].Adhesions) != 0
	})
	newAdhesions := client.Model.GetUnit(u1.Id).Adhesions
	c.Assert(adhesions, DeepEquals, newAdhesions)

	// No change adhesions if new adhesions are invalid
	err = client.ChangeUnitAdhesions(u1.Id,
		map[uint32]float32{0: -1.1, 1: -5.2})
	c.Assert(err, ErrorMatches, `error_invalid_parameter: adhesion must be between -1 and 1`)
	newAdhesions = client.Model.GetUnit(u1.Id).Adhesions
	c.Assert(adhesions, DeepEquals, newAdhesions)

	// Partial change
	adhesions = map[uint32]float32{0: 0.5}
	err = client.ChangeUnitAdhesions(u1.Id, map[uint32]float32{0: 0.5})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		updated := data.Units[u1.Id]
		return math.Abs(float64(updated.Adhesions[0]-0.5)) < 1e-5
	})
	newAdhesions = client.Model.GetUnit(u1.Id).Adhesions
	c.Assert(adhesions, DeepEquals, newAdhesions)
}

func (s *TestSuite) TestUnitChangeHumanFactors(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	f1 := CreateFormation(c, client, 1)
	a1 := CreateAutomat(c, client, f1.Id, 0)
	u1 := CreateUnit(c, client, a1.Id)

	// Error: invalid parameter count
	err := client.ChangeHumanFactorsTest(u1.Id, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: invalid parameter type
	invalidString := swapi.MakeString("invalid")
	validEnumeration := swapi.MakeEnumeration(0)
	err = client.ChangeHumanFactorsTest(u1.Id, swapi.MakeParameters(invalidString, validEnumeration, validEnumeration, validEnumeration))
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	err = client.ChangeHumanFactorsTest(u1.Id, swapi.MakeParameters(validEnumeration, invalidString, validEnumeration, validEnumeration))
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	err = client.ChangeHumanFactorsTest(u1.Id, swapi.MakeParameters(validEnumeration, validEnumeration, invalidString, validEnumeration))
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	err = client.ChangeHumanFactorsTest(u1.Id, swapi.MakeParameters(validEnumeration, validEnumeration, validEnumeration, invalidString))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: invalid tiredness
	err = client.ChangeHumanFactors(u1.Id, sword.UnitAttributes_EnumUnitTiredness(42), sword.UnitAttributes_fanatical, sword.UnitAttributes_veteran, sword.UnitAttributes_calm)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: invalid morale
	err = client.ChangeHumanFactors(u1.Id, sword.UnitAttributes_rested, sword.UnitAttributes_EnumUnitMorale(42), sword.UnitAttributes_veteran, sword.UnitAttributes_calm)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: invalid experience
	err = client.ChangeHumanFactors(u1.Id, sword.UnitAttributes_rested, sword.UnitAttributes_fanatical, sword.UnitAttributes_EnumUnitExperience(42), sword.UnitAttributes_calm)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error: invalid stress
	err = client.ChangeHumanFactors(u1.Id, sword.UnitAttributes_rested, sword.UnitAttributes_fanatical, sword.UnitAttributes_veteran, sword.UnitAttributes_EnumUnitStress(42))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Valid: change human factors
	err = client.ChangeHumanFactors(u1.Id, sword.UnitAttributes_tired, sword.UnitAttributes_high, sword.UnitAttributes_expert, sword.UnitAttributes_worried)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Units[u1.Id]
		return unit.HumanFactors.Tiredness == sword.UnitAttributes_tired &&
			unit.HumanFactors.Morale == sword.UnitAttributes_high &&
			unit.HumanFactors.Experience == sword.UnitAttributes_expert &&
			unit.HumanFactors.Stress == sword.UnitAttributes_worried
	})
}

func CreateTransportedAndCarrier(c *C, client *swapi.Client) (*swapi.Unit, *swapi.Unit) {
	pos := swapi.Point{X: -15.9219, Y: 28.3456}
	formation := CreateFormation(c, client, 1)
	automat := CreateAutomat(c, client, formation.Id, 0)

	// INF.Mechanized infantry platoon with 3 vehicles
	transported, err := client.CreateUnit(automat.Id, 8, pos)
	c.Assert(err, IsNil)

	// AIR.Transport Helicopter Patrol
	carrier, err := client.CreateUnit(automat.Id, 7, pos)
	c.Assert(err, IsNil)

	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	return transported, carrier
}

func Helitransport(c *C, client *swapi.Client, transported, carrier *swapi.Unit) {
	pos := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.9220, Y: 28.3456}

	// Check vehicles aren't away
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Units[transported.Id].EquipmentDotations) != 0
	})

	// Mission 'COMMON - Get transported' on the transported
	heading := swapi.MakeHeading(0)
	dest := swapi.MakePointParam(pos)
	params := swapi.MakeParameters(heading, nil, nil, nil, dest)
	_, err := client.SendUnitOrder(transported.Id, 44528, params)
	c.Assert(err, IsNil)

	// Mission 'LEG_AIR - helitransport' on the carrier
	params = swapi.MakeParameters(heading, nil, nil, nil, swapi.MakeAgent(transported.Id),
		swapi.MakePointParam(to), dest, dest,
		nil, swapi.MakeBoolean(false), swapi.MakeEnumeration(0))
	_, err = client.SendUnitOrder(carrier.Id, 13, params)
	c.Assert(err, IsNil)
}

func CheckAwayEquipment(c *C, client *swapi.Client, awayEquipmentId, transportedId,
	carrierId uint32, number int32) {
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		transported := data.Units[transportedId]
		return transported.EquipmentDotations[awayEquipmentId].Away == number &&
			transported.TransporterId == carrierId
	})
}

func (s *TestSuite) TestUnitRecoverTransporters(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	awayEquipmentId := uint32(15)
	transported, carrier := CreateTransportedAndCarrier(c, client)
	Helitransport(c, client, transported, carrier)

	// Check vehicles are away
	CheckAwayEquipment(c, client, awayEquipmentId, transported.Id, carrier.Id, 3)

	// Check unit is unloaded
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[transported.Id].TransporterId == 0
	})

	// Send recover_transporters magic action
	err := client.RecoverTransporters(transported.Id)
	c.Assert(err, IsNil)

	// Check vehicles aren't away
	CheckAwayEquipment(c, client, awayEquipmentId, transported.Id, 0, 0)
}

func (s *TestSuite) TestDeleteUnitWithAwayEquipments(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	awayEquipmentId := uint32(15)
	transported, carrier := CreateTransportedAndCarrier(c, client)
	Helitransport(c, client, transported, carrier)

	// Check vehicles are away
	CheckAwayEquipment(c, client, awayEquipmentId, transported.Id, carrier.Id, 3)

	// Delete unit during the transport
	err := client.DeleteUnit(transported.Id)
	c.Assert(err, IsNil)

	transported, carrier = CreateTransportedAndCarrier(c, client)
	Helitransport(c, client, transported, carrier)
	// Check vehicles are away
	CheckAwayEquipment(c, client, awayEquipmentId, transported.Id, carrier.Id, 3)

	// Delete unit after the transport
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[transported.Id].TransporterId == 0
	})
	err = client.DeleteUnit(transported.Id)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestUnitRecoverTransportersWithDestroyedEquipments(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	transported, carrier := CreateTransportedAndCarrier(c, client)
	awayEquipmentId := uint32(15)
	awayEquipment := swapi.EquipmentDotation{
		Available: 3,
	}
	// Transported has 3 equipments not loadable
	c.Assert(*client.Model.GetUnit(transported.Id).EquipmentDotations[awayEquipmentId],
		DeepEquals, awayEquipment)

	// Destroy 2 equipments not loadable
	awayEquipment = swapi.EquipmentDotation{
		Available:   1,
		Unavailable: 2,
	}
	err := client.ChangeEquipmentState(transported.Id,
		map[uint32]*swapi.EquipmentDotation{awayEquipmentId: &awayEquipment})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[transported.Id].EquipmentDotations[awayEquipmentId],
			&awayEquipment)
	})

	Helitransport(c, client, transported, carrier)
	// Check vehicles are away
	CheckAwayEquipment(c, client, awayEquipmentId, transported.Id, carrier.Id, 3)

	// Check unit is unloaded
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[transported.Id].TransporterId == 0
	})

	// Send recover_transporters magic action
	err = client.RecoverTransporters(transported.Id)
	c.Assert(err, IsNil)

	// Check vehicles aren't away
	CheckAwayEquipment(c, client, awayEquipmentId, transported.Id, 0, 0)
}

func (s *TestSuite) TestTransferAwayEquipment(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	pos := swapi.Point{X: -15.9219, Y: 28.3456}
	awayEquipmentId := uint32(15)
	unitType := uint32(1)
	transported, carrier := CreateTransportedAndCarrier(c, client)

	unit, err := client.CreateUnit(transported.AutomatId, unitType, pos)
	c.Assert(err, IsNil)

	// Transfer 2 equipments not loadable
	err = client.TransferEquipment(transported.Id, unit.Id, []swapi.Equipment{{awayEquipmentId, 2}})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Units[unit.Id]
		if len(unit.BorrowedEquipments) == 0 {
			return false
		}
		return unit.BorrowedEquipments[0].TypeId == awayEquipmentId &&
			unit.BorrowedEquipments[0].Quantity == 2 &&
			unit.BorrowedEquipments[0].Owner == transported.Id
	})

	Helitransport(c, client, transported, carrier)
	// Check only one equipment is away
	CheckAwayEquipment(c, client, awayEquipmentId, transported.Id, carrier.Id, 1)

	// Transfering back
	err = client.TransferEquipment(unit.Id, transported.Id, []swapi.Equipment{{awayEquipmentId, 2}})
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Units[unit.Id]
		return len(unit.BorrowedEquipments) == 0
	})

	// Check 3 equipments not loadable are away
	CheckAwayEquipment(c, client, awayEquipmentId, transported.Id, carrier.Id, 3)
}

func (s *TestSuite) TestUnitReloadBrain(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)
	tasker := swapi.MakeUnitTasker(unit.Id)

	// too many parameters
	err = client.ReloadBrainTest(tasker, swapi.MakeParameters(
		swapi.MakeString("blah"),
		nil,
	))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid type
	err = client.ReloadBrainTest(tasker, swapi.MakeParameters(
		swapi.MakeBoolean(true),
	))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid unit id
	empty := swapi.MakeParameters()
	err = client.ReloadBrainTest(swapi.MakeUnitTasker(unit.Id+1), empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// missing id
	err = client.ReloadBrainTest(&sword.Tasker{}, empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// invalid model
	err = client.ReloadBrain(tasker, "bad_brain")
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// reload current brain
	err = client.ReloadBrain(tasker, "")
	c.Assert(err, IsNil)

	// Reload automat decisional model
	err = client.ReloadBrain(tasker, "Journalist")
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestLoadUnit(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// some prerequisites
	formation := CreateFormation(c, client, 1)
	automat := CreateAutomat(c, client, formation.Id, 0)
	unit1 := CreateUnit(c, client, automat.Id)
	unit2 := CreateUnit(c, client, automat.Id)

	// missing tasker
	empty := swapi.MakeParameters()
	err := client.LoadUnitTest(&sword.Tasker{}, empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// invalid tasker
	err = client.LoadUnitTest(swapi.MakeUnitTasker(unit2.Id+1), empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// missing parameters
	tasker := swapi.MakeUnitTasker(unit1.Id)
	err = client.LoadUnitTest(tasker, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// too many parameters
	err = client.LoadUnitTest(tasker, swapi.MakeParameters(nil, nil))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid parameter type
	err = client.LoadUnitTest(tasker, swapi.MakeParameters(
		swapi.MakeBoolean(true),
	))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// loading invalid unit
	err = client.LoadUnit(unit1.Id, unit2.Id+1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// loading itself
	err = client.LoadUnit(unit1.Id, unit1.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	err = client.LoadUnit(unit1.Id, unit2.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		u1 := data.Units[unit1.Id]
		u2 := data.Units[unit2.Id]
		return u2.TransporterId == u1.Id &&
			reflect.DeepEqual(u1.TransportedIds, []uint32{u2.Id})
	})
}

func (s *TestSuite) TestUnloadUnit(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// some prerequisites
	formation := CreateFormation(c, client, 1)
	automat := CreateAutomat(c, client, formation.Id, 0)
	unit1 := CreateUnit(c, client, automat.Id)
	unit2 := CreateUnit(c, client, automat.Id)
	err := client.LoadUnit(unit1.Id, unit2.Id)

	// missing tasker
	empty := swapi.MakeParameters()
	err = client.UnloadUnitTest(&sword.Tasker{}, empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// invalid tasker
	err = client.UnloadUnitTest(swapi.MakeUnitTasker(unit2.Id+1), empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// missing parameters
	tasker := swapi.MakeUnitTasker(unit1.Id)
	err = client.UnloadUnitTest(tasker, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// too many parameters
	err = client.UnloadUnitTest(tasker, swapi.MakeParameters(nil, nil))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid parameter type
	err = client.UnloadUnitTest(tasker, swapi.MakeParameters(
		swapi.MakeBoolean(true),
	))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// unloading invalid unit
	err = client.UnloadUnit(unit1.Id, unit2.Id+1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// unloading itself
	err = client.UnloadUnit(unit1.Id, unit1.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	err = client.UnloadUnit(unit1.Id, unit2.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		u1 := data.Units[unit1.Id]
		u2 := data.Units[unit2.Id]
		return u2.TransporterId == 0 &&
			len(u1.TransportedIds) == 0
	})
}

func (s *TestSuite) TestDestroyUnit(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// some prerequisites
	formation := CreateFormation(c, client, 1)
	automat := CreateAutomat(c, client, formation.Id, 0)
	unit1 := CreateUnit(c, client, automat.Id)
	unit2 := CreateUnit(c, client, automat.Id)
	err := client.LoadUnit(unit1.Id, unit2.Id)

	// invalid unit
	err = client.DestroyUnit(123456)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// valid unit
	err = client.DestroyUnit(unit1.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		u1 := data.Units[unit1.Id]
		// Equipments are unavailable
		for _, eq := range u1.EquipmentDotations {
			if eq.Available != 0 {
				return false
			}
		}
		// Crew members are dead
		for _, h := range u1.HumanDotations {
			if h.Quantity > 0 && h.State != eDead {
				return false
			}
		}
		return true
	})

	// It can be destroyed again
	err = client.DestroyUnit(unit1.Id)
	c.Assert(err, IsNil)

	// Destroy a deleted unit
	err = client.DeleteUnit(unit2.Id)
	c.Assert(err, IsNil)
	err = client.DestroyUnit(unit2.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")
}

func (s *TestSuite) TestLogFinishHandlings(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// invalid unit
	err := client.LogFinishHandlings(1000)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	unitId := uint32(12)
	// unit has no handlings
	err = client.LogFinishHandlings(unitId)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// maintenance
	equipmentId := uint32(11)
	dotation := swapi.EquipmentDotation{
		Available: 4,
	}
	data := client.Model.GetData()
	c.Assert(dotation, DeepEquals, *data.Units[unitId].EquipmentDotations[equipmentId])
	c.Assert(data.MaintenanceHandlings, HasLen, 0)
	equipment := swapi.EquipmentDotation{
		Available:  3,
		Repairable: 1,
		Breakdowns: []int32{82},
	}
	err = client.ChangeEquipmentState(unitId, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)
	var handlingId uint32
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, h := range data.MaintenanceHandlings {
			if h.Provider != nil {
				handlingId = h.Id
				return true
			}
		}
		return false
	})
	maintenance := client.Model.GetData().MaintenanceHandlings[handlingId]
	c.Assert(maintenance.Provider.State, Equals, sword.LogMaintenanceHandlingUpdate_moving_to_supply)
	c.Assert(maintenance.Provider.Id, Equals, uint32(24))
	// finish handlings and check equipment repaired
	err = client.LogFinishHandlings(24)
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)
	data = client.Model.GetData()
	unit := data.Units[unitId]
	c.Assert(dotation, DeepEquals, *unit.EquipmentDotations[equipmentId])
	c.Assert(data.MaintenanceHandlings, HasLen, 0)

	// medical
	c.Assert(CheckHumanQuantity(unit.HumanDotations,
		map[int32]int32{eOfficer: 1, eWarrantOfficer: 4, eTrooper: 7}), Equals, true)
	c.Assert(data.MedicalHandlings, HasLen, 0)
	human := swapi.HumanDotation{
		Quantity:     1,
		Rank:         eTrooper,
		State:        eInjured,
		Injury:       eInjuryU2,
		Psyop:        true,
		Contaminated: true,
	}
	err = client.ChangeHumanState(unitId, []*swapi.HumanDotation{&human})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, h := range data.MedicalHandlings {
			if h.Provider != nil {
				handlingId = h.Id
				return true
			}
		}
		return false
	})
	medical := client.Model.GetData().MedicalHandlings[handlingId]
	c.Assert(medical.Provider.State, Equals, sword.LogMedicalHandlingUpdate_waiting_for_evacuation)
	c.Assert(medical.Provider.Id, Equals, uint32(24))
	// finish handlings and check human healed
	err = client.LogFinishHandlings(24)
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)
	unit = client.Model.GetUnit(unitId)
	c.Assert(CheckHumanState(unit.HumanDotations, eTrooper, eHealthy, 6, 0), Equals, true)
	c.Assert(CheckHumanState(unit.HumanDotations, eTrooper, eHealthy, 1, 0), Equals, true)
	data = client.Model.GetData()
	// this is a feature : healed humans do not go back on the field
	c.Assert(data.MedicalHandlings, HasLen, 1)
	c.Assert(data.MedicalHandlings[handlingId].Provider.State, Equals, sword.LogMedicalHandlingUpdate_finished)

	// supply
	c.Assert(client.Model.GetData().SupplyHandlings, HasLen, 0)
	c.Assert(unit.ResourceDotations, NotNil)
	c.Assert(unit.ResourceDotations[1], DeepEquals,
		swapi.ResourceDotation{
			Quantity:  3200,
			Threshold: 10,
		})
	resource := swapi.ResourceDotation{
		Threshold: 10,
	}
	err = client.ChangeDotation(unitId, map[uint32]*swapi.ResourceDotation{1: &resource})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		if data.Units[unitId].ResourceDotations[1] != resource {
			return false
		}
		for _, h := range data.SupplyHandlings {
			handlingId = h.Id
			return true
		}
		return false
	})
	supply := client.Model.GetData().SupplyHandlings[handlingId]
	c.Assert(supply.SupplierId, Equals, uint32(23))
	c.Assert(supply.ProviderId, Equals, uint32(23))
	c.Assert(supply.Convoy, NotNil)
	c.Assert(supply.Convoy.State, Equals, sword.LogSupplyHandlingUpdate_convoy_waiting_for_transporters)
	// finish handlings and check dotation re-supplied
	err = client.LogFinishHandlings(23)
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)
	c.Assert(client.Model.GetUnit(unitId).ResourceDotations[1], DeepEquals,
		swapi.ResourceDotation{
			Quantity:  3200,
			Threshold: 10,
		})
	c.Assert(client.Model.GetData().SupplyHandlings, HasLen, 0)

	// funeral
	c.Assert(client.Model.GetData().FuneralHandlings, HasLen, 0)
	human = swapi.HumanDotation{
		Quantity: 1,
		Rank:     eTrooper,
		State:    eDead,
	}
	err = client.ChangeHumanState(unitId, []*swapi.HumanDotation{&human})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, h := range data.FuneralHandlings {
			if h.Handler != nil {
				handlingId = h.Id
				return true
			}
		}
		return false
	})
	funeral := client.Model.GetData().FuneralHandlings[handlingId]
	c.Assert(funeral.Handler.State, Equals, sword.LogFuneralHandlingUpdate_transporting_unpackaged)
	c.Assert(funeral.Handler.Id, Equals, uint32(23))
	c.Assert(funeral.UnitId, Equals, uint32(unitId))
	// finish handlings and check human buried
	err = client.LogFinishHandlings(23)
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)
	dotations := client.Model.GetUnit(unitId).HumanDotations
	c.Assert(CheckHumanState(dotations, eTrooper, eHealthy, 6, 0), Equals, true)
	c.Assert(CheckHumanState(dotations, eTrooper, eDead, 1, 0), Equals, true)
	funerals := client.Model.GetData().FuneralHandlings
	c.Assert(funerals, HasLen, 1)
	c.Assert(funerals[handlingId].Handler.State, Equals, sword.LogFuneralHandlingUpdate_finished)
}

func (s *TestSuite) TestLogMaintenanceSetManual(c *C) {
	// user without supervision rights can send log maintenance magic action
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallLog))
	defer stopSimAndClient(c, sim, client)

	const formation = 13
	// invalid unit
	err := client.LogMaintenanceSetManual(1000, true)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// invalid formation without logistic base
	err = client.LogMaintenanceSetManual(5, true)
	c.Assert(err, ErrorMatches, "error_invalid_unit: formation must be a logistic base")

	// invalid empty parameter
	err = client.LogMaintenanceSetManualTest(formation, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid parameter
	err = client.LogMaintenanceSetManualTest(formation, swapi.MakeParameters(swapi.MakeInt(1337)))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// setting manual mode updates formation model
	c.Assert(client.Model.GetFormation(formation).LogMaintenanceManual, Equals, false)
	err = client.LogMaintenanceSetManual(formation, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Formations[formation].LogMaintenanceManual
	})

	const automat = 14
	// setting manual mode updates automat model
	c.Assert(client.Model.GetAutomat(automat).LogMaintenanceManual, Equals, false)
	err = client.LogMaintenanceSetManual(automat, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Automats[automat].LogMaintenanceManual
	})
}
