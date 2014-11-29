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
	"code.google.com/p/goprotobuf/proto"
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/swapi/phy"
	"masa/sword/sword"
	"masa/sword/swrun"
	"masa/sword/swtest"
	"math"
)

const (
	// This was "COMMON - Move along itinerary" when the test was written
	MissionMoveAlongId = uint32(445949330)
)

func (s *TestSuite) TestCleanPathAfterTeleport(c *C) {
	opts := NewAllUserOpts(ExCrossroadSmallOrbat)
	opts.RecordUnitPaths()
	sim, client := connectAndWaitModel(c, opts)
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
	c.Assert(len(unit.Path), Equals, 0)

	// Send moveTo mission
	_, err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)

	// Check unit path is sent
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Units[unit.Id].Path) > 0
	})

	// Teleport unit
	err = client.Teleport(swapi.MakeUnitTasker(unit.Id), to)
	c.Assert(err, IsNil)

	// Wait its path is reset
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Units[unit.Id].Path) == 0
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

func (s *TestSuite) TestUnitOrderWithItineraryFollowsItOnlyPartially(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	automat := createAutomat(c, client)
	positions := []swapi.Point{
		{X: -15.9170, Y: 28.2645},
		{X: -15.9200, Y: 28.2645},
		{X: -15.9250, Y: 28.2645},
		{X: -15.9300, Y: 28.2645},
	}

	unit, err := client.CreateUnit(automat.Id, UnitType, swapi.Point{X: -15.9200, Y: 28.2650})
	c.Assert(err, IsNil)

	// Create pathfind
	pathfind, err := client.CreatePathfind(unit.Id, positions...)
	c.Assert(err, IsNil)

	// Send pathfind to unit
	heading := swapi.MakeHeading(0)
	params := swapi.MakeParameters(heading, nil, nil, nil, swapi.MakePathfind(pathfind), swapi.MakePointParam(swapi.Point{X: -15.9250, Y: 28.2650}))
	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	_, err = client.SendUnitOrder(unit.Id, MissionMoveAlongId, params)
	for i := 1; i < 3; i++ {
		p := pathfind.Result[i]
		if p.Waypoint < 0 {
			continue
		}
		waitCondition(c, client.Model, func(m *swapi.ModelData) bool {
			// do not go to first position nor the last point
			c.Check(m.Units[unit.Id].Position, Not(IsNearby), positions[0])
			c.Check(m.Units[unit.Id].Position, Not(IsNearby), positions[3])
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

// Creates a unit moving like a main battle tank, creates an itinerary with
// "path" waypoints, asks the unit to move from "from" to "to" using the
// itinerary and return computed path.
func getMoveAlongItinerary(c *C, phydb *phy.PhysicalData, client *swapi.Client,
	from, to swapi.Point, path []swapi.Point) ([]swapi.Point, []swapi.Point) {

	moveAlongItinerary := uint32(445949284)

	// Create test unit
	party := &swrun.Party{
		Name: "party1",
		Formations: []*swrun.Formation{
			&swrun.Formation{
				Name: "formation1",
				Automats: []*swrun.Automat{
					&swrun.Automat{
						Name: "automat",
						Type: "VW Combi Rally",
						Units: []*swrun.Unit{
							&swrun.Unit{
								Name: "unit",
								Type: "VW Main Battle Combi",
								Pos:  from,
							},
						},
					},
				},
			},
		},
	}
	err := swrun.FindOrCreateEntities(client, phydb, party)
	c.Assert(err, IsNil)
	unit := party.Formations[0].Automats[0].Units[0].Entity
	err = client.SetAutomatMode(unit.AutomatId, false)
	c.Assert(err, IsNil)

	// Create the path
	pathfind, err := client.CreatePathfind(unit.Id, path...)
	c.Assert(err, IsNil)
	itinerary := []swapi.Point{}
	for _, p := range pathfind.Result {
		itinerary = append(itinerary, p.Point)
	}

	// Make move along the path
	_, err = client.SendUnitOrder(unit.Id, moveAlongItinerary, swapi.MakeParameters(
		swapi.MakeHeading(0),
		nil,
		nil,
		nil,
		swapi.MakePathfind(pathfind),
		swapi.MakePointParam(to)))
	c.Assert(err, IsNil)

	points := []swapi.Point{}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		path := data.Units[unit.Id].Path
		if len(path) == 0 {
			return false
		}
		points = make([]swapi.Point, len(path))
		copy(points, path)
		return true
	})
	return points, itinerary
}

type KnownPoint struct {
	X int32
	Y int32
}

func MakeKnownPoint(p swapi.Point) KnownPoint {
	precision := float64(1e5)
	return KnownPoint{
		X: int32(p.X * precision),
		Y: int32(p.Y * precision),
	}
}

// KnownEdge are edged snapped on a grid with its end points ordered to make
// it hashable. The gridding is done to reduce the effect of floating point
// noise.
type KnownEdge struct {
	From KnownPoint
	To   KnownPoint
}

func MakeKnownEdge(p1, p2 swapi.Point) KnownEdge {
	if p1.X > p2.X || p1.X == p2.X && p1.Y > p2.Y {
		p1, p2 = p2, p1
	}
	return KnownEdge{
		From: MakeKnownPoint(p1),
		To:   MakeKnownPoint(p2),
	}
}

// Given a unit actual path and an itinerary passed as argument, getMatchedRatios
// returns a sequence [r1, r2, ... rn] where each rn represents a slice of
// itinerary overlapped by path, in percents. The value is positive if matched,
// negative otherwise.
// For instance, if the itinerary is matched by the path starting at its first
// quarter and left at the beginning of its last quarter, the result is:
//
//   [-25, 50, -25]
//
func getMatchedRatio(path, itinerary []swapi.Point) []int {
	distances := []float64{}
	edges := map[KnownEdge]bool{}
	for i := 1; i < len(itinerary); i++ {
		edges[MakeKnownEdge(itinerary[i-1], itinerary[i])] = true
	}
	total := float64(0)
	for i := 1; i < len(path); i++ {
		p1 := path[i-1]
		p2 := path[i]
		dx := p1.X - p2.X
		dy := p1.Y - p2.Y
		d := math.Sqrt(dx*dx + dy*dy)
		if d == 0 {
			continue
		}
		total += d
		matched := edges[MakeKnownEdge(p1, p2)]
		if !matched {
			d = -d
		}
		if len(distances) == 0 ||
			math.Signbit(d) != math.Signbit(distances[len(distances)-1]) {
			distances = append(distances, d)
		} else {
			distances[len(distances)-1] += d
		}
	}
	percents := []int{}
	for _, d := range distances {
		percent := int(100 * d / total)
		if percent == 0 {
			continue
		}
		last := len(percents) - 1
		if len(percents) == 0 || percent > 0 && percents[last] < 0 ||
			percent < 0 && percents[last] > 0 {
			percents = append(percents, percent)
		} else {
			percents[last] += percent
		}
	}
	return percents
}

func (s *TestSuite) TestMoveAlongItineraryNoBacktrack(c *C) {
	phydb := loadPhysicalData(c, "test")
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExAngersEmpty).RecordUnitPaths())
	defer stopSimAndClient(c, sim, client)

	points, itinerary := getMoveAlongItinerary(c, phydb, client,
		swapi.Point{X: -0.4096, Y: 47.4551},
		swapi.Point{X: 0.0213, Y: 47.2953},
		[]swapi.Point{
			swapi.Point{X: -0.4496, Y: 47.3988},
			swapi.Point{X: -0.3263, Y: 47.3584},
			swapi.Point{X: -0.1528, Y: 47.3077},
			swapi.Point{X: -0.0860, Y: 47.2531},
		})

	// The path goes west to east, crossing a river. We want to check the path
	// will not backtrack to the closest point to the initial position after it
	// crosses the river.
	limit := swapi.Point{X: 47.3999, Y: -0.3256}
	crossed := false
	for _, p := range points {
		if !crossed {
			crossed = p.X > limit.X
		} else {
			c.Assert(p.X, Greater, limit.X)
		}
	}
	ratios := getMatchedRatio(points, itinerary)
	c.Assert(ratios, DeepEquals, []int{-32, 39, -23})
}

func testMoveAlongHorseshoe(c *C, from, to swapi.Point, expectedRatios []int) {
	phydb := loadPhysicalData(c, "test")
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExAngersEmpty).RecordUnitPaths())
	defer stopSimAndClient(c, sim, client)

	points, itinerary := getMoveAlongItinerary(c, phydb, client,
		from,
		to,
		[]swapi.Point{
			swapi.Point{X: -0.4310, Y: 47.2007},
			swapi.Point{X: -0.3452, Y: 47.1160},
			swapi.Point{X: -0.2635, Y: 47.1808},
		})
	ratios := getMatchedRatio(points, itinerary)
	c.Assert(ratios, DeepEquals, expectedRatios)
}

func (s *TestSuite) TestMoveAlongItineraryInsideHorseshoe(c *C) {
	// The path is big U and we move within it. The unit should just move
	// to the end point without using the itinerary.
	testMoveAlongHorseshoe(c,
		swapi.Point{X: -0.3680, Y: 47.1978},
		swapi.Point{X: -0.3241, Y: 47.1666},
		[]int{-100})
}

func (s *TestSuite) TestMoveAlongItineraryAboveHorseshoe(c *C) {
	// The path is big U and we move on a line slightly above it. The unit
	// should avoid the itinerary and reach the destination directly.
	testMoveAlongHorseshoe(c,
		swapi.Point{X: -0.4539, Y: 47.2720},
		swapi.Point{X: -0.1441, Y: 47.2920},
		[]int{-100})
}

func (s *TestSuite) TestMoveAlongItineraryOnHorseshoe(c *C) {
	// The path is big U and from and to point are more or less aligned with
	// the U tips. The unit should walk on the full path.
	// The ratios are not 100 because gaming coordinate widget resolution is
	// not high enough to place the waypoints exactly on roads. Smallish
	// itinerary pieces are skipped at the each waypoints.
	testMoveAlongHorseshoe(c,
		swapi.Point{X: -0.5126, Y: 47.2098},
		swapi.Point{X: -0.2476, Y: 47.1829},
		[]int{-24, 1, -1, 32, -4, 25, -4})
}

func testPathEdgeSplit(c *C, client *swapi.Client, unit *swapi.Unit,
	from, to swapi.Point, split bool, points int,
	minDegrees, maxDegrees float64) []swapi.PathPoint {

	rq := sword.PathfindRequest{
		Unit: swapi.MakeId(unit.Id),
		DisableSplitOnElevationGrid: proto.Bool(!split),
		ReturnSlopes:                proto.Bool(true),
	}
	path, err := client.PathfindRequest(&rq, from, to)
	c.Assert(err, IsNil)
	c.Assert(len(path), Equals, points)

	// Check points are monotonically increasing from west to east
	for i := 1; i < len(path); i++ {
		c.Assert(path[i-1].Point.X, Lesser, path[i].Point.X)
	}
	for _, p := range path {
		degrees := math.Atan(float64(p.Slope)) * 180. / math.Pi
		if degrees == 0 {
			// Ignore zero entries which appear when slopes are not returned
			// or when a sequence of points have the same slope.
			continue
		}
		c.Assert(degrees, GreaterOrEquals, minDegrees)
		c.Assert(degrees, LesserOrEquals, maxDegrees)
	}
	return path
}

func (s *TestSuite) TestPathEdgesSplitOnElevationGrid(c *C) {
	opts := NewAdminOpts(ExGradXYTestEmpty)
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)

	phydb := loadPhysicalData(c, "test")
	from := swapi.Point{X: -15.9384, Y: 28.220}
	to := swapi.Point{X: -15.71, Y: from.Y}
	unit := CreateTestUnit(c, phydb, client, "VW Combi 4x4", from)

	// Minimum slope with disabled slope computation
	testPathEdgeSplit(c, client, unit, from, to, false, 14, 0, 0)

	// https://masagroup.atlassian.net/browse/SWBUG-13524
	// Minimum slope
	testPathEdgeSplit(c, client, unit, from, to, true, 15, 0, 0.2)

	// Slope around 16 degrees
	from.Y = 28.2603
	to.Y = 28.2603
	testPathEdgeSplit(c, client, unit, from, to, true, 19, 16.7, 17.6)
}
