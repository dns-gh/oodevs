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
	"sword"
)

func (s *TestSuite) TestSetAutomatMode(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	c.Assert(automat.Engaged, Equals, true)

	checkEngage := func(engagedId, waitId uint32, engage bool) {
		err := client.SetAutomatMode(engagedId, engage)
		c.Assert(err, IsNil)
	}

	// Invalid automat identifier
	err := client.SetAutomatMode(12456, false)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// This one should work
	checkEngage(automat.Id, automat.Id, false)

	// Engage it again
	checkEngage(automat.Id, automat.Id, true)
}

func (s *TestSuite) TestAutomatReloadBrain(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	tasker := swapi.MakeAutomatTasker(automat.Id)

	// too many parameters
	err := client.ReloadBrainTest(tasker, swapi.MakeParameters(
		swapi.MakeString("blah"), nil))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid type
	err = client.ReloadBrainTest(tasker, swapi.MakeParameters(
		swapi.MakeBoolean(true),
	))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid automat id
	empty := swapi.MakeParameters()
	err = client.ReloadBrainTest(swapi.MakeAutomatTasker(automat.Id+1), empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// missing id
	err = client.ReloadBrainTest(&sword.Tasker{}, empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// invalid model
	err = client.ReloadBrain(tasker, "bad_brain")
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// reload current brain
	err = client.ReloadBrain(tasker, "")
	c.Assert(err, IsNil)

	// Reload automat decisional model
	err = client.ReloadBrain(tasker, "Journalists")
	c.Assert(err, IsNil)
}
