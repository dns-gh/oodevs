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
	"os"
	"os/exec"
	"path/filepath"
	"time"
)

type GamingOpts struct {
	Executable   string
	RunDir       *string
	RootDir      string
	DataDir      string
	SessionName  string
	ExerciseName string
}

func (o *GamingOpts) Check() error {
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

type GamingProcess struct {
	cmd *exec.Cmd
	// Passed channel will be signalled once the process ends
	waitqueue chan chan int

	Opts *GamingOpts
}

// Kill gaming process and return after it exited.
func (gaming *GamingProcess) Kill() {
	if gaming != nil {
		gaming.cmd.Process.Kill()
		gaming.Wait(time.Duration(0))
	}
}

// Return true if gaming stopped before the timeout. Wait indefinitely
// if the timeout is zero.
func (gaming *GamingProcess) Wait(d time.Duration) bool {
	waitch := make(chan int, 1)
	gaming.waitqueue <- waitch

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

func StartGaming(opts *GamingOpts) (*GamingProcess, error) {
	err := opts.Check()
	if err != nil {
		return nil, err
	}
	args := []string{
		"--root-dir=" + opts.RootDir,
		"--data-dir=" + opts.DataDir,
		"--exercise=" + opts.ExerciseName,
		"--session=" + opts.SessionName,
	}
	cmd := exec.Command(opts.Executable, args...)
	cmd.Dir = *opts.RunDir
	gaming := GamingProcess{
		cmd:       cmd,
		waitqueue: make(chan chan int, 1),
		Opts:      opts,
	}
	err = cmd.Start()

	// Process.Wait() cannot be called concurrently, so always wait for process
	// termination and handle wait requests. Callers wanting to wait for
	// termination posts a channel which is signalled when it happens.
	go func() {
		waitch := make(chan int)
		go func() {
			cmd.Wait()
			waitch <- 1
		}()

		terminated := false
		pendings := []chan int{}
		for {
			select {
			case ch := <-gaming.waitqueue:
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
	return &gaming, err
}

// Start a gaming from simulation options and supplied executable and run
// directory. If executable is an empty string, runDir is ignored and both
// are deduced from the simulation executable and run directory.
func StartGamingFromSimAndExe(executable, runDir string, simOpts *SimOpts) (
	*GamingProcess, error) {
	if len(executable) == 0 {
		if len(simOpts.Executable) == 0 {
			return nil, errors.New("simulation executable path is not defined")
		}
		simDir := filepath.Dir(simOpts.Executable)
		simRunDir := ""
		if simOpts.RunDir == nil {
			simRunDir = simDir
		} else {
			simRunDir = *simOpts.RunDir
		}
		// Look in simulation executable location
		p := filepath.Join(simDir, "gaming_app.exe")
		if _, err := os.Stat(p); !os.IsNotExist(err) {
			runDir = simRunDir
			executable = p
		} else {
			// Maybe that is a development environment with the sim in something
			// like out/${platform}/release/applications/sim_app/sim_app.exe.
			p = filepath.Join(simDir, "../gaming_app/gaming_app.exe")
			if _, err := os.Stat(p); !os.IsNotExist(err) {
				runDir = simRunDir
				executable = p
			}
		}
	}

	opts := GamingOpts{
		Executable:   executable,
		RunDir:       &runDir,
		RootDir:      simOpts.RootDir,
		DataDir:      simOpts.DataDir,
		SessionName:  simOpts.SessionName,
		ExerciseName: simOpts.ExerciseName,
	}
	return StartGaming(&opts)
}

// Start a gaming process from simulation options, deduce executable and run
// directory accordingly.
func StartGamingFromSim(simOpts *SimOpts) (*GamingProcess, error) {
	return StartGamingFromSimAndExe("", "", simOpts)
}
