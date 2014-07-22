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
	"masa/sword/swapi"
	"masa/sword/swapi/phy"
	"masa/sword/swapi/simu"
	"masa/sword/sword"
	"masa/sword/swtest"
	"math"
)

// Simulation crashes when terminating because of some race between crowd
// flows and pathfinding.
// http://jira.masagroup.net/browse/SWBUG-11652
func DisableDumpChecksForCrowds() *simu.SessionErrorsOpts {
	return &simu.SessionErrorsOpts{
		IgnoreDumps: true,
	}
}

func CheckHumans(healthy, wounded, dead, contaminated int32, crowd *swapi.Crowd) bool {
	return crowd.Healthy == healthy && crowd.Wounded == wounded &&
		crowd.Dead == dead && crowd.Contaminated == contaminated
}

func CreateCrowdFromParty(c *C, client *swapi.Client, name string) *swapi.Crowd {
	party := client.Model.GetData().FindPartyByName(name)
	c.Assert(party, NotNil)
	// Create crowd in party
	pos := swapi.Point{X: 0, Y: 0}
	crowd, err := client.CreateCrowd(party.Id, 0, CrowdType, pos, 10, 11, 12, "crowd")
	c.Assert(err, IsNil)
	c.Assert(crowd, NotNil)
	return crowd
}

func CreateCrowd(c *C, client *swapi.Client) *swapi.Crowd {
	return CreateCrowdFromParty(c, client, "party")
}

func (s *TestSuite) TestCrowdTotalDestruction(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	healthy, wounded, dead := int32(10), int32(11), int32(12)

	crowd := CreateCrowd(c, client)

	// Check humans composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(healthy, wounded, dead, 0, data.Crowds[crowd.Id])
	})

	// Kill all humans with invalid identifier
	err := client.SendTotalDestruction(12345)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Kill all humans
	err = client.SendTotalDestruction(crowd.Id)
	c.Assert(err, IsNil)

	// Check humans composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(0, 0, dead+wounded+healthy, 0, data.Crowds[crowd.Id])
	})
}

func (s *TestSuite) TestCrowdChangeArmedIndividuals(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	crowd := CreateCrowd(c, client)

	// Check default armed individuals proportion(set in physical database)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Crowds[crowd.Id].ArmedIndividuals == 0.1
	})

	// Error : missing parameter
	err := client.ChangeArmedIndividuals(crowd.Id, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error : Armed individuals must be a percentage between 0 and 100
	err = client.ChangeArmedIndividuals(crowd.Id, 200)
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	err = client.ChangeArmedIndividuals(crowd.Id, -2)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change Armed individuals
	err = client.ChangeArmedIndividuals(crowd.Id, 50)
	c.Assert(err, IsNil)

	// Check armed individuals proportion
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Crowds[crowd.Id].ArmedIndividuals == 0.5
	})
}

func (s *TestSuite) TestCrowdChangeCriticalIntelligence(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, DisableDumpChecksForCrowds())
	defer client.Close()

	crowd := CreateCrowd(c, client)

	// Check empty critical intelligence
	c.Assert(crowd.CriticalIntelligence, Equals, "")

	// Error : missing parameter
	err := client.ChangeCriticalIntelligence(crowd.Id, "")
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change critical intelligence
	err = client.ChangeCriticalIntelligence(crowd.Id, "critical")
	c.Assert(err, IsNil)

	// Check critical intelligence
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Crowds[crowd.Id].CriticalIntelligence == "critical"
	})
}

func (s *TestSuite) TestCrowdChangeHealthState(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	healthy, wounded, contaminated, dead := int32(10), int32(11), int32(0), int32(12)

	crowd := CreateCrowd(c, client)

	// Check human composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(healthy, wounded, dead, contaminated, data.Crowds[crowd.Id])
	})

	// Error : negative parameter
	err := client.ChangeHealthState(crowd.Id, -2, -5, contaminated, -1)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error : all parameters are null
	err = client.ChangeHealthState(crowd.Id, 0, 0, 0, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change human composition
	healthy, wounded, contaminated, dead = int32(23), int32(22), int32(21), int32(51)
	err = client.ChangeHealthState(crowd.Id, healthy, wounded, contaminated, dead)
	c.Assert(err, IsNil)

	// Check new humans composition
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckHumans(healthy, wounded, dead, contaminated, data.Crowds[crowd.Id])
	})
}

func (s *TestSuite) TestCrowdChangeAdhesions(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	crowd := CreateCrowd(c, client)

	// By default, adhesions are empty
	c.Assert(len(crowd.Adhesions), Equals, 0)

	// Error : missing parameter
	err := client.ChangeCrowdAdhesions(crowd.Id, map[uint32]float32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error : adhesion must be between -1 and 1
	adhesions := map[uint32]float32{0: 1.1, 1: 5.2}
	err = client.ChangeCrowdAdhesions(crowd.Id, adhesions)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change crowd adhesions
	adhesions = map[uint32]float32{0: 0.7, 1: -0.5}
	err = client.ChangeCrowdAdhesions(crowd.Id, adhesions)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Crowds[crowd.Id].Adhesions) != 0
	})
	newAdhesions := client.Model.GetData().Crowds[crowd.Id].Adhesions
	swtest.DeepEquals(c, adhesions, newAdhesions)

	// No change adhesions if new adhesions are invalid
	err = client.ChangeCrowdAdhesions(crowd.Id,
		map[uint32]float32{0: -1.1, 1: -5.2})
	c.Assert(err, IsSwordError, "error_invalid_parameter")
	newAdhesions = client.Model.GetData().Crowds[crowd.Id].Adhesions
	swtest.DeepEquals(c, adhesions, newAdhesions)

	// Partial change
	adhesions = map[uint32]float32{0: 0.5}
	err = client.ChangeCrowdAdhesions(crowd.Id, map[uint32]float32{0: 0.5})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		updated := data.Crowds[crowd.Id]
		return math.Abs(float64(updated.Adhesions[0]-0.5)) < 1e-5
	})
	newAdhesions = client.Model.GetData().Crowds[crowd.Id].Adhesions
	swtest.DeepEquals(c, adhesions, newAdhesions)
}

func (s *TestSuite) TestCrowdReloadBrain(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	crowd := CreateCrowd(c, client)
	tasker := swapi.MakeCrowdTasker(crowd.Id)

	// too many parameters
	err := client.ReloadBrainTest(tasker, swapi.MakeParameters(
		swapi.MakeString("blah"),
		nil,
	))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid type
	err = client.ReloadBrainTest(tasker, swapi.MakeParameters(
		swapi.MakeBoolean(true),
	))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid crowd id
	empty := swapi.MakeParameters()
	err = client.ReloadBrainTest(swapi.MakeCrowdTasker(crowd.Id+1), empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// missing id
	err = client.ReloadBrainTest(&sword.Tasker{}, empty)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// invalid model
	err = client.ReloadBrain(tasker, "bad_brain")
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// reload current brain
	err = client.ReloadBrain(tasker, "")
	c.Assert(err, IsNil)

	// Reload crowd decisional model
	err = client.ReloadBrain(tasker, "Rioters")
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestCrowdTeleportation(c *C) {
	opts := NewAdminOpts(ExCrossroadSmallOrbat)
	sim, client := connectAndWaitModel(c, opts)
	defer stopSim(c, sim, DisableDumpChecksForCrowds())
	defer client.Close()
	crowd := CreateCrowd(c, client)
	// Initial crowd has a concentration
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Crowds[crowd.Id].CrowdElements) == 1
	})

	// Send moveTo mission on the crowd
	destination := swapi.Point{X: -15.8193, Y: 28.3456}
	params := swapi.MakeParameters(swapi.MakePointParam(destination))
	_, err := client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, params)
	c.Assert(err, IsNil)

	// Check crowd begins its movement, a flow is created. We assume the
	// simulation is not fast enough for the crowd movement to end between
	// here and the teleportation below, otherwise the mission would be
	// terminated.
	knownElements := map[uint32]*swapi.CrowdElement{}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		done := len(data.Crowds[crowd.Id].CrowdElements) == 2
		if done {
			swapi.DeepCopy(&knownElements, data.Crowds[crowd.Id].CrowdElements)
		}
		return done
	})

	// Teleport the crowd, the flow and concentrations are destroyed.
	// A new concentration is created.
	teleport := swapi.Point{X: -15.8193, Y: 128.3456}
	err = client.Teleport(swapi.MakeCrowdTasker(crowd.Id), teleport)
	c.Assert(err, IsNil)
	elements := client.Model.GetData().Crowds[crowd.Id].CrowdElements
	c.Assert(elements, HasLen, 0)

	// A new flow is created when the crowd restarts its movement.
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		elements := data.Crowds[crowd.Id].CrowdElements
		for id := range elements {
			if knownElements[id] != nil {
				return false
			}
		}
		return len(elements) > 1
	})
}

func (s *TestSuite) TestCrowdChangeExtensions(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	crowd := CreateCrowd(c, client)

	// By default, adhesions are empty.
	c.Assert(len(crowd.Extensions), Equals, 0)

	// Error : missing parameter
	err := client.ChangeExtensions(crowd.Id, nil)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Fill new extensions
	extensions := map[string]string{"name1": "value1", "name2": "value2"}
	err = client.ChangeExtensions(crowd.Id, &extensions)
	c.Assert(err, IsNil)

	// Check new extensions
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Crowds[crowd.Id].Extensions) != 0
	})

	data := client.Model.GetData()
	newExtensions := data.Crowds[crowd.Id].Extensions
	swtest.DeepEquals(c, extensions, newExtensions)

	// Change extension
	err = client.ChangeExtensions(crowd.Id,
		&map[string]string{"name2": "value3"})
	c.Assert(err, IsNil)

	client.Model.WaitTicks(2)
	data = client.Model.GetData()
	newExtensions = data.Crowds[crowd.Id].Extensions
	c.Assert(map[string]string{"name1": "value1", "name2": "value3"},
		DeepEquals, newExtensions)
}

func CheckAttitude(crowd *swapi.Crowd, attitude int32) bool {
	for _, v := range crowd.CrowdElements {
		if v.Attitude != attitude {
			return false
		}
	}
	return true
}

const (
	// Attitude enumeration
	Peaceful  = int32(0)
	Agitated  = int32(1)
	Excited   = int32(2)
	Agressive = int32(3)
)

func (s *TestSuite) TestCrowdChangeAttitude(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, DisableDumpChecksForCrowds())
	defer client.Close()
	crowd := CreateCrowd(c, client)

	// Wait crowd update
	client.Model.WaitTicks(1)

	// Check attitude is peaceful
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckAttitude(data.Crowds[crowd.Id], Peaceful)
	})

	// Error : missing parameter
	err := client.ChangeAttitude(crowd.Id, 0)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error : invalid attitude
	err = client.ChangeAttitude(crowd.Id, 12345)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change attitude(agitated)
	err = client.ChangeAttitude(crowd.Id, Agitated)
	c.Assert(err, IsNil)

	// Check attitude is agitated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckAttitude(data.Crowds[crowd.Id], Agitated)
	})

	// Send moveTo mission on the crowd
	to := swapi.Point{X: -15.8193, Y: 28.3456}
	params := swapi.MakeParameters(swapi.MakePointParam(to))
	_, err = client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, params)
	c.Assert(err, IsNil)

	// Check crowd begin its movement, a flow is created
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return len(data.Crowds[crowd.Id].CrowdElements) == 2
	})

	// Change attitude(excited)
	err = client.ChangeAttitude(crowd.Id, Excited)
	c.Assert(err, IsNil)

	// Check flow and concentation have an excited attitude
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckAttitude(data.Crowds[crowd.Id], Excited)
	})

	// Teleport and check attitude returned to Peaceful
	err = client.Teleport(swapi.MakeCrowdTasker(crowd.Id), to)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckAttitude(data.Crowds[crowd.Id], Peaceful)
	})
}

const (
	MissionDemonstrateCrowdId = uint32(44574)
)

// Helper combining reports source and message filters.
func newReporter(c *C, source uint32, db *phy.PhysicalFile, patterns ...string) (
	*swapi.Reporter, map[uint32]string) {

	ids := swapi.ReportSources(source)
	messages, matched, err := swapi.ReportMessages(db, patterns...)
	c.Assert(err, IsNil)
	filter := func(r *sword.Report) bool {
		return ids(r) && messages(r)
	}
	return swapi.NewReporter(filter), matched
}

func (s *TestSuite) TestCrowdInCheckpoint(c *C) {
	// This test is to verify that the crowd doesn't stay blocked by obstacles
	// and inactive checkpoints.
	// Description:
	// A crows demonstrates. It finds a barricade on its way but doesn't get stuck inside.
	// A police Automat creates a checkpoint on the road of a crowd.
	// This crowd is blocked by this checkpoint until the crowd destroy police units.
	// Then the crowd continue its road until its objective.
	opts := NewAllUserOpts(ExCrossroadSmallOrbat)
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()
	// Create agressive crowd
	party := data.FindPartyByName("another-party")
	c.Assert(party, NotNil)
	// Create crowd in party
	crowd, err := client.CreateCrowd(party.Id, 0, "Motorized Crowd",
		swapi.Point{X: -15.8105, Y: 28.3451}, 600, 0, 0, "crowd")

	err = client.ChangeAttitude(crowd.Id, Agressive)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return CheckAttitude(data.Crowds[crowd.Id], Agressive)
	})

	// Create barricade
	location := swapi.MakePointLocation(swapi.Point{X: -15.8045, Y: 28.3451})
	party = data.FindPartyByName("party")
	c.Assert(party, NotNil)
	object, err := client.CreateObject("barricade", party.Id, location)
	c.Assert(err, IsNil)
	c.Assert(object, NotNil)

	// Create Safety police patrol
	phydb := loadPhysical(c, "worldwide")
	safetyPolicePatrolId := uint32(185)
	safetyPoliceUnit := getUnitTypeFromName(c, phydb, "SAFETY.Police Unit")
	automat := createSpecificAutomat(c, client, "party", safetyPolicePatrolId)
	unitPos := swapi.Point{X: -15.7928, Y: 28.3451}
	for i := 0; i != 2; i++ {
		_, err := client.CreateUnit(automat.Id, safetyPoliceUnit, unitPos)
		c.Assert(err, IsNil)
	}

	// Send SAFETY - Operate a checkpoint (filter crowds)
	operateCheckpoint := uint32(445949258)
	params := swapi.MakeParameters(
		swapi.MakeHeading(0),
		nil,
		swapi.MakeLimit(unitPos.Shift(-0.01, 0.01), unitPos.Shift(0.01, 0.01)),
		swapi.MakeLimit(unitPos.Shift(-0.01, -0.01), unitPos.Shift(0.01, -0.01)),
		swapi.MakePlannedWork("checkpoint", unitPos),
		nil,
		nil,
	)
	_, err = client.SendAutomatOrder(automat.Id, operateCheckpoint, params)
	c.Assert(err, IsNil)

	// Wait Checkpoint
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, object := range data.Objects {
			if object.ObjectType == "checkpoint" {
				return true
			}
		}
		return false
	})

	reporter, _ := newReporter(c, crowd.Id, phydb, "Hard to go through")
	reporter.Start(client.Model)

	// Send demonstrate mission on the crowd
	to := swapi.Point{X: -15.786, Y: 28.3451}
	params = swapi.MakeParameters(swapi.MakePointParam(to))
	_, err = client.SendCrowdOrder(crowd.Id, MissionDemonstrateCrowdId, params)
	c.Assert(err, IsNil)

	// Check crowd position
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Crowds[crowd.Id]
		if len(unit.CrowdElements) != 1 {
			return false
		}
		for _, value := range unit.CrowdElements {
			return isNearby(value.Position, to)
		}
		return false
	})

	reports := reporter.Stop()
	c.Assert(len(reports), Equals, 3) // Barricade once, checkpoint twice
}
