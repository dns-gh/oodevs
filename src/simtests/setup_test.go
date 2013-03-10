package simtests

import (
	"flag"
	"fmt"
	. "launchpad.net/gocheck"
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

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})
