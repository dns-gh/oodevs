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

func (s *TestSuite) TestSetAutomatMode(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	c.Assert(automat.Engaged, Equals, true)

	checkEngage := func(engagedId, waitId uint32, engage bool) {
		err := client.SetAutomatMode(engagedId, engage)
		c.Assert(err, IsNil)
	}

	// Invalid automat identifier
	err := client.SetAutomatMode(12456, false)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// This one should work
	checkEngage(automat.Id, automat.Id, false)

	// Engage it again
	checkEngage(automat.Id, automat.Id, true)
}

func (s *TestSuite) TestAutomatReloadBrain(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	tasker := swapi.MakeAutomatTasker(automat.Id)

	// too many parameters
	err := client.ReloadBrainTest(tasker, swapi.MakeParameters(
		swapi.MakeString("blah"), nil))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid type
	err = client.ReloadBrainTest(tasker, swapi.MakeParameters(
		swapi.MakeBoolean(true),
	))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// invalid automat id
	empty := swapi.MakeParameters()
	err = client.ReloadBrainTest(swapi.MakeAutomatTasker(automat.Id+1), empty)
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

	// Reload automat decisional model
	err = client.ReloadBrain(tasker, "Journalists")
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestCreateAutomat(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	model := client.Model

	formation := getSomeFormation(c, model.GetData())

	// We want two knowledge groups from different parties, one of which
	// matches selected formation.
	knowledgeGroups := model.GetData().KnowledgeGroups
	c.Assert(len(knowledgeGroups), Greater, 1)
	var kg0 *swapi.KnowledgeGroup
	var kg1 *swapi.KnowledgeGroup
	for _, kg := range knowledgeGroups {
		if kg0 == nil && kg.PartyId == formation.PartyId {
			kg0 = kg
		}
		if kg1 == nil && kg.PartyId != formation.PartyId {
			kg1 = kg
		}
	}
	c.Assert(kg0, NotNil)
	c.Assert(kg1, NotNil)

	automatType := AutomatType

	// No parent formation
	_, err := client.CreateAutomat(0, automatType, kg0.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid formation
	_, err = client.CreateAutomat(InvalidIdentifier, automatType, kg0.Id)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid automat type
	_, err = client.CreateAutomat(formation.Id, InvalidIdentifier, kg0.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid knowledge group
	_, err = client.CreateAutomat(formation.Id, automatType, InvalidIdentifier)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Knowledge group not belonging to formation party
	_, err = client.CreateAutomat(formation.Id, automatType, kg1.Id)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Create automat in formation
	a, err := client.CreateAutomat(formation.Id, automatType, kg0.Id)
	c.Assert(err, IsNil)
	c.Assert(a, NotNil)
	c.Assert(a.KnowledgeGroupId, Equals, kg0.Id)
}

func (s *TestSuite) TestCreateAutomatAndUnits(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	model := client.Model

	formation := getSomeFormation(c, model.GetData())
	pos := swapi.Point{X: -15.8219, Y: 28.2456}

	// We want two knowledge groups from different parties, one of which
	// matches selected formation.
	knowledgeGroups := model.GetData().KnowledgeGroups
	c.Assert(len(knowledgeGroups), Greater, 1)
	var kg0 *swapi.KnowledgeGroup
	for _, kg := range knowledgeGroups {
		if kg0 == nil && kg.PartyId == formation.PartyId {
			kg0 = kg
		}
	}
	c.Assert(kg0, NotNil)

	// No parent formation
	_, _, err := client.CreateAutomatAndUnits(0, AutomatType, kg0.Id, pos)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid formation
	_, _, err = client.CreateAutomatAndUnits(InvalidIdentifier, AutomatType, kg0.Id, pos)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Invalid automat type
	_, _, err = client.CreateAutomatAndUnits(formation.Id, InvalidIdentifier, kg0.Id, pos)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	client.Pause()

	// Create INF.Infantry company(contains 6 units)
	automat, units, err := client.CreateAutomatAndUnits(formation.Id, AutomatType, kg0.Id, pos)
	c.Assert(err, IsNil)
	c.Assert(automat, NotNil)
	c.Assert(automat.KnowledgeGroupId, Equals, kg0.Id)
	c.Assert(len(units), Equals, 6)
	for _, unit := range units {
		c.Assert(unit.RawOperationalState, Equals, int32(100))
	}
}
