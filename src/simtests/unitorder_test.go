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
	// This was "COMMON - Move" when the test was written
	MissionMoveId          = uint32(44582)
	MissionAutomatAttackId = uint32(44523)
	MissionMoveCrowdId     = uint32(4490)
	InvalidIdentifier      = uint32(129500)
)

// Test we can send a unit mission and get a successful acknowledgement. The
// mission is not really expected to work successfully at this point, unit
// may not move because of the terrain type and so forth.
func (s *TestSuite) TestGenericMission(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters(
		swapi.MakeHeading(0),
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakePointParam(to))

	// Cannot send order with an invalid unit identifier
	_, err = client.SendUnitOrder(InvalidIdentifier, MissionMoveId, params)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot send order to an engaged unit
	_, err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsSwordError, "error_unit_cannot_receive_order")

	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Cannot order with an invalid mission identifier
	_, err = client.SendUnitOrder(unit.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_mission")

	order, err := client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)
	c.Assert(order.Type, Equals, swapi.UnitOrder)
}

// Test we can send a automat mission and get a successful acknowledgement.
func (s *TestSuite) TestAutomatMission(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}

	limit11 := swapi.Point{X: -15.8241, Y: 28.3241}
	limit12 := swapi.Point{X: -15.8092, Y: 28.3458}
	limit21 := swapi.Point{X: -15.8066, Y: 28.3156}
	limit22 := swapi.Point{X: -15.7941, Y: 28.3410}

	_, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters(
		swapi.MakeHeading(0),
		swapi.MakeNullValue(),
		swapi.MakeLimit(limit11, limit12),
		swapi.MakeLimit(limit21, limit22),
		swapi.MakeNullValue())

	// Cannot send order with an invalid unit identifier
	_, err = client.SendAutomatOrder(InvalidIdentifier, MissionAutomatAttackId, params)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot order with an invalid mission identifier
	_, err = client.SendAutomatOrder(automat.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_mission")

	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Cannot send order to an disengaged automat
	_, err = client.SendAutomatOrder(automat.Id, MissionAutomatAttackId, params)
	c.Assert(err, IsSwordError, "error_unit_cannot_receive_order")

	err = client.SetAutomatMode(automat.Id, true)
	c.Assert(err, IsNil)

	order, err := client.SendAutomatOrder(automat.Id, MissionAutomatAttackId, params)
	c.Assert(err, IsNil)
	c.Assert(order.Type, Equals, swapi.AutomatOrder)
}

// Test we can send a crowd mission and get a successful acknowledgement.
func (s *TestSuite) TestCrowdMission(c *C) {
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

	params := swapi.MakeParameters(
		swapi.MakePointParam(to))

	// Cannot send order with an invalid unit identifier
	_, err = client.SendCrowdOrder(InvalidIdentifier, MissionMoveCrowdId, params)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot order with an invalid mission identifier
	_, err = client.SendCrowdOrder(crowd.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_mission")

	order, err := client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, params)
	c.Assert(err, IsNil)
	c.Assert(order.Type, Equals, swapi.CrowdOrder)
}
