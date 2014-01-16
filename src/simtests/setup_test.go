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
	"flag"
	"fmt"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"log"
	"os"
	"path/filepath"
	"runtime"
	"swapi"
	"swapi/simu"
	"testing"
	"time"
)

const (
	ConnectTimeout = 40 * time.Second
	PostTimeout    = 40 * time.Second
)

var (
	application string
	rootdir     string
	rundir      string
	testPort    int
	showLog     bool
	platform    string
)

func init() {
	flag.StringVar(&application, "application", "",
		"path to simulation_app executable")
	flag.StringVar(&rootdir, "root-dir", "",
		"path to simulation root directory")
	flag.StringVar(&rundir, "run-dir", "",
		"path application run directory, default to application directory")
	flag.IntVar(&testPort, "test-port", 35000,
		"base port for spawned simulations")
	flag.BoolVar(&showLog, "show-log", false, "print simulation log files")

	platform = "vc100_x64"
	if runtime.GOARCH == "386" {
		platform = "vc100"
	}
}

const (
	ExCrossroadSmallOrbat  = "crossroad-small-orbat"
	ExCrossroadSmallEmpty  = "crossroad-small-empty"
	ExCrossroadSmallTest   = "crossroad-small-test"
	ExCrossroadSmallLog    = "crossroad-small-log"
	ExCrossroadSmallScores = "crossroad-small-scores"
	ExGradXYTestEmpty      = "grad-x.y-test-empty"
	ExLandOfStripesEmpty   = "land-of-stripes-empty"
)

func MakeOpts() *simu.SimOpts {
	opts := simu.SimOpts{}
	projectRoot := ""
	if cwd, err := os.Getwd(); err == nil {
		projectRoot, _ = filepath.Abs(filepath.Join(cwd, "..", ".."))
	}

	if len(application) > 0 {
		opts.Executable = application
	} else if len(projectRoot) > 0 {
		opts.Executable = filepath.Join(projectRoot, "run", platform, "simulation_app.exe")
	}
	if len(rootdir) > 0 {
		opts.RootDir = rootdir
	} else if len(projectRoot) > 0 {
		opts.RootDir = filepath.Join(projectRoot, "data")
	}
	opts.DataDir = opts.RootDir
	if len(rundir) > 0 {
		opts.RunDir = &rundir
	}
	opts.ExerciseName = "crossroad-small-empty"
	opts.DispatcherAddr = fmt.Sprintf("localhost:%d", testPort+5)
	opts.SimulationAddr = fmt.Sprintf("localhost:%d", testPort+6)
	opts.EnableTailing = showLog
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

	if len(application) > 0 {
		// Assume replayer_app lives along with the simulation
		opts.Executable = filepath.Join(filepath.Dir(application), "replayer_app.exe")
	} else if len(projectRoot) > 0 {
		opts.Executable = filepath.Join(projectRoot, "run", platform, "replayer_app.exe")
	}
	if len(rootdir) > 0 {
		opts.RootDir = rootdir
	} else if len(projectRoot) > 0 {
		opts.RootDir = filepath.Join(projectRoot, "data")
	}
	if len(rundir) > 0 {
		opts.RunDir = &rundir
	}
	opts.ExerciseName = "crossroad-small-empty"
	opts.ReplayerAddr = fmt.Sprintf("localhost:%d", testPort+5)
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
	Exercise string
	User     string
	Password string
	Step     int
	Paused   bool
	Logger   swapi.MessageLogger
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

func loginAndWaitModel(c *C, sim Simulator, opts *ClientOpts) *swapi.Client {
	client := connectClient(c, sim, opts)
	err := client.Login(opts.User, opts.Password)
	c.Assert(err, IsNil)
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true)
	return client
}

func connectAndWaitModel(c *C, opts *ClientOpts) (
	*simu.SimProcess, *swapi.Client) {
	sim := startSimOnExercise(c, opts)
	client := loginAndWaitModel(c, sim, opts)
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

func getSomeParty(c *C, model *swapi.ModelData) *swapi.Party {
	for _, party := range model.Parties {
		return party
	}
	c.Fatal("no party found")
	return nil
}

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

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func (t *TestSuite) SetUpSuite(c *C) {
	log.Println("application", application)
	log.Println("root-dir", rootdir)
	log.Println("run-dir", rundir)
	log.Println("test-port", testPort)
	log.Println("platform", platform)
}
