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
	"masa/sword/swapi"
	"masa/sword/sword"
)

func (s *TestSuite) TestEnableKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: invalid knowledge group
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable,
		swapi.MakeParameters(), 42)
	c.Assert(err, IsSwordError, "error_invalid_knowledgegroup")

	// error: no knowledge group defined
	data := client.Model.GetData()
	kg := getSomeKnowledgeGroup(c, data)

	// error: no params
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable,
		swapi.MakeParameters(), kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be an boolean
	params := swapi.MakeParameters(nil)
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable,
		params, kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: one parameter only
	params = swapi.MakeParameters(swapi.MakeBoolean(bool(true)), nil)
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable,
		params, kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// test: disable knowledge group
	err = client.EnableKnowledgeGroup(kg.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.KnowledgeGroups[kg.Id].Enabled
	})

	// test: enable knowledge group
	err = client.EnableKnowledgeGroup(kg.Id, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.KnowledgeGroups[kg.Id].Enabled
	})
}

func (s *TestSuite) TestChangeParentKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: no knowledge group defined
	data := client.Model.GetData()
	kg := getSomeKnowledgeGroup(c, data)

	// error: update party with 2 parameters
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_party,
		swapi.MakeParameters(nil, nil), kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: update party parent with 1 parameter
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_party_parent,
		swapi.MakeParameters(nil), kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// find another party Id
	armies := data.Parties
	c.Assert(len(armies), Greater, 1)
	var otherPartyId uint32
	for otherPartyId, _ := range armies {
		if otherPartyId != kg.PartyId {
			break
		}
	}

	// error: update party with other party
	err = client.ChangeKnowledgeGroupSuperiorToArmy(kg.Id, otherPartyId)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: update party parent with unknown knowledge group
	err = client.ChangeKnowledgeGroupSuperiorToKnowledgeGroup(kg.Id, kg.PartyId, 42)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// creation of another knowledge group with the same army
	params := swapi.MakeParameters(
		swapi.MakeIdentifier(uint32(kg.PartyId)),
		swapi.MakeString("Standard"))
	var group *swapi.KnowledgeGroup
	group, err = client.CreateKnowledgeGroupTest(params)
	c.Assert(err, IsNil)

	// change parent to knowledge group
	err = client.ChangeKnowledgeGroupSuperiorToKnowledgeGroup(kg.Id, kg.PartyId, group.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.KnowledgeGroups[kg.Id].ParentId == group.Id
	})

	// change parent back to army
	err = client.ChangeKnowledgeGroupSuperiorToArmy(kg.Id, kg.PartyId)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.KnowledgeGroups[kg.Id].ParentId == 0
	})
}

func (s *TestSuite) TestChangeKnowledgeGroupType(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: no knowledge group defined
	data := client.Model.GetData()
	kg := getSomeKnowledgeGroup(c, data)

	// error: no params
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_type,
		swapi.MakeParameters(), kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a string
	params := swapi.MakeParameters(nil)
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_type,
		params, kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// change parent back to army
	err = client.ChangeKnowledgeGroupType(kg.Id, "Battalion")
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.KnowledgeGroups[kg.Id].Type == "Battalion"
	})
}

func (s *TestSuite) TestAddKnowledgeInKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	model := client.Model.GetData()

	// Two automats in different parties and knowledge groups
	party1 := model.FindPartyByName("party1")
	formation1 := CreateFormation(c, client, party1.Id)
	kg1, err := client.CreateKnowledgeGroup(party1.Id, "Standard")
	c.Assert(err, IsNil)
	automat1, units1, err := client.CreateAutomatAndUnits(formation1.Id, AutomatType,
		kg1.Id, swapi.Point{X: -15.9378, Y: 28.2223})
	c.Assert(err, IsNil)
	unit1 := units1[0]

	party2 := model.FindPartyByName("party2")
	formation2 := CreateFormation(c, client, party2.Id)
	kg2, err := client.CreateKnowledgeGroup(party2.Id, "Standard")
	c.Assert(err, IsNil)
	_, units2, err := client.CreateAutomatAndUnits(formation2.Id, AutomatType,
		kg2.Id, swapi.Point{X: -15.6981, Y: 28.4462})
	c.Assert(err, IsNil)
	unit2 := units2[0]

	// error: no params
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_add_knowledge,
		swapi.MakeParameters(), kg1.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be an identifier, second a quantity
	params := swapi.MakeParameters(nil, nil)
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_add_knowledge,
		params, kg1.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid perception
	_, err = client.CreateUnitKnowledge(kg1.Id, unit2.Id, 42)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// add a unit in knowledge group and check
	const detected = int32(sword.UnitIdentification_detected)
	const identified = int32(sword.UnitIdentification_identified)
	unitKnowledge, err := client.CreateUnitKnowledge(kg1.Id, unit2.Id, detected)
	c.Assert(err, IsNil)
	c.Assert(unitKnowledge.KnowledgeGroupId, Equals, kg1.Id)
	c.Assert(unitKnowledge.UnitId, Equals, unit2.Id)

	// add object in knowledge group
	location := swapi.MakePointLocation(unit2.Position)
	object, err := client.CreateObject("jamming area", kg2.PartyId, location)
	c.Assert(err, IsNil)
	objectKnowledge, err := client.CreateObjectKnowledge(kg1.Id, object.Id, detected)
	c.Assert(err, IsNil)
	c.Assert(objectKnowledge.KnowledgeGroupId, Equals, kg1.Id)
	c.Assert(objectKnowledge.ObjectId, Equals, object.Id)

	// Teleport unit next to its knowledge, it should identify it
	err = client.SetAutomatMode(automat1.Id, false)
	c.Assert(err, IsNil)
	client.Teleport(swapi.MakeUnitTasker(unit1.Id), unit2.Position)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.UnitKnowledges[unitKnowledge.Id] != nil &&
			data.UnitKnowledges[unitKnowledge.Id].IdentificationLevel == identified
	})
}

func (s *TestSuite) TestChangeKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: no knowledge group defined
	data := client.Model.GetData()
	knowledgeGroup := getSomeKnowledgeGroup(c, data)
	automat := getSomeAutomat(c, data)

	// error: invalid tasker (not an automat)
	err := client.ChangeKnowledgeGroupTest(getSomeUnit(c, data).Id, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: no params
	err = client.ChangeKnowledgeGroupTest(automat.Id, swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: parameters must be a knowledge group and a party
	params := swapi.MakeParameters(nil, nil)
	err = client.ChangeKnowledgeGroupTest(automat.Id, params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// creation of another knowledge group with the same army
	group, err := client.CreateKnowledgeGroup(knowledgeGroup.PartyId, "Standard")
	c.Assert(err, IsNil)

	// ... and one belonging to the other party
	otherParty := uint32(0)
	for _, p := range data.Parties {
		if p.Id != knowledgeGroup.PartyId {
			otherParty = p.Id
			break
		}
	}
	c.Assert(otherParty, Greater, uint32(0))
	otherGroup, err := client.CreateKnowledgeGroup(otherParty, "Standard")
	c.Assert(err, IsNil)
	// invalid knowledge group
	err = client.ChangeKnowledgeGroup(automat.Id, 42)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid knowledgegroup party
	err = client.ChangeKnowledgeGroup(automat.Id, otherGroup.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// change knowledge group
	err = client.ChangeKnowledgeGroup(automat.Id, group.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Automats[automat.Id].KnowledgeGroupId == group.Id
	})

	// change knowledge group to itself
	err = client.ChangeKnowledgeGroup(automat.Id, group.Id)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestKnowledgePropagationAmongGroups(c *C) {
	opts := NewAdminOpts(ExParisEstKnowledgeGroups)
	opts.StartPaused()
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)

	// check no knowledge of 'Frere' unit (26) in 'Fils' knowledge group (13)
	// until tick 60
	const soughtUnit = 26
	const polledKnowledgeGroup = 31

	done := make(chan int32, 1)
	client.Model.RegisterHandler(
		func(data *swapi.ModelData, msg *swapi.SwordMessage, err error) bool {
			for _, knowledge := range data.UnitKnowledges {
				if knowledge.UnitId == soughtUnit &&
					knowledge.KnowledgeGroupId == polledKnowledgeGroup {
					done <- data.Tick
					return true
				}
			}
			if err != nil || data.Tick >= 100 {
				done <- -1
				return true
			}
			return false
		})
	_, _, err := client.Resume(0)
	c.Assert(err, IsNil)
	found := <-done
	c.Assert(found, Equals, int32(61))
}
