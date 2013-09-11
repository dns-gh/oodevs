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
)

func (s *TestSuite) TestSetAutomatMode(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	c.Assert(automat.Engaged, Equals, true)

	checkEngage := func(engagedId, waitId uint32, engage bool) {
		err := client.SetAutomatMode(engagedId, engage)
		c.Assert(err, IsNil)
	}

	// Invalid automat identifier
	err := client.SetAutomatMode(12456, false)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// This one should work
	checkEngage(automat.Id, automat.Id, false)

	// Engage it again
	checkEngage(automat.Id, automat.Id, true)

	subAutomat, err := client.CreateAutomat(0, automat.Id, AutomatType,
		automat.KnowledgeGroupId)
	c.Assert(err, IsNil)

	// Changing the sub automat mode fails if parent is engaged (why?)
	err = client.SetAutomatMode(subAutomat.Id, true)
	c.Assert(err, IsSwordError, "error_not_allowed")
	err = client.SetAutomatMode(subAutomat.Id, false)
	c.Assert(err, IsSwordError, "error_not_allowed")

	// Disengaging the parent, does not disengage the children
	checkEngage(automat.Id, automat.Id, false)
	subAutomat = client.Model.GetAutomat(subAutomat.Id)
	c.Assert(subAutomat.Engaged, Equals, true)

	// The sub automat can be disengaged alone
	checkEngage(subAutomat.Id, subAutomat.Id, false)

	// Reengaging the parent, reengage the child
	checkEngage(automat.Id, subAutomat.Id, true)
}
