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
	"masa/sword/swapi/replay"
	"masa/sword/swapi/simu"
	"masa/sword/sword"
	"math/rand"
)

func replayAndWaitModel(c *C, simOpts *simu.SimOpts, clientOpts *ClientOpts) (
	*simu.ReplayProcess, *swapi.Client) {

	replay := startReplay(c, simOpts)
	client := loginAndWaitModel(c, replay, clientOpts)
	return replay, client
}

// Make a short replay session and return simulation options.
func makeSimpleReplay(c *C) (*simu.SimOpts, *swapi.Formation) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)

	// Create new formation and wait a bit
	party := getSomeParty(c, client.Model.GetData())
	formation := CreateFormation(c, client, party.Id)
	client.Model.WaitTicks(1)
	// TODO: make sure the following tick is not empty, the replayer has problems
	// reaching the last non-empty tick.
	CreateFormation(c, client, party.Id)
	return sim.Opts, formation
}

func (s *TestSuite) TestReplayerData(c *C) {
	opts, formation := makeSimpleReplay(c)

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
	err := replay.EnumerateReplayUpdates(opts.GetReplayDir(), handler)
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
	err = replay.EnumerateReplayKeyFrames(opts.GetReplayDir(), keyHandler)
	c.Assert(err, IsNil)
	c.Assert(firstFrame, Greater, int32(0))
	c.Assert(found, Equals, false)

	// Start a replay, login a client, the formation is not there
	replay, client := replayAndWaitModel(c, opts, NewAdminOpts(""))
	defer replay.Kill()
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

type ModelDump struct {
	tick  int32
	model *swapi.ModelData
}

func getReplayDumps(c *C, step int32) (*simu.SimOpts, []ModelDump) {
	cfg := NewAdminOpts(ExCrossroadSmallEmpty)
	cfg.Paused = true
	sim, client := connectAndWaitModel(c, cfg)
	defer stopSimAndClient(c, sim, client)
	dumps := []ModelDump{}
	skip := func(n int32) {
		tick := client.Model.GetTick()
		if tick == 0 {
			tick++ // looks like a bug
		}
		client.Resume(uint32(n))
		client.Model.WaitUntilTickEnds(tick + n)
		data := client.Model.GetData()
		dump := ModelDump{tick: data.Tick, model: data}
		dumps = append(dumps, dump)
	}
	skip(step)

	party := getSomeParty(c, client.Model.GetData())
	formation := CreateFormation(c, client, party.Id)
	skip(step)

	automat := CreateAutomat(c, client, formation.Id, 0)
	skip(step)

	unit := CreateUnit(c, client, automat.Id)
	_ = unit
	skip(step)

	crowd := CreateCrowdFromParty(c, client, party.Name)
	_ = crowd
	skip(step)

	object, err := client.CreateObject("jamming area", party.Id, swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456}))
	_ = object
	c.Assert(err, IsNil)
	skip(step)

	from := swapi.Point{X: -15.8193, Y: 28.3456}
	to := swapi.Point{X: -15.8183, Y: 28.3466}
	object2, err := client.CreateObject("dyke", 0,
		swapi.MakePolygonLocation(
			from, swapi.Point{X: from.X, Y: to.Y},
			to, swapi.Point{X: to.X, Y: from.Y}))
	c.Assert(err, IsNil)
	skip(step)

	kg, err := client.CreateKnowledgeGroup(party.Id, "Standard")
	_ = kg
	c.Assert(err, IsNil)
	skip(step)

	_, err = client.CreateUnitKnowledge(kg.Id, unit.Id, 2)
	c.Assert(err, IsNil)
	skip(step)

	_, err = client.CreateObjectKnowledge(kg.Id, object.Id, 2)
	c.Assert(err, IsNil)
	skip(step)

	_, err = client.CreateObjectKnowledge(kg.Id, object2.Id, 2)
	c.Assert(err, IsNil)
	skip(step)

	_, err = client.CreateCrowdKnowledge(kg.Id, crowd.Id, 2)
	c.Assert(err, IsNil)
	skip(step)

	return sim.Opts, dumps
}

func replayDumps(c *C, cfg *simu.SimOpts, dumps []ModelDump) {
	replay := startReplay(c, cfg)
	defer replay.Kill()
	client := loginAndWaitModel(c, replay, NewAdminOpts(""))
	defer client.Close()
	for _, d := range dumps {
		// TODO: for some reason SkipToTick(N) moves at the end of N+1
		err := client.SkipToTick(d.tick - 1)
		c.Assert(err, IsNil)
		client.Model.WaitUntilTickEnds(d.tick)
		model := client.Model.GetData()
		compareModels(c, d.model, model, cfg.GetSessionDir())
	}
}

func (s *TestSuite) TestReplayerModels(c *C) {
	step := int32(1)
	cfg, dumps := getReplayDumps(c, step)
	c.Assert(len(dumps), Greater, 0)
	// TODO: find a way to test all dumps, the replayer refuses to move right
	// after the last one, possibly because of empty ticks.
	dumps = dumps[:len(dumps)-1]
	replayDumps(c, cfg, dumps)
	// play ticks in reverse order
	j := len(dumps) - 1
	for i := 0; i*2 < len(dumps); i++ {
		dumps[i], dumps[j] = dumps[j], dumps[i]
		j--
	}
	replayDumps(c, cfg, dumps)
	// play ticks in random order
	rd := rand.New(rand.NewSource(0))
	max := len(dumps) - 1
	for i := range dumps {
		if i == max {
			break
		}
		j := i + rd.Intn(max-i)
		dumps[i], dumps[j] = dumps[j], dumps[i]
	}
	replayDumps(c, cfg, dumps)
}

func checkTimetable(c *C, client *swapi.Client, first, last int32, broadcast bool) {
	table, err := client.GetTimetable(first, last, broadcast)
	c.Assert(err, IsNil)
	c.Assert(len(table), Equals, int(last-first+1))
	for i, t := range table {
		// TODO: why the +1?
		c.Assert(*t.Tick, Equals, first+int32(i)+1)
	}
}

func (s *TestSuite) TestReplayerTimetable(c *C) {
	opts, _ := makeSimpleReplay(c)
	replay, client := replayAndWaitModel(c, opts, NewAdminOpts(""))
	defer replay.Kill()
	defer client.Close()
	// ControlReplayInformation is apparently only returned after a skip
	err := client.SkipToTick(1)
	c.Assert(err, IsNil)

	broadcasts := 0
	client.Register(func(msg *swapi.SwordMessage, id, ctx int32, err error) bool {
		if err != nil {
			return true
		}
		if msg != nil &&
			msg.ReplayToClient != nil &&
			msg.ReplayToClient.Message != nil &&
			msg.ReplayToClient.Message.TimeTable != nil {
			broadcasts++
		}
		return false
	})

	info := client.Model.GetData().Replay
	c.Assert(info, NotNil)
	c.Assert(info.FirstTick, Greater, int32(0))
	c.Assert(info.TickCount, Greater, int32(2))

	// Request all ticks
	checkTimetable(c, client, info.FirstTick, info.FirstTick+info.TickCount-1, false)
	c.Assert(broadcasts, Equals, 0)

	// Slice and ask for broadcast
	checkTimetable(c, client, info.FirstTick, info.FirstTick+1, true)
	c.Assert(broadcasts, Equals, 1)

	// Invalid values
	_, err = client.GetTimetable(0, 1, false)
	c.Assert(err, ErrorMatches, ".*greater than zero.*")

	_, err = client.GetTimetable(2, 1, false)
	c.Assert(err, ErrorMatches, ".*equal or less.*")

	_, err = client.GetTimetable(2, 100000, false)
	c.Assert(err, ErrorMatches, ".*equal or less.*")
}
