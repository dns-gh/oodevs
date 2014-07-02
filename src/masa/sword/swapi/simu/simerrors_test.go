// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package simu

import (
	"bytes"
	. "launchpad.net/gocheck"
	"masa/sword/swtest"
)

func getLogGroups(data string) ([]string, error) {
	fp := bytes.NewBufferString(data)
	parser := NewLogParser(fp)
	groups := []string{}
	for parser.Scan() {
		groups = append(groups, parser.Text())
	}
	return groups, parser.Err()
}

func (s *TestSuite) TestSimErrorsLogParser(c *C) {
	// Empty log
	groups, err := getLogGroups("")
	c.Assert(err, IsNil)
	c.Assert(len(groups), Equals, 0)

	// Blank line
	groups, err = getLogGroups(" \n")
	c.Assert(err, IsNil)
	c.Assert(len(groups), Equals, 0)

	// Single line
	groups, err = getLogGroups("[2014-02-17 08:21:59] a\n")
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, groups, []string{
		"[2014-02-17 08:21:59] a\n",
	})

	// Trailing multiline
	log := "" +
		`[2014-02-17 08:21:59] a
[2014-02-17 08:21:59] b
  b
  b
`
	groups, err = getLogGroups(log)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, groups, []string{
		"[2014-02-17 08:21:59] a\n",
		`[2014-02-17 08:21:59] b
  b
  b
`})

	// Trailing single-line
	log = "" +
		`[2014-02-17 08:21:59] a
[2014-02-17 08:21:59] b
  b
  b
[2014-02-17 08:21:59] c
`
	groups, err = getLogGroups(log)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, groups, []string{
		"[2014-02-17 08:21:59] a\n",
		`[2014-02-17 08:21:59] b
  b
  b
`,
		"[2014-02-17 08:21:59] c\n",
	})
}

func (s *TestSuite) TestSimErrorsFindLoggedFatalErrors(c *C) {
	fp := bytes.NewBufferString(`[2014-01-14 11:07:28] a
[2014-01-14 11:07:28] <Simulation> <functERR> err1
[2014-01-14 11:07:28] b
[2014-01-14 11:07:28] <Simulation> <functERR> err2
`)
	result, err := FindLoggedFatalErrors(fp, &SessionErrorsOpts{})
	c.Assert(err, IsNil)
	c.Assert(result, Equals, `[2014-01-14 11:07:28] <Simulation> <functERR> err1
[2014-01-14 11:07:28] <Simulation> <functERR> err2
`)
}

func (s *TestSuite) TestSimErrorsStacktrace(c *C) {
	fp := bytes.NewBufferString(`[2014-01-14 11:07:28] a
[2014-01-14 11:07:28] b
[2014-01-14 11:07:28] c
[2014-01-14 11:07:28] <Simulation> <functERR> Crash - stack trace :
OS-Version: 6.1.7601 (Service Pack 1) 0x100-0x1
1 : trace1
2 : trace2
3 : trace3
[2014-01-14 11:07:28] d
`)
	trace, err := FindStacktrace(fp)
	c.Assert(err, IsNil)
	c.Assert(trace, Equals, ""+
		`[2014-01-14 11:07:28] <Simulation> <functERR> Crash - stack trace :
OS-Version: 6.1.7601 (Service Pack 1) 0x100-0x1
1 : trace1
2 : trace2
3 : trace3
`)
}

func (s *TestSuite) TestSimErrorsLuaStacktrace(c *C) {
	fp := bytes.NewBufferString("" +
		`[2014-02-17 10:37:54] <Logger plugin> <info> **** Time tick 11 - [15:20:38] - Report - a
[2014-02-17 10:37:54] <Logger plugin> <info> **** Time tick 11 - [15:20:38] - Trace - SAFETY.Police traceback:
    resources\integration/SimulationIntegration.lua:265: in function
    [C]: ?
    resources\integration/Object.lua:55: in function 'getObjectPositionsForWork'
[2014-02-17 10:37:54] <Logger plugin> <info> **** Time tick 11 - [15:20:38] - Report - b
`)
	trace, err := FindLuaStacktraces(fp)
	c.Assert(err, IsNil)
	c.Assert(trace, Equals, ""+
		`[2014-02-17 10:37:54] <Logger plugin> <info> **** Time tick 11 - [15:20:38] - Trace - SAFETY.Police traceback:
    resources\integration/SimulationIntegration.lua:265: in function
    [C]: ?
    resources\integration/Object.lua:55: in function 'getObjectPositionsForWork'
`)
}
