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
)

func (s *TestSuite) TestDrawingLimit(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()

	points := []swapi.Point{
		swapi.Point{X: -15.8241, Y: 28.3241},
		swapi.Point{X: -15.8092, Y: 28.3458},
	}

	// Creation
	unitId := getSomeUnit(c, data).Id
	limit, err := client.CreateLimit("MyLimit", unitId, points...)
	c.Assert(err, IsNil)
	c.Assert(limit, NotNil)

	// Update
	limit.Name = "NewName"
	err = client.UpdateLimit(limit)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		line := data.TacticalLines[limit.Id]
		return line != nil && line.Name == limit.Name
	})

	// Destruction
	err = client.DeleteLimit(limit)
	c.Assert(err, IsNil)
	c.Assert(client.Model.GetTacticalLine(limit.Id), IsNil)
}

func (s *TestSuite) TestDrawingPhaseLine(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()

	points := []swapi.Point{
		swapi.Point{X: -15.8241, Y: 28.3241},
		swapi.Point{X: -15.8092, Y: 28.3458},
	}

	// Creation
	unitId := getSomeUnit(c, data).Id
	phaseLine, err := client.CreatePhaseLine("MyPhaseLine", unitId, points...)
	c.Assert(err, IsNil)
	c.Assert(phaseLine, NotNil)

	// Update
	phaseLine.Name = "NewName"
	err = client.UpdatePhaseLine(phaseLine)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		line := data.TacticalLines[phaseLine.Id]
		return line != nil && line.Name == phaseLine.Name
	})

	// Destruction
	err = client.DeletePhaseLine(phaseLine)
	c.Assert(err, IsNil)
	c.Assert(client.Model.GetTacticalLine(phaseLine.Id), IsNil)
}
