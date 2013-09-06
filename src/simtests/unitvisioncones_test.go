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
	"sword"
)

func (s *TestSuite) TestUnitVisionCones(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()
	client2 := loginAndWaitModel(c, sim, "admin", "")
	client2.Register(func(msg *swapi.SwordMessage, context int32, err error) bool {
		if msg != nil && msg.SimulationToClient != nil {
			// should not receive vision cones updates on another client
			c.Assert(msg.SimulationToClient.GetMessage().GetUnitVisionCones(), IsNil)
		}
		return false
	})

	// forcing unit posture to make it stable
	err := client.ChangePosture(11, sword.UnitAttributes_parked_on_self_prepared_area)
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
	err = client.EnableVisionCones(true, 1000)
	c.Assert(err, ErrorMatches, "error_invalid_unit: invalid unit identifier 1000")

	check := func(err error) {
		c.Assert(err, IsNil)
		waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
			return received
		})
		// vision cones not sent again if unit state hasn't changed
		received = false
		client.Model.WaitTicks(1)
		c.Assert(received, Equals, false)
	}

	// registering allows to receive vision cones
	check(client.EnableVisionCones(true, 11))
	// vision cones sent after posture changes
	check(client.ChangePosture(11, sword.UnitAttributes_parked_on_self_prepared_area))
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
	err = client.EnableVisionCones(false, 11)
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

	check := func(ack *sword.ListEnabledVisionConesAck, all bool, units []int, start int, count int) {
		c.Assert(ack.GetAll(), Equals, all)
		c.Assert(len(ack.GetUnits()), Equals, len(units))
		for i, unitId := range ack.GetUnits() {
			c.Assert(unitId.GetId(), Equals, uint32(units[i]))
		}
		c.Assert(ack.GetStart().GetId(), Equals, uint32(start))
		c.Assert(ack.GetCount(), Equals, uint32(count))
	}

	// no unit registered by default
	ack, err := client.ListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	check(ack, false, nil, 0, 0)

	// 3 units registered listed
	err = client.EnableVisionCones(true, 12, 13, 11)
	c.Assert(err, IsNil)
	ack, err = client.ListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	check(ack, false, []int{11, 12, 13}, 0, 3)

	// 3 units registered only first one listed
	ack, err = client.ListEnabledVisionCones(0, 1)
	c.Assert(err, IsNil)
	check(ack, false, []int{11}, 0, 3)

	// 3 units registered only second one listed
	// when starting from the second one
	ack, err = client.ListEnabledVisionCones(12, 1)
	c.Assert(err, IsNil)
	check(ack, false, []int{12}, 12, 3)

	// globally enabling vision cones overwrites singely registered units
	err = client.EnableVisionCones(true)
	c.Assert(err, IsNil)
	ack, err = client.ListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	check(ack, true, nil, 0, 0)

	// singlely registered units are ignored when globally enabled
	err = client.EnableVisionCones(true, 11)
	c.Assert(err, IsNil)
	ack, err = client.ListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	check(ack, true, nil, 0, 0)

	// singlely unregistering units when globally enabled => error
	err = client.EnableVisionCones(false, 11)
	c.Assert(err, ErrorMatches, "error_invalid_unit: cannot unregister a single unit with vision cones globally enabled")

	// globally disabling vision cones disables all updates
	err = client.EnableVisionCones(false)
	c.Assert(err, IsNil)
	ack, err = client.ListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	check(ack, false, nil, 0, 0)

	// globally disabling vision cones overwrites singlely registered units
	err = client.EnableVisionCones(true, 11)
	c.Assert(err, IsNil)
	err = client.EnableVisionCones(false)
	c.Assert(err, IsNil)
	ack, err = client.ListEnabledVisionCones(0, 10)
	c.Assert(err, IsNil)
	check(ack, false, nil, 0, 0)
}
