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

func (s *TestSuite) TestScores(c *C) {
	scoreName := "available-equipments"

	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallScores)
	defer sim.Stop()

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.KnownScores) > 0
	})
	model := client.Model.GetData()
	c.Assert(model.KnownScores, HasLen, 1)
	c.Assert(model.KnownScores[scoreName], NotNil)

	// Wait for the values to be populated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Scores) > 0
	})
	model = client.Model.GetData()
	c.Assert(model.Scores, HasLen, 1)
	c.Assert(model.Scores[scoreName], Equals, float32(16))

	// Destroy some equipment
	units := model.ListUnits()
	c.Assert(len(units), Greater, 0)
	for _, unit := range units {
		err := client.DestroyUnit(unit.Id)
		c.Assert(err, IsNil)
	}

	// Wait for the score to change
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Scores[scoreName] == 0
	})
	model = client.Model.GetData()
	client.Close()

	// Log with another user, the score values are received but not the definitions,
	// despite them being restricted to the admin user.
	client = loginAndWaitModel(c, sim, "alluser", "alluser")
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		value, ok := data.Scores[scoreName]
		return ok && value == 0
	})
	model2 := client.Model.GetData()
	c.Assert(model2.KnownScores, HasLen, 0)
	c.Assert(model2.Scores, DeepEquals, model.Scores)
}
