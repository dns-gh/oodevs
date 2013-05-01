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
	MissionMoveId = uint32(44582)
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

	// Cannot send order to an engaged unit
	err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, ErrorMatches, "error_unit_cannot_receive_order")

	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})
	err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)
}
