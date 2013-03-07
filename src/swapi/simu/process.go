package simu

import (
	"bufio"
	"errors"
	"io"
	"log"
	"net"
	"os"
	"os/exec"
	"path/filepath"
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

	// Network address:port of dispatcher server, default to session if empty.
	DispatcherAddr string

	// Network address:port of simulation server, default to session if empty.
	SimulationAddr string

	// Delay after which the simulation process will be terminated if the
	// startup function fails to connect to its dispatcher server.
	ConnectTimeout time.Duration
}

func (o *SimOpts) GetExerciseDir() string {
	return filepath.Join(o.RootDir, "exercises", o.ExerciseName)
}

func (o *SimOpts) GetSessionDir() string {
	return filepath.Join(o.GetExerciseDir(), "sessions", o.SessionName)
}

func (o *SimOpts) GetSimLogPath() string {
	return filepath.Join(o.GetSessionDir(), "Sim.log")
}

func (o *SimOpts) GetDispatcherLogPath() string {
	return filepath.Join(o.GetSessionDir(), "Dispatcher.log")
}

type SimProcess struct {
	cmd        *exec.Cmd
	tailch     chan int // terminate tail goroutine
	sessionDir string
	quitAll    sync.WaitGroup

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
		sim.tailch <- 1
		sim.quitAll.Wait()
		sim.cmd.Process.Kill()
		sim.cmd.Wait()
	}
}

func (sim *SimProcess) GetLogPath() string {
	return filepath.Join(sim.sessionDir, "Sim.log")
}

func (sim *SimProcess) Wait(d time.Duration) {
	waitch := make(chan int)
	go func() {
		sim.cmd.Wait()
		waitch <- 1
	}()

	timeoutch := make(chan int)
	go func() {
		time.Sleep(d)
		timeoutch <- 1
	}()

	select {
	case <-waitch:
		break
	case <-timeoutch:
		sim.Kill()
	}
}

// Tail each input path. If path does not exist, try to open it repeatedly.
// Once it is opened, keep logging all file lines until a read fails or the
// quit channel is signalled.
func tail(paths []string, quit chan int, done *sync.WaitGroup) {
	defer func() {
		done.Done()
	}()

	readers := make([]func(), 0, len(paths))
	for _, path := range paths {
		// Duplicate variable for the closure
		logPath := path
		pending := ""
		var reader *bufio.Reader
		stopped := false
		fn := func() {
			if stopped {
				return
			}
			if reader == nil {
				fp, err := os.Open(logPath)
				if err != nil {
					return
				}
				defer fp.Close()
				reader = bufio.NewReader(fp)
			}

			for {
				line, err := reader.ReadString('\n')
				pending += line
				if err != nil {
					if err != io.EOF {
						stopped = true
					}
					return
				}
				log.Print(pending)
				pending = ""
			}
		}
		readers = append(readers, fn)
	}

	ticker := time.NewTicker(1 * time.Second)
	for {
		select {
		case <-ticker.C:
			break
		case <-quit:
			return
		}
		for _, reader := range readers {
			reader()
		}
	}
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

func waitForExit(waitch chan error, process *os.Process) {
	process.Wait()
	waitch <- nil
}

// Start a simulation using supplied options. Return a running SimProcess
// or nil on error. On success, the simulation process will be ready to
// receive incoming connections.
func StartSim(opts *SimOpts) (*SimProcess, error) {
	if len(opts.Executable) <= 0 {
		return nil, errors.New("simulation_app executable path is not defined")
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

	cmd := exec.Command(opts.Executable, args...)
	rundir := opts.RunDir
	if rundir == nil {
		p := filepath.Dir(opts.Executable)
		rundir = &p
	}
	cmd.Dir = *rundir
	sim := SimProcess{
		cmd:            cmd,
		tailch:         make(chan int, 1),
		sessionDir:     sessionDir,
		DispatcherAddr: gamingServer,
	}
	err = cmd.Start()

	sim.quitAll.Add(1)
	logFiles := []string{
		opts.GetSimLogPath(),
		opts.GetDispatcherLogPath(),
	}
	go tail(logFiles, sim.tailch, &sim.quitAll)

	connectTimeout := opts.ConnectTimeout
	if connectTimeout <= 0 {
		connectTimeout = 10 * time.Second
	}

	// Wait for either the simulation to terminate, fails to open a server
	// socket, or succeeds. Fails in the two first cases.
	netch := make(chan error)
	procch := make(chan error)
	go waitForNetwork(netch, gamingServer, connectTimeout)
	go waitForExit(procch, sim.cmd.Process)
	select {
	case err := <-netch:
		if err != nil {
			sim.Kill()
			return nil, err
		}
		break
	case <-procch:
		return nil, errors.New("failed to start simulation")
	}
	return &sim, err
}
