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

func (s *TestSuite) TestCreateEmptyObject(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Test invalid army identifier
	_, err := client.CreateObject("invalid", 1245, location)
	c.Assert(err, IsSwordError, "error_invalid_object")

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Test invalid object type
	_, err = client.CreateObject("invalid", party.Id, location)
	c.Assert(err, IsSwordError, "error_invalid_object")

	// Test valid
	object, err := client.CreateObject("jamming area", party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Objects) == 1
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
	object, err := client.CreateObject("jamming area", party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	// Destroy invalid object
	err = client.DeleteObject(1234)
	c.Assert(err, IsSwordError, "error_invalid_object")

	// Test valid
	err = client.DeleteObject(object.Id)
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Objects) == 0
	})
}

func createObstacleAttributeParameter(activated bool, activation,
	activity int32) *sword.MissionParameter_Value {
	return swapi.MakeList(
		swapi.MakeIdentifier(uint32(sword.ObjectMagicAction_obstacle)), // attribute type
		swapi.MakeIdentifier(0),                                        // unused parameter
		swapi.MakeBoolean(activated),                                   // activated
		swapi.MakeQuantity(activation),                                 // activation time
		swapi.MakeQuantity(activity),                                   // activity time
	)
}

// Registers a handler to be called with the next object added to the model
// until it is destroyed or the handler returns true. The last call notifying
// the destruction is done with a nil Object.
func watchNextObject(model *swapi.Model,
	handler func(model *swapi.ModelData, obj *swapi.Object) bool) chan error {

	done := make(chan error)
	objects := model.GetData().Objects
	objectId := uint32(0)
	model.RegisterHandler(func(model *swapi.ModelData, err error) bool {
		if err != nil {
			done <- err
			return true
		}
		result := false
		if objectId == 0 {
			if len(objects) == len(model.Objects) {
				return false
			}
			for id, obj := range model.Objects {
				if objects[id] == nil {
					objectId = obj.Id
					break
				}
			}
		} else {
			if model.Objects[objectId] == nil {
				result = true
			}
		}
		if handler(model, model.Objects[objectId]) {
			result = true
		}
		if result {
			done <- nil
		}
		return result
	})
	return done
}

func checkBetween(c *C, value, minInclusive, maxInclusive int32) {
	c.Assert(value, Greater, minInclusive-1)
	c.Assert(value, Lesser, maxInclusive+1)
}

func checkTime(c *C, duration, expected, tick int32) {
	value := expected / duration
	checkBetween(c, tick, value, value+2)
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
	object, err := client.CreateObject("mined area (linear and destructible)",
		party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)
	c.Assert(object.Activated, Equals, false)

	// Create mined area, activated by default
	object, err = client.CreateObject("mined area (linear and destructible)",
		party.Id, location, createObstacleAttributeParameter(true, 0, 0))
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Objects[object.Id].Activated
	})

	// Create mined area, activated after a delay
	duration := int32(0)
	done := watchNextObject(client.Model, func(model *swapi.ModelData,
		object *swapi.Object) bool {

		if duration == 0 {
			duration = model.Tick
		}
		if object != nil && object.Activated {
			duration = model.Tick - duration
			return true
		}
		return false
	})
	delay := int32(120) // 120 seconds
	object, err = client.CreateObject("mined area (linear and destructible)",
		party.Id, location, createObstacleAttributeParameter(false, delay, 0))
	c.Assert(err, IsNil)
	c.Assert(object.Activated, Equals, false)

	// Check delay
	err = <-done
	c.Assert(err, IsNil)
	checkTime(c, data.TickDuration, delay, duration)

	// Create mined area, activated by default with an activity time
	duration = int32(0)
	done = watchNextObject(client.Model, func(model *swapi.ModelData,
		object *swapi.Object) bool {

		if object != nil {
			if duration == 0 && object.Activated {
				duration = model.Tick
			} else if duration != 0 && !object.Activated {
				duration = model.Tick - duration
				return true
			}
		}
		return false
	})
	object, err = client.CreateObject("mined area (linear and destructible)",
		party.Id, location, createObstacleAttributeParameter(true, 0, delay))
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	// Check delay
	err = <-done
	checkTime(c, data.TickDuration, delay, duration)
}

func (s *TestSuite) TestTimeLimitAttribute(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	data := client.Model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create mined area, activated by default
	delay := int32(120) // 120 seconds
	params := swapi.MakeList(
		swapi.MakeIdentifier(uint32(sword.ObjectMagicAction_time_limit)), // attribute type
		swapi.MakeQuantity(delay),                                        // activity time
	)

	duration := int32(0)
	done := watchNextObject(client.Model, func(model *swapi.ModelData,
		object *swapi.Object) bool {

		if duration == 0 {
			duration = model.Tick
		}
		if object == nil {
			duration = model.Tick - duration
		}
		return object == nil
	})
	object, err := client.CreateObject("mined area (linear and destructible)",
		party.Id, location, params)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	// Check delay
	err = <-done
	c.Assert(err, IsNil)
	checkTime(c, data.TickDuration, delay, duration)
}

func (s *TestSuite) TestBypassAttribute(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create mined area by default
	object, err := client.CreateObject("mined area (linear and destructible)",
		party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)
	c.Assert(object.Bypass, Equals, int32(0))

	// Create mined area with bypass attribute(29%)
	percentage := int32(29)
	params := swapi.MakeList(
		swapi.MakeIdentifier(uint32(sword.ObjectMagicAction_bypass)), // attribute type
		swapi.MakeQuantity(percentage),                               // % of bypass
	)
	object, err = client.CreateObject("mined area (linear and destructible)",
		party.Id, location, params)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	// Check bypass
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Objects[object.Id].Bypass == percentage
	})
}

func (s *TestSuite) TestAltitudeAttribute(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create mined area by default
	object, err := client.CreateObject("dyke", party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)
	c.Assert(object.Altitude, Equals, int32(0))

	// Create dyke of 122m
	altitude := int32(122)
	params := swapi.MakeList(
		swapi.MakeIdentifier(uint32(sword.ObjectMagicAction_altitude_modifier)), // attribute type
		swapi.MakeQuantity(altitude),                                            // height
	)
	object, err = client.CreateObject("mined area (linear and destructible)",
		party.Id, location, params)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	// Check altitude
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Objects[object.Id].Altitude == altitude
	})
}

func (s *TestSuite) TestUpdateConstructionAttribute(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	location := swapi.MakePointLocation(swapi.Point{X: -15.8193, Y: 28.3456})

	// Get a party identifier
	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	// Create Object
	object, err := client.CreateObject("installation", party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	// Check object is constructed
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Objects[object.Id].Construction == 100
	})

	// Update construction attribute
	params := swapi.MakeList(
		swapi.MakeIdentifier(uint32(sword.ObjectMagicAction_construction)), // attribute type
		swapi.MakeIdentifier(0),                                            // unused
		swapi.MakeQuantity(0),                                              // unused
		swapi.MakeFloat(0),                                                 // unused
		swapi.MakeQuantity(5),                                              // % construction
	)
	err = client.UpdateObject(object.Id, params)
	c.Assert(err, IsNil)

	// Check object is updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Objects[object.Id].Construction == 5
	})
}
