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
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters(swapi.MakeHeading(0), nil, nil, nil,
		swapi.MakePointParam(to))

	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Check unit path is empty
	c.Assert(unit.PathPoints, Equals, uint32(0))

	// Send moveTo mission
	_, err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)

	// Check unit path is sent
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit.Id].PathPoints != uint32(0)
	})

	// Teleport unit
	err = client.Teleport(swapi.MakeUnitTasker(unit.Id), to)
	c.Assert(err, IsNil)

	// Wait its path is reset
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit.Id].PathPoints == uint32(0)
	})
}

func (s *TestSuite) TestPathfindRequest(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	client2 := loginAndWaitModel(c, sim, NewAllUserOpts(ExCrossroadSmallOrbat))
	client2.Register(func(msg *swapi.SwordMessage, id, context int32, err error) bool {
		if msg != nil && msg.SimulationToClient != nil {
			c.Assert(msg.SimulationToClient.GetMessage().GetPathfindRequestAsk(), IsNil)
		}
		return false
	})

	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	// Invalid unit id
	_, err = client.PathfindRequest(12345, from, to)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Pathfind request from -> to
	points, err := client.PathfindRequest(unit.Id, from, to)
	c.Assert(err, IsNil)
	c.Assert(len(points), Greater, 1)
	c.Assert(from, IsNearby, points[0])
	c.Assert(to, IsNearby, points[len(points)-1])

	// Pathfind request from -> from
	points, err = client.PathfindRequest(unit.Id, from, from)
	c.Assert(err, IsNil)
	c.Assert(len(points), Equals, 2)
	c.Assert(from, IsNearby, points[0])
	c.Assert(from, IsNearby, points[1])
}
