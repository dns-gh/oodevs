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

func (s *TestSuite) TestEnableKnowledgeGroup(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid knowledge group
	err := client.EnableKnowledgeGroupTest(swapi.MakeParameters(), 42)
	c.Assert(err, ErrorMatches, "error_invalid_knowledgegroup")

	// error: no knowledge group defined
	data := client.Model.GetData()
	kgs := data.ListKnowledgeGroups()
	c.Assert(len(kgs), Greater, 0)
	kg := kgs[0]

	// error: no params
	err = client.EnableKnowledgeGroupTest(swapi.MakeParameters(), kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: first parameter must be an boolean
	params := swapi.MakeParameters(swapi.MakeNullValue())
	err = client.EnableKnowledgeGroupTest(params, kg.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: one parameter only
	params = swapi.MakeParameters(swapi.MakeBoolean(bool(true)), swapi.MakeNullValue())
	err = client.EnableKnowledgeGroupTest(params, kg.Id)
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
