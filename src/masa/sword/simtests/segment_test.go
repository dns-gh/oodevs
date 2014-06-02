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
	"masa/sword/swapi"
	"masa/sword/sword"
)

func (s *TestSuite) TestSegmentRequest(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	position := swapi.Point{X: -15.8640, Y: 28.2507}
	// no terrain means all terrains
	terrains := []sword.TerrainType{}
	count := 1
	segments, err := client.SegmentRequest(position, terrains, uint32(count))
	c.Assert(err, IsNil)
	c.Assert(segments, HasLen, count)
	// unavailable terrain yields no result
	terrains = append(terrains, sword.TerrainType_ice)
	segments, err = client.SegmentRequest(position, terrains, uint32(count))
	c.Assert(err, IsNil)
	c.Assert(segments, HasLen, 0)
	// all roads terrain finds a segment
	terrains = []sword.TerrainType{
		sword.TerrainType_highway,
		sword.TerrainType_large_road,
		sword.TerrainType_medium_road,
		sword.TerrainType_small_road,
	}
	segments, err = client.SegmentRequest(position, terrains, uint32(count))
	c.Assert(err, IsNil)
	c.Assert(segments, HasLen, count)
	// find more segments
	count = 7
	segments, err = client.SegmentRequest(position, terrains, uint32(count))
	c.Assert(err, IsNil)
	c.Assert(segments, HasLen, count)
}
