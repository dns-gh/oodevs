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
