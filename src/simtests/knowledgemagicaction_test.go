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
	"sword"
)

func (s *TestSuite) TestEnableKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid knowledge group
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, swapi.MakeParameters(), 42)
	c.Assert(err, ErrorMatches, "error_invalid_knowledgegroup")

	// error: no knowledge group defined
	data := client.Model.GetData()
	kgs := data.ListKnowledgeGroups()
	c.Assert(len(kgs), Greater, 0)
	kg := kgs[0]

	// error: no params
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, swapi.MakeParameters(), kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: first parameter must be an boolean
	params := swapi.MakeParameters(swapi.MakeNullValue())
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, params, kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: one parameter only
	params = swapi.MakeParameters(swapi.MakeBoolean(bool(true)), swapi.MakeNullValue())
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, params, kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// test: disable knowledge group
	err = client.EnableKnowledgeGroup(kg.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindKnowledgeGroup(kg.Id).Enabled
	})

	// test: enable knowledge group
	err = client.EnableKnowledgeGroup(kg.Id, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindKnowledgeGroup(kg.Id).Enabled
	})
}

func (s *TestSuite) TestChangeParentKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: no knowledge group defined
	data := client.Model.GetData()
	kgs := data.ListKnowledgeGroups()
	c.Assert(len(kgs), Greater, 0)
	kg := kgs[0]

	// error: update party with 2 parameters
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_party, swapi.MakeParameters(swapi.MakeNullValue(), swapi.MakeNullValue()), kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: update party parent with 1 parameter
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_party_parent, swapi.MakeParameters(swapi.MakeNullValue()), kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

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
	c.Assert(err, ErrorMatches, "error_invalid_party")

	// error: update party parent with unknown knowledge group
	err = client.ChangeKnowledgeGroupSuperiorToKnowledgeGroup(kg.Id, kg.PartyId, 42)
	c.Assert(err, ErrorMatches, "error_invalid_superior")

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
		return data.FindKnowledgeGroup(kg.Id).ParentId == group.Id
	})

	// change parent back to army
	err = client.ChangeKnowledgeGroupSuperiorToArmy(kg.Id, kg.PartyId)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindKnowledgeGroup(kg.Id).ParentId == 0
	})
}

func (s *TestSuite) TestChangeKnowledgeGroupType(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: no knowledge group defined
	data := client.Model.GetData()
	kgs := data.ListKnowledgeGroups()
	c.Assert(len(kgs), Greater, 0)
	kg := kgs[0]

	// error: no params
	err := client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_type, swapi.MakeParameters(), kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: first parameter must be a string
	params := swapi.MakeParameters(swapi.MakeNullValue())
	err = client.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_type, params, kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// change parent back to army
	err = client.ChangeKnowledgeGroupType(kg.Id, "Battalion")
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindKnowledgeGroup(kg.Id).Type == "Battalion"
	})
}
