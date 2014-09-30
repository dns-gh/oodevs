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
	"masa/sword/swapi/simu"
	"masa/sword/sword"
	"os"
	"reflect"
	"regexp"
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
	urban := swapi.NewUrban(21, "Bloc urbain [21]", 100,
		map[string]*swapi.ResourceNetwork{
			"Running Water": {Name: "Running Water",
				Consumption: 1, Critical: true, Activated: false, Production: 2,
				StockMax: 3},
			"Natural Gas": {Name: "Natural Gas",
				Consumption: 4, Critical: false, Activated: true, Production: 5,
				StockMax: 6},
		})
	err = client.ChangeResourceNetwork(urban)
	c.Assert(err, IsNil)

	// model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(urban, data.Objects[21])
	})

	// the magic action changes resources network values totally
	urban = swapi.NewUrban(21, "Bloc urbain [21]", 100,
		map[string]*swapi.ResourceNetwork{
			"Electricity": {Name: "Electricity",
				Consumption: 7, Critical: true, Activated: true, Production: 8,
				StockMax: 9},
		})
	err = client.ChangeResourceNetwork(urban)
	c.Assert(err, IsNil)

	// model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(urban, data.Objects[21])
	})
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

	parent := getSomeKnowledgeGroup(c, client.Model.GetData(), 0)
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

func TriggerBreakdown(c *C, client *swapi.Client) uint32 {
	unit := client.Model.GetUnit(8)
	equipmentId := uint32(39)
	MOBBreakdown2EBEvac := int32(13)
	equipment := swapi.Equipment{
		Available:  1,
		Repairable: 1,
		Breakdowns: []int32{MOBBreakdown2EBEvac},
	}
	err := client.ChangeEquipmentState(unit.Id, map[uint32]*swapi.Equipment{equipmentId: &equipment})
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
	return handlingId
}

func WaitStateEntered(c *C, client *swapi.Client, handlingId uint32,
	state sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus) {

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		h := data.MaintenanceHandlings[handlingId]
		return h != nil && h.Provider != nil &&
			h.Provider.State == state
	})
}

func WaitStateLeft(c *C, client *swapi.Client, handlingId uint32,
	state sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus) {

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		h := data.MaintenanceHandlings[handlingId]
		return h == nil || h.Provider != nil &&
			h.Provider.State != state
	})
}

const (
	automatLog = 14
	repairTeam = 19
)

func (s *TestSuite) TestSelectMaintenanceTransporter(c *C) {
	opts := NewAllUserOpts(ExCrossroadSmallLog)
	opts.Step = 300
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)
	const TRANSHeavyEquipmentTransporterSystem = 24

	// error: invalid parameters count, 2 mandatory parameters expected
	err := client.SelectMaintenanceTransporterTest(swapi.MakeParameters())
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid number of parameters: want between 2 and 3, got 0")

	// error: first parameter must be an identifier
	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters(swapi.MakeEmpty(), swapi.MakeIdentifier(TRANSHeavyEquipmentTransporterSystem)))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: parameter\\[0\\] is missing")

	// error: second parameter must be an identifier
	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters(swapi.MakeIdentifier(TRANSHeavyEquipmentTransporterSystem), swapi.MakeEmpty()))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: parameter\\[1\\] is missing")

	// error: third parameter must be an identifier
	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters(swapi.MakeIdentifier(TRANSHeavyEquipmentTransporterSystem), swapi.MakeIdentifier(TRANSHeavyEquipmentTransporterSystem), swapi.MakeEmpty()))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: parameter\\[2\\] is missing")

	// error: first parameter must be a valid request identifier
	err = client.SelectMaintenanceTransporter(1000, TRANSHeavyEquipmentTransporterSystem)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid log request identifier")

	SetManualMaintenance(c, client, automatLog)
	handlingId := TriggerBreakdown(c, client)

	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_transporter_selection)

	// error: second parameter is an unknown equipment type identifier
	err = client.SelectMaintenanceTransporter(handlingId, 1000)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid equipment type identifier")

	// error: second parameter is not an equipment type identifier managed by unit
	err = client.SelectMaintenanceTransporter(handlingId, 57)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no component of specified type available for maintenance transporter selection")

	// error: second parameter is a valid equipment type identifier but cannot haul
	err = client.SelectMaintenanceTransporter(handlingId, 26)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no component of specified type available for maintenance transporter selection")

	// trigger select maintenance transporter
	err = client.SelectMaintenanceTransporter(handlingId, TRANSHeavyEquipmentTransporterSystem)
	c.Assert(err, IsNil)

	WaitStateLeft(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_transporter_selection)
}

func (s *TestSuite) TestSelectMaintenanceTransporterWithAgentAsDestination(c *C) {
	opts := NewAllUserOpts(ExCrossroadSmallLog)
	opts.Step = 300
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)
	const TRANSHeavyEquipmentTransporterSystem = 24

	// Teleport repair team away
	err := client.SetAutomatMode(16, false)
	c.Assert(err, IsNil)
	to := swapi.Point{X: -15.6967, Y: 28.2224}
	err = client.Teleport(swapi.MakeUnitTasker(repairTeam), to)
	c.Assert(err, IsNil)

	SetManualMaintenance(c, client, automatLog)
	handlingId := TriggerBreakdown(c, client)

	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_transporter_selection)

	// error: third parameter is an invalid agent identifier
	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters(swapi.MakeIdentifier(handlingId),
		swapi.MakeIdentifier(TRANSHeavyEquipmentTransporterSystem),
		swapi.MakeAgent(0)))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid destination agent identifier")

	// trigger select maintenance transporter with destination
	endTick := client.Model.GetData().MaintenanceHandlings[handlingId].Provider.EndTick
	c.Assert(endTick, Equals, int32(0))

	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters(swapi.MakeIdentifier(handlingId),
		swapi.MakeIdentifier(TRANSHeavyEquipmentTransporterSystem),
		swapi.MakeAgent(repairTeam)))
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		h := data.MaintenanceHandlings[handlingId]
		return h != nil && h.Provider != nil &&
			h.Provider.EndTick != endTick
	})
}

func (s *TestSuite) TestSelectDiagnosisTeam(c *C) {
	opts := NewAllUserOpts(ExCrossroadSmallLog)
	opts.Step = 300
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)

	// error: invalid parameters count, parameters expected
	err := client.SelectDiagnosisTeamTest(swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	const MobilityRepairsTeam = 91

	// error: first parameter must be an identifier
	err = client.SelectDiagnosisTeamTest(swapi.MakeParameters(nil, swapi.MakeIdentifier(MobilityRepairsTeam)))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	err = client.SelectDiagnosisTeam(1000, MobilityRepairsTeam)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid log request identifier")

	SetManualMaintenance(c, client, automatLog)
	handlingId := TriggerBreakdown(c, client)

	// error: not in diagnosis team waiting state
	err = client.SelectDiagnosisTeam(handlingId, MobilityRepairsTeam)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: cannot select a diagnosis team for a transport consign")

	// skip transporter selection
	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_transporter_selection)
	err = client.SelectNewLogisticState(handlingId)
	c.Assert(err, IsNil)

	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_diagnosis_team_selection)

	// error: second parameter must be a valid identifier
	err = client.SelectDiagnosisTeam(handlingId, 1000)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid equipment type identifier")

	// error: component type specified not available
	err = client.SelectDiagnosisTeam(handlingId, 39)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no component of specified type available for diagnosis team selection")

	err = client.SelectDiagnosisTeam(handlingId, MobilityRepairsTeam)
	c.Assert(err, IsNil)
	WaitStateLeft(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_diagnosis_team_selection)
}

func (s *TestSuite) TestSelectRepairTeam(c *C) {
	opts := NewAllUserOpts(ExCrossroadSmallLog)
	opts.Step = 300
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)

	// error: invalid parameters count, parameters expected
	err := client.SelectRepairTeamTest(swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	const MobilityRepairsTeam = 91

	// error: first parameter must be an identifier
	err = client.SelectRepairTeamTest(swapi.MakeParameters(nil, swapi.MakeIdentifier(MobilityRepairsTeam)))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	err = client.SelectRepairTeam(1000, MobilityRepairsTeam)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid log request identifier")

	SetManualMaintenance(c, client, automatLog)
	handlingId := TriggerBreakdown(c, client)

	// error: not a repair consign
	err = client.SelectRepairTeam(handlingId, MobilityRepairsTeam)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: cannot select a repair team for a transport consign")

	// skip transporter selection
	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_transporter_selection)
	err = client.SelectNewLogisticState(handlingId)
	c.Assert(err, IsNil)

	// skip diagnosis team selection
	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_diagnosis_team_selection)
	err = client.SelectNewLogisticState(handlingId)
	c.Assert(err, IsNil)

	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_repair_team_selection)

	// error: second parameter must be a valid identifier
	err = client.SelectRepairTeam(handlingId, 1000)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid equipment type identifier")

	// error: component type specified not available
	err = client.SelectRepairTeam(handlingId, 39)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no component of specified type available for repair team selection")

	err = client.SelectRepairTeam(handlingId, MobilityRepairsTeam)
	c.Assert(err, IsNil)
	WaitStateLeft(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_repair_team_selection)

	// error: not in repair team waiting state
	err = client.SelectRepairTeam(handlingId, MobilityRepairsTeam)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: repair consign not in a waiting for repair team selection state")
}
