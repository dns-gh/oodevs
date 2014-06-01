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
	. "launchpad.net/gocheck"
	"strings"
	"swapi"
	"swapi/simu"
)

func checkEcho(c *C, client *swapi.Client, s string) {
	result, err := client.Echo(s)
	c.Assert(err, IsNil)
	c.Assert(result, Equals, s)
}

func (s *TestSuite) TestEcho(c *C) {
	opts, session := makeOptsAndSession()
	opts.TestCommands = true
	opts.ExerciseName = ExCrossroadSmallEmpty
	WriteSession(c, opts, session)
	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil)
	defer stopSim(c, sim, nil)
	client := connectAndWait(c, sim, "admin", "")
	defer client.Close()

	checkEcho(c, client, "")
	checkEcho(c, client, "something")

	for i := 0; i != 31; i++ {
		checkEcho(c, client, strings.Repeat("a", i*1024))
	}
}
