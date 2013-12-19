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

const (
	// Resource type with smoke effect with a 0 dispersion factor
	ResourceTypeWithSmokeNoDispersion = uint32(106)
	// Resource type with smoke effect with a 1 dispersion factor
	ResourceTypeWithSmokeDispersion = uint32(107)
)

func (s *TestSuite) TestFireOrderOnLocationMakesSmoke(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallTest))
	defer sim.Stop()
	point := swapi.Point{X: -15.8241, Y: 28.3241}

	// firing 1 round of smoke ammunition with a dispersion factor of 0
	err := client.CreateFireOnLocation(point, ResourceTypeWithSmokeNoDispersion, 1)
	c.Assert(err, IsNil)

	var major1, minor1 float64
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, effect := range data.FireEffects {
			c.Assert(effect.Type, Equals, sword.StartFireEffect_smoke)
			c.Assert(effect.Location.Type, Equals, sword.Location_ellipse)
			c.Assert(effect.Location.Points, HasLen, 3)
			c.Assert(point, IsNearby, *effect.Location.Points[0])
			major1 = Distance(point, *effect.Location.Points[1])
			minor1 = Distance(point, *effect.Location.Points[2])
			return true
		}
		return false
	})

	// waiting for the effect to end
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FireEffects) == 0
	})

	// firing 2 rounds of the same smoke ammunition
	err = client.CreateFireOnLocation(point, ResourceTypeWithSmokeNoDispersion, 2)
	c.Assert(err, IsNil)

	var major2, minor2 float64
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, effect := range data.FireEffects {
			c.Assert(effect.Type, Equals, sword.StartFireEffect_smoke)
			c.Assert(effect.Location.Type, Equals, sword.Location_ellipse)
			c.Assert(effect.Location.Points, HasLen, 3)
			c.Assert(point, IsNearby, *effect.Location.Points[0])
			major2 = Distance(point, *effect.Location.Points[1])
			minor2 = Distance(point, *effect.Location.Points[2])
			return true
		}
		return false
	})
	// effect is the same because dispersion factor is 0
	c.Assert(major1, Equals, major2)
	c.Assert(minor1, Equals, minor2)

	// waiting for the effect to end
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FireEffects) == 0
	})

	// firing 1 round of a different smoke ammunition with a dispersion factor of 1
	err = client.CreateFireOnLocation(point, ResourceTypeWithSmokeDispersion, 1)
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, effect := range data.FireEffects {
			c.Assert(effect.Type, Equals, sword.StartFireEffect_smoke)
			c.Assert(effect.Location.Type, Equals, sword.Location_ellipse)
			c.Assert(effect.Location.Points, HasLen, 3)
			c.Assert(point, IsNearby, *effect.Location.Points[0])
			major2 = Distance(point, *effect.Location.Points[1])
			minor2 = Distance(point, *effect.Location.Points[2])
			return true
		}
		return false
	})
	// effect is the same because 1 round doesn't get altered by dispersion factor
	c.Assert(major1, Equals, major2)
	c.Assert(minor1, Equals, minor2)

	// waiting for the effect to end
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FireEffects) == 0
	})

	// firing 2 rounds of this different smoke ammunition with a dispersion factor of 1
	err = client.CreateFireOnLocation(point, ResourceTypeWithSmokeDispersion, 2)
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, effect := range data.FireEffects {
			c.Assert(effect.Type, Equals, sword.StartFireEffect_smoke)
			c.Assert(effect.Location.Type, Equals, sword.Location_ellipse)
			c.Assert(effect.Location.Points, HasLen, 3)
			c.Assert(point, IsNearby, *effect.Location.Points[0])
			major2 = Distance(point, *effect.Location.Points[1])
			minor2 = Distance(point, *effect.Location.Points[2])
			return true
		}
		return false
	})
	// effect is increased by the number of rounds (when dispersion factor is 1)
	c.Assert(2*major1, IsClose, major2)
	c.Assert(2*minor1, IsClose, minor2)
}
