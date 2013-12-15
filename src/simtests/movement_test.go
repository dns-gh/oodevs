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
	MissionMoveTestId = uint32(5)
	CombiVWAutomatId  = uint32(2)
	CombiVWUnitId     = uint32(2)
)

func CreateCombiVW(c *C, client *swapi.Client, pos swapi.Point) *swapi.Unit {
	model := client.Model.GetData()
	f := getSomeFormation(c, model)
	kg := getSomeKnowledgeGroup(c, model)
	c.Assert(f.PartyId, Equals, kg.PartyId)
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
	tolerance := int32(3)
	c.Assert(speed, Greater, expectedSpeed-tolerance)
	c.Assert(speed, Lesser, expectedSpeed+tolerance)
}

func (s *TestSuite) TestSlopeSpeedModulation(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExGradXYTestEmpty))
	defer sim.Stop()

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
	//    speed * ( 1.0 - factor*slope)
	//
	// and factor = 0.5 for this unit

	// slope: 0 - 0.05
	checkSlopeSpeed(28.2235, 130, true)
	// slope: 0.25 - 0.30
	checkSlopeSpeed(28.2584, 113, true)
	// slope: 0.50 - 0.55
	checkSlopeSpeed(28.3023, 90, true)
	// Does not accelerate when moving downward
	checkSlopeSpeed(28.3023, 130, false)
	// Downslope can stop the move too
	checkSpeed(c, client, swapi.Point{X: -15.7153, Y: 28.257}, swapi.Point{X: -15.7787, Y: 28.2258}, 0)
}

func (s *TestSuite) TestTerrainSpeedModulation(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExLandOfStripesEmpty))
	defer sim.Stop()

	// 10km/h forest
	lngForest, latForest := -15.732, 28.4374
	checkSpeed(c, client, swapi.Point{X: lngForest, Y: latForest},
		swapi.Point{X: lngForest - 0.05, Y: latForest}, 10)

	// 90km/h urbain
	lngUrbain, latUrbain := -15.732, 28.3568
	checkSpeed(c, client, swapi.Point{X: lngUrbain, Y: latUrbain},
		swapi.Point{X: lngUrbain - 0.05, Y: latUrbain}, 90)
}
