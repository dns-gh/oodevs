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
	. "launchpad.net/gocheck"
	"masa/sword/swtest"
	"path"
)

func (s *TestSuite) TestParsingError(c *C) {
	data := `
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<session>
</session>
`
	_, err := ReadSession([]byte(data))
	c.Assert(err, NotNil)
}

func (s *TestSuite) TestParsing(c *C) {
	data := `
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<session>
    <config>
        <gaming>
            <network server="localhost:10001"/>
        </gaming>
    </config>
</session>
`
	session, err := ReadSession([]byte(data))
	c.Assert(err, IsNil)
	c.Assert(session.GamingServer, Matches, "localhost:10001")
}

func (s *TestSuite) TestCreateDefaultSession(c *C) {
	session := CreateDefaultSession()
	c.Assert(session, NotNil)
}

func (s *TestSuite) TestWriteSession(c *C) {
	session := CreateDefaultSession()
	session.Recorder = &RecorderPlugin{
		FragmentFreq: 201,
		KeyframeFreq: 101,
	}
	session.GamingServer = "masagroup.net"
	session.EndTick = 42
	session.Paused = true
	session.RandomBreakdowns = true
	session.Seed = 1
	session.TimeFactor = 999
	session.TimeStep = 333
	data, err := WriteSession(session)
	c.Assert(err, IsNil)

	expected := `<session>
  <config>
    <dispatcher>
      <network client="localhost:10000" server="masagroup.net"></network>
      <plugins>
        <recorder fragmentfreq="201" keyframefreq="101"></recorder>
      </plugins>
    </dispatcher>
    <gaming>
      <network server="masagroup.net"></network>
    </gaming>
    <simulation>
      <GarbageCollector setpause="100" setstepmul="100"></GarbageCollector>
      <checkpoint frequency="100000h" keep="1" usecrc="true"></checkpoint>
      <debug decisional="false" diadebugger="false" diadebuggerport="15000" networklogger="true" networkloggerport="20000" pathfind="false" random-breakdowns="true"></debug>
      <decisional useonlybinaries="false"></decisional>
      <dispatcher embedded="true"></dispatcher>
      <network port="10000"></network>
      <orbat checkcomposition="false"></orbat>
      <pathfinder threads="1" max-calculation-time="12h"></pathfinder>
      <profiling enabled="false" command="false" hook="false"></profiling>
      <random seed="1"></random>
      <random0 deviation="0.5" distribution="0" mean="0.5"></random0>
      <random1 deviation="0.5" distribution="0" mean="0.5"></random1>
      <random2 deviation="0.5" distribution="0" mean="0.5"></random2>
      <random3 deviation="0.5" distribution="0" mean="0.5"></random3>
      <time end-tick="42" latency="0" factor="999" paused="true" step="333"></time>
    </simulation>
  </config>
  <meta>
    <comment></comment>
    <date>20101109T141911</date>
    <name>test</name>
  </meta>
</session>`
	swtest.DeepEquals(c, string(data), expected)
}

func createTestSession() *Session {
	s := CreateDefaultSession()
	s.EndTick = 55
	return s
}

func checkSessionFile(c *C, sessionPath string) {
	s2, err := ReadSessionFile(sessionPath)
	c.Assert(err, IsNil)
	c.Assert(s2.EndTick, Equals, 55)
}

func (s *TestSuite) TestWriteSessionFile(c *C) {
	tempDir := c.MkDir()

	session := createTestSession()
	p := path.Join(tempDir, "dirtocreate", "session.xml")
	err := WriteSessionFile(session, p)
	c.Assert(err, IsNil)
	checkSessionFile(c, p)
}

func (s *TestSuite) TestWriteNewSessionFile(c *C) {
	tempDir := c.MkDir()

	session := createTestSession()
	p, err := WriteNewSessionFile(session, tempDir)
	c.Assert(err, IsNil)
	checkSessionFile(c, p)
}
