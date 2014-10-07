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

type SupplyCheckContext struct {
	data       *swapi.ModelData
	unitId     uint32
	handlingId uint32
	verbose    bool
	deleted    bool
	last       sword.LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus
	group      sync.WaitGroup
	errors     chan error
}

type SupplyChecker interface {
	Check(c *C, ctx *SupplyCheckContext, msg *sword.SimToClient_Content) bool
}

type SupplyEmptyChecker struct {
}

func (cc *SupplyEmptyChecker) Check(c *C, ctx *SupplyCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogSupplyHandlingCreation, IsNil)
	c.Check(msg.LogSupplyHandlingUpdate, IsNil)
	c.Check(msg.LogSupplyHandlingDestruction, IsNil)
	return true
}

type SupplyCreateChecker struct {
	supplier *sword.Tasker
	provider *sword.Tasker
}

func (cc *SupplyCreateChecker) Check(c *C, ctx *SupplyCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogSupplyHandlingUpdate, IsNil)
	c.Check(msg.LogSupplyHandlingDestruction, IsNil)
	m := msg.LogSupplyHandlingCreation
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("+ %+v\n", m)
	}
	c.Check(ctx.handlingId, Equals, uint32(0))
	ctx.handlingId = m.GetRequest().GetId()
	c.Check(swapi.GetParentEntityId(m.GetSupplier()), Equals, swapi.GetTaskerId(cc.supplier))
	c.Check(swapi.GetParentEntityId(m.GetTransportersProvider()), Equals, swapi.GetTaskerId(cc.provider))
	return true
}

type SupplyUpdateChecker struct {
	name string
}

func (cc *SupplyUpdateChecker) Check(c *C, ctx *SupplyCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogSupplyHandlingCreation, IsNil)
	c.Check(msg.LogSupplyHandlingDestruction, IsNil)
	m := msg.LogSupplyHandlingUpdate
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("* %+v\n", m)
	}
	c.Check(ctx.handlingId, Not(Equals), uint32(0))
	c.Check(m.GetRequest().GetId(), Equals, ctx.handlingId)
	value, ok := sword.LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus_value[cc.name]
	next := sword.LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus(value)
	c.Check(ok, Equals, true)
	c.Check(m.GetState(), Equals, next)
	for _, request := range m.GetSupplyRequests() {
		c.Check(ctx.data.SupplyRequests[request.GetId()], Not(Equals), nil)
	}
	ctx.last = next
	return true
}

type SupplyDeleteChecker struct{}

func (SupplyDeleteChecker) Check(c *C, ctx *SupplyCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogSupplyHandlingCreation, IsNil)
	c.Check(msg.LogSupplyHandlingUpdate, IsNil)
	m := msg.LogSupplyHandlingDestruction
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("- %+v\n", m)
	}
	c.Check(ctx.handlingId, Not(Equals), uint32(0))
	c.Check(m.GetRequest().GetId(), Equals, ctx.handlingId)
	ctx.deleted = true
	return true
}

type SupplyCallback func()

func checkSupply(c *C, client *swapi.Client, unit *swapi.Unit, offset int, callback SupplyCallback, checkers ...SupplyChecker) {
	client.Pause()
	check := SupplyCheckContext{
		data:   client.Model.GetData(),
		unitId: unit.Id,
		errors: make(chan error),
	}
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
		if len(checkers) == 0 {
			return true
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
	callback()
	client.Resume(0)
	select {
	case <-quit:
	case <-time.After(WaitTimeout):
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

func checkSupplyUpdates(c *C, client *swapi.Client, unit *swapi.Unit, supplier *sword.Tasker, provider *sword.Tasker, callback SupplyCallback) {
	checkers := []SupplyChecker{&SupplyCreateChecker{supplier, provider},
		&SupplyUpdateChecker{"convoy_waiting_for_transporters"},
		&SupplyUpdateChecker{"convoy_setup"},
		&SupplyUpdateChecker{"convoy_moving_to_loading_point"},
		&SupplyUpdateChecker{"convoy_loading"},
		&SupplyUpdateChecker{"convoy_moving_to_unloading_point"},
		&SupplyUpdateChecker{"convoy_unloading"},
		&SupplyUpdateChecker{"convoy_moving_back_to_loading_point"},
		&SupplyUpdateChecker{"convoy_finished"},
		&SupplyDeleteChecker{},
	}
	checkSupply(c, client, unit, -1, callback, checkers...)
}

func (s *TestSuite) TestSupplyHandlingsBaseLowThreshold(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	supplierId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	supplier := swapi.MakeAutomatTasker(supplierId)

	// Capacity = 10, Quantity => 6 => 60%, above low threshold, nothing happens
	removeElectrogen_1 := func() {
		err := client.ChangeResource(unit.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_1): {
					Quantity:      6,
					LowThreshold:  50,
					HighThreshold: 100,
				}})
		c.Assert(err, IsNil)
	}
	checkSupply(c, client, unit, -1, removeElectrogen_1, []SupplyChecker{&SupplyEmptyChecker{}}...)

	// Capacity = 10, Quantity => 4 => 40%, below low threshold, a supply request is created
	removeElectrogen_2 := func() {
		err := client.ChangeResource(unit.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_1): {
					Quantity:      4,
					LowThreshold:  50,
					HighThreshold: 100,
				}})
		c.Assert(err, IsNil)
	}
	checkSupplyUpdates(c, client, unit, supplier, supplier, removeElectrogen_2)
}

func (s *TestSuite) TestSupplyHandlingsBaseHighThreshold(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := d.Units[132]
	unit2 := d.Units[133]
	unit3 := d.Units[133]
	supplierId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	supplier := swapi.MakeAutomatTasker(supplierId)

	// Capacity = 10, Quantity => 6 => 60%, above high threshold, nothing happens
	removeElectrogen_1 := func() {
		err := client.ChangeResource(unit2.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_1): {
					Quantity:      6,
					LowThreshold:  30,
					HighThreshold: 50,
				}})
		c.Assert(err, IsNil)
	}
	checkSupply(c, client, unit2, -1, removeElectrogen_1, []SupplyChecker{&SupplyEmptyChecker{}}...)

	// Capacity = 10, Quantity => 4 => 40%, between high and low threshold, should be resupplied when another unit resupplies
	removeElectrogen_2 := func() {
		err := client.ChangeResource(unit3.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_1): {
					Quantity:      4,
					LowThreshold:  30,
					HighThreshold: 50,
				}})
		c.Assert(err, IsNil)
	}
	checkSupply(c, client, unit3, -1, removeElectrogen_2, []SupplyChecker{&SupplyEmptyChecker{}}...)

	// Capacity = 10, Quantity => 4 => 40%, below low threshold, a supply request is created
	removeElectrogen_3 := func() {
		err := client.ChangeResource(unit.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_1): {
					Quantity:      4,
					LowThreshold:  50,
					HighThreshold: 100,
				}})
		c.Assert(err, IsNil)
	}
	checkSupply(c, client, unit2, -1, removeElectrogen_2, []SupplyChecker{&SupplyEmptyChecker{}}...)
	checkSupplyUpdates(c, client, unit3, supplier, supplier, removeElectrogen_3)
}

func deployCrossroadLogAutomat(c *C, client *swapi.Client, automatId uint32) {
	MissionLogDeploy := uint32(8)

	_, err := client.SendAutomatOrder(automatId, MissionLogDeploy,
		swapi.MakeParameters(swapi.MakeHeading(0), nil,
			swapi.MakeLimit(
				swapi.Point{X: -15.9137, Y: 28.42},
				swapi.Point{X: -15.9137, Y: 28.25}),
			swapi.MakeLimit(
				swapi.Point{X: -15.7153, Y: 28.42},
				swapi.Point{X: -15.7153, Y: 28.25}),
			nil))
	c.Assert(err, IsNil)
}

// Deploy all crossroad-log supply automats between terrain wide limits.
func deployCrossroadLogSupply(c *C, client *swapi.Client) {
	automatNames := []string{
		"Supply Log Automat F2",
		"Supply Log Automat F3",
		"Supply Log Automat F4",
	}
	d := client.Model.GetData()

	for _, name := range automatNames {
		automatId := getSomeAutomatByName(c, d, name).Id
		deployCrossroadLogAutomat(c, client, automatId)
	}
}

func (s *TestSuite) TestSupplyHandlingsBase(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	supplierId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	supplier := swapi.MakeAutomatTasker(supplierId)

	deployCrossroadLogSupply(c, client)

	removeElectrogen_1 := func() {
		err := client.ChangeResource(unit.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_1): {
					Quantity:      0,
					LowThreshold:  50,
					HighThreshold: 100,
				}})
		c.Assert(err, IsNil)
	}
	checkSupplyUpdates(c, client, unit, supplier, supplier, removeElectrogen_1)

	automat := getSomeAutomatByName(c, d, "Supply Mobile Infantry Platoon")
	supplier2Id := getSomeFormationByName(c, d, "Supply F2").Id
	supplier2 := swapi.MakeFormationTasker(supplier2Id)
	// change links to have different superiors e.g. a 'nominal' and a 'current'
	err := client.LogisticsChangeLinks(swapi.MakeAutomatTasker(automat.Id), []uint32{supplierId, supplier2Id})
	c.Assert(err, IsNil)
	removeElectrogen_2 := func() {
		err := client.ChangeResource(unit.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_2): {
					Quantity:      0,
					LowThreshold:  50,
					HighThreshold: 100,
				}})
		c.Assert(err, IsNil)
	}
	// the 'current' superior is searched first
	checkSupplyUpdates(c, client, unit, supplier2, supplier2, removeElectrogen_2)
	// the 'nominal' superior is searched if the 'current' superior cannot handle the request
	checkSupplyUpdates(c, client, unit, supplier, supplier, removeElectrogen_1)
}

func (s *TestSuite) TestSupplyHandlingsBaseToBase(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Supply Log Unit 1c")
	supplierId := getSomeFormationByName(c, d, "Supply F3").Id
	supplier := swapi.MakeFormationTasker(supplierId)
	automat := getSomeAutomatByName(c, d, "Supply Log Automat 1c")
	provider := swapi.MakeAutomatTasker(automat.Id)

	deployCrossroadLogSupply(c, client)

	quotas := map[uint32]int32{
		uint32(electrogen_1): 1000,
		uint32(electrogen_3): 1000,
	}
	err := client.LogisticsSupplyChangeQuotas(supplierId, provider, quotas)
	c.Assert(err, IsNil)
	removeElectrogen_1 := func() {
		err := client.RecoverStocks(unit.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_1): {
					Quantity:      0,
					LowThreshold:  50,
					HighThreshold: 100,
				}})
		c.Assert(err, IsNil)
	}
	checkSupplyUpdates(c, client, unit, supplier, provider, removeElectrogen_1)

	supplier2Id := getSomeFormationByName(c, d, "Supply F4").Id
	supplier2 := swapi.MakeFormationTasker(supplier2Id)
	// change links to have different superiors e.g. a 'nominal' and a 'current'
	err = client.LogisticsChangeLinks(provider, []uint32{supplierId, supplier2Id})
	c.Assert(err, IsNil)
	err = client.LogisticsSupplyChangeQuotas(supplierId, provider, quotas)
	c.Assert(err, IsNil)
	err = client.LogisticsSupplyChangeQuotas(supplier2Id, provider, quotas)
	c.Assert(err, IsNil)
	removeElectrogen_3 := func() {
		err := client.RecoverStocks(unit.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_3): {
					Quantity:      0,
					LowThreshold:  50,
					HighThreshold: 100,
				}})
		c.Assert(err, IsNil)
	}
	// the 'current' superior is searched first
	checkSupplyUpdates(c, client, unit, supplier2, provider, removeElectrogen_3)
	// the 'nominal' superior is searched if the 'current' superior cannot handle the request
	checkSupplyUpdates(c, client, unit, supplier, provider, removeElectrogen_1)
}

func SetManualSupply(c *C, client *swapi.Client, id uint32, mode bool) {
	err := client.SetManualSupply(id, mode)
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		automat := data.Automats[id]
		if automat != nil {
			return automat.LogSupplyManual
		}
		return data.Formations[id].LogSupplyManual
	})
}

func (s *TestSuite) TestSupplyHandlingsBaseManual(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	supplierId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	supplier := swapi.MakeAutomatTasker(supplierId)

	// no automatic convoy in manual supply
	SetManualSupply(c, client, supplierId, true)
	err := client.ChangeResource(unit.Id,
		map[uint32]*swapi.Resource{
			uint32(electrogen_1): {
				Quantity:      0,
				LowThreshold:  50,
				HighThreshold: 100,
			}})
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)
	c.Assert(client.Model.GetData().SupplyHandlings, HasLen, 0)

	// convoy launched when switching back to automatic supply
	checkSupplyUpdates(c, client, unit, supplier, supplier,
		func() { SetManualSupply(c, client, supplierId, false) })
}

func (s *TestSuite) TestSupplyHandlingsBaseToBaseManual(c *C) {
	c.Skip("unreliable, see http://jira.masagroup.net/browse/SWBUG-12855")
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Supply Log Unit 1c")
	supplierId := getSomeFormationByName(c, d, "Supply F3").Id
	supplier := swapi.MakeFormationTasker(supplierId)
	automat := getSomeAutomatByName(c, d, "Supply Log Automat 1c")
	provider := swapi.MakeAutomatTasker(automat.Id)
	quotas := map[uint32]int32{
		uint32(electrogen_1): 1000,
	}
	err := client.LogisticsSupplyChangeQuotas(supplierId, provider, quotas)
	c.Assert(err, IsNil)

	// no automatic convoy in manual supply
	SetManualSupply(c, client, supplierId, true)
	err = client.RecoverStocks(unit.Id,
		map[uint32]*swapi.Resource{
			uint32(electrogen_1): {
				Quantity:      0,
				LowThreshold:  50,
				HighThreshold: 100,
			}})
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)
	c.Assert(client.Model.GetData().SupplyHandlings, HasLen, 0)

	// convoy launched when switching back to automatic supply
	checkSupplyUpdates(c, client, unit, supplier, provider,
		func() { SetManualSupply(c, client, supplierId, false) })
}
