// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package swrun

import (
	"code.google.com/p/goprotobuf/proto"
	"fmt"
	"masa/sword/swapi"
	"masa/sword/swapi/simu"
	"masa/sword/swtest"
	"os"
	"path/filepath"
	"time"
)

// Creates default simulation configuration and adjust it from command line
// arguments.
func MakeOpts(cfg *swtest.Config) *simu.SimOpts {
	opts := simu.SimOpts{}
	projectRoot := ""
    if cwd, err := os.Getwd(); err == nil {
        projectRoot, _ = filepath.Abs(filepath.Join(cwd, "../../../.."))
    }

	if len(cfg.Application) > 0 {
		opts.Executable = cfg.Application
	} else if len(projectRoot) > 0 {
		// By default, make it so simtests run without additional configuration
		opts.Executable = filepath.Join(
			projectRoot, "out", cfg.Platform, "release/simulation_app.exe")
		runDir := filepath.Join(projectRoot, "run", cfg.Platform)
		opts.RunDir = &runDir
	}
	if len(cfg.RootDir) > 0 {
		opts.RootDir = cfg.RootDir
	} else if len(projectRoot) > 0 {
		opts.RootDir = filepath.Join(projectRoot, "data")
		opts.ExercisesDir = filepath.Join(projectRoot, "data/tests/gosword")
	}
	opts.DataDir = opts.RootDir
	if len(cfg.RunDir) > 0 {
		opts.RunDir = &cfg.RunDir
	}
	if len(cfg.ExercisesDir) > 0 {
		opts.ExercisesDir = cfg.ExercisesDir
	}
	opts.ExerciseName = "crossroad-small-empty"
	opts.DispatcherAddr = fmt.Sprintf("localhost:%d", cfg.TestPort+5)
	opts.SimulationAddr = fmt.Sprintf("localhost:%d", cfg.TestPort+6)
	opts.EnableTailing = cfg.ShowLog
	opts.ConnectTimeout = cfg.Timeout
	return &opts
}

// Creates default simulation options and default session and link them
// together.
func MakeOptsAndSession(cfg *swtest.Config) (*simu.SimOpts, *simu.Session) {
	opts := MakeOpts(cfg)
	session := simu.CreateDefaultSession()
	session.GamingServer = opts.DispatcherAddr
	return opts, session
}

type ClientOpts struct {
	Exercise       string
	User           string
	Password       string
	Step           int
	Paused         bool
	Logger         swapi.MessageLogger
	WaitTimeout    time.Duration
	StartGaming    bool
	Seed           int
	PathfindDir    string
	PathfindFilter string
}

// Prints input and output client messages to stderr.
func (opts *ClientOpts) AddLogger() *ClientOpts {
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

// Start a gaming_app after starting the simulation and before connecting the
// test client. First the simulation will be paused (if not already), then
// gaming will be started and call will wait for the simulation to be unpaused
// before moving on.
func (opts *ClientOpts) AddGaming() *ClientOpts {
	opts.StartGaming = true
	return opts
}

// Fix simulation seed, might help with tests with random events like weapon
// fires.
func (opts *ClientOpts) FixSeed() *ClientOpts {
	opts.Seed = 1
	return opts
}

func (opts *ClientOpts) DumpPathfinds(path string) *ClientOpts {
	opts.PathfindDir = path
	return opts
}

func (opts *ClientOpts) FilterPathfinds(filter string) *ClientOpts {
	opts.PathfindFilter = filter
	return opts
}

func (opts *ClientOpts) StartPaused() *ClientOpts {
	opts.Paused = true
	return opts
}

// Starts a simulation with supplied options.
func StartSimOnExercise(clientOpts *ClientOpts, cfg *swtest.Config) (*simu.SimProcess, error) {
	opts, session := MakeOptsAndSession(cfg)
	opts.ExerciseName = clientOpts.Exercise
	opts.PathfindDir = clientOpts.PathfindDir
	opts.PathfindFilter = clientOpts.PathfindFilter
	session.Paused = clientOpts.Paused
	if clientOpts.Step > 0 {
		session.TimeStep = clientOpts.Step
	}
	err := opts.WriteSession(session)
	if err != nil {
		return nil, err
	}
	return simu.StartSim(opts)
}

// Implemented by simulation and replayer.
type Simulator interface {
	GetClientAddr() string
}

// Creates and connects a client to supplied simulator.
func ConnectClient(sim Simulator, opts *ClientOpts, cfg *swtest.Config) (*swapi.Client, error) {
	if opts == nil {
		opts = &ClientOpts{}
	}
	client, err := swapi.NewClient(sim.GetClientAddr())
	if err != nil {
		return nil, err
	}
	client.Logger = opts.Logger
	client.PostTimeout = cfg.Timeout
	client.Model.WaitTimeout = cfg.Timeout
	if opts.WaitTimeout != 0 {
		timeout := opts.WaitTimeout
		if timeout < 0 {
			timeout = 0
		}
		client.Model.WaitTimeout = timeout
	}
	swapi.ConnectClient(client)
	return client, nil
}

// Creates, connects, authenticates a client on a simulator and wait for the
// initial state to be received and processed.
func LoginAndWaitModel(sim Simulator, opts *ClientOpts, cfg *swtest.Config) (*swapi.Client, error) {

	client, err := ConnectClient(sim, opts, cfg)
	if err != nil {
		return nil, err
	}
	err = client.Login(opts.User, opts.Password)
	if err != nil {
		client.Close()
		return nil, err
	}
	ok := client.Model.WaitReady(10 * time.Second)
	if !ok {
		client.Close()
		return nil, fmt.Errorf("client timed out before initialization completed")
	}
	return client, nil
}

// Starts and connects a gaming to a running simulation in admin mode, after
// pausing the simulation.
func startGaming(sim *simu.SimProcess, cfg *swtest.Config) error {
	admin, err := ConnectClient(sim, &ClientOpts{WaitTimeout: -1}, cfg)
	if err != nil {
		return err
	}
	defer admin.Close()
	key, err := admin.GetAuthenticationKey()
	if err != nil {
		return err
	}
	err = admin.LoginWithAuthenticationKey("", "", key)
	if err != nil {
		return err
	}
	_, err = admin.Pause()
	if err != nil {
		return err
	}
	_, err = simu.StartGamingFromSim(sim.Opts)
	if err != nil {
		return err
	}
	admin.Model.WaitTicks(1)
	return nil
}

// Starts a simulation, connects a client and waits for it to be ready.
func ConnectAndWaitModel(opts *ClientOpts, cfg *swtest.Config) (
	*simu.SimProcess, *swapi.Client, error) {

	sim, err := StartSimOnExercise(opts, cfg)
	if err != nil {
		return nil, nil, err
	}
	ok := false
	defer func() {
		if !ok {
			sim.Stop()
		}
	}()
	if opts.StartGaming {
		err := startGaming(sim, cfg)
		if err != nil {
			return nil, nil, err
		}
	}
	client, err := LoginAndWaitModel(sim, opts, cfg)
	if err != nil {
		return nil, nil, err
	}
	ok = true
	return sim, client, nil
}

// Stops the simulation and check its log files for errors.
func StopSim(sim *simu.SimProcess, opts *simu.SessionErrorsOpts) error {
	sim.Stop()
	session := sim.Opts.GetSessionDir()
	err := simu.CheckSessionErrors(session, opts)
	if err != nil {
		return fmt.Errorf("errors found in session %s:\n%s", session, err)
	}
	return nil
}

// Stops a client and a simulation and check log files for errors.
func StopSimAndClient(sim *simu.SimProcess, client *swapi.Client) error {
	client.Close()
	return StopSim(sim, nil)
}
