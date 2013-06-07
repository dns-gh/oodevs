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
	"time"
)

func (s *TestSuite) TestControlLocalWeatherCreation(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	model := client.Model
	weathers := model.GetData().LocalWeathers
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
		StartTime:   time.Now().AddDate(0, 0, +1),
		EndTime:     time.Now().AddDate(0, 0, -1),
		TopLeft:     swapi.Point{X: 1.0, Y: 2.0},
		BottomRight: swapi.Point{X: 3.0, Y: 4.0},
	}
	remote, err := client.CreateLocalWeather(&local)
	c.Assert(err, IsNil)
	c.Assert(remote, DeepEquals, &local)
	/*
		weathers = model.GetData().LocalWeathers
		c.Assert(weathers, HasLen, 1)
	*/
}
