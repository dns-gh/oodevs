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
	point := swapi.Point{X: -15.8241, Y: 28.3241}

	// error: invalid parameters count, 3 parameters expected
	params := swapi.MakeParameters()
	err := client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_target")

	// error: first parameter must be a location or a point
	params = swapi.MakeParameters(
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakeNullValue())
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_target")

	// error: second parameter must be a resource type
	params.Elem[0] = swapi.MakePointParam(point)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	// error: second parameter must be a valid resource type
	params.Elem[1] = swapi.MakeResourceType(uint32(12345))
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	// error: second parameter must be a resource type with a indirect fire
	params.Elem[1] = swapi.MakeResourceType(ResourceTypeWithoutIndirectFire)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	// error: third parameter must be a real
	params.Elem[1] = swapi.MakeResourceType(ResourceTypeWithIndirectFire)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_iteration")

	// error: third parameter must be a positive real number
	params.Elem[2] = swapi.MakeFloat(float32(-3))
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_iteration")

	// indirect fire with a point
	params.Elem[2] = swapi.MakeFloat(float32(5))
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsNil)

	// indirect fire with a location
	params.Elem[0] = swapi.MakeLocationParam(point)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsNil)

	err = client.CreateFireOnLocation(point, ResourceTypeWithIndirectFire, 2)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestResourceNetworkChange(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid parameters count, 2 parameters expected
	params := swapi.MakeParameters()
	err := client.ChangeResourceNetworkTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: first parameter must be a location or an identifier
	params = swapi.MakeParameters(
		swapi.MakeNullValue(),
		swapi.MakeNullValue())
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(1000))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// first parameter with a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(21))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsNil)

	// error: second parameter must be a list with valid resource names
	params.Elem[1] = swapi.MakeParameter(
		swapi.MakeList(swapi.MakeString("Running Water")),
		swapi.MakeList(swapi.MakeString("invalid resource name")))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// second parameter with valid resource names
	params.Elem[1] = swapi.MakeParameter(
		swapi.MakeList(swapi.MakeString("Running Water")),
		swapi.MakeList(swapi.MakeString("Natural Gas")))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsNil)
}
