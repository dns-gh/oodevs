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

const ExCrossroadSmallOrbat = "crossroad-small-orbat"
const ExCrossroadSmallEmpty = "crossroad-small-empty"
const ExCrossroadSmallLog = "crossroad-small-log"
const ExCrossroadSmallScores = "crossroad-small-scores"
const ExGradXYTestEmpty = "grad-x.y-test-empty"
const ExLandOfStripesEmpty = "land-of-stripes-empty"

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

func startSimOnExercise(c *C, exercise string, endTick int, paused bool,
	step int) *simu.SimProcess {

	opts, session := makeOptsAndSession()
	opts.ExerciseName = exercise

	session.Paused = paused
	if step > 0 {
		session.TimeStep = step
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
	s.Paused = paused
	err = simu.WriteSessionFile(s, opts.GetSessionFile())
	c.Assert(err, IsNil)

	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil) // failed to start the simulation
	return sim
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

func connectClient(c *C, sim *simu.SimProcess) *swapi.Client {
	client, err := swapi.Connect(sim.DispatcherAddr)
	c.Assert(err, IsNil) // failed to connect to simulation
	client.PostTimeout = PostTimeout
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

func loginAndWaitModel(c *C, sim *simu.SimProcess, user, password string) *swapi.Client {

	client := connectClient(c, sim)
	err := client.Login(user, password)
	c.Assert(err, IsNil) // login failed
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // model initialization timed out
	return client
}

func connectAndWaitModelWithStep(c *C, user, password, exercise string, step int) (
	*simu.SimProcess, *swapi.Client) {

	sim := startSimOnExercise(c, exercise, 1000, false, step)
	client := loginAndWaitModel(c, sim, user, password)
	return sim, client
}

func connectAndWaitModel(c *C, user, password, exercise string) (
	*simu.SimProcess, *swapi.Client) {

	sim := startSimOnExercise(c, exercise, 1000, false, 0)
	client := loginAndWaitModel(c, sim, user, password)
	return sim, client
}

func connectAllUserAndWait(c *C, exercise string) (*simu.SimProcess, *swapi.Client) {
	return connectAndWaitModel(c, "alluser", "alluser", exercise)
}

func addClientLogger(client *swapi.Client) {
	handler := func(msg *swapi.SwordMessage, context int32, err error) bool {
		if err != nil {
			return true
		}
		log.Println(msg)
		return false
	}
	client.Register(handler)
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

func createAutomatForParty(c *C, client *swapi.Client, partyName string) *swapi.Automat {
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

	automat, err := client.CreateAutomat(formation.Id, AutomatType, kg.Id)
	c.Assert(err, IsNil)
	c.Assert(automat, NotNil)
	return automat
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
