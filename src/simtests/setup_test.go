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
)

var (
	projectRoot string
	application string
	rootdir     string
	rundir      string
	testPort    int
	showLog     bool
	platform    string
)

func init() {
	flag.StringVar(&projectRoot, "projectRoot", os.Getenv("GOSWORD_ROOT"),
		"path to gosword project root directory")
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

func MakeOpts() *simu.SimOpts {
	opts := simu.SimOpts{}
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
	return &opts
}

func startSimOnExercise(c *C, exercise string, endTick int,
	paused bool) *simu.SimProcess {

	opts := MakeOpts()
	opts.ExerciseName = exercise
	opts.ConnectTimeout = ConnectTimeout

	session := simu.CreateDefaultSession()
	session.Paused = paused
	session.GamingServer = opts.DispatcherAddr
	WriteSession(c, opts, session)
	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil) // failed to start the simulation
	return sim
}

func startSimOnCheckpoint(c *C, exercise, session, checkpoint string, endTick int,
	paused bool) *simu.SimProcess {

	opts := MakeOpts()
	opts.ExerciseName = exercise
	opts.ConnectTimeout = ConnectTimeout
	opts.SessionName = session
	opts.CheckpointName = checkpoint

	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil) // failed to start the simulation
	return sim
}

func connectClient(c *C, sim *simu.SimProcess) *swapi.Client {
	client, err := swapi.Connect(sim.DispatcherAddr)
	client.PostTimeout = 10 * time.Second
	client.Model.SetErrorHandler(func(data *swapi.ModelData, msg *swapi.SwordMessage,
		err error) error {
		if !c.Failed() {
			c.Check(err, IsNil)
		}
		return nil
	})
	c.Assert(err, IsNil) // failed to connect to simulation
	return client
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

	c.Assert(len(party.Formations), Greater, 0)
	var formation *swapi.Formation
	for _, f := range party.Formations {
		formation = f
		break
	}
	// Find a suitable knowledge group matching the formation, this should be
	// simpler...
	var kg *swapi.KnowledgeGroup
	for _, g := range data.ListKnowledgeGroups() {
		if g.PartyId == formation.PartyId {
			kg = g
			break
		}
	}
	c.Assert(kg, NotNil)

	automat, err := client.CreateAutomat(formation.Id, 0, AutomatType, kg.Id)
	c.Assert(err, IsNil)
	c.Assert(automat, NotNil)
	return automat
}

func createAutomat(c *C, client *swapi.Client) *swapi.Automat {
	return createAutomatForParty(c, client, "party")
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func (t *TestSuite) SetUpSuite(c *C) {
	log.Println("projectRoot", projectRoot)
	log.Println("application", application)
	log.Println("rootdir", rootdir)
	log.Println("rundir", rundir)
	log.Println("testPort", testPort)
	log.Println("platform", platform)
}
