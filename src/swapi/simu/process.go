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

	// Delay after which the simulation process will be terminated if the
	// startup function fails to connect to its dispatcher server.
	ConnectTimeout time.Duration
}

func (o *SimOpts) GetExerciseDir() string {
	return filepath.Join(o.DataDir, "exercises", o.ExerciseName)
}

type SimProcess struct {
	cmd        *exec.Cmd
	tailch     chan int // terminate tail goroutine
	sessionDir string
	quitAll    sync.WaitGroup
}

func (sim *SimProcess) Kill() {
	sim.tailch <- 1
	sim.quitAll.Wait()
	sim.cmd.Process.Kill()
	sim.cmd.Wait()
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

// If path does not exist, try to open it repeatedly. Once it is opened, keep
// logging all file lines until a read fails or the quit channel is signalled.
func tail(path string, quit chan int, done *sync.WaitGroup) {
	defer func() {
		done.Done()
	}()

	ticker := time.NewTicker(1 * time.Second)
	var reader *bufio.Reader
	pending := ""
	for {
		select {
		case <-ticker.C:
			break
		case <-quit:
			return
		}
		if reader == nil {
			fp, err := os.Open(path)
			if err != nil {
				continue
			}
			defer fp.Close()
			reader = bufio.NewReader(fp)
		}

		for {
			line, err := reader.ReadString('\n')
			pending += line
			if err != nil {
				if err != io.EOF {
					return
				}
				break
			}
			log.Print(pending)
			pending = ""
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

	cmd := exec.Command(opts.Executable,
		"--root-dir="+opts.RootDir,
		"--data-dir="+opts.DataDir,
		"--exercise="+opts.ExerciseName,
		"--session="+opts.SessionName,
		"--verbose")
	rundir := opts.RunDir
	if rundir == nil {
		p := filepath.Dir(opts.Executable)
		rundir = &p
	}
	cmd.Dir = *rundir
	sim := SimProcess{
		cmd:        cmd,
		tailch:     make(chan int, 1),
		sessionDir: sessionDir,
	}
	err = cmd.Start()

	sim.quitAll.Add(1)
	go tail(sim.GetLogPath(), sim.tailch, &sim.quitAll)

	connectTimeout := opts.ConnectTimeout
	if connectTimeout <= 0 {
		connectTimeout = 10 * time.Second
	}

	// Wait for either the simulation to terminate, fails to open a server
	// socket, or succeeds. Fails in the two first cases.
	netch := make(chan error)
	procch := make(chan error)
	go waitForNetwork(netch, session.GamingServer, connectTimeout)
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
