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
	"testing"
)

func stopSim(c *C, sim *SimProcess) {
	sim.Stop()
	if c.Failed() {
		return
	}
	session := sim.Opts.GetSessionDir()
	err := CheckSessionErrors(session, nil)
	if err != nil {
		c.Fatalf("errors found in session %s:\n%s", session, err)
	}
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})
