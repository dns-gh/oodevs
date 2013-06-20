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

func CreateCrowd(c *C, client *swapi.Client) *swapi.Crowd {
	pos := swapi.Point{X: 0, Y: 0}
	crowdName := "crowd"
	healthy, wounded, dead := int32(10), int32(11), int32(12)

	party := client.Model.GetData().FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create crowd in party
	crowd, err := client.CreateCrowd(party.Id, 0, CrowdType, pos, healthy,
		wounded, dead, crowdName)
	c.Assert(err, IsNil)
	c.Assert(crowd, NotNil)
	return crowd
}

func (s *TestSuite) TestCrowdTotalDestruction(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	healthy, wounded, dead := int32(10), int32(11), int32(12)

	crowd := CreateCrowd(c, client)

	// Check humans composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(healthy, wounded, dead, 0, data.FindCrowd(crowd.Id))
	})

	// Kill all humans with invalid identifier
	err := client.SendTotalDestruction(12345)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Kill all humans
	err = client.SendTotalDestruction(crowd.Id)
	c.Assert(err, IsNil)

	// Check humans composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(0, 0, dead+wounded+healthy, 0, data.FindCrowd(crowd.Id))
	})
}

func (s *TestSuite) TestCrowdChangeArmedIndividuals(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()

	crowd := CreateCrowd(c, client)

	// Check default armed individuals proportion(set in physical database)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindCrowd(crowd.Id).ArmedIndividuals == 0.1
	})

	// Error : missing parameter
	err := client.ChangeArmedIndividuals(crowd.Id, 0)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error : Armed individuals must be a percentage between 0 and 100
	err = client.ChangeArmedIndividuals(crowd.Id, 200)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")
	err = client.ChangeArmedIndividuals(crowd.Id, -2)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Change Armed individuals
	err = client.ChangeArmedIndividuals(crowd.Id, 50)
	c.Assert(err, IsNil)

	// Check armed individuals proportion
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindCrowd(crowd.Id).ArmedIndividuals == 0.5
	})
}

func (s *TestSuite) TestCrowdChangeCriticalIntelligence(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()

	crowd := CreateCrowd(c, client)

	// Check empty critical intelligence
	c.Assert(crowd.CriticalIntelligence, Equals, "")

	// Error : missing parameter
	err := client.ChangeCriticalIntelligence(crowd.Id, "")
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Change critical intelligence
	err = client.ChangeCriticalIntelligence(crowd.Id, "critical")
	c.Assert(err, IsNil)

	// Check critical intelligence
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindCrowd(crowd.Id).CriticalIntelligence == "critical"
	})
}

func (s *TestSuite) TestCrowdChangeHealthState(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	healthy, wounded, contaminated, dead := int32(10), int32(11), int32(0), int32(12)

	crowd := CreateCrowd(c, client)

	// Check human composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(healthy, wounded, dead, contaminated, data.FindCrowd(crowd.Id))
	})

	// Error : negative parameter
	err := client.ChangeHealthState(crowd.Id, -2, -5, contaminated, -1)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error : all parameters are null
	err = client.ChangeHealthState(crowd.Id, 0, 0, 0, 0)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Change human composition
	healthy, wounded, contaminated, dead = int32(23), int32(22), int32(21), int32(51)
	err = client.ChangeHealthState(crowd.Id, healthy, wounded, contaminated, dead)
	c.Assert(err, IsNil)

	// Check new humans composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(healthy, wounded, dead, contaminated, data.FindCrowd(crowd.Id))
	})
}

func (s *TestSuite) TestCrowdChangeAdhesions(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	crowd := CreateCrowd(c, client)

	// By default, adhesions are empty.
	c.Assert(len(crowd.Adhesions), Equals, 0)

	// Error : missing parameter
	err := client.ChangeAdhesions(crowd.Id, map[uint32]float32{})
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error : adhesion must be between -1 and 1
	adhesions := map[uint32]float32{0: 1.1, 1: 5.2}
	err = client.ChangeAdhesions(crowd.Id, adhesions)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Change crowd adhesions
	adhesions = map[uint32]float32{0: 0.7, 1: -0.5}
	err = client.ChangeAdhesions(crowd.Id, adhesions)
	c.Assert(err, IsNil)

	// Check new adhesions
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FindCrowd(crowd.Id).Adhesions) != 0
	})

	newAdhesions := client.Model.GetData().FindCrowd(crowd.Id).Adhesions
	c.Assert(adhesions, DeepEquals, newAdhesions)

	// No change adhesions if new adhesions are invalid
	err = client.ChangeAdhesions(crowd.Id,
		map[uint32]float32{0: -1.1, 1: -5.2})
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	newAdhesions = client.Model.GetData().FindCrowd(crowd.Id).Adhesions
	c.Assert(adhesions, DeepEquals, newAdhesions)
}

func (s *TestSuite) TestCrowdReloadBrain(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	crowd := CreateCrowd(c, client)

	// Error : missing parameter
	err := client.ReloadBrain(crowd.Id, "")
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error : invalid model
	err = client.ReloadBrain(crowd.Id, "invalid")
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Reload crowd decisional model
	err = client.ReloadBrain(crowd.Id, "Rioters")
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestCrowdElements(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	crowd := CreateCrowd(c, client)

	// Initial crowd has a concentration
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FindCrowd(crowd.Id).CrowdElements) == 1
	})

	// Send moveTo mission on the crowd
	to := swapi.Point{X: -15.8193, Y: 28.3456}
	params := swapi.MakeParameters(swapi.MakePointParam(to))
	err := client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, params)
	c.Assert(err, IsNil)

	// Check crowd begin its movement, a flow is created
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FindCrowd(crowd.Id).CrowdElements) == 2
	})

	// Reset movement, the flow is destroyed.
	err = client.TeleportCrowd(crowd.Id, to)
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FindCrowd(crowd.Id).CrowdElements) == 1
	})
}
