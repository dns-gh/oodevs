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

const (
	FragOrderNbcSuitOn          = uint32(345)
	FragOrderCrowdPauseMovement = uint32(44571)
)

func (s *TestSuite) TestAutomatFragOrder(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	_, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters()

	// Cannot send frag order with an invalid automat identifier
	err = client.SendAutomatFragOrder(InvalidIdentifier, FragOrderNbcSuitOn, params)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Cannot send frag order with an invalid frag order identifier
	err = client.SendAutomatFragOrder(automat.Id, InvalidIdentifier, params)
	c.Assert(err, ErrorMatches, "error_invalid_frag_order")

	// Disengage automat
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	// Cannot send frag order to an disengaged automat
	err = client.SendAutomatFragOrder(automat.Id, FragOrderNbcSuitOn, params)
	c.Assert(err, ErrorMatches, "error_unit_cannot_receive_order")

	// Engage automat
	err = client.SetAutomatMode(automat.Id, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	err = client.SendAutomatFragOrder(automat.Id, FragOrderNbcSuitOn, params)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestCrowdFragOrder(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	data := client.Model.GetData()
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}

	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	crowd, err := client.CreateCrowd(party.Id, 0, CrowdType, from, 10, 10, 10, "crowd")
	c.Assert(err, IsNil)
	c.Assert(crowd, NotNil)

	params := swapi.MakeParameters()

	// Cannot send frag order with an invalid frag order identifier
	err = client.SendCrowdFragOrder(crowd.Id, InvalidIdentifier, params)
	c.Assert(err, ErrorMatches, "error_invalid_frag_order")

	// Crowd frag order is unavailable without a mission
	err = client.SendCrowdFragOrder(crowd.Id, FragOrderCrowdPauseMovement, params)
	c.Assert(err, ErrorMatches, "error_invalid_frag_order")

	missionParams := swapi.MakeParameters(
		swapi.MakePointParam(to))

	// Send moveTo mission for activate frag order
	err = client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, missionParams)
	c.Assert(err, IsNil)

	// Wait one tick for the mission to be applied
	client.Model.WaitTicks(1)

	err = client.SendCrowdFragOrder(crowd.Id, FragOrderCrowdPauseMovement, params)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestUnitFragOrder(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters()

	// Cannot send frag order to an engaged unit
	err = client.SendUnitFragOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, ErrorMatches, "error_unit_cannot_receive_order")

	// Disengage automat
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	// Cannot send frag order with an invalid frag order identifier
	err = client.SendUnitFragOrder(unit.Id, InvalidIdentifier, params)
	c.Assert(err, ErrorMatches, "error_invalid_frag_order")

	err = client.SendUnitFragOrder(unit.Id, FragOrderNbcSuitOn, params)
	c.Assert(err, IsNil)
}
