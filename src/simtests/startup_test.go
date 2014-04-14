// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package simtests

import (
	"fmt"
	"io/ioutil"
	. "launchpad.net/gocheck"
	"regexp"
	"strings"
	"swapi/simu"
	"time"
)

func CheckSimFailed(c *C, err error, sim *simu.SimProcess) {
	c.Assert(err, NotNil) // simulation should not have started
	c.Assert(err, ErrorMatches, "(?s).*failed to start server process.*")

	if sim.Success() {
		c.Fatal("simulation should have exited on error")
	}

}

func ReadTextFile(c *C, path string) string {
	data, err := ioutil.ReadFile(path)
	c.Assert(err, IsNil, Commentf("failed to read: %v", path))
	return string(data)
}

func WriteSession(c *C, opts *simu.SimOpts, session *simu.Session) {
	err := opts.WriteSession(session)
	c.Assert(err, IsNil) // failed to write session
}

func (s *TestSuite) TestDispatcherMisconfiguration(c *C) {
	session := simu.CreateDefaultSession()
	session.EndTick = 3

	opts := MakeOpts()
	opts.ConnectTimeout = 600 * time.Second
	WriteSession(c, opts, session)

	// Remove dispatcher/network element
	re, err := regexp.Compile(`<network client.*?</network>`)
	c.Assert(err, IsNil) // failed to compile regular expression

	sessionPath := opts.GetSessionFile()
	content := re.ReplaceAllString(ReadTextFile(c, sessionPath), "")
	err = ioutil.WriteFile(sessionPath, []byte(content), 0644)
	c.Assert(err, IsNil) // failed to rewrite session file

	sim, err := simu.StartSim(opts)
	defer sim.Stop()
	CheckSimFailed(c, err, sim)

	// Sim.log should say something about the dispatcher and dispatcher.log
	simLog := ReadTextFile(c, opts.GetSimLogPath())
	expected := "(?s).*<(fatalERR|functERR)> dispatcher: failed to create dispatcher.*"
	c.Assert(simLog, Matches, expected)

	// Dispatcher.log should mention the missing XML element
	// [2013-03-04 10:46:24] <Dispatcher> <functERR> session.xml (line 4, column 17)
	//   : Node 'dispatcher' does not have a child named 'network'
	expected = "(?s).*Node 'dispatcher' does not have a child named 'network'.*"
	dispatcherLog := ReadTextFile(c, opts.GetDispatcherLogPath())
	c.Assert(dispatcherLog, Matches, expected)
}

func (s *TestSuite) TestDispatcherAddressCollision(c *C) {
	startSim := func(simOffset int) (*simu.SimProcess, error) {
		session := simu.CreateDefaultSession()

		opts := MakeOpts()
		WriteSession(c, opts, session)
		opts.SimulationAddr = fmt.Sprintf("localhost:%d", Cfg.TestPort+simOffset+6)
		opts.TailPrefix = fmt.Sprintf("sim+%v", simOffset)
		return simu.StartSim(opts)
	}

	sim1, err := startSim(0)
	defer sim1.Stop()
	c.Assert(err, IsNil) // simulation failed to start

	sim2, err := startSim(1)
	defer sim2.Stop()
	sim2.Wait(60 * time.Second)

	if sim2.Success() {
		c.Fatal("simulation with colliding dispatcher should have failed")
	}
	logData := ReadTextFile(c, sim2.Opts.GetDispatcherLogPath())
	c.Assert(logData, Matches, "(?s).*(Une seule utilisation de chaque)|(Only one usage of each socket address).*")
}

// Return true if any line of sim.log or dispatcher.log is eventually matched
// by "matcher". False if the simulation ends or times out before it happens.
// "matcher" must return true on successful matches.
func waitForMatchingLog(c *C, sim *simu.SimProcess,
	timeout time.Duration, matcher simu.TailHandler) bool {

	quitch := make(chan int)
	tailch := make(chan int, 1)
	timeoutch := make(chan int, 1)

	files := []string{
		sim.Opts.GetSimLogPath(),
		sim.Opts.GetDispatcherLogPath(),
	}
	go func() {
		done := false
		simu.TailFiles(files, quitch, func(line string) bool {
			done = done || matcher(line)
			return done
		})
		tailch <- 0
	}()
	go func() {
		ok := 0
		if !sim.Wait(timeout) {
			ok = 1
		}
		timeoutch <- ok
	}()
	select {
	case <-tailch:
		return true
	case ok := <-timeoutch:
		if ok != 0 {
			c.Fatal("waitForMatchingLo timed out")
		}
		break
	}
	return false
}

// Test corner cases of session end-tick parameter
func (s *TestSuite) TestLowEndTickValues(c *C) {

	// Start a simulation and fail if the simulation failed to start or if
	// it reaches endTick + 1.
	runSim := func(c *C, endTick int) bool {
		session := simu.CreateDefaultSession()
		session.EndTick = endTick
		opts := MakeOpts()
		WriteSession(c, opts, session)
		sim, err := simu.StartSim(opts)
		c.Assert(err, IsNil)  // simulation failed to start"
		c.Assert(sim, NotNil) // simulation failed to start"
		defer sim.Stop()

		pattern := fmt.Sprintf(
			"<Simulation> <info> **** Time tick %d", endTick+1)
		return waitForMatchingLog(c, sim, 180*time.Second, func(line string) bool {
			return strings.Contains(line, pattern)
		})
	}
	testLowTick := func(c *C, endTick int) {
		ok := runSim(c, endTick)
		if ok {
			c.Fatalf("simulation ticked up to %d with end-tick=%d",
				endTick+1, endTick)
		}
	}
	// Simulation should tick only once
	testLowTick(c, 1)
	// Simulation should tick only twice
	testLowTick(c, 2)

	// end-tick=0 means infinity, check it ticks once
	if !runSim(c, 0) {
		c.Fatal("simulation did not tick with end-tick=0")
	}
}

// Test stopping the simulation through protobuf API
func (s *TestSuite) TestStoppingSimProcess(c *C) {
	session := simu.CreateDefaultSession()
	opts := MakeOpts()
	WriteSession(c, opts, session)
	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil)
	defer sim.Kill()

	err = sim.Stop()
	c.Assert(err, IsNil)
	c.Assert(sim.Success(), Equals, true)
}
