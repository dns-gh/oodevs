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
	"fmt"
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/sword"
	"sync"
	"time"
)

func makeBreakdown(c *C, id uint32, eq *swapi.Equipment, count int32, breakdown BreakdownType) map[uint32]*swapi.Equipment {
	c.Assert(eq.Available, Greater, count)
	return map[uint32]*swapi.Equipment{
		id: {
			Available:  eq.Available - count,
			Repairable: count,
			Breakdowns: []int32{int32(breakdown)},
		},
	}
}

type BreakdownType int32
type ResourceType int32

const (
	// replace with swapi/phy
	// breakdowns
	electronic_1 BreakdownType = 108
	electronic_2               = 109
	electronic_3               = 110
	mobility_1                 = 111
	mobility_2                 = 112
	mobility_3                 = 113
	// resources
	electrogen_1 ResourceType = 108
	electrogen_2              = 109
	electrogen_3              = 110
	gyroscope_1               = 111
	gyroscope_2               = 112
	gyroscope_3               = 113
)

type MaintenanceCheckContext struct {
	data        *swapi.ModelData
	breakdown   BreakdownType
	unitId      uint32
	handlingId  uint32
	equipmentId uint32
	verbose     bool
	diagnosed   bool
	deleted     bool
	last        sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus
	group       sync.WaitGroup
	errors      chan error
}

type MaintenanceChecker interface {
	Check(c *C, ctx *MaintenanceCheckContext, msg *sword.SimToClient_Content) bool
}

type MaintenanceCreateChecker struct{}

func (MaintenanceCreateChecker) Check(c *C, ctx *MaintenanceCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogMaintenanceHandlingUpdate, IsNil)
	c.Check(msg.LogMaintenanceHandlingDestruction, IsNil)
	m := msg.LogMaintenanceHandlingCreation
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("+ %+v\n", m)
	}
	c.Check(ctx.handlingId, Equals, uint32(0))
	c.Check(m.GetUnit().GetId(), Equals, ctx.unitId)
	c.Check(m.GetEquipement().GetId(), Equals, ctx.equipmentId)
	c.Check(m.GetBreakdown().GetId(), Equals, uint32(ctx.breakdown))
	ctx.handlingId = m.GetRequest().GetId()
	return true
}

func IsProvider(d *swapi.ModelData, obtained uint32, expected *sword.Tasker, isStrict bool) bool {
	if obtained == 0 {
		return expected.Automat == nil && expected.Formation == nil
	}
	if isStrict {
		return swapi.GetTaskerId(expected) == obtained
	}
	// abuse profile queries to check whether our unit is in automat/formation
	profile := swapi.Profile{}
	if expected.Automat != nil {
		profile.ReadOnlyAutomats = map[uint32]struct{}{
			expected.Automat.GetId(): {},
		}
	} else if expected.Formation != nil {
		profile.ReadOnlyFormations = map[uint32]struct{}{
			expected.Formation.GetId(): {},
		}
	}
	return d.IsUnitInProfile(obtained, &profile)
}

type MaintenanceUpdateChecker struct {
	name     string
	provider *sword.Tasker
}

func (cc *MaintenanceUpdateChecker) check(c *C, ctx *MaintenanceCheckContext, msg *sword.SimToClient_Content, strict bool) bool {
	c.Check(msg.LogMaintenanceHandlingCreation, IsNil)
	c.Check(msg.LogMaintenanceHandlingDestruction, IsNil)
	m := msg.LogMaintenanceHandlingUpdate
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("* %+v\n", m)
	}
	c.Check(ctx.handlingId, Not(Equals), uint32(0))
	c.Check(m.GetRequest().GetId(), Equals, ctx.handlingId)
	c.Check(IsProvider(ctx.data, m.GetProvider().GetId(), cc.provider, strict), Equals, true)
	value, ok := sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus_value[cc.name]
	next := sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus(value)
	c.Check(ok, Equals, true)
	c.Check(m.GetState(), Equals, next)
	if ctx.diagnosed != m.GetDiagnosed() {
		c.Check(ctx.last, Equals, sword.LogMaintenanceHandlingUpdate_diagnosing)
	}
	ctx.diagnosed = ctx.diagnosed || m.GetDiagnosed()
	ctx.last = next
	return true
}

func (cc *MaintenanceUpdateChecker) Check(c *C, ctx *MaintenanceCheckContext, msg *sword.SimToClient_Content) bool {
	return cc.check(c, ctx, msg, false)
}

type MaintenanceStrictUpdateChecker struct {
	MaintenanceUpdateChecker
}

func (cc *MaintenanceStrictUpdateChecker) Check(c *C, ctx *MaintenanceCheckContext, msg *sword.SimToClient_Content) bool {
	return cc.check(c, ctx, msg, true)
}

type MaintenanceApplyChecker struct {
	MaintenanceChecker
	operand func(ctx *MaintenanceCheckContext) error
}

func (m *MaintenanceApplyChecker) Check(c *C, ctx *MaintenanceCheckContext, msg *sword.SimToClient_Content) bool {
	ok := m.MaintenanceChecker.Check(c, ctx, msg)
	if ok {
		// we are inside a gosword register callback hence we cannot call
		// callbacks which would use swapi.Client. We do it in another
		// goroutine and wait for all of them to finish at the end of all tests
		ctx.group.Add(1)
		go func() {
			defer ctx.group.Done()
			ctx.errors <- m.operand(ctx)
		}()
	}
	return ok
}

type MaintenanceDeleteChecker struct{}

func (MaintenanceDeleteChecker) Check(c *C, ctx *MaintenanceCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogMaintenanceHandlingCreation, IsNil)
	c.Check(msg.LogMaintenanceHandlingUpdate, IsNil)
	m := msg.LogMaintenanceHandlingDestruction
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("- %+v\n", m)
	}
	c.Check(ctx.handlingId, Not(Equals), uint32(0))
	c.Check(m.GetRequest().GetId(), Equals, ctx.handlingId)
	c.Check(m.GetUnit().GetId(), Equals, ctx.unitId)
	ctx.deleted = true
	return true
}

func checkMaintenance(c *C, admin, client *swapi.Client, unit *swapi.Unit,
	offset int, breakdown BreakdownType, checkers ...MaintenanceChecker) {

	_, err := admin.Pause()
	c.Assert(err, IsNil)
	check := MaintenanceCheckContext{
		data:      client.Model.GetData(),
		unitId:    unit.Id,
		breakdown: breakdown,
		errors:    make(chan error),
	}
	eqid, eq := getSomeEquipment(c, unit)
	check.equipmentId = eqid
	idx := 0
	quit := make(chan struct{})
	ctx := client.Register(func(msg *swapi.SwordMessage, id, ctx int32, err error) bool {
		if err != nil {
			c.Error(err)
			close(quit)
			return true
		}
		if msg == nil ||
			msg.SimulationToClient == nil {
			return false
		}
		m := msg.SimulationToClient.GetMessage()
		prev := idx
		if checkers[idx].Check(c, &check, m) {
			idx++
		}
		if c.Failed() {
			c.Log("checker error at index ", prev+offset)
		}
		if c.Failed() || idx == len(checkers) || check.deleted {
			close(quit)
			return true
		}
		return false
	})
	defer client.Unregister(ctx)
	err = client.ChangeEquipmentState(unit.Id, makeBreakdown(c, eqid, eq, 1, breakdown))
	c.Assert(err, IsNil)
	_, _, err = admin.Resume(0)
	c.Assert(err, IsNil)
	select {
	case <-quit:
	case <-time.After(Timeout):
		c.Error("timeout")
	}
	go func() {
		check.group.Wait()
		close(check.errors)
	}()
	for err := range check.errors {
		c.Check(err, IsNil)
	}
	c.Assert(idx, Equals, len(checkers))
}

func checkMaintenanceUpdates(c *C, admin, client *swapi.Client, unit *swapi.Unit,
	breakdown BreakdownType, updates []MaintenanceUpdateChecker) {

	checkers := []MaintenanceChecker{MaintenanceCreateChecker{}}
	for i := range updates {
		checkers = append(checkers, &updates[i])
	}
	checkers = append(checkers, MaintenanceDeleteChecker{})
	checkMaintenance(c, admin, client, unit, -1, breakdown, checkers...)
}

func setParts(client *swapi.Client, provider *sword.Tasker, qty int32, resources ...ResourceType) error {
	d := client.Model.GetData()
	for _, u := range d.Units {
		if IsProvider(d, u.Id, provider, false) {
			next := map[uint32]*swapi.Resource{}
			for _, id := range resources {
				next[uint32(id)] = &swapi.Resource{Quantity: qty}
			}
			err := client.ChangeResource(u.Id, next)
			if err != nil {
				return err
			}
		}
	}
	return nil
}

func (s *TestSuite) TestMaintenanceHandlingsBase(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2 := swapi.MakeAutomatTasker(getSomeAutomatByName(c, d, "Maintenance Automat 1").Id)
	bld := swapi.MakeFormationTasker(getSomeFormationByName(c, d, "Maintenance BLD").Id)
	blt := swapi.MakeFormationTasker(getSomeFormationByName(c, d, "Maintenance BLT").Id)
	zero := &sword.Tasker{}
	checkMaintenanceUpdates(c, admin, client, unit, electronic_1, []MaintenanceUpdateChecker{
		{"moving_to_supply", tc2},
		{"diagnosing", tc2},
		{"waiting_for_repairer", tc2},
		{"repairing", tc2},
		{"moving_back", tc2},
		{"finished", zero},
	})
	checkMaintenanceUpdates(c, admin, client, unit, mobility_1, []MaintenanceUpdateChecker{
		{"transporter_moving_to_supply", tc2},
		{"transporter_loading", tc2},
		{"transporter_moving_back", tc2},
		{"transporter_unloading", tc2},
		{"diagnosing", tc2},
		{"waiting_for_repairer", tc2},
		{"repairing", tc2},
		{"moving_back", tc2},
		{"finished", zero},
	})
	checkMaintenanceUpdates(c, admin, client, unit, electronic_2, []MaintenanceUpdateChecker{
		{"moving_to_supply", tc2},
		{"diagnosing", tc2},
		{"searching_upper_levels", tc2},
		{"moving_to_supply", bld},
		{"waiting_for_repairer", bld},
		{"repairing", bld},
		{"moving_back", bld},
		{"finished", zero},
	})
	checkMaintenanceUpdates(c, admin, client, unit, mobility_2, []MaintenanceUpdateChecker{
		{"transporter_moving_to_supply", tc2},
		{"transporter_loading", tc2},
		{"transporter_moving_back", tc2},
		{"transporter_unloading", tc2},
		{"diagnosing", tc2},
		{"searching_upper_levels", tc2},
		{"waiting_for_transporter", bld},
		{"transporter_moving_to_supply", bld},
		{"transporter_loading", bld},
		{"transporter_moving_back", bld},
		{"transporter_unloading", bld},
		{"waiting_for_repairer", bld},
		{"repairing", bld},
		{"moving_back", bld},
		{"finished", zero},
	})
	checkMaintenanceUpdates(c, admin, client, unit, electronic_3, []MaintenanceUpdateChecker{
		{"moving_to_supply", tc2},
		{"diagnosing", tc2},
		{"searching_upper_levels", tc2},
		{"moving_to_supply", bld},
		{"searching_upper_levels", bld},
		{"moving_to_supply", blt},
		{"waiting_for_repairer", blt},
		{"repairing", blt},
		{"moving_back", blt},
		{"finished", zero},
	})
	checkMaintenanceUpdates(c, admin, client, unit, mobility_3, []MaintenanceUpdateChecker{
		{"transporter_moving_to_supply", tc2},
		{"transporter_loading", tc2},
		{"transporter_moving_back", tc2},
		{"transporter_unloading", tc2},
		{"diagnosing", tc2},
		{"searching_upper_levels", tc2},
		{"waiting_for_transporter", bld},
		{"transporter_moving_to_supply", bld},
		{"transporter_loading", bld},
		{"transporter_moving_back", bld},
		{"transporter_unloading", bld},
		{"searching_upper_levels", bld},
		{"waiting_for_transporter", blt},
		{"transporter_moving_to_supply", blt},
		{"transporter_loading", blt},
		{"transporter_moving_back", blt},
		{"transporter_unloading", blt},
		{"waiting_for_repairer", blt},
		{"repairing", blt},
		{"moving_back", blt},
		{"finished", zero},
	})
}

func checkUpdateThenApply(state string, provider *sword.Tasker, strict bool, op func(ctx *MaintenanceCheckContext) error) MaintenanceChecker {
	updater := MaintenanceUpdateChecker{state, provider}
	var checker MaintenanceChecker = &updater
	if strict {
		checker = &MaintenanceStrictUpdateChecker{updater}
	}
	return &MaintenanceApplyChecker{checker, op}
}

func checkUpdate(state string, provider *sword.Tasker) MaintenanceChecker {
	return &MaintenanceUpdateChecker{state, provider}
}

func (s *TestSuite) TestMaintenanceHandlingsWithMissingParts(c *C) {
	c.Skip("unreliable, see http://jira.masagroup.net/browse/SWBUG-11961")

	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2 := swapi.MakeAutomatTasker(getSomeAutomatByName(c, d, "Maintenance Automat 1").Id)
	zero := &sword.Tasker{}
	err := setParts(client, tc2, 0, electrogen_1)
	c.Assert(err, IsNil)
	checkMaintenance(c, admin, client, unit, 0, electronic_1,
		MaintenanceCreateChecker{},
		checkUpdate("moving_to_supply", tc2),
		checkUpdate("diagnosing", tc2),
		checkUpdateThenApply("waiting_for_parts", tc2, false,
			func(ctx *MaintenanceCheckContext) error {
				return setParts(client, tc2, 10, electrogen_1)
			}),
		checkUpdate("waiting_for_repairer", tc2),
		checkUpdate("repairing", tc2),
		checkUpdate("moving_back", tc2),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
	err = setParts(client, tc2, 0, gyroscope_1)
	c.Assert(err, IsNil)
	checkMaintenance(c, admin, client, unit, 0, mobility_1,
		MaintenanceCreateChecker{},
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdate("diagnosing", tc2),
		checkUpdateThenApply("waiting_for_parts", tc2, false,
			func(ctx *MaintenanceCheckContext) error {
				return setParts(client, tc2, 10, gyroscope_1)
			}),
		checkUpdate("waiting_for_repairer", tc2),
		checkUpdate("repairing", tc2),
		checkUpdate("moving_back", tc2),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
}

func SetManualMaintenance(c *C, client *swapi.Client, id uint32) {
	err := client.SetManualMaintenance(id, true)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		automat := data.Automats[id]
		if automat != nil {
			return automat.LogMaintenanceManual
		}
		return data.Formations[id].LogMaintenanceManual
	})
}

func (s *TestSuite) TestMaintenanceHandlingsWithAutomaticSelection(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bld := swapi.MakeFormationTasker(bldId)
	zero := &sword.Tasker{}

	SetManualMaintenance(c, client, tc2Id)
	SetManualMaintenance(c, client, bldId)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", tc2),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdateThenApply("waiting_for_diagnosis_team_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("diagnosing", tc2),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdateThenApply("waiting_for_transporter_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", bld),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdateThenApply("waiting_for_repair_team_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_parts", bld),
		checkUpdate("waiting_for_repairer", bld),
		checkUpdate("repairing", bld),
		checkUpdate("moving_back", bld),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
}

func (s *TestSuite) TestMaintenanceHandlingsWithManualSelection(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	bld := swapi.MakeFormationTasker(bldId)
	zero := &sword.Tasker{}
	const TowTruck = 4
	const gyroscrew_1 = 6
	const gyroscrew_2 = 7

	SetManualMaintenance(c, client, tc2Id)
	SetManualMaintenance(c, client, bldId)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectMaintenanceTransporter(ctx.handlingId, TowTruck)
			}),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdateThenApply("waiting_for_diagnosis_team_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectDiagnosisTeam(ctx.handlingId, gyroscrew_1)
			}),
		checkUpdate("diagnosing", tc2),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdateThenApply("waiting_for_transporter_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectMaintenanceTransporter(ctx.handlingId, TowTruck)
			}),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdateThenApply("waiting_for_repair_team_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectRepairTeam(ctx.handlingId, gyroscrew_2)
			}),
		checkUpdate("repairing", bld),
		checkUpdate("moving_back", bld),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
}

func (s *TestSuite) TestMaintenanceHandlingsWithManualTransporterMultipleSelection(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	const TowTruck = 4
	SetManualMaintenance(c, client, tc2Id)
	handlingId := uint32(0)
	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				handlingId = ctx.handlingId
				return nil
			}),
	)
	_, err := admin.Pause()
	c.Assert(err, IsNil)
	err = client.SelectMaintenanceTransporter(handlingId, TowTruck)
	c.Assert(err, IsNil)
	err = client.SelectMaintenanceTransporter(handlingId, TowTruck)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: transport consign already has a repair team selected")
}

func (s *TestSuite) TestMaintenanceHandlingsWithManualDiagnosisTeamMultipleSelection(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	const TowTruck = 4
	const gyroscrew_1 = 6
	SetManualMaintenance(c, client, tc2Id)
	handlingId := uint32(0)
	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectMaintenanceTransporter(ctx.handlingId, TowTruck)
			}),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdateThenApply("waiting_for_diagnosis_team_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				handlingId = ctx.handlingId
				return nil
			}),
	)
	_, err := admin.Pause()
	c.Assert(err, IsNil)
	err = client.SelectDiagnosisTeam(handlingId, gyroscrew_1)
	c.Assert(err, IsNil)
	err = client.SelectDiagnosisTeam(handlingId, gyroscrew_1)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: diagnosis consign already has a diagnosis team selected")
}

func (s *TestSuite) TestMaintenanceHandlingsWithManualRepairerMultipleSelection(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	bld := swapi.MakeFormationTasker(bldId)
	const TowTruck = 4
	const gyroscrew_1 = 6
	const gyroscrew_2 = 7
	SetManualMaintenance(c, client, tc2Id)
	SetManualMaintenance(c, client, bldId)
	handlingId := uint32(0)
	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectMaintenanceTransporter(ctx.handlingId, TowTruck)
			}),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdateThenApply("waiting_for_diagnosis_team_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectDiagnosisTeam(ctx.handlingId, gyroscrew_1)
			}),
		checkUpdate("diagnosing", tc2),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdateThenApply("waiting_for_transporter_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectMaintenanceTransporter(ctx.handlingId, TowTruck)
			}),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdateThenApply("waiting_for_repair_team_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				handlingId = ctx.handlingId
				return nil
			}),
	)
	_, err := admin.Pause()
	c.Assert(err, IsNil)
	err = client.SelectRepairTeam(handlingId, gyroscrew_2)
	c.Assert(err, IsNil)
	err = client.SelectRepairTeam(handlingId, gyroscrew_2)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: repair consign not in a waiting for repair team selection state")
}

func (s *TestSuite) TestMaintenanceHandlingsWithBaseSwitchedBackToAutomatic(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	bld := swapi.MakeFormationTasker(bldId)
	zero := &sword.Tasker{}

	toAutomatic := func(id uint32) error {
		// set automat back to automatic mode
		err := client.SetManualMaintenance(id, false)
		if err != nil {
			return err
		}
		ok := client.Model.WaitCondition(func(data *swapi.ModelData) bool {
			return data.Automats[tc2Id].LogMaintenanceManual == false
		})
		if !ok {
			return fmt.Errorf("condition not reached")
		}
		return nil
	}

	SetManualMaintenance(c, client, tc2Id)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return toAutomatic(tc2Id)
			}),
		checkUpdate("waiting_for_transporter", tc2),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdate("diagnosing", tc2),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdate("waiting_for_transporter", bld),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdate("waiting_for_repairer", bld),
		checkUpdate("repairing", bld),
		checkUpdate("moving_back", bld),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)

	SetManualMaintenance(c, client, tc2Id)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", tc2),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdateThenApply("waiting_for_diagnosis_team_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return toAutomatic(tc2Id)
			}),
		checkUpdate("diagnosing", tc2),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdate("waiting_for_transporter", bld),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdate("waiting_for_repairer", bld),
		checkUpdate("repairing", bld),
		checkUpdate("moving_back", bld),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)

	SetManualMaintenance(c, client, bldId)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdate("diagnosing", tc2),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdateThenApply("waiting_for_transporter_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", bld),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdateThenApply("waiting_for_repair_team_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return toAutomatic(bldId)
			}),
		checkUpdate("waiting_for_parts", bld),
		checkUpdate("waiting_for_repairer", bld),
		checkUpdate("repairing", bld),
		checkUpdate("moving_back", bld),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
}

func (s *TestSuite) TestMaintenanceTransferToLogisticSuperiorForTransporting(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bld := swapi.MakeFormationTasker(getSomeFormationByName(c, d, "Maintenance BLD").Id)
	zero := &sword.Tasker{}

	SetManualMaintenance(c, client, tc2Id)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.TransferToLogisticSuperior(ctx.handlingId)
			}),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdate("waiting_for_transporter", bld),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdateThenApply("diagnosing", bld, false,
			func(ctx *MaintenanceCheckContext) error {
				err := client.TransferToLogisticSuperior(ctx.handlingId)
				ok, msg := IsSwordError.Check([]interface{}{err, "error_invalid_parameter"}, nil)
				if !ok {
					return fmt.Errorf(msg)
				}
				return nil
			}),
		checkUpdate("waiting_for_repairer", bld),
		checkUpdate("repairing", bld),
		checkUpdate("moving_back", bld),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
}

func (s *TestSuite) TestMaintenanceTransferToLogisticSuperiorForDiagnosing(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bld := swapi.MakeFormationTasker(getSomeFormationByName(c, d, "Maintenance BLD").Id)
	zero := &sword.Tasker{}

	SetManualMaintenance(c, client, tc2Id)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", tc2),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdateThenApply("waiting_for_diagnosis_team_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.TransferToLogisticSuperior(ctx.handlingId)
			}),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdate("waiting_for_transporter", bld),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdate("diagnosing", bld),
		checkUpdate("waiting_for_repairer", bld),
		checkUpdate("repairing", bld),
		checkUpdate("moving_back", bld),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
}

func (s *TestSuite) TestMaintenanceTransferToLogisticSuperiorForRepair(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	bld := swapi.MakeFormationTasker(bldId)
	bltId := getSomeFormationByName(c, d, "Maintenance BLT").Id
	blt := swapi.MakeFormationTasker(bltId)
	zero := &sword.Tasker{}

	SetManualMaintenance(c, client, tc2Id)
	SetManualMaintenance(c, client, bldId)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", tc2),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdateThenApply("waiting_for_diagnosis_team_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("diagnosing", tc2),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdateThenApply("waiting_for_transporter_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", bld),
		checkUpdate("transporter_moving_to_supply", bld),
		checkUpdate("transporter_loading", bld),
		checkUpdate("transporter_moving_back", bld),
		checkUpdate("transporter_unloading", bld),
		checkUpdateThenApply("waiting_for_repair_team_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.TransferToLogisticSuperior(ctx.handlingId)
			}),
		checkUpdate("searching_upper_levels", bld),
		checkUpdate("waiting_for_parts", blt),
		checkUpdate("waiting_for_repairer", blt),
		checkUpdate("repairing", blt),
		checkUpdate("moving_back", blt),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
}

func (s *TestSuite) TestMaintenanceSuperiorUnableToRepair(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	bld := swapi.MakeFormationTasker(bldId)
	bltId := getSomeFormationByName(c, d, "Maintenance BLT").Id
	blt := swapi.MakeFormationTasker(bltId)

	SetManualMaintenance(c, client, tc2Id)
	SetManualMaintenance(c, client, bldId)
	SetManualMaintenance(c, client, bltId)

	phydb := loadPhysical(c, "worldwide")
	reporter, _ := newReporter(c, unit.Id, phydb, "Unable to repair")
	reporter.Start(client.Model)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.TransferToLogisticSuperior(ctx.handlingId)
			}),
		checkUpdate("searching_upper_levels", tc2),
		checkUpdateThenApply("waiting_for_transporter_selection", bld, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.TransferToLogisticSuperior(ctx.handlingId)
			}),
		checkUpdate("searching_upper_levels", bld),
		checkUpdateThenApply("waiting_for_transporter_selection", blt, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.TransferToLogisticSuperior(ctx.handlingId)
			}),
		checkUpdate("searching_upper_levels", blt),
	)
	client.Model.WaitTicks(2)
	reports := reporter.Stop()
	c.Assert(len(reports), Equals, 1)
}

func (s *TestSuite) TestMaintenanceAbortsWhenCrewDies(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	zero := &sword.Tasker{}

	SetManualMaintenance(c, client, tc2Id)

	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", tc2),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		checkUpdateThenApply("waiting_for_diagnosis_team_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				return client.ChangeHumanState(unit.Id,
					[]*swapi.Human{
						{
							Quantity: 1,
							Rank:     0,
							State:    2,
						},
						{
							Quantity: 1,
							Rank:     1,
							State:    2,
						},
						{
							Quantity: 3,
							Rank:     2,
							State:    2,
						},
					})
			}),
		checkUpdate("finished", zero),
		MaintenanceDeleteChecker{},
	)
}
