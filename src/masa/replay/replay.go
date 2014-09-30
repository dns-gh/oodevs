// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package main

import (
	"encoding/xml"
	"flag"
	"fmt"
	"net"
	"os"
	"os/exec"
	"path/filepath"
	"strconv"
	"strings"
	"sync"
	"time"
)

func main() {
	err := start()
	if err != nil {
		fmt.Println(err)
		os.Exit(-1)
	}
}

type Context struct {
	root       string
	exercise   string
	session    string
	dispatcher int
	timeline   int
}

func start() error {
	ctx, err := parseArgs()
	if err != nil {
		return err
	}
	err = installVcredist(ctx.root)
	if err != nil {
		return err
	}
	err = parsePorts(ctx)
	if err != nil {
		return err
	}
	dispatcher := fmt.Sprintf("localhost:%d", ctx.dispatcher)
	replayer, err := startProcess(ctx, "replayer_app",
		"--dispatcher-address", dispatcher)
	if err != nil {
		return err
	}
	err = waitAddress(dispatcher)
	if err != nil {
		return err
	}
	timeline, err := startTimeline(ctx)
	if err != nil {
		return err
	}
	gaming, err := startProcess(ctx, "gaming_app",
		"--host", dispatcher,
		"--timeline", fmt.Sprintf("localhost:%d", ctx.timeline),
	)
	if err != nil {
		return err
	}
	return monitorProcesses(replayer, timeline, gaming)
}

func hasDirectory(path string) error {
	info, err := os.Stat(path)
	if err != nil || !info.IsDir() {
		return fmt.Errorf("invalid directory %s", path)
	}
	return nil
}

func (c *Context) getExerciseDir() string {
	return filepath.Join(c.root, "exercises", c.exercise)
}

func (c *Context) getSessionDir() string {
	return filepath.Join(c.getExerciseDir(), "sessions", c.session)
}

func parseArgs() (*Context, error) {
	ctx := Context{}
	flag.Parse()
	if flag.NArg() != 3 {
		return nil, fmt.Errorf("usage: replay <root> <exercise> <session>")
	}
	args := flag.Args()
	ctx.root = args[0]
	err := hasDirectory(ctx.root)
	if err != nil {
		return nil, err
	}
	ctx.exercise = args[1]
	err = hasDirectory(ctx.getExerciseDir())
	if err != nil {
		return nil, err
	}
	ctx.session = args[2]
	err = hasDirectory(ctx.getSessionDir())
	return &ctx, err
}

type xDispatcher struct {
	Value string `xml:"server,attr"`
}

type xTimeline struct {
	Value string `xml:"url,attr"`
}

type xNetwork struct {
	Dispatcher xDispatcher `xml:"config>dispatcher>network"`
	Timeline   xTimeline   `xml:"config>timeline"`
}

func parsePort(host string) (int, error) {
	if strings.Contains(host, ":") {
		_, value, err := net.SplitHostPort(host)
		if err != nil {
			return 0, err
		}
		host = value
	}
	port, err := strconv.Atoi(host)
	if err != nil {
		return 0, err
	}
	if port <= 0 || port >= 1<<16 {
		return 0, fmt.Errorf("invalid dispatcher port %d", port)
	}
	return port, nil
}

func parsePorts(ctx *Context) error {
	file, err := os.Open(filepath.Join(ctx.getSessionDir(), "session.xml"))
	if err != nil {
		return err
	}
	defer file.Close()
	data := xNetwork{}
	err = xml.NewDecoder(file).Decode(&data)
	if err != nil {
		return err
	}
	ctx.dispatcher, err = parsePort(data.Dispatcher.Value)
	if err != nil {
		return err
	}
	ctx.timeline, err = parsePort(data.Timeline.Value)
	if err != nil {
		return err
	}
	return nil
}

func startProcess(ctx *Context, name string, args ...string) (*exec.Cmd, error) {
	args = append([]string{
		"--root-dir", ctx.root,
		"--exercise", ctx.exercise,
		"--session", ctx.session,
	}, args...)
	cmd := exec.Command(
		filepath.Join(ctx.root, "bin", name+".exe"),
		args...,
	)
	cmd.Dir = filepath.Join(ctx.root, "bin")
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	return cmd, cmd.Start()
}

func startTimeline(ctx *Context) (*exec.Cmd, error) {
	cmd := exec.Command(
		filepath.Join(ctx.root, "bin", "timeline_server.exe"),
		"--port", strconv.Itoa(ctx.timeline),
		"--run", filepath.Join(ctx.getSessionDir(), "timeline.run"),
		"--sword", fmt.Sprintf("localhost:%d", ctx.dispatcher),
	)
	cmd.Dir = filepath.Join(ctx.root, "bin")
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	return cmd, cmd.Start()
}

func waitAddress(address string) error {
	deadline := time.Now().Add(1 * time.Minute)
	for {
		if time.Now().After(deadline) {
			return fmt.Errorf("timeout on %s", address)
		}
		link, err := net.DialTimeout("tcp", address, 10*time.Second)
		if err == nil {
			link.Close()
			return nil
		}
	}
}

type ProcessError struct {
	process *exec.Cmd
	err     error
}

func monitorProcesses(procs ...*exec.Cmd) error {
	wait := &sync.WaitGroup{}
	done := make(chan ProcessError, len(procs))
	wait.Add(len(procs))
	for _, p := range procs {
		go func(p *exec.Cmd) {
			done <- ProcessError{p, p.Wait()}
			wait.Done()
		}(p)
	}
	reply := <-done
	for _, p := range procs {
		p.Process.Kill()
	}
	wait.Wait()
	if reply.err != nil {
		return fmt.Errorf("%s: %s", filepath.Base(reply.process.Path), reply.err)
	}
	return nil
}
