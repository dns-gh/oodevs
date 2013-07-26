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
	"fmt"
	"log"
	"net"
	"os"
	"os/exec"
	"path/filepath"
	"swapi"
	"sync"
	"time"
)

type SimOpts struct {
	Executable   string
	RunDir       *string
	RootDir      string
	DataDir      string
	SessionName  string
	ExerciseName string
	Legacy       bool

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
	o.SessionName = filepath.Base(filepath.Dir(sessionPath))
	return nil
}

type SimProcess struct {
	cmd    *exec.Cmd
	tailch chan int // terminate tail goroutine
	// Passed channel will be signalled once the process ends
	waitqueue  chan chan int
	sessionDir string
	quitAll    sync.WaitGroup

	Opts           *SimOpts
	DispatcherAddr string
}

// Return true if the process exited on success, false otherwise
func (sim *SimProcess) Success() bool {
	if sim != nil && sim.cmd != nil && sim.cmd.ProcessState != nil {
		return sim.cmd.ProcessState.Success()
	}
	return false
}

func (sim *SimProcess) Kill() {
	if sim != nil {
		sim.cmd.Process.Kill()
		sim.Wait(time.Duration(0))
	}
}

func logAndStop(host, user, password string) error {
	client, err := swapi.NewClient(host)
	if err == nil {
		client.EnableModel = false
		go client.Run()
		defer client.Close()
		err = client.Login(user, password)
		if err == nil {
			err = client.Stop()
		}
	}
	return err
}

// Connect to the simulation and execute a Login/Stop sequence using
// supervisor credentials found in the profiles.xml.
func (sim *SimProcess) Stop() error {
	if sim == nil {
		return errors.New("simulation is stopped already")
	}
	// We need a supervisor to stop the simulation
	profiles, err := ReadProfilesFile(sim.Opts.GetProfilesFile())
	if err == nil {
		admin := profiles.FindSupervisor()
		if admin != nil {
			err = logAndStop(sim.DispatcherAddr, admin.Name, admin.Password)
			if sim.Wait(10 * time.Second) {
				return nil
			}
			err = errors.New("Wait timed out")
		} else {
			err = errors.New("could not find supervisor profile")
		}
	}
	sim.Kill()
	return err
}

// Return true if the simulation stopped before the timeout. Wait indefinitely
// if the timeout is zero.
func (sim *SimProcess) Wait(d time.Duration) bool {
	waitch := make(chan int, 1)
	sim.waitqueue <- waitch

	timeoutch := make(chan int)
	go func() {
		// Infinite timeout is duration is zero
		if d.Nanoseconds() > 0 {
			time.Sleep(d)
			timeoutch <- 1
		}
	}()

	select {
	case <-waitch:
		break
	case <-timeoutch:
		return false
	}
	return true
}

// Repeatedly try to connect to host. Fail if no connection can be made
// before timeout.
func waitForNetwork(waitch chan error, host string, timeout time.Duration) {
	const minLoopDelay = 500 * time.Millisecond
	end := time.Now().Add(timeout)
	for {
		start := time.Now()
		conn, err := net.Dial("tcp", host)
		if err == nil {
			swapi.Disconnect(conn, 5*time.Second)
			conn.Close()
			waitch <- nil
			return
		}
		now := time.Now()
		if now.Sub(start) < minLoopDelay {
			time.Sleep(start.Add(minLoopDelay).Sub(now))
		}
		if !time.Now().Before(end) {
			break
		}
	}
	waitch <- errors.New("connection timed out")
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
	sessionDir := filepath.Join(opts.RootDir, "exercises", opts.ExerciseName,
		"sessions", opts.SessionName)
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
	legacy := 0
	if opts.Legacy {
		legacy = 1
	}
	args = append(args, fmt.Sprintf("--legacy=%d", legacy))

	cmd := exec.Command(opts.Executable, args...)
	cmd.Dir = *opts.RunDir
	sim := SimProcess{
		cmd:            cmd,
		tailch:         make(chan int, 1),
		waitqueue:      make(chan chan int, 1),
		sessionDir:     sessionDir,
		Opts:           opts,
		DispatcherAddr: gamingServer,
	}
	err = cmd.Start()

	// Process.Wait() cannot be called concurrently, so always wait for process
	// termination and handle wait requests. Callers wanting to wait for
	// termination posts a channel which is signalled when it happens.
	go func() {
		waitch := make(chan int)
		go func() {
			cmd.Wait()
			sim.tailch <- 1
			sim.quitAll.Wait()
			waitch <- 1
		}()

		terminated := false
		pendings := []chan int{}
		for {
			select {
			case ch := <-sim.waitqueue:
				if terminated {
					ch <- 1
				} else {
					pendings = append(pendings, ch)
				}
			case <-waitch:
				terminated = true
				for _, ch := range pendings {
					ch <- 1
				}
			}
		}
	}()

	if opts.EnableTailing {
		sim.quitAll.Add(1)
		logFiles := []string{
			opts.GetSimLogPath(),
			opts.GetDispatcherLogPath(),
		}
		go func() {
			prefix := opts.TailPrefix
			if len(prefix) > 0 {
				prefix = prefix + ": "
			}
			defer sim.quitAll.Done()
			TailFiles(logFiles, sim.tailch, func(line string) bool {
				log.Printf(prefix + line)
				return false
			})
		}()
	}

	connectTimeout := opts.ConnectTimeout
	if connectTimeout <= 0 {
		connectTimeout = 10 * time.Second
	}

	// Wait for either the simulation to terminate, fails to open a server
	// socket, or succeeds. Fails in the two first cases.
	netch := make(chan error)
	go waitForNetwork(netch, gamingServer, connectTimeout)
	procch := make(chan int, 1)
	sim.waitqueue <- procch
	select {
	case err := <-netch:
		if err != nil {
			sim.Kill()
			return &sim, err
		}
		break
	case <-procch:
		if !sim.Success() {
			return &sim, errors.New("failed to start simulation")
		}
	}
	return &sim, err
}
