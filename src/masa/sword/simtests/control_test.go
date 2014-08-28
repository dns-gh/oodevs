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
	"time"
)

func (s *TestSuite) TestPauseStopResume(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	model := client.Model

	tick, err := client.Pause()
	c.Assert(err, IsNil)
	c.Assert(tick, Greater, int32(0))
	// Pausing a paused simulation is a no-op
	_, err = client.Pause()
	c.Assert(err, IsNil)

	// Test WaitUntilTick() while we are here
	model.WaitTimeout = 2 * time.Second
	if model.WaitUntilTick(tick + 1) {
		c.Fatal("simulation ticked while paused")
	}
	model.WaitTimeout = 1 * time.Minute

	// Resume without pausing again afterwards
	tick, delay, err := client.Resume(0)
	c.Assert(err, IsNil)
	c.Assert(tick, Greater, int32(0))
	c.Assert(delay, Equals, int32(0))
	// Should tick now
	if !model.WaitUntilTick(tick + 1) {
		c.Fatal("simulation failed to resume")
	}

	// Resuming again is a no-op
	_, _, err = client.Resume(0)
	c.Assert(err, IsNil)

	// Resuming again with a delay will pause after the delay
	// Note the pause happens after the end of (tick + delay - 1) and before
	// the start of (tick + delay), hence the -1 below.
	tick, delay, err = client.Resume(1)
	c.Assert(err, IsNil)
	if !model.WaitUntilTickEnds(tick + delay - 1) {
		c.Fatal("simulation failed to resume")
	}
	// Should tick once
	model.WaitTimeout = 2 * time.Second
	// Not twice
	if model.WaitUntilTickEnds(tick + delay) {
		c.Fatal("simulation ticked while paused")
	}
	model.WaitTimeout = 1 * time.Minute

	// Stop the simulation
	err = client.Stop()
	c.Assert(err, IsNil)
	if !sim.Wait(10 * time.Second) {
		c.Fatal("simulation refused to stop when asked gently")
	}
	c.Assert(sim.Success(), Equals, true)

}

func (s *TestSuite) TestControlRights(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)

	_, err := client.Pause()
	c.Assert(err, IsSwordError, "error_forbidden")

	// Privilege checking should happen before validity checks, that is
	// resuming a non-paused simulation
	_, _, err = client.Resume(0)
	c.Assert(err, IsSwordError, "error_forbidden")

	err = client.Stop()
	c.Assert(err, IsSwordError, "error_forbidden")
}

func (s *TestSuite) TestControlChangeDateTime(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	invalid, err := swapi.GetTime("20100908T060504")
	c.Assert(err, IsNil)
	err = client.ChangeTime(invalid)
	c.Assert(err, IsSwordError, "error_invalid_date_time")
	valid, err := swapi.GetTime("20200908T060504")
	c.Assert(err, IsNil)
	err = client.ChangeTime(valid)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Time.After(valid)
	})
}

func (s *TestSuite) TestControlChangeTimeFactor(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)

	// Invalid values
	_, err := client.SetTimeFactor(0)
	c.Assert(err, NotNil)
	_, err = client.SetTimeFactor(-1)
	c.Assert(err, NotNil)

	// Check response and model update
	factor, err := client.SetTimeFactor(42)
	c.Assert(err, IsNil)
	c.Assert(factor, Equals, int32(42))
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.TimeFactor == factor
	})
}

func (s *TestSuite) TestControlCheckpointSetFrequency(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)

	// Invalid value
	err := client.SetCheckpointFrequency(-1)
	c.Assert(err, NotNil)

	// Check response and model update
	err = client.SetCheckpointFrequency(42)
	c.Assert(err, IsNil)
}
