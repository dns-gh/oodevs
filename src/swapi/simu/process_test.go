package simu

import (
	"flag"
	"fmt"
	"io/ioutil"
	"path/filepath"
	"regexp"
	"strings"
	"testing"
	"time"
)

func TestSimOpts(t *testing.T) {
	opts := SimOpts{}
	opts.RootDir = "data/dir"
	opts.ExerciseName = "exercisename"

	d := opts.GetExerciseDir()
	if filepath.ToSlash(d) != "data/dir/exercises/exercisename" {
		t.Fatalf("invalid exercise dir: %v", d)
	}
}

// Test the simulation can be started successfully and ticks a bit.
func TestSuccess(t *testing.T) {
	opts := SimOpts{}
	opts.Executable = application
	opts.RootDir = rootdir
	opts.DataDir = rootdir
	opts.ExerciseName = "worldwide/Egypt"
	opts.DispatcherAddr = fmt.Sprintf("localhost:%d", testPort)
	opts.SimulationAddr = fmt.Sprintf("localhost:%d", testPort+1)

	exDir := opts.GetExerciseDir()
	session := CreateDefaultSession()
	session.EndTick = 3
	session.Paused = false
	sessionPath, err := WriteNewSessionFile(session, exDir)
	if err != nil {
		t.Fatal("failed to write the session")
	}
	opts.SessionName = filepath.Base(filepath.Dir(sessionPath))
	sim, err := StartSim(&opts)
	defer sim.Kill()
	if err != nil {
		t.Fatalf("simulation failed to start: %v", err)
	}
	sim.Wait(60 * time.Second)
	if !sim.Success() {
		t.Fatal("simulation should have terminated successfully")
	}
}

// Test SimProcess fails fast when started with an invalid configuration and
// taking enough time to detect it that exec module does not pick it.
// See the funcErr in Sim.log
func TestDelayedStartupFailure(t *testing.T) {
	opts := SimOpts{}
	opts.Executable = application
	opts.RootDir = rootdir
	opts.DataDir = rootdir
	opts.ExerciseName = "worldwide/Egypt"
	opts.ConnectTimeout = 20 * time.Second
	opts.DispatcherAddr = fmt.Sprintf("localhost:%d", testPort)
	opts.SimulationAddr = fmt.Sprintf("localhost:%d", testPort+1)

	exDir := opts.GetExerciseDir()
	session := CreateDefaultSession()
	session.EndTick = 3
	session.Paused = false
	sessionPath, err := WriteNewSessionFile(session, exDir)
	if err != nil {
		t.Fatal("failed to write the session")
	}

	// Remove dispatcher/network element
	data, err := ioutil.ReadFile(sessionPath)
	re, err := regexp.Compile(`<network client.*?</network>`)
	if err != nil {
		t.Fatalf("failed to compile regular expression: %v", err)
	}
	content := re.ReplaceAllString(string(data), "")
	if err = ioutil.WriteFile(sessionPath, []byte(content), 0644); err != nil {
		t.Fatalf("failed to rewrite session file: %v", err)
	}

	opts.SessionName = filepath.Base(filepath.Dir(sessionPath))
	sim, err := StartSim(&opts)
	defer sim.Kill()
	if err == nil {
		t.Fatalf("simulation should not have started")
	}
	if !strings.Contains(err.Error(), "failed to start simulation") {
		t.Fatalf("unexpected failure: %v", err)
	}
	if sim.Success() {
		t.Fatal("simulation should have exited on error")
	}
}

var application string
var rootdir string
var testPort int

func init() {
	flag.StringVar(&application, "application", "",
		"path to simulation_app executable")
	flag.StringVar(&rootdir, "root-dir", "",
		"path to simulation root directory")
	flag.IntVar(&testPort, "test-port", 35000,
		"base port for spawned simulations")
}
