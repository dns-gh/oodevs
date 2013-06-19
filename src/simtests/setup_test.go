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
	"swapi"
	"swapi/simu"
	"testing"
)

var (
	application string
	rootdir     string
	rundir      string
	testPort    int
	legacy      bool
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
	flag.BoolVar(&legacy, "legacy", false, "run in legacy mode")
}

const ExCrossroadSmallOrbat = "crossroad-small-orbat"
const ExCrossroadSmallEmpty = "crossroad-small-empty"

func MakeOpts() *simu.SimOpts {
	opts := simu.SimOpts{}
	opts.Executable = application
	opts.RootDir = rootdir
	opts.DataDir = rootdir
	if len(rundir) > 0 {
		opts.RunDir = &rundir
	}
	opts.ExerciseName = "crossroad-small-empty"
	opts.DispatcherAddr = fmt.Sprintf("localhost:%d", testPort+5)
	opts.SimulationAddr = fmt.Sprintf("localhost:%d", testPort+6)
	opts.Legacy = legacy
	return &opts
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

func createAutomat(c *C, client *swapi.Client) *swapi.Automat {
	data := client.Model.GetData()

	party := data.FindPartyByName("party")
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

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func (t *TestSuite) SetUpSuite(c *C) {
	log.Println("application", application)
	log.Println("rootdir", rootdir)
	log.Println("rundir", rundir)
	log.Println("testPort", testPort)
	log.Println("legacy", legacy)
}
