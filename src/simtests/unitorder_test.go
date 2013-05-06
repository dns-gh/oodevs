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
	InvalidIdentifier      = uint32(129500)
)

// Test we can send a unit mission and get a successful acknowledgement. The
// mission is not really expected to work successfully at this point, unit
// may not move because of the terrain type and so forth.
func (s *TestSuite) TestGenericMission(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.MakePoint(-15.9219, 28.3456)
	to := swapi.MakePoint(-15.8193, 28.3456)
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters(
		swapi.MakeHeading(0),
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakePointParam(to))

	// Cannot send order with an invalid unit identifier
	err = client.SendUnitOrder(InvalidIdentifier, MissionMoveId, params)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Cannot send order to an engaged unit
	err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, ErrorMatches, "error_unit_cannot_receive_order")

	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	// Cannot order with an invalid mission identifier
	err = client.SendUnitOrder(unit.Id, InvalidIdentifier, params)
	c.Assert(err, ErrorMatches, "error_invalid_mission")

	err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)
}

// Test we can send a automat mission and get a successful acknowledgement.
func (s *TestSuite) TestAutomatMission(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.MakePoint(-15.9219, 28.3456)

	limit11 := swapi.MakePoint(-15.8241, 28.3241)
	limit12 := swapi.MakePoint(-15.8092, 28.3458)
	limit21 := swapi.MakePoint(-15.8066, 28.3156)
	limit22 := swapi.MakePoint(-15.7941, 28.3410)

	_, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters(
		swapi.MakeHeading(0),
		swapi.MakeNullValue(),
		swapi.MakeLimit(limit11, limit12),
		swapi.MakeLimit(limit21, limit22),
		swapi.MakeNullValue())

	// Cannot send order with an invalid unit identifier
	err = client.SendAutomatOrder(InvalidIdentifier, MissionAutomatAttackId, params)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Cannot order with an invalid mission identifier
	err = client.SendAutomatOrder(automat.Id, InvalidIdentifier, params)
	c.Assert(err, ErrorMatches, "error_invalid_mission")

	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	// Cannot send order to an disengaged automat
	err = client.SendAutomatOrder(automat.Id, MissionAutomatAttackId, params)
	c.Assert(err, ErrorMatches, "error_unit_cannot_receive_order")

	err = client.SetAutomatMode(automat.Id, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	err = client.SendAutomatOrder(automat.Id, MissionAutomatAttackId, params)
	c.Assert(err, IsNil)
}
