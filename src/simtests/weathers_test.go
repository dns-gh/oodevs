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

func (s *TestSuite) TestControlGlobalWeather(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model.GetData()

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return !data.Time.IsZero()
	})

	// Check initial global meteo(weather.xml)
	expected := swapi.Weather{Temperature: 20,
		WindSpeed:     0,
		WindDirection: 0,
		CloudFloor:    0,
		CloudCeil:     0,
		CloudDensity:  0,
		Precipitation: sword.WeatherAttributes_dry,
		Lightning:     sword.WeatherAttributes_clear_day,
	}
	actual := model.GlobalWeather
	c.Assert(expected, Equals, actual)

	// Error: missing parameters
	err := client.UpdateGlobalWeather(nil)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error: wind speed negative
	expected.WindSpeed = -6
	err = client.UpdateGlobalWeather(&expected)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error: cloud floor negative
	expected.WindSpeed = 0
	expected.CloudFloor = -5
	err = client.UpdateGlobalWeather(&expected)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error: cloud ceil negative
	expected.CloudFloor = 0
	expected.CloudCeil = -5
	err = client.UpdateGlobalWeather(&expected)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error: cloud density negative
	expected.CloudCeil = 0
	expected.CloudDensity = -5
	err = client.UpdateGlobalWeather(&expected)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error: precipitation enumeration invalid
	expected.CloudDensity = 0
	expected.Precipitation = 12345
	err = client.UpdateGlobalWeather(&expected)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Error: invalid direction
	expected.Precipitation = 0
	expected.WindDirection = 400
	err = client.UpdateGlobalWeather(&expected)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")
	expected.WindDirection = -400
	err = client.UpdateGlobalWeather(&expected)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Update global weather
	expected = swapi.Weather{Temperature: 10,
		WindSpeed:     1,
		WindDirection: 2,
		CloudFloor:    3,
		CloudCeil:     4,
		CloudDensity:  5,
		Precipitation: sword.WeatherAttributes_snow,
		Lightning:     sword.WeatherAttributes_clear_day,
	}
	err = client.UpdateGlobalWeather(&expected)
	c.Assert(err, IsNil)

	// Wait and check update global weather
	client.Model.WaitTicks(2)
	actual = client.Model.GetData().GlobalWeather
	c.Assert(expected, Equals, actual)
}

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
