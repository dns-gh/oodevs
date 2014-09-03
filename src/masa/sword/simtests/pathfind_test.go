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
	"masa/sword/swtest"
)

const (
	// This was "COMMON - Move along itinerary" when the test was written
	MissionMoveAlongId = uint32(445949330)
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

func (s *TestSuite) TestUnitPathfindRequest(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	client2 := loginAndWaitModel(c, sim, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer client2.Close()
	seen := false
	handlerId := client2.Register(func(msg *swapi.SwordMessage, id, context int32, err error) bool {
		if msg == nil ||
			msg.SimulationToClient == nil ||
			msg.SimulationToClient.GetMessage() == nil {
			return false
		}
		seen = seen || msg.SimulationToClient.GetMessage().GetComputePathfindAck() != nil
		return seen
	})

	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	// Invalid unit id
	_, err = client.UnitPathfindRequest(12345, from, to)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid path
	_, err = client.UnitPathfindRequest(unit.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Pathfind request from -> to
	points, err := client.UnitPathfindRequest(unit.Id, from, to)
	c.Assert(err, IsNil)
	c.Assert(len(points), Greater, 1)
	c.Assert(from, IsNearby, points[0].Point)
	c.Assert(to, IsNearby, points[len(points)-1].Point)

	// Pathfind request from -> from
	points, err = client.UnitPathfindRequest(unit.Id, from, from)
	c.Assert(err, IsNil)
	c.Assert(len(points), Equals, 2)
	c.Assert(from, IsNearby, points[0].Point)
	c.Assert(from, IsNearby, points[1].Point)

	// No other client can receive the acknowledge
	client2.Unregister(handlerId)
	c.Assert(seen, Equals, false)
}

func (s *TestSuite) TestEquipmentListPathfindRequest(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	client2 := loginAndWaitModel(c, sim, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer client2.Close()
	seen := false
	handlerId := client2.Register(func(msg *swapi.SwordMessage, id, context int32, err error) bool {
		if msg == nil ||
			msg.SimulationToClient == nil ||
			msg.SimulationToClient.GetMessage() == nil {
			return false
		}
		seen = seen || msg.SimulationToClient.GetMessage().GetComputePathfindAck() != nil
		return seen
	})

	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}

	// Invalid eq id
	_, err := client.EquipmentListPathfindRequest(12345, from, to)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	const equipment = 11
	// Invalid path
	_, err = client.EquipmentListPathfindRequest(equipment)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Pathfind request from -> to
	points, err := client.EquipmentListPathfindRequest(equipment, from, to)
	c.Assert(err, IsNil)
	c.Assert(len(points), Greater, 1)
	c.Assert(from, IsNearby, points[0].Point)
	c.Assert(to, IsNearby, points[len(points)-1].Point)

	// Pathfind request from -> from
	points, err = client.EquipmentListPathfindRequest(equipment, from, from)
	c.Assert(err, IsNil)
	c.Assert(len(points), Equals, 2)
	c.Assert(from, IsNearby, points[0].Point)
	c.Assert(from, IsNearby, points[1].Point)

	// No other client can receive the acknowledge
	client2.Unregister(handlerId)
	c.Assert(seen, Equals, false)
}

func CheckWaypoints(c *C, waypoints []swapi.Point, result []swapi.PathPoint,
	waypointsReached []bool) {
	// Check waypoint count
	c.Assert(int32(len(waypoints)-1), Equals, result[len(result)-1].Waypoint)
	// Check waypoint value
	for _, value := range result {
		if index := value.Waypoint; index != -1 {
			if value.Reached {
				c.Assert(isNearby(waypoints[index], value.Point), Equals, true)
			}
			c.Assert(value.Reached, Equals, waypointsReached[index])
		}
	}
}

func (s *TestSuite) TestPointOrder(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	automat := createAutomat(c, client)
	positions := []swapi.Point{
		{X: -15.9248, Y: 28.2645},
		{X: -15.8429, Y: 28.3308},
		{X: -15.8640, Y: 28.2507},
		{X: -15.8946, Y: 28.3189},
	}

	unit, err := client.CreateUnit(automat.Id, UnitType, positions[0])
	c.Assert(err, IsNil)

	points, err := client.UnitPathfindRequest(unit.Id, positions...)
	c.Assert(err, IsNil)
	CheckWaypoints(c, positions, points, []bool{true, true, true, true})

	// Compute path with an invalid position
	positions = []swapi.Point{
		{X: -15.918268777007073, Y: 28.3155935041111},
		{X: -15.932060713923917, Y: 28.325101964202737},
		{X: -15.951392003907085, Y: 28.352895879429273}, // invalid
		{X: -15.932953364977887, Y: 28.365605325676778},
		{X: -15.946757028026328, Y: 28.378285449675836}, // invalid
	}

	points, err = client.UnitPathfindRequest(unit.Id, positions...)
	c.Assert(err, IsNil)
	CheckWaypoints(c, positions, points, []bool{true, true, false, true, false})
}

func (s *TestSuite) TestCreateDestroyPathfind(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	automat := createAutomat(c, client)
	positions := []swapi.Point{
		{X: -15.9248, Y: 28.2645},
		{X: -15.8429, Y: 28.3308},
		{X: -15.8640, Y: 28.2507},
		{X: -15.8946, Y: 28.3189},
	}

	unit, err := client.CreateUnit(automat.Id, UnitType, positions[0])
	c.Assert(err, IsNil)

	// Invalid unit id
	_, err = client.CreatePathfind(12345, positions...)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid position count
	_, err = client.CreatePathfind(unit.Id, positions[0])
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid not ignoring dynamic objects
	_, err = client.CreatePathfindTest(unit.Id, "pathfind", false, positions...)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Create pathfind
	pathfind, err := client.CreatePathfind(unit.Id, positions...)
	c.Assert(err, IsNil)
	CheckWaypoints(c, positions, pathfind.Result, []bool{true, true, true, true})

	swtest.DeepEquals(c, unit.Id, pathfind.UnitId)
	swtest.DeepEquals(c, positions, pathfind.Positions)

	// Invalid pathfind id
	err = client.DestroyPathfind(12345)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Destroy pathfind
	err = client.DestroyPathfind(pathfind.Id)
	c.Assert(err, IsNil)
	c.Assert(client.Model.GetData().Pathfinds, HasLen, 0)
}

func (s *TestSuite) TestUnitOrderWithItineraryParameter(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	automat := createAutomat(c, client)
	positions := []swapi.Point{
		{X: -15.9248, Y: 28.2645},
		{X: -15.9216, Y: 28.2718},
		{X: -15.9150, Y: 28.2659},
		{X: -15.9248, Y: 28.2645},
		{X: -15.9252, Y: 28.2713},
	}

	unit, err := client.CreateUnit(automat.Id, UnitType, positions[0])
	c.Assert(err, IsNil)

	// Create pathfind
	pathfind, err := client.CreatePathfind(unit.Id, positions...)
	c.Assert(err, IsNil)

	// Send pathfind to unit
	heading := swapi.MakeHeading(0)
	params := swapi.MakeParameters(heading, nil, nil, nil, swapi.MakePathfind(pathfind))
	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	_, err = client.SendUnitOrder(unit.Id, MissionMoveAlongId, params)
	c.Assert(err, IsNil)
	for _, p := range pathfind.Result {
		if p.Waypoint < 0 {
			continue
		}
		waitCondition(c, client.Model, func(m *swapi.ModelData) bool {
			return isNearby(p.Point, m.Units[unit.Id].Position)
		})
	}
}

func (s *TestSuite) TestUnitOrderWithItineraryRevertedIfAgentCloserToDestination(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	automat := createAutomat(c, client)
	positions := []swapi.Point{
		{X: -15.9248, Y: 28.2645},
		{X: -15.9216, Y: 28.2718},
		{X: -15.9150, Y: 28.2659},
		{X: -15.9248, Y: 28.2645},
		{X: -15.9252, Y: 28.2713},
	}

	unit, err := client.CreateUnit(automat.Id, UnitType, positions[4])
	c.Assert(err, IsNil)

	// Create pathfind
	pathfind, err := client.CreatePathfind(unit.Id, positions...)
	c.Assert(err, IsNil)

	// Send pathfind to unit
	heading := swapi.MakeHeading(0)
	params := swapi.MakeParameters(heading, nil, nil, nil, swapi.MakePathfind(pathfind))
	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	_, err = client.SendUnitOrder(unit.Id, MissionMoveAlongId, params)
	c.Assert(err, IsNil)
	for i := len(pathfind.Result) - 1; i >= 0; i-- {
		p := pathfind.Result[i]
		if p.Waypoint < 0 {
			continue
		}
		waitCondition(c, client.Model, func(m *swapi.ModelData) bool {
			return isNearby(p.Point, m.Units[unit.Id].Position)
		})
	}
}

func (s *TestSuite) TestPathfindName(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	automat := createAutomat(c, client)
	positions := []swapi.Point{
		{X: -15.9248, Y: 28.2645},
		{X: -15.9216, Y: 28.2718},
		{X: -15.9150, Y: 28.2659},
		{X: -15.9248, Y: 28.2645},
		{X: -15.9252, Y: 28.2713},
	}

	unit, err := client.CreateUnit(automat.Id, UnitType, positions[0])
	c.Assert(err, IsNil)

	// Create pathfind
	name := "pathfind_test"
	pathfind, err := client.CreatePathfindTest(unit.Id, name, true, positions...)
	c.Assert(err, IsNil)
	c.Assert(pathfind.Name, Equals, name)

	// Pathfind name updated
	pathfind.Name = "newName"
	err = client.UpdatePathfind(swapi.MakePathfind(pathfind))
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Pathfinds[pathfind.Id].Name == "newName"
	})
}

func (s *TestSuite) TestPathfindDeletingUnitsDeletesRelatedPathfinds(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	positions := []swapi.Point{
		{X: -15.9248, Y: 28.2645},
		{X: -15.9216, Y: 28.2718},
		{X: -15.9150, Y: 28.2659},
	}
	unit, err := client.CreateUnit(automat.Id, UnitType, positions[0])
	c.Assert(err, IsNil)
	pathfind, err := client.CreatePathfindTest(unit.Id, "johndoe", true, positions...)
	c.Assert(err, IsNil)
	err = client.DeleteUnit(unit.Id)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		_, ok := data.Pathfinds[pathfind.Id]
		return !ok
	})
}
