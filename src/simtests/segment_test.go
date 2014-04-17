// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package simtests

import (
	. "launchpad.net/gocheck"
	"swapi"
	"sword"
)

func (s *TestSuite) TestSegmentRequest(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	// no terrain means all terrains
	terrains := []sword.SegmentRequest_Terrain{}
	count := 1
	segments, err := client.SegmentRequest(swapi.Point{X: -15.8640, Y: 28.2507}, terrains, count)
	c.Assert(err, IsNil)
	c.Assert(segments, HasLen, count)
	// unavailable terrain yields no result
	terrains = append(terrains, sword.SegmentRequest_glacier)
	segments, err = client.SegmentRequest(swapi.Point{X: -15.8640, Y: 28.2507}, terrains, count)
	c.Assert(err, IsNil)
	c.Assert(segments, HasLen, 0)
	// all roads terrain finds a segment
	terrains = []sword.SegmentRequest_Terrain{
		sword.SegmentRequest_highway,
		sword.SegmentRequest_main_road,
		sword.SegmentRequest_secondary_road,
		sword.SegmentRequest_country_road,
	}
	segments, err = client.SegmentRequest(swapi.Point{X: -15.8640, Y: 28.2507}, terrains, count)
	c.Assert(err, IsNil)
	c.Assert(segments, HasLen, count)
	// find more segments
	count = 7
	segments, err = client.SegmentRequest(swapi.Point{X: -15.8640, Y: 28.2507}, terrains, count)
	c.Assert(err, IsNil)
	c.Assert(segments, HasLen, count)
}
