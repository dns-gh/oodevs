// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package simtests

import (
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/sword"
)

func ActivateRadarOnCircle(client *swapi.Client, unitId, radarClass uint32,
	center swapi.Point, radius float64) error {

	script := `
    function activateRadar()
        local center = DEC_Geometrie_CreerPointLatLong({{.lat}}, {{.long}})
        local loc = DEC_Geometrie_CreerLocalisationCercle(center, {{.radius}})
        local radar = DEC_Perception_ActiverRadarSurLocalisation({{.radarClass}}, loc)
        return tostring(radar)
    end`
	_, err := client.ExecTemplate(unitId, "activateRadar", script,
		map[string]interface{}{
			"lat":        center.Y,
			"long":       center.X,
			"radius":     radius,
			"radarClass": radarClass,
		})
	return err
}

func (s *TestSuite) TestSpecialSensor(c *C) {
	phydb := loadPhysical(c, "test")

	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallTest))
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()
	party1 := getPartyByName(c, data, "party1")
	formation1 := CreateFormation(c, client, party1.Id)
	kg1 := getSomeKnowledgeGroup(c, data, party1.Id)

	// Create radar
	automatType1 := getAutomatTypeFromName(c, phydb, "VW Combi Rally")
	automat1, err := client.CreateAutomat(formation1.Id, automatType1, kg1.Id)
	c.Assert(err, IsNil)
	unitType1 := getUnitTypeFromName(c, phydb, "VW Combi Radar")
	radar, err := client.CreateUnit(automat1.Id, unitType1,
		swapi.Point{X: -15.9268, Y: 28.3453})
	c.Assert(err, IsNil)

	// Create target
	party2 := getPartyByName(c, data, "party2")
	formation2 := CreateFormation(c, client, party2.Id)
	kg2 := getSomeKnowledgeGroup(c, data, party2.Id)
	automat2, err := client.CreateAutomat(formation2.Id, automatType1, kg2.Id)
	c.Assert(err, IsNil)
	unitType2 := getUnitTypeFromName(c, phydb, "VW Combi")
	target, err := client.CreateUnit(automat2.Id, unitType2,
		swapi.Point{X: -15.7591, Y: 28.3450})
	c.Assert(err, IsNil)

	waitAndCheckIfDetected := func(targetId uint32) bool {
		client.Model.WaitTicks(3)
		data := client.Model.GetData()
		for _, kg := range data.UnitKnowledges {
			if kg.KnowledgeGroupId == kg1.Id && kg.UnitId == targetId {
				return true
			}
		}
		return false
	}

	// Radar not activated: no detection
	c.Assert(waitAndCheckIfDetected(target.Id), Equals, false)

	// Activate it, should see something now
	err = ActivateRadarOnCircle(client, radar.Id, 0, target.Position, 10000)
	c.Assert(waitAndCheckIfDetected(target.Id), Equals, true)

	// This radar cannot see through sand-storm
	weatherData := swapi.LocalWeather{
		Weather: swapi.Weather{
			Temperature:   30,
			WindSpeed:     10,
			WindDirection: 90,
			CloudFloor:    0,
			CloudCeil:     2000,
			CloudDensity:  100,
			Precipitation: sword.WeatherAttributes_sand_storm,
			Lightning:     sword.WeatherAttributes_clear_day,
		},
		StartTime:   data.Time.AddDate(0, 0, -1),
		EndTime:     data.Time.AddDate(0, 0, +1),
		TopLeft:     swapi.Point{X: -15.8021, Y: 28.3856},
		BottomRight: swapi.Point{X: -15.7085, Y: 28.3170},
	}
	weather, err := client.CreateLocalWeather(&weatherData)
	c.Assert(err, IsNil)

	target, err = client.CreateUnit(automat2.Id, unitType2, target.Position)
	c.Assert(err, IsNil)
	c.Assert(waitAndCheckIfDetected(target.Id), Equals, false)

	// Should detect it once the storm quiets down
	err = client.DestroyLocalWeather(weather.Id)
	c.Assert(err, IsNil)
	c.Assert(waitAndCheckIfDetected(target.Id), Equals, true)
}
