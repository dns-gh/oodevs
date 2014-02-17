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
	"code.google.com/p/goprotobuf/proto"
	"fmt"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"log"
	"os"
	"path/filepath"
	"swapi"
	"swapi/phy"
	"swapi/simu"
	"swtest"
	"testing"
	"time"
)

const (
	ConnectTimeout = 40 * time.Second
	PostTimeout    = 40 * time.Second
	WaitTimeout    = 60 * time.Second
)

var (
	Cfg *swtest.Config
)

func init() {
	Cfg = swtest.ParseFlags()
}

const (
	ExCrossroadLog         = "crossroad-log"
	ExCrossroadSmallEmpty  = "crossroad-small-empty"
	ExCrossroadSmallLog    = "crossroad-small-log"
	ExCrossroadSmallOrbat  = "crossroad-small-orbat"
	ExCrossroadSmallScores = "crossroad-small-scores"
	ExCrossroadSmallTest   = "crossroad-small-test"
	ExGradXYTestEmpty      = "grad-x.y-test-empty"
	ExLandOfStripesEmpty   = "land-of-stripes-empty"
)

func MakeOpts() *simu.SimOpts {
	opts := simu.SimOpts{}
	projectRoot := ""
	if cwd, err := os.Getwd(); err == nil {
		projectRoot, _ = filepath.Abs(filepath.Join(cwd, "..", ".."))
	}

	if len(Cfg.Application) > 0 {
		opts.Executable = Cfg.Application
	} else if len(projectRoot) > 0 {
		opts.Executable = filepath.Join(projectRoot, "run", Cfg.Platform, "simulation_app.exe")
	}
	if len(Cfg.RootDir) > 0 {
		opts.RootDir = Cfg.RootDir
	} else if len(projectRoot) > 0 {
		opts.RootDir = filepath.Join(projectRoot, "data")
	}
	opts.DataDir = opts.RootDir
	if len(Cfg.RunDir) > 0 {
		opts.RunDir = &Cfg.RunDir
	}
	opts.ExerciseName = "crossroad-small-empty"
	opts.DispatcherAddr = fmt.Sprintf("localhost:%d", Cfg.TestPort+5)
	opts.SimulationAddr = fmt.Sprintf("localhost:%d", Cfg.TestPort+6)
	opts.EnableTailing = Cfg.ShowLog
	opts.ConnectTimeout = ConnectTimeout
	return &opts
}

func makeOptsAndSession() (*simu.SimOpts, *simu.Session) {
	opts := MakeOpts()
	session := simu.CreateDefaultSession()
	session.GamingServer = opts.DispatcherAddr
	return opts, session
}

func startSimOnExercise(c *C, cfg *ClientOpts) *simu.SimProcess {
	opts, session := makeOptsAndSession()
	opts.ExerciseName = cfg.Exercise
	session.Paused = cfg.Paused
	if cfg.Step > 0 {
		session.TimeStep = cfg.Step
	}
	WriteSession(c, opts, session)
	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil) // failed to start the simulation
	return sim
}

func startSimOnCheckpoint(c *C, exercise, session, checkpoint string, endTick int,
	paused bool) *simu.SimProcess {

	opts := MakeOpts()
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
		projectRoot, _ = filepath.Abs(filepath.Join(cwd, "..", ".."))
	}

	if len(Cfg.Application) > 0 {
		// Assume replayer_app lives along with the simulation
		opts.Executable = filepath.Join(filepath.Dir(Cfg.Application), "replayer_app.exe")
	} else if len(projectRoot) > 0 {
		opts.Executable = filepath.Join(projectRoot, "run", Cfg.Platform, "replayer_app.exe")
	}
	if len(Cfg.RootDir) > 0 {
		opts.RootDir = Cfg.RootDir
	} else if len(projectRoot) > 0 {
		opts.RootDir = filepath.Join(projectRoot, "data")
	}
	if len(Cfg.RunDir) > 0 {
		opts.RunDir = &Cfg.RunDir
	}
	opts.ExerciseName = "crossroad-small-empty"
	opts.ReplayerAddr = fmt.Sprintf("localhost:%d", Cfg.TestPort+5)
	opts.ConnectTimeout = ConnectTimeout
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

type ClientOpts struct {
	Exercise    string
	User        string
	Password    string
	Step        int
	Paused      bool
	Logger      swapi.MessageLogger
	WaitTimeout time.Duration
	StartGaming bool
}

func NewAllUserOpts(exercise string) *ClientOpts {
	return &ClientOpts{
		Exercise: exercise,
		User:     "alluser",
		Password: "alluser",
	}
}

func NewAdminOpts(exercise string) *ClientOpts {
	return &ClientOpts{
		Exercise: exercise,
		User:     "admin",
		Password: "",
	}
}

type Simulator interface {
	GetClientAddr() string
}

func connectClient(c *C, sim Simulator, opts *ClientOpts) *swapi.Client {
	if opts == nil {
		opts = &ClientOpts{}
	}
	client, err := swapi.NewClient(sim.GetClientAddr())
	c.Assert(err, IsNil)
	client.Logger = opts.Logger
	client.PostTimeout = PostTimeout
	client.Model.WaitTimeout = WaitTimeout
	if opts.WaitTimeout != 0 {
		timeout := opts.WaitTimeout
		if timeout < 0 {
			timeout = 0
		}
		client.Model.WaitTimeout = timeout
	}
	swapi.ConnectClient(client)
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

func AddLogger(opts *ClientOpts) *ClientOpts {
	opts.Logger = func(in bool, size int, msg *swapi.SwordMessage) {
		prefix := "in "
		if !in {
			prefix = "out"
		}
		fmt.Fprintf(os.Stderr, "%s %v\n", prefix,
			proto.CompactTextString(msg.GetMessage()))
	}
	return opts
}

// Start a gaming_app after starting the simulation and before connecting the
// test client. First the simulation will be paused (if not already), then
// gaming will be started and call will wait for the simulation to be unpaused
// before moving on.
func AddGaming(opts *ClientOpts) *ClientOpts {
	opts.StartGaming = true
	return opts
}

func loginAndWaitModel(c *C, sim Simulator, opts *ClientOpts) *swapi.Client {
	client := connectClient(c, sim, opts)
	err := client.Login(opts.User, opts.Password)
	c.Assert(err, IsNil)
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true)
	return client
}

func startGaming(c *C, sim *simu.SimProcess) {
	admin := connectClient(c, sim, &ClientOpts{WaitTimeout: -1})
	defer admin.Close()
	key, err := admin.GetAuthenticationKey()
	c.Assert(err, IsNil)
	err = admin.LoginWithAuthenticationKey("", "", key)
	c.Assert(err, IsNil)
	err = admin.Pause()
	c.Assert(err, IsNil)
	_, err = simu.StartGamingFromSim(sim.Opts)
	c.Assert(err, IsNil)
	admin.Model.WaitTicks(1)
}

func connectAndWaitModel(c *C, opts *ClientOpts) (
	*simu.SimProcess, *swapi.Client) {
	sim := startSimOnExercise(c, opts)
	ok := false
	defer func() {
		if !ok {
			sim.Stop()
		}
	}()
	if opts.StartGaming {
		startGaming(c, sim)
	}
	client := loginAndWaitModel(c, sim, opts)
	ok = true
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
	sim.Stop()
	session := sim.Opts.GetSessionDir()
	err := simu.CheckSessionErrors(session, opts)
	if err != nil {
		c.Fatalf("errors found in session %s:\n%s", session, err)
	}
}

func stopSimAndClient(c *C, sim *simu.SimProcess, client *swapi.Client) {
	client.Close()
	stopSim(c, sim, nil)
}

func loadWWPhysical(c *C) *phy.PhysicalFile {
	wd, err := os.Getwd()
	c.Assert(err, IsNil)
	path := filepath.Join(wd, "../../data/data/models/ada/physical/worldwide")
	phydb, err := phy.ReadPhysical(path)
	c.Assert(err, IsNil)
	return phydb
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func (t *TestSuite) SetUpSuite(c *C) {
	log.Println("application", Cfg.Application)
	log.Println("root-dir", Cfg.RootDir)
	log.Println("run-dir", Cfg.RunDir)
	log.Println("test-port", Cfg.TestPort)
	log.Println("platform", Cfg.Platform)
}
