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
	"io/ioutil"
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/swapi/simu"
	"masa/sword/swtest"
	"path/filepath"
	"strings"
	"time"
)

func (s *TestSuite) TestCheckpointMessages(c *C) {
	errOpts := simu.SessionErrorsOpts{
		IgnorePatterns: []string{
			// One test explicitely triggers a checkpoint error
			"Can't save checkpoint",
		},
	}
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, &errOpts)
	defer client.Close()

	check := func(input, errmsg string) {
		boundaries := []string{}
		ctx := client.Register(func(msg *swapi.SwordMessage, id, context int32, err error) bool {
			if err != nil {
				return true
			}
			if msg.SimulationToClient == nil || msg.SimulationToClient.GetMessage() == nil {
				return false
			}
			m := msg.SimulationToClient.GetMessage()
			if reply := m.GetControlCheckpointSaveBegin(); reply != nil {
				boundaries = append(boundaries, "+"+reply.GetName())
			} else if reply := m.GetControlCheckpointSaveEnd(); reply != nil {
				boundaries = append(boundaries, "-"+reply.GetName())
			}
			return false
		})

		checkpoint, snapshot, err := client.CreateCheckpoint(input, false)
		c.Assert(snapshot, IsNil)
		if len(errmsg) > 0 {
			c.Assert(checkpoint, Equals, "")
			c.Assert(err, ErrorMatches, errmsg)
		} else {
			c.Assert(err, IsNil)
			if len(input) > 0 {
				c.Assert(checkpoint, Equals, input)
			} else {
				c.Assert(checkpoint, Matches, `\d{4}_\d{2}_\d{2}__\d{2}h\d{2}m\d{2}`)
			}
			c.Assert(len(boundaries), Greater, 0)
		}
		if len(boundaries) > 0 {
			c.Assert(boundaries, HasLen, 2)
			c.Assert(boundaries[0][:1], Equals, "+")
			c.Assert(boundaries[1][:1], Equals, "-")
			c.Assert(boundaries[0][1:], Equals, boundaries[1][1:])
		}
		client.Unregister(ctx)
	}

	// Must have supervisor rights
	check("", ".*rights check failed.*")
	client.Close()
	client = loginAndWaitModel(c, sim, NewAdminOpts(""))

	// No checkpoint name
	check("", "")

	// Named checkpoint
	validname := "some spacy name with numb3rs and _"
	check(validname, "")

	// Overwrite
	check(validname, "")

	// Invalid UTF-8 sequence
	name := []byte("invalid utf-8")
	name = append(name, byte(140))
	check(string(name), ".*invalid utf-8.*")

	// Invalid file name
	check("some invalid ? name", ".*invalid checkpoint name.*")

	// Invalid very long filename (limited at 255 chars on windows)
	check(strings.Repeat("a", 300), ".*create_directory.*")
}

func (s *TestSuite) TestCheckpointSendState(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	check := func(sendState bool) {
		boundaries := ""
		ctx := client.Register(func(msg *swapi.SwordMessage, id, context int32, err error) bool {
			if err != nil {
				return true
			}
			if msg.SimulationToClient == nil || msg.SimulationToClient.GetMessage() == nil {
				return false
			}
			m := msg.SimulationToClient.GetMessage()
			if reply := m.GetControlCheckpointSaveBegin(); reply != nil {
				boundaries += "<"
			} else if reply := m.GetControlCheckpointSaveEnd(); reply != nil {
				boundaries += ">"
			} else if reply := m.GetControlSendCurrentStateBegin(); reply != nil {
				boundaries += "("
			} else if reply := m.GetControlSendCurrentStateEnd(); reply != nil {
				boundaries += ")"
			} else if len(boundaries) > 0 && boundaries[len(boundaries)-1] != '>' {
				boundaries += "o"
			}
			return false
		})

		_, snapshot, err := client.CreateCheckpoint("checkpoint", sendState)
		c.Assert(err, IsNil)
		if sendState {
			c.Assert(boundaries, Matches, `o*<\(o+\)>o*`)
			c.Assert(snapshot, NotNil)
		} else {
			c.Assert(boundaries, Matches, `o*<>o*`)
			c.Assert(snapshot, IsNil)
		}
		client.Unregister(ctx)
	}

	check(false)
	check(true)
}

// Bugs nonwithstanding, models are not exactly the same across reloads,
// this function rewrites some fields to not interfere with the comparison.
func normalizeModel(model *swapi.ModelData) *swapi.ModelData {
	n := &swapi.ModelData{}
	swapi.DeepCopy(n, model)
	// Tick and time are not part of checkpoint data and requires a tick
	// to be set.
	n.Tick = 0
	n.LastTick = 0
	n.TickDuration = 0
	n.Time = time.Time{}
	// Orders are not restored upon checkpoint (bug?)
	n.Orders = nil
	n.Profiles = nil
	n.Replay = nil
	for _, u := range n.Units {
		// Pathfinds are not restored?
		u.PathPoints = 0
		u.Speed = 0
	}
	return n
}

// Compare m1 and m2 for quasi-equality, fail and display a diff on mismatch.
func compareModels(c *C, m1, m2 *swapi.ModelData, debugDir string) {
	n1 := swtest.Stringify(normalizeModel(m1))
	n2 := swtest.Stringify(normalizeModel(m2))
	if n1 == n2 {
		return
	}
	// Saving the output is helpful for debugging
	err := ioutil.WriteFile(filepath.Join(debugDir, "model-before.txt"),
		[]byte(n1), 0644)
	c.Assert(err, IsNil)
	err = ioutil.WriteFile(filepath.Join(debugDir, "model-after.txt"),
		[]byte(n2), 0644)
	c.Assert(err, IsNil)

	swtest.DeepEquals(c, n2, n1)
}

func loadCheckpointAndWaitModel(c *C, user, password, exercise, session, checkpoint string) (
	*simu.SimProcess, *swapi.Client) {
	sim := startSimOnCheckpoint(c, exercise, session, checkpoint, 1000, true)
	opts := ClientOpts{
		User:     user,
		Password: password,
	}
	client := loginAndWaitModel(c, sim, &opts)
	return sim, client
}

func copyFile(src, dst string) error {
	data, err := ioutil.ReadFile(src)
	if err != nil {
		return err
	}
	return ioutil.WriteFile(dst, data, 0644)
}

func checkpointAndRestart(c *C, sim *simu.SimProcess, client *swapi.Client) (
	*simu.SimProcess, *swapi.Client, *swapi.ModelData) {

	checkpoint, snapshot, err := client.CreateCheckpoint("", true)
	c.Assert(err, IsNil)
	client.Close()
	sim.Stop()

	// Protobuf.log is overwritten when loading the checkpoint (bug?). Keep
	// it here as it is really useful to compare what was sent with what was
	// reloaded.
	protoPath := sim.Opts.GetProtoLogPath()
	protoExt := filepath.Ext(protoPath)
	protoCopy := protoPath[:len(protoPath)-len(protoExt)] + "-before" + protoExt
	err = copyFile(protoPath, protoCopy)
	c.Assert(err, IsNil)

	sim, client = loadCheckpointAndWaitModel(c, "admin", "", sim.Opts.ExerciseName,
		sim.Opts.SessionName, checkpoint)
	return sim, client, snapshot
}

// Create a checkpoint from sim, stop it, reload the checkpoint, and compare the
// new state with the previous one. If stop is true, stop the simulation,
// otherwise return it in a running state.
func checkpointAndCompare(c *C, sim *simu.SimProcess, client *swapi.Client, stop bool) (
	*simu.SimProcess, *swapi.Client) {

	sim, client, before := checkpointAndRestart(c, sim, client)
	valid := false
	defer func() {
		if !valid || stop {
			stopSimAndClient(c, sim, client)
		}
	}()
	after := client.Model.GetData()
	compareModels(c, before, after, sim.Opts.GetSessionDir())
	if !stop {
		valid = true
		client.Resume(0)
	}
	return sim, client
}

func checkpointCompareAndStop(c *C, sim *simu.SimProcess, client *swapi.Client) {
	checkpointAndCompare(c, sim, client, true)
}

// Test SimProcess --checkpoint. This should be with other SimProcess tests but
// it is really difficult to craft checkpoint inputs, so we generate one here
// from a running exercise and reload it.
// There will be other tests to check the checkpoint content.
func (s *TestSuite) TestCheckpointRestart(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	party := client.Model.GetData().FindPartyByName("party1")
	c.Assert(party, NotNil)
	CreateFormation(c, client, party.Id)
	automat := createAutomatForParty(c, client, "party1")
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	sim, client, _ = checkpointAndRestart(c, sim, client)
	defer stopSimAndClient(c, sim, client)
	unit2 := client.Model.GetUnit(unit.Id)
	c.Assert(unit2, NotNil)
}

func (s *TestSuite) TestCheckpointCrowd(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	party := client.Model.GetData().FindPartyByName("party1")
	c.Assert(party, NotNil)

	// Create a crowd and give it a mission
	pos := swapi.Point{X: -15.9219, Y: 28.3456}
	crowd, err := client.CreateCrowd(party.Id, 0, CrowdType, pos, 100, 0, 0, "crowd")
	c.Assert(err, IsNil)
	to := swapi.Point{X: -15.9219, Y: 28.346}
	params := swapi.MakeParameters(swapi.MakePointParam(to))
	_, err = client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, params)
	c.Assert(err, IsNil)

	checkpointCompareAndStop(c, sim, client)
}

func (s *TestSuite) TestCheckpointUnit(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	party := client.Model.GetData().FindPartyByName("party1")
	c.Assert(party, NotNil)
	CreateFormation(c, client, party.Id)
	automat := createAutomatForParty(c, client, "party1")
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	unit, err := client.CreateUnitWithName(automat.Id, UnitType, from,
		"some unit name", false)
	c.Assert(err, IsNil)
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Give a mission
	heading := swapi.MakeHeading(0)
	dest := swapi.MakePointParam(swapi.Point{X: -15.8193, Y: 28.3456})
	params := swapi.MakeParameters(heading, nil, nil, nil, dest)
	_, err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)

	checkpointCompareAndStop(c, sim, client)
}

func (s *TestSuite) TestCheckpointAutomat(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallLog))
	defer stopSimAndClient(c, sim, client)
	// Need one which is a logistic base
	automat := client.Model.GetAutomat(14)
	c.Assert(automat, NotNil)
	err := client.SetManualMaintenance(automat.Id, true)
	c.Assert(err, IsNil)
	err = client.SetManualSupply(automat.Id, true)
	c.Assert(err, IsNil)
	checkpointCompareAndStop(c, sim, client)
}

func (s *TestSuite) TestCheckpointFormation(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallLog))
	defer stopSimAndClient(c, sim, client)
	// Need one which is a logistic base
	formation := client.Model.GetFormation(13)
	c.Assert(formation, NotNil)
	err := client.SetManualMaintenance(formation.Id, true)
	c.Assert(err, IsNil)
	err = client.SetManualSupply(formation.Id, true)
	c.Assert(err, IsNil)
	checkpointCompareAndStop(c, sim, client)
}

func (s *TestSuite) TestCheckpointLogConvoy(c *C) {
	opts := NewAdminOpts(ExCrossroadSmallLog)
	opts.Step = 300
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)

	// Find the supply base
	model := client.Model.GetData()
	supplyAutomat := getSomeAutomatByName(c, model, "LOG.Supply logistic area")
	c.Assert(supplyAutomat, NotNil)

	// Deploy it
	MissionLogDeploy := uint32(44584)
	heading := swapi.MakeHeading(0)
	limit1 := swapi.MakeLimit(
		swapi.Point{X: -15.8302, Y: 28.3765},
		swapi.Point{X: -15.825, Y: 28.3413})
	limit2 := swapi.MakeLimit(
		swapi.Point{X: -15.7983, Y: 28.3765},
		swapi.Point{X: -15.7991, Y: 28.3413})
	params := swapi.MakeParameters(heading, nil, limit1, limit2, nil)
	_, err := client.SendAutomatOrder(supplyAutomat.Id, MissionLogDeploy, params)
	c.Assert(err, IsNil)

	// Once scout ammunitions are depleted, a convoy should be generated, with
	// a pathfind, eventually.
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, u := range data.Units {
			if strings.Contains(u.Name, "LOG.Convoy") && u.PathPoints > 0 {
				return true
			}
		}
		return false
	})

	checkpointCompareAndStop(c, sim, client)
}

func (s *TestSuite) TestCheckpointPathfind(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallLog))
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()
	unit := getSomeUnit(c, data)
	c.Assert(unit, NotNil)
	positions := []swapi.Point{
		{X: -15.9248, Y: 28.2645},
		{X: -15.8429, Y: 28.3308},
		{X: -15.8640, Y: 28.2507},
		{X: -15.8946, Y: 28.3189},
	}
	_, err := client.CreatePathfind(unit.Id, positions...)
	c.Assert(err, IsNil)
	data = client.Model.GetData()
	c.Assert(data.Pathfinds, HasLen, 1)
	checkpointCompareAndStop(c, sim, client)
}

func (s *TestSuite) TestCheckpointTimeskips(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallLog))
	defer stopSimAndClient(c, sim, client)
	valid, err := swapi.GetTime("20200908T060504")
	c.Assert(err, IsNil)
	err = client.ChangeTime(valid)
	c.Assert(err, IsNil)
	checkpointCompareAndStop(c, sim, client)
}
