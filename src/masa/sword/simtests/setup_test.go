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
	"fmt"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"log"
	"masa/sword/swapi"
	"masa/sword/swapi/phy"
	"masa/sword/swapi/simu"
	"masa/sword/swrun"
	"masa/sword/swtest"
	"os"
	"path/filepath"
	"runtime"
	"testing"
)

var (
	Cfg *swtest.Config
)

func init() {
	Cfg = swtest.ParseFlags()
}

const (
	ExCrossroadLog            = "crossroad-log"
	ExCrossroadSmallEmpty     = "crossroad-small-empty"
	ExCrossroadSmallLog       = "crossroad-small-log"
	ExCrossroadSmallOrbat     = "crossroad-small-orbat"
	ExCrossroadSmallScores    = "crossroad-small-scores"
	ExCrossroadSmallTest      = "crossroad-small-test"
	ExGradXYTestEmpty         = "grad-x.y-test-empty"
	ExLandOfStripesEmpty      = "land-of-stripes-empty"
	ExParisEstKnowledgeGroups = "Paris_Est-knowledge-groups"
)

func startSimOnExercise(c *C, cfg *swrun.ClientOpts) *simu.SimProcess {
	opts, session := swrun.MakeOptsAndSession(Cfg)
	opts.ExerciseName = cfg.Exercise
	opts.PathfindDir = cfg.PathfindDir
	opts.PathfindFilter = cfg.PathfindFilter
	session.Paused = cfg.Paused
	if cfg.Step > 0 {
		session.TimeStep = cfg.Step
	}
	err := opts.WriteSession(session)
	c.Assert(err, IsNil)
	WriteSession(c, opts, session)
	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil)
	return sim
}

func startSimOnCheckpoint(c *C, exercise, session, checkpoint string, endTick int,
	paused bool) *simu.SimProcess {

	opts := swrun.MakeOpts(Cfg)
	opts.ExerciseName = exercise
	opts.SessionName = session
	opts.CheckpointName = checkpoint

	s, err := simu.ReadSessionFile(opts.GetSessionFile())
	c.Assert(err, IsNil)
	s.EndTick = endTick
	s.Paused = paused
	err = simu.WriteSessionFile(s, opts.GetSessionFile())
	c.Assert(err, IsNil)

	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil) // failed to start the simulation
	return sim
}

func MakeReplayOpts() *simu.ReplayOpts {
	opts := simu.ReplayOpts{}
	projectRoot := ""
	if cwd, err := os.Getwd(); err == nil {
		projectRoot, _ = filepath.Abs(filepath.Join(cwd, "../../../.."))
	}

	if len(Cfg.Application) > 0 {
		// Assume replayer_app lives along with the simulation
		opts.Executable = filepath.Join(filepath.Dir(Cfg.Application), "replayer_app.exe")
	} else if len(projectRoot) > 0 {
		opts.Executable = filepath.Join(
			projectRoot, "out", Cfg.Platform, "release/replayer_app.exe")
		runDir := filepath.Join(projectRoot, "run", Cfg.Platform)
		opts.RunDir = &runDir
	}
	if len(Cfg.RootDir) > 0 {
		opts.RootDir = Cfg.RootDir
	} else if len(projectRoot) > 0 {
		opts.RootDir = filepath.Join(projectRoot, "data")
		opts.ExercisesDir = filepath.Join(projectRoot, "data/tests/gosword")
	}
	if len(Cfg.RunDir) > 0 {
		opts.RunDir = &Cfg.RunDir
	}
	if len(Cfg.ExercisesDir) > 0 {
		opts.ExercisesDir = Cfg.ExercisesDir
	}
	opts.ExerciseName = "crossroad-small-empty"
	opts.ReplayerAddr = fmt.Sprintf("localhost:%d", Cfg.TestPort+5)
	opts.ConnectTimeout = Cfg.Timeout
	return &opts
}

func startReplay(c *C, simOpts *simu.SimOpts) *simu.ReplayProcess {
	opts := MakeReplayOpts()
	opts.ExerciseName = simOpts.ExerciseName
	opts.SessionName = simOpts.SessionName

	replay, err := simu.StartReplay(opts)
	c.Assert(err, IsNil)
	return replay
}

func checkOrderAckSequences(c *C, client *swapi.Client) {
	client.Model.RegisterHandlerTimeout(0,
		func(model *swapi.ModelData, msg *swapi.SwordMessage, err error) bool {

			if err != nil {
				return true
			}
			if msg.SimulationToClient == nil {
				return false
			}
			m := msg.SimulationToClient.GetMessage()
			id := uint32(0)
			switch {
			case m == nil:
			case m.OrderAck != nil:
				id = m.OrderAck.GetId()
			case m.FragOrderAck != nil:
				id = m.FragOrderAck.GetId()
			}
			if id != 0 {
				_, ok := model.Orders[id]
				c.Check(ok, Equals, false) // the order arrives before the ack!
			}
			return false
		})
}

func StartSimOnExercise(c *C, clientOpts *swrun.ClientOpts) *simu.SimProcess {
	sim, err := swrun.StartSimOnExercise(clientOpts, Cfg)
	c.Assert(err, IsNil)
	return sim
}

func NewAllUserOpts(exercise string) *swrun.ClientOpts {
	return &swrun.ClientOpts{
		Exercise: exercise,
		User:     "alluser",
		Password: "alluser",
	}
}

func NewAdminOpts(exercise string) *swrun.ClientOpts {
	return &swrun.ClientOpts{
		Exercise: exercise,
		User:     "admin",
		Password: "",
	}
}

type Simulator interface {
	GetClientAddr() string
}

func connectClient(c *C, sim Simulator, opts *swrun.ClientOpts) *swapi.Client {
	client, err := swrun.ConnectClient(sim, opts, Cfg)
	c.Assert(err, IsNil)
	client.Model.SetErrorHandler(func(data *swapi.ModelData, msg *swapi.SwordMessage,
		err error) error {
		if !c.Failed() {
			c.Check(err, IsNil)
		}
		return nil
	})
	checkOrderAckSequences(c, client)
	return client
}

func loginAndWaitModel(c *C, sim Simulator, opts *swrun.ClientOpts) *swapi.Client {
	client, err := swrun.LoginAndWaitModel(sim, opts, Cfg)
	c.Assert(err, IsNil)
	return client
}

func connectAndWaitModel(c *C, opts *swrun.ClientOpts) (
	*simu.SimProcess, *swapi.Client) {

	sim, client, err := swrun.ConnectAndWaitModel(opts, Cfg)
	c.Assert(err, IsNil)
	return sim, client
}

func waitCondition(c *C, model *swapi.Model, cond func(data *swapi.ModelData) bool) {
	ok := model.WaitCondition(cond)
	c.Assert(ok, Equals, true)
}

const (
	// Random existing automat identifier, we should parse the physical
	// database instead.
	AutomatType = uint32(123)
	// ARMOR.MBT platoon
	UnitType = uint32(61)
	// Standard Crowd
	CrowdType = "Standard Crowd"
)

func createSpecificAutomat(c *C, client *swapi.Client, partyName string, automatType uint32) *swapi.Automat {
	data := client.Model.GetData()

	party := data.FindPartyByName(partyName)
	c.Assert(party, NotNil)

	formations := getPartyFormations(data, party.Id)
	c.Assert(len(formations), Greater, 0)
	formation := data.Formations[uint32(formations[0])]
	// Find a suitable knowledge group matching the formation, this should be
	// simpler...
	var kg *swapi.KnowledgeGroup
	for _, g := range data.KnowledgeGroups {
		if g.PartyId == formation.PartyId {
			kg = g
			break
		}
	}
	c.Assert(kg, NotNil)

	automat, err := client.CreateAutomat(formation.Id, automatType, kg.Id)
	c.Assert(err, IsNil)
	c.Assert(automat, NotNil)
	return automat
}

func createAutomatForParty(c *C, client *swapi.Client, partyName string) *swapi.Automat {
	return createSpecificAutomat(c, client, partyName, AutomatType)
}

func createAutomat(c *C, client *swapi.Client) *swapi.Automat {
	return createAutomatForParty(c, client, "party")
}

func readFileAsString(c *C, path string) string {
	data, err := ioutil.ReadFile(path)
	c.Assert(err, IsNil)
	return string(data)
}

func stopSim(c *C, sim *simu.SimProcess, opts *simu.SessionErrorsOpts) {
	err := swrun.StopSim(sim, opts)
	if err != nil {
		c.Fatal(err)
	}
}

func stopSimAndClient(c *C, sim *simu.SimProcess, client *swapi.Client) {
	err := swrun.StopSimAndClient(sim, client)
	if err != nil {
		c.Fatal(err)
	}
}

func loadPhysical(c *C, name string) *phy.PhysicalFile {
	wd, err := os.Getwd()
	c.Assert(err, IsNil)
	path := filepath.Join(wd, "../../../../data/data/models/ada/physical", name)
	phydb, err := phy.ReadPhysical(path)
	c.Assert(err, IsNil)
	return phydb
}

func loadPhysicalData(c *C, name string) *phy.PhysicalData {
	wd, err := os.Getwd()
	c.Assert(err, IsNil)
	path := filepath.Join(wd, "../../../../data/data/models/ada/physical", name)
	data, err := phy.ReadPhysicalData(path)
	c.Assert(err, IsNil)
	return data
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func (t *TestSuite) SetUpSuite(c *C) {
	log.Println("config: application", Cfg.Application)
	log.Println("config: root-dir", Cfg.RootDir)
	log.Println("config: run-dir", Cfg.RunDir)
	log.Println("config: test-port", Cfg.TestPort)
	log.Println("config: platform", Cfg.Platform)
	log.Println("config: cpu", runtime.GOMAXPROCS(0))
}
