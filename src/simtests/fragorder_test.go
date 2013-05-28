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
	"swapi"
)

const (
	FragOrderNbcSuitOn = uint32(345)
)

func (s *TestSuite) TestAutomatFragOrder(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	automat := createAutomat(c, client)
	from := swapi.MakePoint(-15.9219, 28.3456)
	_, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	params := swapi.MakeParameters()

	// Cannot send frag order with an invalid automat identifier
	err = client.SendFragOrder(InvalidIdentifier, FragOrderNbcSuitOn, params)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Cannot send frag order with an invalid frag order identifier
	err = client.SendFragOrder(automat.Id, InvalidIdentifier, params)
	c.Assert(err, ErrorMatches, "error_invalid_frag_order")

	// Disengage automat
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	// Cannot send frag order to an disengaged automat
	err = client.SendFragOrder(automat.Id, FragOrderNbcSuitOn, params)
	c.Assert(err, ErrorMatches, "error_unit_cannot_receive_order")

	// Engage automat
	err = client.SetAutomatMode(automat.Id, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automat.Id).Engaged
	})

	err = client.SendFragOrder(automat.Id, FragOrderNbcSuitOn, params)
	c.Assert(err, IsNil)
}
