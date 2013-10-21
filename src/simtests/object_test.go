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
	. "launchpad.net/gocheck"
	"swapi"
	"sword"
)

func (s *TestSuite) TestCreateEmptyObject(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Test invalid army identifier
	_, err := client.CreateDefaultObject("invalid", 1245, location)
	c.Assert(err, IsSwordError, "error_invalid_party")

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Test invalid object type
	_, err = client.CreateDefaultObject("invalid", party.Id, location)
	c.Assert(err, IsSwordError, "error_invalid_object")

	// Test valid
	object, err := client.CreateDefaultObject("jamming area", party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.ListObjects()) == 1
	})
}

func (s *TestSuite) TestDestroyEmptyObject(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create Object
	object, err := client.CreateDefaultObject("jamming area", party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	// Destroy invalid object
	err = client.DeleteObject(1234)
	c.Assert(err, IsSwordError, "error_invalid_object")

	// Test valid
	err = client.DeleteObject(object.Id)
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.ListObjects()) == 0
	})
}

func createObstacleAttributeParameter(activated bool, activation,
	activity int32) *sword.MissionParameter {
	return swapi.MakeParameter(
		swapi.MakeList(
			swapi.MakeIdentifier(uint32(sword.ObjectMagicAction_obstacle)), // attribute type
			swapi.MakeIdentifier(0),                                        // unused parameter
			swapi.MakeBoolean(activated),                                   // activated
			swapi.MakeQuantity(activation),                                 // activation time
			swapi.MakeQuantity(activity),                                   // activity time
		),
	)
}

func CheckTime(duration, expected, tick int32) error {
	value := expected / duration
	if tick >= value && tick <= value+2 {
		return nil
	}
	return fmt.Errorf("Difference between the expected and actual duration")
}

func (s *TestSuite) TestObstacleAttribute(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create mined area (linear and destructible) by default
	// object isn't activated
	object, err := client.CreateDefaultObject("mined area (linear and destructible)",
		party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)
	c.Assert(object.Activated, Equals, false)

	// Create mined area, activated by default
	object, err = client.CreateObject("mined area (linear and destructible)",
		party.Id, location, []*sword.MissionParameter{createObstacleAttributeParameter(true, 0, 0)})
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.FindObject(object.Id).Activated
	})

	// Create mined area, activated after a delay
	delay := int32(120) // 120 seconds
	object, err = client.CreateObject("mined area (linear and destructible)",
		party.Id, location, []*sword.MissionParameter{createObstacleAttributeParameter(false, delay, 0)})
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)
	c.Assert(object.Activated, Equals, false)

	tick := client.Model.GetData().Tick
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		if data.FindObject(object.Id).Activated {
			tick = data.Tick - tick
			return true
		}
		return false
	})
	// Check delay
	c.Assert(CheckTime(client.Model.GetData().TickDuration, delay, tick), IsNil)

	// Create mined area, activated by default with an activity time
	object, err = client.CreateObject("mined area (linear and destructible)",
		party.Id, location, []*sword.MissionParameter{createObstacleAttributeParameter(true, 0, delay)})
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	tick = 0
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		if data.FindObject(object.Id).Activated {
			tick = data.Tick
			return true
		}
		return false
	})

	// Wait area is disabled
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		if !data.FindObject(object.Id).Activated {
			tick = data.Tick - tick
			return true
		}
		return false
	})
	// Check delay
	c.Assert(CheckTime(client.Model.GetData().TickDuration, delay, tick), IsNil)
}

func (s *TestSuite) TestTimeLimitAttribute(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create mined area, activated by default
	delay := int32(120) // 120 seconds
	params := swapi.MakeParameter(
		swapi.MakeList(
			swapi.MakeIdentifier(uint32(sword.ObjectMagicAction_time_limit)), // attribute type
			swapi.MakeQuantity(delay),                                        // activity time
		),
	)
	object, err := client.CreateObject("mined area (linear and destructible)",
		party.Id, location, []*sword.MissionParameter{params})
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)
	tick := client.Model.GetData().Tick

	// Wait object is destroyed
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		if data.FindObject(object.Id) == nil {
			tick = data.Tick - tick
			return true
		}
		return false
	})
	// Check delay
	c.Assert(CheckTime(client.Model.GetData().TickDuration, delay, tick), IsNil)
}
