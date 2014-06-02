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
)

func (s *TestSuite) TestScores(c *C) {
	scoreEq := "available-equipments"
	scoreDist := "cp-distance"

	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallScores))
	defer stopSimAndClient(c, sim, client)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.KnownScores) > 0
	})
	model := client.Model.GetData()
	c.Assert(model.KnownScores, DeepEquals, map[string]struct{}{
		scoreEq:   struct{}{},
		scoreDist: struct{}{},
	})

	// Wait for the values to be populated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Scores) > 1
	})
	model = client.Model.GetData()
	c.Assert(model.Scores, DeepEquals, map[string]float32{
		scoreEq:   16,
		scoreDist: 100.48342,
	})

	// Destroy some equipment
	units := model.Units
	c.Assert(len(units), Greater, 0)
	for _, unit := range units {
		err := client.DestroyUnit(unit.Id)
		c.Assert(err, IsNil)
	}

	// Wait for the score to change
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Scores[scoreEq] == 0
	})
	model = client.Model.GetData()
	client.Close()

	// Log with another user, the score values are received but not the definitions,
	// despite them being restricted to the admin user.
	client = loginAndWaitModel(c, sim, NewAllUserOpts(""))
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Scores) > 1
	})
	model2 := client.Model.GetData()
	c.Assert(model2.KnownScores, DeepEquals, map[string]struct{}{
		scoreDist: struct{}{},
	})
	c.Assert(model2.Scores, DeepEquals, model.Scores)
}
