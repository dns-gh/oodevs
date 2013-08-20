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

func (s *TestSuite) TestUnitVisionCones(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// forcing unit posture to make it stable
	err := client.ChangePosture(11, swapi.MakeParameters(swapi.MakeEnumeration(5)))
	c.Assert(err, IsNil)

	automatId := client.Model.GetUnit(11).AutomatId
	// disengaging unit automat to be able to send all needed magic actions
	err = client.SetAutomatMode(automatId, false)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.FindAutomat(automatId).Engaged
	})

	received := false
	listener := func(msg *swapi.SwordMessage, context int32, err error) bool {
		if msg != nil && msg.SimulationToClient != nil {
			m := msg.SimulationToClient.GetMessage().GetUnitVisionCones()
			if m != nil && m.GetUnit().GetId() == 11 {
				received = true
			}
		}
		return false
	}
	client.Register(listener)

	// no vision cones received by default
	received = false
	client.Model.WaitTicks(1)
	c.Assert(received, Equals, false)

	// registration fails when unit does not exist
	err = client.SendControlEnableVisionCones(1000, true)
	c.Assert(err, ErrorMatches, "error_invalid_unit: invalid unit identifier 1000")

	check := func(err error) {
		c.Assert(err, IsNil)
		received = false
		waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
			return received
		})
		c.Assert(received, Equals, true)
		// vision cones not sent again if unit state hasn't changed
		received = false
		client.Model.WaitTicks(1)
		c.Assert(received, Equals, false)
	}

	// registering allows to receive vision cones
	check(client.SendControlEnableVisionCones(11, true))
	// vision cones sent after posture changes
	check(client.ChangePosture(11, swapi.MakeParameters(swapi.MakeEnumeration(5))))
	// vision cones sent after position changes
	check(client.TeleportUnit(11, swapi.Point{X: -15.8219, Y: 28.2456}))
	// vision cones sent after direction changes
	// $$$$ MCO : we need a magic action
	// vision cones sent after speed changes
	// $$$$ MCO : we need a magic action
	// vision cones sent after elongation factor changes
	// $$$$ MCO : we need a magic action
	// vision cones sent after (un)loading
	// $$$$ MCO : we need a magic action
	// vision cones sent after being (un)loaded
	// $$$$ MCO : we need a magic action
	// vision cones sent after components changed
	check(client.TransferEquipment(11, 12, []swapi.Equipment{{11, 1}}))
	// vision cones sent after unit surrenders
	check(client.Surrender(automatId, 2))
	// vision cones sent after unit cancels surrender
	check(client.CancelSurrender(automatId))
	// visions cones not sent anymore after unregistering
	err = client.SendControlEnableVisionCones(11, false)
	c.Assert(err, IsNil)
	err = client.Surrender(automatId, 2)
	c.Assert(err, IsNil)
	received = false
	client.Model.WaitTicks(1)
	c.Assert(received, Equals, false)
}

func (s *TestSuite) TestListVisionCones(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	ack, err := client.SendListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	c.Assert(len(ack.GetUnits()), Equals, 0)
	c.Assert(ack.GetStart().GetId(), Equals, uint32(0))
	c.Assert(ack.GetCount(), Equals, uint32(0))

	err = client.SendControlEnableVisionCones(12, true)
	c.Assert(err, IsNil)
	err = client.SendControlEnableVisionCones(13, true)
	c.Assert(err, IsNil)
	err = client.SendControlEnableVisionCones(11, true)
	c.Assert(err, IsNil)

	ack, err = client.SendListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	c.Assert(ack.GetAll(), Equals, false)
	c.Assert(len(ack.GetUnits()), Equals, 3)
	c.Assert(ack.GetUnits()[0].GetId(), Equals, uint32(11))
	c.Assert(ack.GetUnits()[1].GetId(), Equals, uint32(12))
	c.Assert(ack.GetUnits()[2].GetId(), Equals, uint32(13))
	c.Assert(ack.GetStart().GetId(), Equals, uint32(0))
	c.Assert(ack.GetCount(), Equals, uint32(3))

	ack, err = client.SendListEnabledVisionCones(0, 1)
	c.Assert(err, IsNil)
	c.Assert(ack.GetAll(), Equals, false)
	c.Assert(len(ack.GetUnits()), Equals, 1)
	c.Assert(ack.GetUnits()[0].GetId(), Equals, uint32(11))
	c.Assert(ack.GetStart().GetId(), Equals, uint32(0))
	c.Assert(ack.GetCount(), Equals, uint32(3))

	ack, err = client.SendListEnabledVisionCones(12, 1)
	c.Assert(err, IsNil)
	c.Assert(ack.GetAll(), Equals, false)
	c.Assert(len(ack.GetUnits()), Equals, 1)
	c.Assert(ack.GetUnits()[0].GetId(), Equals, uint32(12))
	c.Assert(ack.GetStart().GetId(), Equals, uint32(12))
	c.Assert(ack.GetCount(), Equals, uint32(3))

	err = client.SendControlEnableAllVisionCones(true)
	c.Assert(err, IsNil)
	ack, err = client.SendListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	c.Assert(ack.GetAll(), Equals, true)
	c.Assert(len(ack.GetUnits()), Equals, 0)
	c.Assert(ack.GetStart().GetId(), Equals, uint32(0))
	c.Assert(ack.GetCount(), Equals, uint32(0))

	err = client.SendControlEnableVisionCones(11, true)
	c.Assert(err, IsNil)
	ack, err = client.SendListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	c.Assert(ack.GetAll(), Equals, false)
	c.Assert(len(ack.GetUnits()), Equals, 1)
	c.Assert(ack.GetUnits()[0].GetId(), Equals, uint32(11))
	c.Assert(ack.GetStart().GetId(), Equals, uint32(0))
	c.Assert(ack.GetCount(), Equals, uint32(1))

	err = client.SendControlEnableAllVisionCones(false)
	c.Assert(err, IsNil)
	ack, err = client.SendListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	c.Assert(ack.GetAll(), Equals, false)
	c.Assert(len(ack.GetUnits()), Equals, 0)
	c.Assert(ack.GetStart().GetId(), Equals, uint32(0))
	c.Assert(ack.GetCount(), Equals, uint32(0))
}
