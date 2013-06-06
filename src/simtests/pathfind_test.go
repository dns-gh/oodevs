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

func (s *TestSuite) TestCleanPathAfterTeleport(c *C) {
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

	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	// Check unit path is empty
	c.Assert(unit.PathPoints, Equals, uint32(0))

	// Send moveTo mission
	err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)

	// Check unit path is sent
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindUnit(unit.Id).PathPoints != uint32(0)
	})

	// Teleport unit
	err = client.TeleportUnit(unit.Id, to)
	c.Assert(err, IsNil)

	// Wait its path is reset
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindUnit(unit.Id).PathPoints == uint32(0)
	})
}
