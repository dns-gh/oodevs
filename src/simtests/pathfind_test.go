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
	defer client2.Close()
	// Pathfind response filtering is broken. See:
	// http://jira.masagroup.net/browse/SWBUG-12113?focusedCommentId=45398#comment-45398
	// seen := false
	// handlerId := client2.Register(func(msg *swapi.SwordMessage, id, context int32, err error) bool {
	// 	if msg.SimulationToClient == nil || msg.SimulationToClient.GetMessage() == nil {
	// 		return false
	// 	}
	// 	seen = msg.SimulationToClient.GetMessage().GetPathfindRequestAck() != nil
	// 	return seen
	// })

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

	// No other client can receive the acknowledge
	// client2.Unregister(handlerId)
	// c.Assert(seen, Equals, false)
}

func CheckPointOrder(objectives, points []swapi.Point) bool {
	index := 0
	for _, value1 := range objectives {
		content := points[index:len(points)]
		found := false
		for k, value2 := range content {
			if isNearby(value1, value2) {
				index = k + 1
				found = true
				break
			}
		}
		if !found {
			return false
		}
	}
	return true
}

func (s *TestSuite) TestOrderPoints(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	automat := createAutomat(c, client)
	positions := []swapi.Point{swapi.Point{X: -15.9296, Y: 28.3601},
		swapi.Point{X: -15.8423, Y: 28.4106},
		swapi.Point{X: -15.9339, Y: 28.3316},
		swapi.Point{X: -15.7327, Y: 28.251}}

	unit, err := client.CreateUnit(automat.Id, UnitType, positions[0])
	c.Assert(err, IsNil)

	points, err := client.PathfindRequest(unit.Id, positions...)
	c.Assert(err, IsNil)
	c.Assert(CheckPointOrder(positions, points), Equals, true)
}
