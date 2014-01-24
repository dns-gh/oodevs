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
	"os"
	"reflect"
	"regexp"
	"swapi"
	"swapi/simu"
	"sword"
)

const (
	// Resource type without indirect fire
	ResourceTypeWithoutIndirectFire = uint32(1)
	// Resource type with indirect fire
	ResourceTypeWithIndirectFire = uint32(8)
)

func (s *TestSuite) TestChangeDiplomacy(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	//check with no parameters
	params := swapi.MakeParameters()
	err := client.ChangeDiplomacyTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	//check with wrong party
	err = client.ChangeDiplomacy(0, 2, sword.EnumDiplomacy_enemy)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	err = client.ChangeDiplomacy(2, 0, sword.EnumDiplomacy_enemy)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	//check with wrong param number
	params = swapi.MakeParameters(
		swapi.MakeIdentifier(1),
		swapi.MakeIdentifier(2))
	err = client.ChangeDiplomacyTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	//check with all parameters
	model := client.Model.GetData()
	parties := model.Parties
	party1 := parties[1]
	party2 := parties[2]
	c.Assert(party1, NotNil)
	c.Assert(party2, NotNil)
	diplomacy := sword.EnumDiplomacy_enemy
	err = client.ChangeDiplomacy(party1.Id, party2.Id, diplomacy)
	c.Assert(err, IsNil)

	parties = client.Model.GetData().Parties
	party1 = parties[1]
	party2 = parties[2]
	c.Assert(party1.Diplomacies[party2.Id], Equals, sword.EnumDiplomacy_enemy)
}

func (s *TestSuite) TestFireOrderOnLocationCreation(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	point := swapi.Point{X: -15.8241, Y: 28.3241}

	// error: invalid parameters count, 3 parameters expected
	params := swapi.MakeParameters()
	err := client.CreateFireOnLocationTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a location or a point
	params = swapi.MakeParameters(nil, nil, nil)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: second parameter must be a resource type
	params.Elem[0] = swapi.MakePointParam(point)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: second parameter must be a valid resource type
	params.Elem[1] = swapi.MakeResourceType(uint32(12345))
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: second parameter must be a resource type with a indirect fire
	params.Elem[1] = swapi.MakeResourceType(ResourceTypeWithoutIndirectFire)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: third parameter must be a real
	params.Elem[1] = swapi.MakeResourceType(ResourceTypeWithIndirectFire)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: third parameter must be a positive real number
	params.Elem[2] = swapi.MakeFloat(float32(-3))
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: invalid parameters count, 2 parameters expected
	params := swapi.MakeParameters()
	err := client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a location or an identifier
	params = swapi.MakeParameters(nil, nil)
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(1000))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// first parameter with a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(21))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsNil)

	// error: second parameter must be a list with valid resource names
	params.Elem[1] = swapi.MakeParameter(
		swapi.MakeList(swapi.MakeString("Running Water")),
		swapi.MakeList(swapi.MakeString("invalid resource name")))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// second parameter with valid resource names
	params.Elem[1] = swapi.MakeParameter(
		swapi.MakeList(swapi.MakeString("Running Water")),
		swapi.MakeList(swapi.MakeString("Natural Gas")))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsNil)

	// update full parameters
	urban := swapi.NewUrban(21, "Bloc urbain [21]",
		map[string]*swapi.ResourceNetwork{
			"Running Water": &swapi.ResourceNetwork{Name: "Running Water",
				Consumption: 1, Critical: true, Activated: false, Production: 2,
				StockMax: 3},
			"Natural Gas": &swapi.ResourceNetwork{Name: "Natural Gas",
				Consumption: 4, Critical: false, Activated: true, Production: 5,
				StockMax: 6},
		})
	err = client.ChangeResourceNetwork(urban)
	c.Assert(err, IsNil)

	// model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(urban, data.Urbans[21])
	})

	// the magic action changes resources network values totally
	urban = swapi.NewUrban(21, "Bloc urbain [21]",
		map[string]*swapi.ResourceNetwork{
			"Electricity": &swapi.ResourceNetwork{Name: "Electricity",
				Consumption: 7, Critical: true, Activated: true, Production: 8,
				StockMax: 9},
		})
	err = client.ChangeResourceNetwork(urban)
	c.Assert(err, IsNil)

	// model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(urban, data.Urbans[21])
	})
}

func getSomeKnowledgeGroup(c *C, data *swapi.ModelData) *swapi.KnowledgeGroup {
	for _, k := range data.KnowledgeGroups {
		return k
	}
	c.Fatal("missing knowledge groups")
	return nil
}

func (s *TestSuite) TestKnowledgeGroupCreation(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: invalid parameters count, parameters expected
	params := swapi.MakeParameters()
	group, err := client.CreateKnowledgeGroupTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be an identifier
	params = swapi.MakeParameters(nil, nil)
	group, err = client.CreateKnowledgeGroupTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(1000))
	group, err = client.CreateKnowledgeGroupTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	parent := getSomeKnowledgeGroup(c, client.Model.GetData())
	c.Assert(parent, Not(IsNil))

	// error: second parameter must be a valid knowledge group type identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(parent.Id))
	params.Elem[1] = swapi.MakeString("invalid knowledge group type")
	group, err = client.CreateKnowledgeGroupTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// first parameter with a valid identifier
	// second parameter with a valid knowledge group type identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(parent.Id))
	params.Elem[1] = swapi.MakeString("Standard")
	group, err = client.CreateKnowledgeGroupTest(params)
	c.Assert(err, IsNil)
	c.Assert(group.ParentId, Equals, parent.Id)
	c.Assert(group.PartyId, Equals, parent.PartyId)
	c.Assert(group.Name, Not(IsNil))

	// first parameter with a valid identifier
	// second parameter with a valid party identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(parent.PartyId))
	params.Elem[1] = swapi.MakeString("Standard")
	group, err = client.CreateKnowledgeGroupTest(params)
	c.Assert(err, IsNil)
	c.Assert(group.PartyId, Equals, parent.PartyId)
	c.Assert(group.ParentId, Equals, uint32(0))
	c.Assert(group.Name, Not(IsNil))
}

func (s *TestSuite) TestTriggerError(c *C) {
	triggerError := func(kind string) {
		opts, session := makeOptsAndSession()
		opts.TestCommands = true
		opts.ExerciseName = ExCrossroadSmallEmpty
		WriteSession(c, opts, session)
		sim, err := simu.StartSim(opts)
		c.Assert(err, IsNil) // failed to start the simulation
		defer sim.Stop()

		client := connectAndWait(c, sim, "admin", "")
		defer client.Close()
		err = client.TriggerError(kind)
		c.Assert(err, ErrorMatches, "connection closed")

		// A crash generates a .dmp file
		dmps, err := simu.ListDmpFiles(opts.DebugDir)
		c.Assert(err, IsNil)
		c.Assert(dmps, HasLen, 1)

		// And the sim.log should contain a stack trace related to the error
		fp, err := os.Open(opts.GetSimLogPath())
		c.Assert(err, IsNil)
		defer fp.Close()
		trace, err := simu.FindStacktrace(fp)
		c.Assert(err, IsNil)
		reStack := regexp.MustCompile(
			`(?s)Crash - stack trace.*(filename not available|\\simulation_kernel\\)`)
		c.Assert(reStack.FindStringSubmatch(trace), NotNil)

		// There is at least one functErr, the crash one
		fp.Seek(0, 0)
		errors, err := simu.FindLoggedFatalErrors(fp, &simu.SessionErrorsOpts{})
		c.Assert(err, IsNil)
		c.Assert(len(errors), Greater, 0)

		err = simu.CheckSessionErrors(opts.GetSessionDir(), nil)
		c.Assert(err, NotNil)
	}

	kinds := []string{"null_pointer", "stack_overflow"}
	for _, kind := range kinds {
		triggerError(kind)
	}

	// The command is ignored if the simulation was not started with --test-commands
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	err := client.TriggerError("null_pointer")
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestSelectTransporter(c *C) {
	opts := NewAdminOpts(ExCrossroadSmallLog)
	opts.Step = 300
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallLog))
	defer stopSimAndClient(c, sim, client)

	// error: invalid parameters count, parameters expected
	params := swapi.MakeParameters()
	err := client.SelectTransporterTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be an identifier
	params = swapi.MakeParameters(nil)
	err = client.SelectTransporterTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(1000))
	err = client.SelectTransporterTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// set automat to manual mode
	err = client.LogMaintenanceSetManual(14, true)
	c.Assert(err, IsNil)
	// trigger a breakdown
	unit := client.Model.GetUnit(8)
	equipmentId := uint32(39)
	equipment := swapi.EquipmentDotation{
		Available:  1,
		Repairable: 1,
		Breakdowns: []int32{11},
	}
	err = client.ChangeEquipmentState(unit.Id, map[uint32]*swapi.EquipmentDotation{equipmentId: &equipment})
	c.Assert(err, IsNil)
	var handlingId uint32
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, h := range data.MaintenanceHandlings {
			if h.Provider != nil {
				handlingId = h.Id
				return true
			}
		}
		return false
	})
	// wait for state to be wait for selection
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.MaintenanceHandlings[handlingId].Provider.State == sword.LogMaintenanceHandlingUpdate_waiting_for_selection
	})
	// trigger select new state
	params.Elem[0] = swapi.MakeIdentifier(handlingId)
	err = client.SelectTransporterTest(params)
	c.Assert(err, IsNil)
	// check state has changed
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.MaintenanceHandlings[handlingId].Provider.State != sword.LogMaintenanceHandlingUpdate_waiting_for_selection
	})
}
