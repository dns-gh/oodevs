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
	"os"
	"path/filepath"
	"regexp"
)

const (
	MissionMoveTestId = uint32(5)
	CombiVWAutomatId  = uint32(2)
	CombiVWUnitId     = uint32(2)
)

func CreateCombiVW(c *C, client *swapi.Client, pos swapi.Point) *swapi.Unit {
	model := client.Model.GetData()
	f := getSomeFormation(c, model)
	kg := getSomeKnowledgeGroup(c, model, f.PartyId)
	automat, err := client.CreateAutomat(f.Id, CombiVWAutomatId, kg.Id)
	c.Assert(err, IsNil)
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	unit, err := client.CreateUnit(automat.Id, CombiVWUnitId, pos)
	c.Assert(err, IsNil)
	return unit
}

func checkSpeed(c *C, client *swapi.Client, from, to swapi.Point, expectedSpeed int32) {
	unit := CreateCombiVW(c, client, from)
	_, err := client.SendUnitOrder(unit.Id, MissionMoveTestId,
		swapi.MakeParameters(swapi.MakeHeading(0), nil, nil, nil,
			swapi.MakePointParam(to)))
	c.Assert(err, IsNil)
	speed := int32(0)
	tick := client.Model.GetTick()
	waitCondition(c, client.Model, func(m *swapi.ModelData) bool {
		if m.Tick < tick+3 {
			return false
		}
		speed = m.Units[unit.Id].Speed
		return true
	})
	tolerance := int32(2)
	c.Assert(speed, Greater, expectedSpeed-tolerance)
	c.Assert(speed, Lesser, expectedSpeed+tolerance)
}

func (s *TestSuite) TestSlopeSpeedModulation(c *C) {
	opts := NewAdminOpts(ExGradXYTestEmpty)
	opts.DumpPathfinds("pf") // for test coverage
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)

	lngFrom, lngTo := -15.9384, -15.7028
	checkSlopeSpeed := func(lat float64, expectedSpeed int32, upward bool) {
		from := swapi.Point{X: lngFrom, Y: lat}
		to := swapi.Point{X: lngTo, Y: lat}
		if !upward {
			from, to = to, from
		}
		checkSpeed(c, client, from, to, expectedSpeed)
	}

	// Speed formula is like:
	//
	//    speed * ( 1.0 - deceleration*slope/slopemax)
	//
	// and for this unit:
	baseSpeed := float64(130)
	deceleration := 0.5
	slopeMax := 0.7
	// Compute expected speed for VW Combi located somewhere between minSlope
	// and maxSlope.
	expectedSpeed := func(minSlope, maxSlope float64) int32 {
		slope := 0.5 * (minSlope + maxSlope)
		return int32(baseSpeed * (1.0 - deceleration*(slope/slopeMax)))
	}

	checkSlopeSpeed(28.2235, expectedSpeed(0, 0.05), true)
	checkSlopeSpeed(28.2571, expectedSpeed(0.25, 0.30), true)
	checkSlopeSpeed(28.3023, expectedSpeed(0.60, 0.65), true)
	// Does not accelerate when moving downward
	checkSlopeSpeed(28.3023, int32(baseSpeed), false)
	// Downslope can stop the move too
	checkSpeed(c, client, swapi.Point{X: -15.7153, Y: 28.257},
		swapi.Point{X: -15.7787, Y: 28.2258}, 0)

	found := false
	pfDir := filepath.Join(sim.Opts.GetSessionDir(), "pf")
	rePath := regexp.MustCompile(`^pathfind_\d+_\d+$`)
	err := filepath.Walk(pfDir, func(path string, fi os.FileInfo, err error) error {
		if err != nil || fi.IsDir() {
			return err
		}
		found = found || rePath.MatchString(filepath.Base(path))
		return nil
	})
	c.Assert(err, IsNil)
	c.Assert(found, Equals, true)
}

func (s *TestSuite) TestTerrainSpeedModulation(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExLandOfStripesEmpty))
	defer stopSimAndClient(c, sim, client)

	// 10km/h forest
	lngForest, latForest := -15.732, 28.4374
	checkSpeed(c, client, swapi.Point{X: lngForest, Y: latForest},
		swapi.Point{X: lngForest - 0.05, Y: latForest}, 10)

	// 90km/h urbain
	lngUrbain, latUrbain := -15.732, 28.3568
	checkSpeed(c, client, swapi.Point{X: lngUrbain, Y: latUrbain},
		swapi.Point{X: lngUrbain - 0.05, Y: latUrbain}, 90)
}
