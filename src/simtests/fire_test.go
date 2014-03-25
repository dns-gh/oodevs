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
	defer stopSimAndClient(c, sim, client)
	point := swapi.Point{X: -15.8241, Y: 28.3241}

	getEffectEllipse := func() (float64, float64) {
		var effect swapi.FireEffect
		waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
			for _, e := range data.FireEffects {
				effect = *e
				return true
			}
			return false
		})
		c.Assert(effect, NotNil)
		c.Assert(effect.Type, Equals, sword.StartFireEffect_smoke)
		c.Assert(effect.Location.Type, Equals, sword.Location_ellipse)
		c.Assert(effect.Location.Points, HasLen, 3)
		c.Assert(point, IsNearby, effect.Location.Points[0])
		return Distance(point, effect.Location.Points[1]),
			Distance(point, effect.Location.Points[2])
	}

	// firing 1 round of smoke ammunition with a dispersion factor of 0
	err := client.CreateFireOnLocation(point, ResourceTypeWithSmokeNoDispersion, 1)
	c.Assert(err, IsNil)

	major1, minor1 := getEffectEllipse()

	// waiting for the effect to end
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FireEffects) == 0
	})

	// firing 2 rounds of the same smoke ammunition
	err = client.CreateFireOnLocation(point, ResourceTypeWithSmokeNoDispersion, 2)
	c.Assert(err, IsNil)
	major2, minor2 := getEffectEllipse()
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
	major2, minor2 = getEffectEllipse()
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
	major2, minor2 = getEffectEllipse()
	// effect is increased by the number of rounds (when dispersion factor is 1)
	c.Assert(2*major1, IsClose, major2)
	c.Assert(2*minor1, IsClose, minor2)
}

func setupFireTest(c *C, client *swapi.Client) (*swapi.Unit, *swapi.Unit) {
	phydb := loadPhysical(c, "worldwide")
	d := client.Model.GetData()
	// create artillery
	CreateFormation(c, client, d.FindPartyByName("party1").Id)
	automat1 := createAutomatForParty(c, client, "party1")
	err := client.SetAutomatMode(automat1.Id, false)
	c.Assert(err, IsNil)
	from := swapi.Point{X: -15.716149, Y: 28.429614}
	firer, err := client.CreateUnit(automat1.Id,
		getUnitTypeFromName(c, phydb, "ART.Artillery firing platoon"),
		from)
	c.Assert(err, IsNil)
	// create radar
	CreateFormation(c, client, d.FindPartyByName("party2").Id)
	automat2 := createAutomatForParty(c, client, "party2")
	err = client.SetAutomatMode(automat2.Id, false)
	c.Assert(err, IsNil)
	watcher, err := client.CreateUnit(automat2.Id,
		getUnitTypeFromName(c, phydb, "ART.Firefinder radar"),
		swapi.Point{X: -15.8079, Y: 28.3373})
	c.Assert(err, IsNil)
	// deploy firer
	MissionArtilleryDeploy := uint32(44580)
	_, err = client.SendUnitOrder(firer.Id, MissionArtilleryDeploy,
		swapi.MakeParameters(
			swapi.MakeHeading(0),
			nil, nil, nil,
			swapi.MakeLocationParam(from),
		))
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[firer.Id].Installation == 100
	})
	return firer, watcher
}

func (s *TestSuite) TestIndirectFireMakesFlyingShell(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	firer, watcher := setupFireTest(c, client)
	automat := client.Model.GetAutomat(watcher.AutomatId)
	// detect indirect fires
	MissionDetectIndirectFires := uint32(44594906)
	_, err := client.SendUnitOrder(watcher.Id, MissionDetectIndirectFires,
		swapi.MakeParameters(
			swapi.MakeHeading(0),
			nil, nil, nil,
			swapi.MakeLocation(swapi.MakePolygonLocation(
				swapi.Point{X: -15.8122, Y: 28.3455},
				swapi.Point{X: -15.8149, Y: 28.3198},
				swapi.Point{X: -15.7841, Y: 28.3222},
			)),
		))
	c.Assert(err, IsNil)
	// fire
	c.Assert(client.Model.GetData().UnitKnowledges, HasLen, 0)
	c.Assert(client.Model.GetData().FireDetections, HasLen, 0)
	params := swapi.MakeParameters(
		swapi.MakeResourceType(13), // Antitank High Explosive Shell
		swapi.MakeFloat(1),
		swapi.MakePointParam(
			swapi.Point{X: -15.88594, Y: 28.25975},
		),
	)
	_, err = client.SendUnitFragOrder(firer.Id, 368, params) // Fire
	c.Assert(err, IsNil)
	// check fire detection created
	var p *swapi.FireDetection
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, fp := range data.FireDetections {
			p = fp
			return true
		}
		return false
	})
	c.Assert(p.Firer, Equals, firer.Id)
	c.Assert(p.Units, DeepEquals, []uint32{watcher.Id})
	// check knowledge created
	var k *swapi.UnitKnowledge
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, uk := range data.UnitKnowledges {
			k = uk
			return true
		}
		return false
	})
	c.Assert(k.UnitId, Equals, firer.Id)
	c.Assert(k.KnowledgeGroupId, Equals, automat.KnowledgeGroupId)
	// check fire detection destroyed
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.FireDetections) == 0
	})
}
