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

const (
	FragOrderNbcSuitOn          = uint32(345)
	FragOrderChangeAttitude     = uint32(355)
	FragOrderCrowdPauseMovement = uint32(44571)
)

func (s *TestSuite) TestAutomatFragOrder(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	_, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters()

	// Cannot send frag order without tasker
	_, err = client.SendAutomatFragOrder(0, FragOrderNbcSuitOn, params)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot send frag order with an invalid automat identifier
	_, err = client.SendAutomatFragOrder(InvalidIdentifier, FragOrderNbcSuitOn, params)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot send frag order with an invalid frag order identifier
	_, err = client.SendAutomatFragOrder(automat.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_frag_order")

	// Cannot send frag order without required parameters
	_, err = client.SendAutomatFragOrder(automat.Id, FragOrderChangeAttitude, params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Disengage automat
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Cannot send frag order to an disengaged automat
	_, err = client.SendAutomatFragOrder(automat.Id, FragOrderNbcSuitOn, params)
	c.Assert(err, IsSwordError, "error_unit_cannot_receive_order")

	// Engage automat
	err = client.SetAutomatMode(automat.Id, true)
	c.Assert(err, IsNil)

	_, err = client.SendAutomatFragOrder(automat.Id, FragOrderNbcSuitOn, params)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestCrowdFragOrder(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
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
	_, err = client.SendCrowdFragOrder(crowd.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_frag_order")

	// Crowd frag order is unavailable without a mission
	_, err = client.SendCrowdFragOrder(crowd.Id, FragOrderCrowdPauseMovement, params)
	c.Assert(err, IsSwordError, "error_invalid_frag_order")

	// Cannot send frag order without required parameters
	_, err = client.SendAutomatFragOrder(crowd.Id, FragOrderChangeAttitude, params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	missionParams := swapi.MakeParameters(
		swapi.MakePointParam(to))

	// Send moveTo mission for activate frag order
	order, err := client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, missionParams)
	c.Assert(err, IsNil)
	c.Assert(order.Kind, Equals, swapi.CrowdOrder)

	// Wait one tick for the mission to be applied
	client.Model.WaitTicks(1)

	_, err = client.SendCrowdFragOrder(crowd.Id, FragOrderCrowdPauseMovement, params)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestUnitFragOrder(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters()

	// Cannot send frag order to an engaged unit
	_, err = client.SendUnitFragOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsSwordError, "error_unit_cannot_receive_order")

	// Disengage automat
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Cannot send frag order with an invalid frag order identifier
	_, err = client.SendUnitFragOrder(unit.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_frag_order")

	// Cannot send frag order without required parameters
	_, err = client.SendAutomatFragOrder(unit.Id, FragOrderChangeAttitude, params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	_, err = client.SendUnitFragOrder(unit.Id, FragOrderNbcSuitOn, params)
	c.Assert(err, IsNil)
}
