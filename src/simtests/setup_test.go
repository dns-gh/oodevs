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

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})
