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
	"errors"
	"masa/sword/swapi"
	"os"
	"path/filepath"
	"time"
)

const (
	StopTimeout = 10 * time.Second
)

type SimOpts struct {
	Executable     string
	RunDir         *string
	RootDir        string
	DataDir        string
	ExercisesDir   string // Ignored if empty
	SessionName    string
	ExerciseName   string
	CheckpointName string
	DebugDir       string
	// Enable simulation internal testing commands
	TestCommands bool

	// Network address:port of dispatcher server, default to session if empty.
	DispatcherAddr string

	// Network address:port of simulation server, default to session if empty.
	SimulationAddr string

	// Delay after which the simulation process will be terminated if the
	// startup function fails to connect to its dispatcher server.
	ConnectTimeout time.Duration

	// Tail log files, output to log module
	EnableTailing bool
	// Prefix the tail output with supplied string
	TailPrefix string
	// Pathfinds dump directory. Relative paths are resolve relatively to the
	// the session directory. An empty value disables the feature.
	PathfindDir    string
	PathfindFilter string
}

func CheckPaths(directory bool, paths ...string) error {
	for _, path := range paths {
		fi, err := os.Stat(path)
		if err != nil {
			return errors.New("'" + path + "' is invalid " + err.Error())
		}
		if fi.IsDir() != directory {
			if directory {
				return errors.New("'" + path + "' is not a directory")
			} else {
				return errors.New("'" + path + "' is not a file")
			}
		}
	}
	return nil
}

func IsFile(paths ...string) error      { return CheckPaths(false, paths...) }
func IsDirectory(paths ...string) error { return CheckPaths(true, paths...) }

func (o *SimOpts) Check() error {
	err := IsFile(o.Executable)
	if err != nil {
		return err
	}
	if o.RunDir == nil {
		p := filepath.Dir(o.Executable)
		o.RunDir = &p
	}
	return IsDirectory(*o.RunDir, o.RootDir, o.DataDir)
}

func (o *SimOpts) GetExerciseDir() string {
	if len(o.ExercisesDir) > 0 {
		return filepath.Join(o.ExercisesDir, o.ExerciseName)
	}
	return filepath.Join(o.RootDir, "exercises", o.ExerciseName)
}

func (o *SimOpts) GetSessionDir() string {
	return filepath.Join(o.GetExerciseDir(), "sessions", o.SessionName)
}

func (o *SimOpts) GetSessionFile() string {
	return filepath.Join(o.GetSessionDir(), "session.xml")
}

func (o *SimOpts) GetProfilesFile() string {
	return filepath.Join(o.GetExerciseDir(), "profiles.xml")
}

func (o *SimOpts) GetSimLogPath() string {
	return filepath.Join(o.GetSessionDir(), "Sim.log")
}

func (o *SimOpts) GetDispatcherLogPath() string {
	return filepath.Join(o.GetSessionDir(), "Dispatcher.log")
}

func (o *SimOpts) GetProtoLogPath() string {
	return filepath.Join(o.GetSessionDir(), "Protobuf.log")
}

func (o *SimOpts) GetReplayDir() string {
	return filepath.Join(o.GetSessionDir(), "record")
}

// Write the input session at a suitable place in the configured exercise
// and set SimOpts.SessionName accordingly
func (o *SimOpts) WriteSession(session *Session) error {
	if len(o.ExerciseName) <= 0 {
		return errors.New("exercise name is not set")
	}
	exDir := o.GetExerciseDir()
	sessionPath, err := WriteNewSessionFile(session, exDir)
	if err != nil {
		return err
	}
	sessionDir := filepath.Dir(sessionPath)
	o.DebugDir = filepath.Join(sessionDir, "debug")
	o.SessionName = filepath.Base(sessionDir)
	return nil
}

type SimProcess struct {
	*ServerProcess

	Opts *SimOpts
}

// Connects and asks the simulation to stop. Returns true if the stop command
// was issued, false if some error prevented doing it. Note that even the
// command was issued, there is no guarantee the simulation received it,
// as Stop() seem unreliable. What you know is if it was not issued there is
// little reason for the simulation to stop unless it reaches it max-tick,
// hence less reasons to wait for it.
func logAndStop(host string, replayer bool) (bool, error) {
	client, err := swapi.NewClient(host)
	if err != nil {
		return false, err
	}
	client.EnableModel = false
	client.PostTimeout = 30 * time.Second
	go client.Run()
	defer client.Close()
	key, err := client.GetAuthenticationKey()
	if err != nil {
		return false, err
	}
	err = client.LoginWithAuthenticationKey("", "", key)
	if err != nil {
		return false, err
	}
	if replayer {
		err = client.ReplayStop()
	} else {
		err = client.Stop()
	}
	return true, err
}

// Connect to the simulation and execute a Login/Stop sequence using
// supervisor.
func (sim *SimProcess) Stop() error {
	if sim == nil {
		return errors.New("simulation is stopped already")
	}
	stopped, err := logAndStop(sim.ClientAddr, false)
	if stopped {
		if sim.Wait(StopTimeout) {
			return nil
		}
		err = errors.New("wait timed out")
	}
	sim.Kill()
	return err
}

// Start a simulation using supplied options. On success, the simulation
// process will be ready to receive incoming connections. On error, the
// simulation might still be returned if the connection timed out or the
// process terminated before a connection can be made.
func StartSim(opts *SimOpts) (*SimProcess, error) {
	err := opts.Check()
	if err != nil {
		return nil, err
	}
	sessionDir := opts.GetSessionDir()
	session, err := ReadSessionFile(filepath.Join(sessionDir, "session.xml"))
	if err != nil {
		return nil, err
	}
	gamingServer := session.GamingServer

	args := []string{
		"--root-dir=" + opts.RootDir,
		"--data-dir=" + opts.DataDir,
		"--exercise=" + opts.ExerciseName,
		"--session=" + opts.SessionName,
		"--verbose",
	}
	if len(opts.DispatcherAddr) > 0 {
		args = append(args, "--dispatcher-address="+opts.DispatcherAddr)
		gamingServer = opts.DispatcherAddr
	}
	if len(opts.SimulationAddr) > 0 {
		args = append(args, "--simulation-address="+opts.SimulationAddr)
	}
	if len(opts.CheckpointName) > 0 {
		args = append(args, "--checkpoint="+opts.CheckpointName)
	}
	if len(opts.DebugDir) > 0 {
		args = append(args, "--debug-dir="+opts.DebugDir)
	}
	if opts.TestCommands {
		args = append(args, "--test-commands")
	}
	if len(opts.PathfindDir) > 0 {
		pfDir := opts.PathfindDir
		if !filepath.IsAbs(pfDir) {
			pfDir, err = filepath.Abs(filepath.Join(sessionDir, pfDir))
			if err != nil {
				return nil, err
			}
		}
		args = append(args, "--dump-pathfinds="+pfDir)
	}
	if len(opts.PathfindFilter) > 0 {
		args = append(args, "--filter-pathfinds="+opts.PathfindFilter)
	}
	if len(opts.ExercisesDir) > 0 {
		args = append(args, "--exercises-dir="+opts.ExercisesDir)
	}

	logFiles := []string{}
	if opts.EnableTailing {
		logFiles = []string{
			opts.GetSimLogPath(),
			opts.GetDispatcherLogPath(),
		}
	}

	server, err := StartServer(opts.Executable, gamingServer, *opts.RunDir,
		args, logFiles, opts.TailPrefix, opts.ConnectTimeout, swapi.Disconnect)
	sim := &SimProcess{server, opts}
	return sim, err
}
