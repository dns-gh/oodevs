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

func CheckHumans(healthy, wounded, dead, contaminated int32, crowd *swapi.Crowd) bool {
	return crowd.Healthy == healthy && crowd.Wounded == wounded &&
		crowd.Dead == dead && crowd.Contaminated == contaminated
}

func (s *TestSuite) TestCrowdTotalDestruction(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	data := client.Model.GetData()
	pos := swapi.Point{X: 0, Y: 0}
	//model := client.Model
	crowdName := "crowd"
	healthy, wounded, dead := int32(10), int32(11), int32(12)

	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create crowd in party
	crowd, err := client.CreateCrowd(party.Id, 0, CrowdType, pos, healthy,
		wounded, dead, crowdName)
	c.Assert(err, IsNil)
	c.Assert(crowd, NotNil)

	// Check humans composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(healthy, wounded, dead, 0, data.FindCrowd(crowd.Id))
	})

	// Kill all humans with invalid identifier
	err = client.SendTotalDestruction(12345)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Kill all humans
	err = client.SendTotalDestruction(crowd.Id)
	c.Assert(err, IsNil)

	// Check humans composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(0, 0, dead+wounded+healthy, 0, data.FindCrowd(crowd.Id))
	})
}
