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
	"sword"
)

func (s *TestSuite) TestEnableKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer sim.Stop()

	// error: invalid knowledge group
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, swapi.MakeParameters(), 42)
	c.Assert(err, IsSwordError, "error_invalid_knowledgegroup")

	// error: no knowledge group defined
	data := client.Model.GetData()
	kg := getSomeKnowledgeGroup(c, data)

	// error: no params
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, swapi.MakeParameters(), kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be an boolean
	params := swapi.MakeParameters(nil)
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, params, kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: one parameter only
	params = swapi.MakeParameters(swapi.MakeBoolean(bool(true)), nil)
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, params, kg.Id)
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
	defer sim.Stop()

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
	defer sim.Stop()

	// error: no knowledge group defined
	data := client.Model.GetData()
	kg := getSomeKnowledgeGroup(c, data)

	// error: no params
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_type, swapi.MakeParameters(), kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a string
	params := swapi.MakeParameters(nil)
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_type, params, kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// change parent back to army
	err = client.ChangeKnowledgeGroupType(kg.Id, "Battalion")
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.KnowledgeGroups[kg.Id].Type == "Battalion"
	})
}

func (s *TestSuite) TestAddKnowledgeInKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer sim.Stop()

	// error: no knowledge group defined
	data := client.Model.GetData()
	kgs := data.KnowledgeGroups
	c.Assert(len(kgs), Greater, 0)

	// error: no units defined
	unit := getSomeUnit(c, data)
	unitKgId := data.Automats[unit.AutomatId].KnowledgeGroupId

	// error: no second knowledge group
	var kg *swapi.KnowledgeGroup
	for _, kg = range kgs {
		if unitKgId != kg.Id {
			break
		}
	}
	c.Assert(unitKgId, Not(Equals), kg.Id)

	// error: no params
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_add_knowledge, swapi.MakeParameters(), kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be an identifier, second a quantity
	params := swapi.MakeParameters(nil, nil)
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_add_knowledge, params, kg.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: invalid perception
	_, err = client.AddUnitKnowledgeInKnowledgeGroup(kg.Id, unit.Id, 42)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// add a unit in knowledge group and check
	unitKnowledge, err := client.AddUnitKnowledgeInKnowledgeGroup(kg.Id, unit.Id, 2)
	c.Assert(err, IsNil)
	c.Assert(unitKnowledge.KnowledgeGroupId, Equals, kg.Id)
	c.Assert(unitKnowledge.UnitId, Equals, unit.Id)

	// add object in knowledge group
	location := swapi.MakePointLocation(unit.Position)
	object, err := client.CreateObject("jamming area", kg.PartyId, location)
	c.Assert(err, IsNil)
	objectKnowledge, err := client.AddObjectKnowledgeInKnowledgeGroup(kg.Id, object.Id, 2)
	c.Assert(err, IsNil)
	c.Assert(objectKnowledge.KnowledgeGroupId, Equals, kg.Id)
	c.Assert(objectKnowledge.ObjectId, Equals, object.Id)
}

func getSomeUnit(c *C, data *swapi.ModelData) *swapi.Unit {
	for _, a := range data.Units {
		return a
	}
	c.Fatal("missing units")
	return nil
}

func getSomeUnitByName(c *C, data *swapi.ModelData, substring string) *swapi.Unit {
	for _, a := range data.Units {
		if strings.Contains(a.Name, substring) {
			return a
		}
	}
	c.Fatal("no unit whose name contains'" + substring + "'")
	return nil
}

func getSomeAutomat(c *C, data *swapi.ModelData) *swapi.Automat {
	for _, a := range data.Automats {
		return a
	}
	c.Fatal("missing automats")
	return nil
}

func (s *TestSuite) TestChangeKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer sim.Stop()

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
