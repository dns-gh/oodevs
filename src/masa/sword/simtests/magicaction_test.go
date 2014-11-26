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
	"masa/sword/swrun"
	"masa/sword/swtest"
	"math"
	"os"
	"regexp"
	"time"
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
		opts, session := swrun.MakeOptsAndSession(Cfg)
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

// Creates a mobility breakdown on one unit supported by the maintenance
// logistic chain in crossroad-log exercise.
func TriggerBreakdown(c *C, client *swapi.Client, phydb *phy.PhysicalData) uint32 {
	unit := getSomeUnitByName(c, client.Model.GetData(), "Maintenance Mobile Infantry 2")
	citroen, err := phydb.Components.Find("Citroën 2CV")
	c.Assert(err, IsNil)
	mobility1, err := phydb.Breakdowns.Find("mobility 1")
	c.Assert(err, IsNil)
	equipment := swapi.Equipment{
		Available:  4,
		Repairable: 1,
		Breakdowns: []int32{int32(mobility1.Id)},
	}
	err = client.ChangeEquipmentState(unit.Id, map[uint32]*swapi.Equipment{citroen.Id: &equipment})
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

func (s *TestSuite) TestSelectMaintenanceTransporter(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)

	// Map transporting equipements
	phydb := loadPhysicalData(c, "test")
	towTruck, err := phydb.Components.Find("Tow Truck")
	c.Assert(err, IsNil)
	gyroScrew, err := phydb.Components.Find("Gyroscrew 1")
	c.Assert(err, IsNil)
	otherTransporter, err := phydb.Components.Find("equipment")
	c.Assert(err, IsNil)

	maintenanceAutomat := getSomeAutomatByName(c, client.Model.GetData(), "Maintenance Automat 1")

	// error: invalid parameters count, 2 mandatory parameters expected
	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters())
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid number of parameters: "+
		"want between 2 and 3, got 0")

	// error: first parameter must be an identifier
	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters(swapi.MakeEmpty(),
		swapi.MakeIdentifier(towTruck.Id)))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: parameter\\[0\\] is missing")

	// error: second parameter must be an identifier
	err = client.SelectMaintenanceTransporterTest(
		swapi.MakeParameters(swapi.MakeIdentifier(towTruck.Id),
			swapi.MakeEmpty()))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: parameter\\[1\\] is missing")

	// error: third parameter must be an identifier
	err = client.SelectMaintenanceTransporterTest(
		swapi.MakeParameters(swapi.MakeIdentifier(towTruck.Id),
			swapi.MakeIdentifier(towTruck.Id),
			swapi.MakeEmpty()))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: parameter\\[2\\] is missing")

	// error: first parameter must be a valid request identifier
	err = client.SelectMaintenanceTransporter(1000, towTruck.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid log request identifier")

	SetManualMaintenance(c, client, maintenanceAutomat.Id)
	handlingId := TriggerBreakdown(c, client, phydb)

	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_transporter_selection)

	// error: second parameter is an unknown equipment type identifier
	err = client.SelectMaintenanceTransporter(handlingId, 100000)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid equipment type identifier")

	// error: second parameter is not an equipment type identifier managed by unit
	err = client.SelectMaintenanceTransporter(handlingId, otherTransporter.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no component of specified"+
		" type available for maintenance transporter selection")

	// error: second parameter is a valid equipment type identifier but cannot haul
	err = client.SelectMaintenanceTransporter(handlingId, gyroScrew.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no component of specified"+
		" type available for maintenance transporter selection")

	// trigger select maintenance transporter
	err = client.SelectMaintenanceTransporter(handlingId, towTruck.Id)
	c.Assert(err, IsNil)

	WaitStateLeft(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_transporter_selection)
}

func (s *TestSuite) TestSelectMaintenanceTransporterWithAgentAsDestination(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)

	// Map transporting equipements
	phydb := loadPhysicalData(c, "test")
	towTruck, err := phydb.Components.Find("Tow Truck")
	c.Assert(err, IsNil)

	data := client.Model.GetData()
	maintenanceAutomat := getSomeAutomatByName(c, data, "Maintenance Automat 1")
	repairTeam := getSomeUnitByName(c, data, "Maintenance Log Unit 1")

	// Teleport repair team away
	to := swapi.Point{X: -15.8501, Y: 28.3248}
	err = client.Teleport(swapi.MakeUnitTasker(repairTeam.Id), to)
	c.Assert(err, IsNil)

	SetManualMaintenance(c, client, maintenanceAutomat.Id)
	handlingId := TriggerBreakdown(c, client, phydb)

	WaitStateEntered(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_transporter_selection)

	// error: third parameter is an invalid agent identifier
	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters(
		swapi.MakeIdentifier(handlingId),
		swapi.MakeIdentifier(towTruck.Id),
		swapi.MakeAgent(0)))
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid destination agent identifier")

	// trigger select maintenance transporter with destination
	endTick := client.Model.GetData().MaintenanceHandlings[handlingId].Provider.EndTick
	c.Assert(endTick, Equals, int32(0))

	err = client.SelectMaintenanceTransporterTest(swapi.MakeParameters(
		swapi.MakeIdentifier(handlingId),
		swapi.MakeIdentifier(towTruck.Id),
		swapi.MakeAgent(repairTeam.Id)))
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		h := data.MaintenanceHandlings[handlingId]
		return h != nil && h.Provider != nil &&
			h.Provider.EndTick != endTick
	})
}

func (s *TestSuite) TestSelectDiagnosisTeam(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	phydb := loadPhysicalData(c, "test")
	towTruck, err := phydb.Components.Find("Tow Truck")
	c.Assert(err, IsNil)
	gyroScrew, err := phydb.Components.Find("Gyroscrew 1")
	c.Assert(err, IsNil)
	otherEquipment, err := phydb.Components.Find("equipment")
	c.Assert(err, IsNil)

	// error: invalid parameters count, parameters expected
	err = client.SelectDiagnosisTeamTest(swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	data := client.Model.GetData()
	maintenanceAutomat := getSomeAutomatByName(c, data, "Maintenance Automat 1")

	// error: first parameter must be an identifier
	err = client.SelectDiagnosisTeamTest(swapi.MakeParameters(
		nil, swapi.MakeIdentifier(towTruck.Id)))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	err = client.SelectDiagnosisTeam(1000, towTruck.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid log request identifier")

	SetManualMaintenance(c, client, maintenanceAutomat.Id)
	handlingId := TriggerBreakdown(c, client, phydb)

	// error: not in diagnosis team waiting state
	err = client.SelectDiagnosisTeam(handlingId, towTruck.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: cannot select a diagnosis"+
		" team for a transport consign")

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
	err = client.SelectDiagnosisTeam(handlingId, otherEquipment.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no component of "+
		"specified type available for diagnosis team selection")

	err = client.SelectDiagnosisTeam(handlingId, gyroScrew.Id)
	c.Assert(err, IsNil)
	WaitStateLeft(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_diagnosis_team_selection)
}

func (s *TestSuite) TestSelectRepairTeam(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)

	phydb := loadPhysicalData(c, "test")
	gyroScrew, err := phydb.Components.Find("Gyroscrew 1")
	c.Assert(err, IsNil)
	otherEquipment, err := phydb.Components.Find("equipment")
	c.Assert(err, IsNil)

	maintenanceAutomat := getSomeAutomatByName(c, client.Model.GetData(), "Maintenance Automat 1")

	// error: invalid parameters count, parameters expected
	err = client.SelectRepairTeamTest(swapi.MakeParameters())
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be an identifier
	err = client.SelectRepairTeamTest(swapi.MakeParameters(
		nil, swapi.MakeIdentifier(gyroScrew.Id)))
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	err = client.SelectRepairTeam(1000, gyroScrew.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: invalid log request identifier")

	SetManualMaintenance(c, client, maintenanceAutomat.Id)
	handlingId := TriggerBreakdown(c, client, phydb)

	// error: not a repair consign
	err = client.SelectRepairTeam(handlingId, gyroScrew.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: cannot select a repair"+
		" team for a transport consign")

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
	err = client.SelectRepairTeam(handlingId, otherEquipment.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: no component of"+
		" specified type available for repair team selection")

	err = client.SelectRepairTeam(handlingId, gyroScrew.Id)
	c.Assert(err, IsNil)
	WaitStateLeft(c, client, handlingId,
		sword.LogMaintenanceHandlingUpdate_waiting_for_repair_team_selection)

	// error: not in repair team waiting state
	err = client.SelectRepairTeam(handlingId, gyroScrew.Id)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: repair consign not in"+
		" a waiting for repair team selection state")
}

func (s *TestSuite) TestReportCreation(c *C) {
	sim, client := connectAndWaitModel(c,
		NewAdminOpts(ExCrossroadLog).EnableTestCommands())
	defer stopSimAndClient(c, sim, client)
	phydb := loadPhysical(c, "test")
	data := client.Model.GetData()
	count := uint32(1)
	// Invalid report identifier
	err := client.CreateReport(count, InvalidIdentifier, InvalidIdentifier)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid source identifier
	startMissionId := uint32(79)
	err = client.CreateReport(count, startMissionId, InvalidIdentifier)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	unitId := getSomeUnit(c, data).Id
	reporter, _ := newReporter(c, unitId, phydb,
		"^work just started",
		"^Level %1 of CBRN protection put on",
	)
	reporter.Start(client.Model)

	err = client.CreateReport(count, startMissionId, unitId)
	c.Assert(err, IsNil)

	CBRNProtectionId := uint32(525)
	err = client.CreateReport(3, CBRNProtectionId, unitId, swapi.MakeInt(345))
	c.Assert(err, IsNil)

	client.Model.WaitTicks(1)
	reports := reporter.Stop()
	c.Assert(len(reports), Equals, 4)
}

func ParseTime(c *C, value *sword.DateTime) time.Time {
	t, err := swapi.GetTime(value.GetData())
	c.Assert(err, IsNil)
	return t
}

func CheckReportOrder(c *C, first, second *sword.Report) {
	firstTime := ParseTime(c, first.GetTime())
	secondTime := ParseTime(c, second.GetTime())
	c.Assert(firstTime.Equal(secondTime) || firstTime.After(secondTime), Equals, true)
	c.Assert(first.GetReport().GetId(), Lesser, second.GetReport().GetId())
}

func CreateReports(c *C, client *swapi.Client) {
	// Create a bunch of reports
	startMissionId := uint32(79)
	unitId := getSomeUnit(c, client.Model.GetData()).Id
	err := client.CreateReport(10, startMissionId, unitId)
	c.Assert(err, IsNil)

	// Create a report with a parameter
	CBRNProtectionId := uint32(525)
	err = client.CreateReport(1, CBRNProtectionId, unitId, swapi.MakeInt(345))
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestListReports(c *C) {
	sim, client := connectAndWaitModel(c,
		NewAdminOpts(ExCrossroadLog).EnableTestCommands())
	defer stopSimAndClient(c, sim, client)

	// Check the size of reports
	reports, _, err := client.ListReports(0, 0)
	c.Assert(err, IsNil)
	c.Assert(len(reports), Equals, 0)

	CreateReports(c, client)

	_, err = client.Pause()
	c.Assert(err, IsNil)

	// Get all reports
	allReports, next, err := client.ListReports(math.MaxInt32, 0)
	c.Assert(err, IsNil)
	c.Assert(len(allReports), Lesser, math.MaxInt32)
	c.Assert(next, Equals, uint32(0))

	// Get reports by package of two reports
	reports = []*sword.Report{}
	part, next, err := client.ListReports(2, 0)
	c.Assert(len(part), Equals, 2)
	c.Assert(next, Greater, uint32(0))
	reports = append(reports, part...)
	for next != 0 {
		part, next, err = client.ListReports(2, next)
		c.Assert(len(part) == 2 && next != 0 || len(part) <= 2 && next == 0, Equals, true)
		reports = append(reports, part...)
	}
	// Check equality between 'allReports' and 'reports'
	swtest.DeepEquals(c, allReports, reports)

	// Check chronological order
	for i := 0; i < len(reports)-1; i++ {
		CheckReportOrder(c, reports[i], reports[i+1])
	}
	if len(reports) != 0 {
		CheckReportOrder(c, reports[0], reports[len(reports)-1])
	}
}

func (s *TestSuite) TestCheckpointListReports(c *C) {
	sim, client := connectAndWaitModel(c,
		NewAdminOpts(ExCrossroadLog).EnableTestCommands())
	defer stopSimAndClient(c, sim, client)

	CreateReports(c, client)

	_, err := client.Pause()
	c.Assert(err, IsNil)

	// Get all reports
	allReports, _, err := client.ListReports(math.MaxInt32, 0)
	c.Assert(err, IsNil)

	// Create checkpoint
	sim, client, _ = checkpointAndRestart(c, sim, client)
	defer stopSimAndClient(c, sim, client)

	reports, _, err := client.ListReports(math.MaxInt32, 0)
	c.Assert(err, IsNil)
	// Check equality between 'allReports' and 'reports'
	swtest.DeepEquals(c, allReports, reports)
}

func (s *TestSuite) TestHideActions(c *C) {
	opts := NewAllUserOpts(ExCrossroadLog)
	sim, clientA := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, clientA)
	clientB := connectAndWait(c, sim, opts.User, opts.Password)
	defer clientB.Close()
	listenerA := ModelListener{actions: true}
	listenerB := ModelListener{actions: true}
	clientA.Model.RegisterListener(listenerA.Notify)
	clientB.Model.RegisterListener(listenerB.Notify)

	// check we don't see clientA action
	clientA.HideActions()
	d := clientA.Model.GetData()
	supplier := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	SetManualSupply(c, clientA, supplier, true)
	// we assume waiting one tick is sufficient
	// to sync on previous action
	clientB.Model.WaitTicks(1)
	listenerA.Check(c)
	listenerB.Check(c)

	// check we still see clientB action
	SetManualSupply(c, clientB, supplier, false)
	clientB.Model.WaitTicks(1)
	// close listeners to avoid data races in listeners
	clientA.Close()
	clientB.Close()
	c.Assert(listenerA.events, HasLen, 1)
	event := swapi.ModelEvent{
		Tag: swapi.ActionCreate,
		Id:  listenerA.events[0].Id, // ignore server-side id
	}
	listenerA.Check(c, event)
	listenerB.Check(c, event)
}

func (s *TestSuite) TestReplayListReports(c *C) {
	sim, client := connectAndWaitModel(c,
		NewAdminOpts(ExCrossroadLog).EnableTestCommands())
	defer stopSimAndClient(c, sim, client)

	// Create reports
	CreateReports(c, client)

	// Pause sim after 4 tick
	tick, delay, err := client.Resume(4)
	c.Assert(err, IsNil)

	// Create reports
	CreateReports(c, client)

	tickBis := tick + delay
	c.Assert(tickBis, Greater, tick)

	client.Model.WaitUntilTickEnds(tickBis)

	// Get all reports
	allReports, _, err := client.ListReports(math.MaxInt32, 0)
	c.Assert(err, IsNil)

	sim.Stop()
	client.Close()
	replay := startReplay(c, sim.Opts)
	defer replay.Stop()
	client = loginAndWaitModel(c, replay, NewAdminOpts(""))
	defer client.Close()

	// Check all reports in replay
	replayReports, _, err := client.ReplayListReports(math.MaxInt32, 0, 0)
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, allReports, replayReports)

	// Check one part of reports in replay
	reports, _, err := client.ReplayListReports(math.MaxInt32, 0, uint32(tick))
	c.Assert(err, IsNil)
	c.Assert(len(replayReports), Greater, len(reports))
	swtest.DeepEquals(c, replayReports[len(replayReports)-len(reports):], reports)
}

func (s *TestSuite) TestListReportsInitialization(c *C) {
	opts := NewAdminOpts(ExCrossroadLog).EnableTestCommands().RecordReports()
	sim, clientA := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, clientA)

	// Create reports
	CreateReports(c, clientA)

	_, err := clientA.Pause()
	c.Assert(err, IsNil)

	// List reports
	reportsA := clientA.Model.GetData().Reports
	c.Assert(len(reportsA), Greater, 0)

	opts.Paused = true
	clientB := connectClient(c, sim, opts)
	err = clientB.Login(opts.User, opts.Password)
	c.Assert(err, IsNil)
	ok := clientB.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true)
	defer clientB.Close()

	// Check report initialization
	reportsB := clientB.Model.GetData().Reports
	swtest.DeepEquals(c, reportsA, reportsB)
}
