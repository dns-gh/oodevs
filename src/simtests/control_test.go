package simtests

import (
	. "launchpad.net/gocheck"
	"time"
)

func (s *TestSuite) TestPauseStopResume(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallEmpty)
	defer sim.Kill()
	model := client.Model

	err := client.Pause()
	c.Assert(err, IsNil)
	err = client.Pause()
	c.Assert(err, ErrorMatches, "error_already_paused")

	// Wait a bit to see if it ticks, not super reliable but locally ticks
	// are around 500ms
	tick1 := model.GetData().Tick
	time.Sleep(3 * time.Second)
	tick2 := model.GetData().Tick
	c.Assert(tick1, Equals, tick2)

	// Test WaitUntilTick() while we are here
	model.WaitTimeout = 2 * time.Second
	if model.WaitUntilTick(tick1 + 1) {
		c.Fatal("simulation ticked while paused")
	}
	model.WaitTimeout = 1 * time.Minute

	// Resume without pausing again afterwards
	err = client.Resume(0)
	c.Assert(err, IsNil) // simulation failed to resume
	err = client.Resume(0)
	c.Assert(err, ErrorMatches, "error_not_paused")
	// Should tick now
	if !model.WaitUntilTick(tick1 + 1) {
		c.Fatal("simulation failed to resume")
	}

	// Resuming again with a delay should fail too 
	err = client.Resume(1)
	c.Assert(err, ErrorMatches, "error_not_paused")
	// Test resuming with a delay
	err = client.Pause()
	c.Assert(err, IsNil)
	tick1 = model.GetData().Tick
	err = client.Resume(1)
	if !model.WaitUntilTick(tick1 + 1) {
		c.Fatal("simulation failed to resume")
	}
	// Should tick once
	model.WaitTimeout = 2 * time.Second
	// Not twice
	if model.WaitUntilTick(tick1 + 2) {
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
	sim, client := connectAndWaitModel(c, "alluser", "alluser", ExCrossroadSmallEmpty)
	defer sim.Kill()

	err := client.Pause()
	c.Assert(err, ErrorMatches, "error_forbidden")

	// Privilege checking should happen before validity checks, that is
	// resuming a non-paused simulation
	err = client.Resume(0)
	c.Assert(err, ErrorMatches, "error_forbidden")

	err = client.Stop()
	c.Assert(err, ErrorMatches, "error_forbidden")
}
