package simtests

import (
	"flag"
	"fmt"
	"io/ioutil"
	"regexp"
	"strings"
	"swapi/simu"
	"testing"
	"time"
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

func CheckSimFailed(t *testing.T, err error, sim *simu.SimProcess) {
	if err == nil {
		t.Fatalf("simulation should not have started")
	}
	if !strings.Contains(err.Error(), "failed to start simulation") {
		t.Fatalf("unexpected failure: %v", err)
	}

	/* Probably SWORD-1549

	if sim.Success() {
		t.Fatal("simulation should have exited on error")
	}
	*/
}

func ReadTextFile(t *testing.T, path string) string {
	data, err := ioutil.ReadFile(path)
	if err != nil {
		t.Fatalf("failed to read %v: %v", path, err)
	}
	return string(data)
}

func MakeOpts() *simu.SimOpts {
	opts := simu.SimOpts{}
	opts.Executable = application
	opts.RootDir = rootdir
	opts.DataDir = rootdir
	if len(rundir) > 0 {
		opts.RunDir = &rundir
	}
	opts.ExerciseName = "tests/crossroad-empty"
	opts.DispatcherAddr = fmt.Sprintf("localhost:%d", testPort+5)
	opts.SimulationAddr = fmt.Sprintf("localhost:%d", testPort+6)
	return &opts
}

func WriteSession(t *testing.T, opts *simu.SimOpts, session *simu.Session) {
	if err := opts.WriteSession(session); err != nil {
		t.Fatal("failed to write the session")
	}
}

func TestDispatcherMisconfiguration(t *testing.T) {
	session := simu.CreateDefaultSession()
	session.EndTick = 3

	opts := MakeOpts()
	opts.ConnectTimeout = 600 * time.Second
	WriteSession(t, opts, session)

	// Remove dispatcher/network element
	re, err := regexp.Compile(`<network client.*?</network>`)
	if err != nil {
		t.Fatalf("failed to compile regular expression: %v", err)
	}
	sessionPath := opts.GetSessionFile()
	content := re.ReplaceAllString(ReadTextFile(t, sessionPath), "")
	if err = ioutil.WriteFile(sessionPath, []byte(content), 0644); err != nil {
		t.Fatalf("failed to rewrite session file: %v", err)
	}

	sim, err := simu.StartSim(opts)
	defer sim.Kill()
	CheckSimFailed(t, err, sim)

	// Sim.log should say something about the dispatcher and dispatcher.log
	simLog := ReadTextFile(t, opts.GetSimLogPath())
	expected := "<functERR> dispatcher: failed to create dispatcher"
	if !strings.Contains(simLog, expected) {
		t.Fatal("cannot find dispatcher error in sim.log")
	}

	// Dispatcher.log should mention the missing XML element
	// [2013-03-04 10:46:24] <Dispatcher> <functERR> session.xml (line 4, column 17)
	//   : Node 'dispatcher' does not have a child named 'network'
	expected = "Node 'dispatcher' does not have a child named 'network'"
	dispatcherLog := ReadTextFile(t, opts.GetDispatcherLogPath())
	if !strings.Contains(dispatcherLog, expected) {
		t.Fatal("cannot find session.xml error in dispatcher.log")
	}
}

func TestDispatcherAddressCollision(t *testing.T) {
	startSim := func(simOffset int) (*simu.SimProcess, error) {
		session := simu.CreateDefaultSession()

		opts := MakeOpts()
		WriteSession(t, opts, session)
		opts.SimulationAddr = fmt.Sprintf("localhost:%d", testPort+simOffset+6)
		opts.TailPrefix = fmt.Sprintf("sim+%v", simOffset)
		return simu.StartSim(opts)
	}

	sim1, err := startSim(0)
	defer sim1.Kill()
	if err != nil {
		t.Fatalf("simulation failed to start: %v", err)
	}

	sim2, err := startSim(1)
	defer sim2.Kill()
	sim2.Wait(60 * time.Second)
	// This should be fixed by SWORD-1549
	//
	//if sim2.Success() {
	//	    t.Fatal("simulation with colliding dispatcher should have failed")
	//    }
	logData := ReadTextFile(t, sim2.Opts.GetDispatcherLogPath())
	if !strings.Contains(logData, "Une seule utilisation de chaque") {
		t.Fatal("dispatcher.log says nothing about address collision")
	}
}

// Return true if any line of sim.log or dispatcher.log is eventually matched
// by "matcher". False if the simulation ends or times out before it happens.
// "matcher" must return true on successful matches.
func waitForMatchingLog(t *testing.T, sim *simu.SimProcess,
	timeout time.Duration, matcher simu.TailHandler) bool {

	quitch := make(chan int)
	tailch := make(chan int, 1)
	timeoutch := make(chan int, 1)

	files := []string{
		sim.Opts.GetSimLogPath(),
		sim.Opts.GetDispatcherLogPath(),
	}
	go func() {
		done := false
		simu.TailFiles(files, quitch, func(line string) bool {
			done = done || matcher(line)
			return done
		})
		tailch <- 0
	}()
	go func() {
		ok := 0
		if !sim.Wait(timeout) {
			ok = 1
		}
		timeoutch <- ok
	}()
	select {
	case <-tailch:
		return true
	case ok := <-timeoutch:
		if ok != 0 {
			t.Fatal("waitForMatchingLog timed out")
		}
		break
	}
	return false
}

// Test corner cases of session end-tick parameter
func TestLowEndTickValues(t *testing.T) {

	// Start a simulation and fail if the simulation failed to start or if
	// it reaches endTick + 1.
	runSim := func(t *testing.T, endTick int) bool {
		session := simu.CreateDefaultSession()
		session.EndTick = endTick
		opts := MakeOpts()
		WriteSession(t, opts, session)
		sim, err := simu.StartSim(opts)
		if sim == nil {
			t.Fatalf("simulation failed to start %v", err)
		}
		defer sim.Kill()

		pattern := fmt.Sprintf(
			"<Simulation> <info> **** Time tick %d", endTick+1)
		return waitForMatchingLog(t, sim, 180*time.Second, func(line string) bool {
			return strings.Contains(line, pattern)
		})
	}
	testLowTick := func(t *testing.T, endTick int) {
		ok := runSim(t, endTick)
		if ok {
			// SWBUG-10020:
			//
			// t.Fatalf("simulation ticked up to %d with end-tick=%d",
			//    endTick + 1, endTick)
		}
	}
	// Simulation should tick only once
	testLowTick(t, 1)
	// Simulation should tick only twice
	testLowTick(t, 2)

	// end-tick=0 means infinity, check it ticks once
	if !runSim(t, 0) {
		t.Fatal("simulation did not tick with end-tick=0")
	}
}
