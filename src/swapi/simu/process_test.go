package simu

import (
	"flag"
	"path/filepath"
	"strings"
	"testing"
	"time"
)

func TestSimOpts(t *testing.T) {
	opts := SimOpts{}
	opts.DataDir = "data/dir"
	opts.ExerciseName = "exercisename"

	d := opts.GetExerciseDir()
	if filepath.ToSlash(d) != "data/dir/exercises/exercisename" {
		t.Fatalf("invalid exercise dir: %v", d)
	}
}

// Test SimProcess fails fast when started with an invalid configuration and
// taking enough time to detect it that exec module does not pick it.
// See the funcErr in Sim.log
func TestDelayedStartupFailure(t *testing.T) {
	opts := SimOpts{}
	opts.Executable = application
	opts.RootDir = rootdir
	// Invalid data directory
	opts.DataDir = rootdir
	opts.ExerciseName = "worldwide/Egypt"
	opts.ConnectTimeout = 20 * time.Second

	exDir := opts.GetExerciseDir()
	session := CreateDefaultSession()
	session.EndTick = 4
	session.Paused = false
	sessionPath, err := WriteNewSessionFile(session, exDir)
	if err != nil {
		t.Fatal("failed to write the session")
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
}

var application string
var rootdir string

func init() {
	flag.StringVar(&application, "application", "",
		"path to simulation_app executable")
	flag.StringVar(&rootdir, "root-dir", "",
		"path to simulation root directory")
}
