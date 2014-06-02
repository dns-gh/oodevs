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
	"masa/sword/swapi"
	"masa/sword/sword"
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
	// This wait should not be necessary, created units should be ready to
	// receive orders. May be related to http://jira.masagroup.net/browse/SWBUG-11903
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[watcher.Id].RawOperationalState > 0
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
	// deploy firer
	MissionArtilleryDeploy := uint32(44580)
	_, err = client.SendUnitOrder(firer.Id, MissionArtilleryDeploy,
		swapi.MakeParameters(
			swapi.MakeHeading(0),
			nil, nil, nil,
			swapi.MakeLocationParam(firer.Position),
		))
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[firer.Id].Installation == 100
	})
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
	p := swapi.FireDetection{}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, fp := range data.FireDetections {
			swapi.DeepCopy(&p, fp)
			return true
		}
		return false
	})
	c.Assert(p.Firer, Equals, firer.Id)
	c.Assert(p.Units, DeepEquals, []uint32{watcher.Id})
	// check knowledge created
	k := swapi.UnitKnowledge{}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, uk := range data.UnitKnowledges {
			swapi.DeepCopy(&k, uk)
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

func (s *TestSuite) TestDirectFireOrder(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSimAndClient(c, sim, client)
	firer, watcher := setupFireTest(c, client)
	// invalid tasker
	err := client.CreateDirectFireOrderOnUnit(0, 0)
	c.Assert(err, IsSwordError, "error_invalid_unit")
	// invalid target
	err = client.CreateDirectFireOrderOnUnit(firer.Id, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	// tasker == target
	err = client.CreateDirectFireOrderOnUnit(firer.Id, firer.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	// valid
	err = client.CreateDirectFireOrderOnUnit(firer.Id, watcher.Id)
	c.Assert(err, IsNil)
}

func checkUnitFireDamages(msg *sword.UnitsFireDamages) error {
	if msg == nil {
		return nil
	}
	for _, d := range msg.Elem {
		for _, eq := range d.Equipments.Elem {
			// Damages cannot recreate/repair units
			if *eq.Available > 0 || *eq.Unavailable < 0 || *eq.Repairable < 0 {
				return fmt.Errorf("invalid damage message: %v", msg)
			}
		}
	}
	return nil
}

func checkUnitsFireDamages(msg *swapi.SwordMessage) error {
	if msg.SimulationToClient == nil {
		return nil
	}
	m := msg.SimulationToClient.GetMessage()
	switch {
	case m.Explosion != nil:
		return checkUnitFireDamages(m.Explosion.UnitsDamages)
	case m.StopCrowdFire != nil:
		return checkUnitFireDamages(m.StopCrowdFire.UnitsDamages)
	case m.StopUnitFire != nil:
		return checkUnitFireDamages(m.StopUnitFire.UnitsDamages)
	}
	return nil
}

func (s *TestSuite) TestFireOrderCreationOnUnit(c *C) {
	opts := NewAdminOpts(ExCrossroadSmallOrbat)
	opts.FixSeed() // help with mortar shots
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)

	// Check unit damages messages
	var damagesErrors []error
	handlerId := client.Register(func(msg *swapi.SwordMessage, id, ctx int32, err error) bool {
		if err != nil {
			return true
		}
		if err := checkUnitsFireDamages(msg); err != nil {
			damagesErrors = append(damagesErrors, err)
		}
		return false
	})

	f1 := CreateFormation(c, client, 1)
	f2 := CreateFormation(c, client, 2)

	// Create 2 automats
	data := client.Model.GetData()
	kg0 := getAnyKnowledgeGroupIdWithPartyIndex(c, data, 0)
	kg1 := getAnyKnowledgeGroupIdWithPartyIndex(c, data, 1)
	a1 := CreateAutomat(c, client, f1.Id, kg0)
	a2 := CreateAutomat(c, client, f2.Id, kg1)

	// Create 2 mortar units
	const infMortarTroopType = 31
	reporter, err := client.CreateUnit(a1.Id, infMortarTroopType, swapi.Point{X: -15.8219, Y: 28.2456})
	c.Assert(err, IsNil)
	target, err := client.CreateUnit(a2.Id, infMortarTroopType, swapi.Point{X: -15.8219, Y: 28.2456})
	c.Assert(err, IsNil)

	const fullyOperational = 100
	// Waiting for the target initialization
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Units[target.Id]
		return !unit.Neutralized && unit.RawOperationalState == fullyOperational
	})

	// Adding the target in reporter's knowledges
	const identifiedLevel = 0
	targetKnowledge, err := client.CreateUnitKnowledge(a1.KnowledgeGroupId, target.Id, identifiedLevel)
	c.Assert(err, IsNil)

	// Launching a magic strike with good parameters
	const dotation81mmHighExplosiveShell = 9
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, dotation81mmHighExplosiveShell, 1)
	c.Assert(err, IsNil)

	// testing strike effect: neutralization and attrition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Units[target.Id]
		return unit.Neutralized && unit.RawOperationalState < fullyOperational
	})

	// Testing wrong reporter identifier
	err = client.CreateFireOrderOnUnit(0, targetKnowledge.Id, dotation81mmHighExplosiveShell, 1)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Testing wrong knowledge identifier
	err = client.CreateFireOrderOnUnit(reporter.Id, 0, dotation81mmHighExplosiveShell, 1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Testing wrong dotation identifier
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, 0, 1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Testing direct fire dotation
	const directFireDotation9mmBullet = 17
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, directFireDotation9mmBullet, 1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Testing negative or empty iterations
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, dotation81mmHighExplosiveShell, -1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, dotation81mmHighExplosiveShell, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Testing strike on non illuminated target with guided dotation
	const dotation120mmHightExplosiveShellGuided = 34
	err = client.CreateFireOrderOnUnit(reporter.Id, targetKnowledge.Id, dotation120mmHightExplosiveShellGuided, 1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	client.Unregister(handlerId)
	c.Assert(damagesErrors, IsNil)
}

func (s *TestSuite) TestActiveProtection(c *C) {
	c.Skip("unreliable, http://jira.masagroup.net/browse/SWBUG-12467")
	// VW Active Protection has:
	// - Hard kill active protection from "Shell Of Death", comsumming
	//   2 "ammmunition"
	// - Counter protection from "Shell of Death", consumming one "smoke1"
	// - No protection against "Shell of Death Improved"

	phydb := loadPhysical(c, "test")

	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallTest))
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()

	party1 := getPartyByName(c, data, "party1")
	formation1 := CreateFormation(c, client, party1.Id)
	kg1 := getAnyKnowledgeGroupInParty(c, data, party1.Id)

	// Create target with active protection
	automatType1 := getAutomatTypeFromName(c, phydb, "VW Combi Rally")
	automat1, err := client.CreateAutomat(formation1.Id, automatType1, kg1.Id)
	c.Assert(err, IsNil)
	unitType1 := getUnitTypeFromName(c, phydb, "VW Active Protection")
	pos := swapi.Point{X: -15.9268, Y: 28.3453}
	target, err := client.CreateUnit(automat1.Id, unitType1, pos)
	c.Assert(err, IsNil)
	ammunitionId := uint32(96)
	smoke1Id := uint32(106)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[target.Id].Resources[ammunitionId].Quantity == 100 &&
			data.Units[target.Id].Resources[smoke1Id].Quantity == 100
	})

	reporter, reportIds := newReporter(c, target.Id, phydb,
		"^Hard-kill active protection successful",
		"^Active protection successful",
		"^neutralized$",
	)
	reporter.Start(client.Model)

	// Test hard-kill protection
	ShellOfDeathId := uint32(114)
	err = client.CreateFireOnLocation(pos, ShellOfDeathId, 1)
	c.Assert(err, IsNil)
	// TODO: smoke1 should not be consumed by hard-kill protection
	// http://jira.masagroup.net/browse/SWBUG-12458
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[target.Id].Resources[ammunitionId].Quantity == 98 &&
			data.Units[target.Id].Resources[smoke1Id].Quantity == 99
	})

	// Deplete "ammunition" and test counter protection
	err = client.ChangeResource(target.Id, map[uint32]*swapi.Resource{
		ammunitionId: &swapi.Resource{
			Quantity: 1,
		},
	})
	c.Assert(err, IsNil)
	err = client.CreateFireOnLocation(pos, ShellOfDeathId, 1)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[target.Id].Resources[ammunitionId].Quantity == 1 &&
			data.Units[target.Id].Resources[smoke1Id].Quantity == 98
	})

	// VW Active Protection has no protection against this
	client.Model.WaitTicks(1)
	reporter.AddNilReport()

	ShellOfDeathImprovedId := uint32(115)
	err = client.CreateFireOnLocation(pos, ShellOfDeathImprovedId, 1)
	c.Assert(err, IsNil)

	client.Model.WaitTicks(1)
	reports := reporter.Stop()
	c.Assert(swapi.PrintReports(reports, reportIds), Equals, ""+
		`^Hard-kill active protection successful
^Active protection successful

^neutralized$
`)
}
