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
	"swapi/replay"
	"sword"
)

func (s *TestSuite) TestReplayerData(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallEmpty))
	defer sim.Stop()

	// Create new formation and wait a bit
	party := getSomeParty(c, client.Model.GetData())
	formation := CreateFormation(c, client, party.Id)
	client.Model.WaitTicks(1)
	sim.Stop()

	// Replay messages and find the tick the formation was created.
	creationTick := int32(0)
	found := false
	handler := func(frame replay.Frame, msg *sword.SimToClient) error {
		m := msg.GetMessage()
		// Do not break on found to enumerate all messages
		if !found && m.ControlBeginTick != nil {
			creationTick = *m.ControlBeginTick.CurrentTick
		} else if !found && m.FormationCreation != nil &&
			*m.FormationCreation.Formation.Id == formation.Id {
			found = true
		}
		return nil
	}
	err := replay.EnumerateReplayUpdates(sim.Opts.GetReplayDir(), handler)
	c.Assert(err, IsNil)
	c.Assert(found, Equals, true)
	c.Assert(creationTick, Greater, int32(0))

	// The formation should not be in the first keyframe
	firstFrame := int32(-1)
	found = false
	keyHandler := func(frame replay.KeyFrame, msg *sword.SimToClient) error {
		if firstFrame < 0 {
			firstFrame = frame.FrameNum
		} else if firstFrame != frame.FrameNum {
			return nil
		}
		m := msg.GetMessage()
		if m.FormationCreation != nil &&
			*m.FormationCreation.Formation.Id == formation.Id {
			found = true
		}
		return nil
	}
	err = replay.EnumerateReplayKeyFrames(sim.Opts.GetReplayDir(), keyHandler)
	c.Assert(err, IsNil)
	c.Assert(firstFrame, Greater, int32(0))
	c.Assert(found, Equals, false)

	// Start a replay, login a client, the formation is not there
	replay := startReplay(c, sim.Opts)
	defer replay.Kill()
	client = loginAndWaitModel(c, replay, NewAdminOpts(""))
	defer client.Close()
	c.Assert(client.Model.GetFormation(formation.Id), IsNil)

	// The replayer operates step-by-step, fast forward to creation tick
	err = client.SkipToTick(creationTick)
	c.Assert(err, IsNil)
	c.Assert(client.Model.GetFormation(formation.Id), NotNil)

	// Move back before creation tick
	err = client.SkipToTick(creationTick - 1)
	c.Assert(err, IsNil)
	c.Assert(client.Model.GetFormation(formation.Id), IsNil)
}
