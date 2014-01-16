// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package simu

import (
	"flag"
	"fmt"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"os"
	"path/filepath"
	"regexp"
	"runtime"
	"time"
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

func (s *TestSuite) TestSimOpts(c *C) {
	opts := SimOpts{}
	opts.RootDir = "data/dir"
	opts.ExerciseName = "exercisename"

	d := opts.GetExerciseDir()
	c.Assert(filepath.ToSlash(d), Equals, "data/dir/exercises/exercisename")
}

func MakeOpts() *SimOpts {
	opts := SimOpts{}
	projectRoot := ""
	if cwd, err := os.Getwd(); err == nil {
		projectRoot, _ = filepath.Abs(filepath.Join(cwd, "..", "..", ".."))
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
	opts.DispatcherAddr = fmt.Sprintf("localhost:%d", testPort)
	opts.SimulationAddr = fmt.Sprintf("localhost:%d", testPort+1)
	opts.ConnectTimeout = 600 * time.Second
	opts.EnableTailing = showLog
	return &opts
}

// Test the simulation can be started successfully and ticks a bit.
func (s *TestSuite) TestSuccess(c *C) {
	opts := MakeOpts()

	exDir := opts.GetExerciseDir()
	session := CreateDefaultSession()
	session.EndTick = 3
	sessionPath, err := WriteNewSessionFile(session, exDir)
	c.Assert(err, IsNil)

	opts.SessionName = filepath.Base(filepath.Dir(sessionPath))
	sim, err := StartSim(opts)
	defer stopSim(c, sim)
	// Since the simulation can run the 3 ticks before the test connection
	// returns successfully, we cannot check the error returned by StartSim
	// unless we have a way to start in paused mode and unpause it. Instead,
	// just check the exit status
	c.Assert(sim, NotNil, Commentf("simulation failed to start: %v", err))

	sim.Wait(60 * time.Second)
	c.Assert(sim.Success(), Equals, true)

	err = CheckSessionErrors(opts.GetSessionDir())
	c.Assert(err, IsNil)
}

// Test SimProcess fails fast when started with an invalid configuration and
// taking enough time to detect it that exec module does not pick it.
// See the funcErr in Sim.log
func (s *TestSuite) TestDelayedStartupFailure(c *C) {
	opts := MakeOpts()

	exDir := opts.GetExerciseDir()
	session := CreateDefaultSession()
	session.EndTick = 3
	session.Paused = false
	sessionPath, err := WriteNewSessionFile(session, exDir)
	c.Assert(err, IsNil)

	// Remove dispatcher/network element
	data, err := ioutil.ReadFile(sessionPath)
	re, err := regexp.Compile(`<network client.*?</network>`)
	c.Assert(err, IsNil)

	content := re.ReplaceAllString(string(data), "")
	err = ioutil.WriteFile(sessionPath, []byte(content), 0644)
	c.Assert(err, IsNil)

	opts.SessionName = filepath.Base(filepath.Dir(sessionPath))
	sim, err := StartSim(opts)
	defer sim.Stop()

	c.Assert(err, ErrorMatches, "failed to start server process")
	if sim.Success() {
		c.Fatal("simulation should have exited on error")
	}
}

func (s *TestSuite) TestRotatingLogs(c *C) {
	opts := MakeOpts()

	exDir := opts.GetExerciseDir()
	session := CreateDefaultSession()
	session.EndTick = 2
	session.Paused = false
	session.SimLog.Count = 3
	session.SimLog.Size = 256
	session.SimLog.Unit = "bytes"
	sessionPath, err := WriteNewSessionFile(session, exDir)
	c.Assert(err, IsNil)
	sessionDir := filepath.Dir(sessionPath)
	opts.DebugDir = filepath.Join(sessionDir, "debug")

	opts.SessionName = filepath.Base(sessionDir)
	sim, err := StartSim(opts)
	defer stopSim(c, sim)
	sim.Wait(60 * time.Second)
	c.Assert(sim.Success(), Equals, true)

	// Check log files
	found := map[string]struct{}{}
	matcher := regexp.MustCompile(`Sim(\.\d{8}T\d{6})?\.log(?:\.(\d+))?`)
	entries, err := ioutil.ReadDir(sessionDir)
	c.Assert(err, IsNil)
	for _, d := range entries {
		matches := matcher.FindStringSubmatch(d.Name())
		if len(matches) == 0 {
			continue
		}
		found[matches[1]+"-"+matches[2]] = struct{}{}
		st, err := os.Stat(filepath.Join(sessionDir, d.Name()))
		c.Assert(err, IsNil)
		c.Assert(st.IsDir(), Equals, false)
		// Files are rotated when the size exceeds the threshold, keep some
		// room for an additional line.
		c.Assert(st.Size(), Lesser, int64(2*session.SimLog.Size))
	}
	c.Assert(found, HasLen, 3)
	c.Assert(found[""], NotNil)
}

func (s *TestSuite) TestTimeOptions(c *C) {
	opts := MakeOpts()

	keys := []struct {
		Name  string
		Value int
	}{
		{"factor", 0},
		{"factor", -1},
		{"step", 0},
		{"step", -1},
	}

	for _, key := range keys {
		exDir := opts.GetExerciseDir()
		session := CreateDefaultSession()
		session.EndTick = 2
		if key.Name == "factor" {
			session.TimeFactor = key.Value
		} else if key.Name == "step" {
			session.TimeStep = key.Value
		} else {
			c.Fatalf("unknown key %s", key.Name)
		}
		sessionPath, err := WriteNewSessionFile(session, exDir)
		c.Assert(err, IsNil)
		sessionDir := filepath.Dir(sessionPath)
		opts.DebugDir = filepath.Join(sessionDir, "debug")
		opts.SessionName = filepath.Base(sessionDir)
		sim, err := StartSim(opts)
		defer sim.Stop()
		sim.Wait(60 * time.Second)
		c.Assert(sim.Success(), Equals, false)
		dmps, err := ListDmpFiles(opts.DebugDir)
		c.Assert(err, IsNil)
		c.Assert(dmps, HasLen, 0)
	}
}

// Test starting gaming process
func (s *TestSuite) TestStartGamingFromSim(c *C) {
	simOpts := MakeOpts()
	exDir := simOpts.GetExerciseDir()
	session := CreateDefaultSession()
	session.EndTick = 3
	sessionPath, err := WriteNewSessionFile(session, exDir)
	c.Assert(err, IsNil)
	simOpts.SessionName = filepath.Base(filepath.Dir(sessionPath))

	gaming, err := StartGamingFromSim(simOpts)
	c.Assert(err, IsNil)
	defer gaming.Kill()
	// Let it run a couple of seconds, it should timeout
	ok := gaming.Wait(3 * time.Second)
	c.Assert(ok, Equals, false)
}
