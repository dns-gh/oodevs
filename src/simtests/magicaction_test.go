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
	// Resource type without indirect fire
	ResourceTypeWithoutIndirectFire = uint32(1)
	// Resource type with indirect fire
	ResourceTypeWithIndirectFire = uint32(8)
)

func (s *TestSuite) TestControlLocalWeatherCreation(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.Time.IsZero()
	})

	model := client.Model.GetData()
	weathers := model.LocalWeathers
	c.Assert(weathers, HasLen, 0)
	local := swapi.LocalWeather{
		Weather: swapi.Weather{
			Temperature:   30,
			WindSpeed:     10,
			WindDirection: 90,
			CloudFloor:    1000,
			CloudCeil:     2000,
			CloudDensity:  100,
			Precipitation: sword.WeatherAttributes_rain,
			Lightning:     sword.WeatherAttributes_full_moon_night,
		},
		StartTime:   model.Time.AddDate(0, 0, -1),
		EndTime:     model.Time.AddDate(0, 0, +1),
		TopLeft:     swapi.Point{X: 1, Y: 2},
		BottomRight: swapi.Point{X: 3, Y: 4},
	}
	remote, err := client.CreateLocalWeather(&local)
	c.Assert(err, IsNil)
	c.Assert(remote, Not(IsNil))
	local.Id = remote.Id

	// overwrite lighting which is not supported yet
	remote.Lightning = local.Lightning
	// ignore precision issues when comparing points
	if Nearby(remote.TopLeft, local.TopLeft) {
		remote.TopLeft = local.TopLeft
	}
	if Nearby(remote.BottomRight, local.BottomRight) {
		remote.BottomRight = local.BottomRight
	}
	c.Assert(remote, DeepEquals, &local)

	weathers = client.Model.GetData().LocalWeathers
	c.Assert(weathers, HasLen, 1)
}

func (s *TestSuite) TestFireOrderCreation(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.Time.IsZero()
	})

	// error: invalid parameters count, 3 parameters expected
	params := swapi.MakeParameters()
	err := client.CreateFireOnLocation(params)
	c.Assert(err, ErrorMatches, "error_invalid_target")

	// error: first parameter must be a location or a point
	params = swapi.MakeParameters(
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakeNullValue())
	err = client.CreateFireOnLocation(params)
	c.Assert(err, ErrorMatches, "error_invalid_target")

	params.Elem[0] = swapi.MakePointParam(swapi.Point{X: -15.8241, Y: 28.3241})

	// error: second parameter must be a resource type
	err = client.CreateFireOnLocation(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	params.Elem[1] = swapi.MakeResourceType(uint32(12345))

	// error: second parameter must be a valid resource type
	err = client.CreateFireOnLocation(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	params.Elem[1] = swapi.MakeResourceType(ResourceTypeWithoutIndirectFire)

	// error: second parameter must be a resource type with a indirect fire
	err = client.CreateFireOnLocation(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	params.Elem[1] = swapi.MakeResourceType(ResourceTypeWithIndirectFire)

	// error: third parameter must be a real
	err = client.CreateFireOnLocation(params)
	c.Assert(err, ErrorMatches, "error_invalid_iteration")

	params.Elem[2] = swapi.MakeFloat(float32(5))

	// indirect fire with a point
	err = client.CreateFireOnLocation(params)
	c.Assert(err, IsNil)

	params.Elem[0] = swapi.MakeLocationParam(swapi.Point{X: -15.8241, Y: 28.3241})

	// indirect fire with a location
	err = client.CreateFireOnLocation(params)
	c.Assert(err, IsNil)
}
