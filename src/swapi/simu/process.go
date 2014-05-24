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
	"io"
	"log"
	"net"
	"os/exec"
	"strings"
	"sync"
	"time"
)

type ServerProcess struct {
	cmd    *exec.Cmd
	tailch chan int // terminate tail goroutine
	// Passed channel will be signalled once the process ends
	waitqueue chan chan struct{}
	quitAll   sync.WaitGroup

	ClientAddr string
}

// Return an address where a client can connect.
func (s *ServerProcess) GetClientAddr() string {
	return s.ClientAddr
}

// Return true if the process exited on success, false otherwise
func (s *ServerProcess) Success() bool {
	if s != nil && s.cmd != nil && s.cmd.ProcessState != nil {
		return s.cmd.ProcessState.Success()
	}
	return false
}

func (s *ServerProcess) Kill() {
	if s != nil {
		s.cmd.Process.Kill()
		s.Wait(time.Duration(0))
	}
}

// Return true if the server stopped before the timeout. Wait indefinitely
// if the timeout is zero.
func (s *ServerProcess) Wait(d time.Duration) bool {
	waitch := make(chan struct{})
	s.waitqueue <- waitch

	timeoutch := make(chan struct{})
	go func() {
		// Infinite timeout is duration is zero
		if d.Nanoseconds() > 0 {
			time.Sleep(d)
			close(timeoutch)
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

type disconnector func(link io.ReadWriter, timeout time.Duration) bool

// Repeatedly try to connect to host. Fail if no connection can be made
// before timeout. If a connection succeeds and disconnectFn is supplied,
// it will be called to let a chance to close the connection gracefully.
// The callback must return true if the connection was closed properly.
func waitForNetwork(waitch chan error, host string, timeout time.Duration,
	disconnectFn disconnector) {
	const minLoopDelay = 500 * time.Millisecond
	end := time.Now().Add(timeout)
	for {
		start := time.Now()
		conn, err := net.Dial("tcp", host)
		if err == nil {
			if disconnectFn != nil {
				disconnectFn(conn, 5*time.Second)
			}
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

// Start a server process using supplied options. On success, the server
// process will be ready to receive incoming connections. On error, the
// server might still be returned if the connection timed out or the
// process terminated before a connection can be made.
func StartServer(executable, addr, runDir string, args []string, logFiles []string,
	logPrefix string, connectTimeout time.Duration, disconnectFn disconnector) (
	*ServerProcess, error) {

	cmd := exec.Command(executable, args...)
	cmd.Dir = runDir
	s := ServerProcess{
		cmd:        cmd,
		tailch:     make(chan int, 1),
		waitqueue:  make(chan chan struct{}, 1),
		ClientAddr: addr,
	}
	err := cmd.Start()
	if err != nil {
		return nil, err
	}

	// Process.Wait() cannot be called concurrently, so always wait for process
	// termination and handle wait requests. Callers wanting to wait for
	// termination posts a channel which is signalled when it happens.
	go func() {
		waitch := make(chan struct{})
		go func() {
			cmd.Wait()
			s.tailch <- 1
			s.quitAll.Wait()
			close(waitch)
		}()

		pendings := []chan struct{}{}
		for {
			select {
			case ch := <-s.waitqueue:
				if waitch == nil {
					close(ch)
				} else {
					pendings = append(pendings, ch)
				}
			case <-waitch:
				waitch = nil
				for _, ch := range pendings {
					close(ch)
				}
			}
		}
	}()

	if len(logFiles) > 0 {
		s.quitAll.Add(1)
		go func() {
			if len(logPrefix) > 0 {
				logPrefix = logPrefix + ": "
			}
			defer s.quitAll.Done()
			TailFiles(logFiles, s.tailch, func(line string) bool {
				log.Printf(logPrefix + line)
				return false
			})
		}()
	}

	if connectTimeout <= 0 {
		connectTimeout = 10 * time.Second
	}

	// Wait for either the server to terminate, fails to open a server
	// socket, or succeeds. Fails in the two first cases.
	netch := make(chan error, 1)
	go waitForNetwork(netch, addr, connectTimeout, disconnectFn)
	procch := make(chan struct{})
	s.waitqueue <- procch
	select {
	case err := <-netch:
		if err != nil {
			s.Kill()
			return &s, err
		}
		break
	case <-procch:
		if !s.Success() {
			return &s, errors.New("failed to start server process " + executable + " " + strings.Join(args, " "))
		}
	}
	return &s, err
}
