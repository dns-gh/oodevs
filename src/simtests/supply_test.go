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
	"swapi"
	"sword"
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
	case <-time.After(1 * time.Minute):
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

func checkSupplyUpdates(c *C, client *swapi.Client, unit *swapi.Unit, supplier *sword.Tasker, provider *sword.Tasker, callback SupplyCallback, updates []SupplyUpdateChecker) {
	checkers := []SupplyChecker{&SupplyCreateChecker{supplier, provider}}
	for i := range updates {
		checkers = append(checkers, &updates[i])
	}
	checkers = append(checkers, SupplyDeleteChecker{})
	checkSupply(c, client, unit, -1, callback, checkers...)
}

func (s *TestSuite) TestSupplyHandlingsBase(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	supplierId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	supplier := swapi.MakeAutomatTasker(supplierId)
	removeElectrogen_1 := func() {
		err := client.ChangeDotation(unit.Id,
			map[uint32]*swapi.ResourceDotation{
				uint32(electrogen_1): &swapi.ResourceDotation{
					Quantity:  0,
					Threshold: 50,
				}})
		c.Assert(err, IsNil)
	}
	checkSupplyUpdates(c, client, unit, supplier, supplier, removeElectrogen_1, []SupplyUpdateChecker{
		{"convoy_waiting_for_transporters"},
		{"convoy_setup"},
		{"convoy_moving_to_loading_point"},
		{"convoy_loading"},
		{"convoy_moving_to_unloading_point"},
		{"convoy_unloading"},
		{"convoy_moving_back_to_loading_point"},
		{"convoy_finished"},
	})
	automat := getSomeAutomatByName(c, d, "Supply Mobile Infantry Platoon")
	supplier2Id := getSomeFormationByName(c, d, "Supply F2").Id
	supplier2 := swapi.MakeFormationTasker(supplier2Id)
	// change links to have different superiors e.g. a 'nominal' and a 'current'
	err := client.LogisticsChangeLinks(automat.Id, []uint32{supplierId, supplier2Id})
	c.Assert(err, IsNil)
	removeElectrogen_2 := func() {
		err := client.ChangeDotation(unit.Id,
			map[uint32]*swapi.ResourceDotation{
				uint32(electrogen_2): &swapi.ResourceDotation{
					Quantity:  0,
					Threshold: 50,
				}})
		c.Assert(err, IsNil)
	}
	// the 'current' superior is searched first
	checkSupplyUpdates(c, client, unit, supplier2, supplier2, removeElectrogen_2, []SupplyUpdateChecker{
		{"convoy_waiting_for_transporters"},
		{"convoy_setup"},
		{"convoy_moving_to_loading_point"},
		{"convoy_loading"},
		{"convoy_moving_to_unloading_point"},
		{"convoy_unloading"},
		{"convoy_moving_back_to_loading_point"},
		{"convoy_finished"},
	})
	// the 'nominal' superior is searched if the 'current' superior cannot handle the request
	checkSupplyUpdates(c, client, unit, supplier, supplier, removeElectrogen_1, []SupplyUpdateChecker{
		{"convoy_waiting_for_transporters"},
		{"convoy_setup"},
		{"convoy_moving_to_loading_point"},
		{"convoy_loading"},
		{"convoy_moving_to_unloading_point"},
		{"convoy_unloading"},
		{"convoy_moving_back_to_loading_point"},
		{"convoy_finished"},
	})
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
	quotas := map[uint32]int32{
		uint32(electrogen_1): 1000,
		uint32(electrogen_3): 1000,
	}
	err := client.LogisticsSupplyChangeQuotas(supplierId, provider, quotas)
	c.Assert(err, IsNil)
	removeElectrogen_1 := func() {
		err := client.RecoverStocks(unit.Id,
			map[uint32]*swapi.ResourceDotation{
				uint32(electrogen_1): &swapi.ResourceDotation{
					Quantity:  0,
					Threshold: 50,
				}})
		c.Assert(err, IsNil)
	}
	checkSupplyUpdates(c, client, unit, supplier, provider, removeElectrogen_1, []SupplyUpdateChecker{
		{"convoy_waiting_for_transporters"},
		{"convoy_setup"},
		{"convoy_moving_to_loading_point"},
		{"convoy_loading"},
		{"convoy_moving_to_unloading_point"},
		{"convoy_unloading"},
		{"convoy_moving_back_to_loading_point"},
		{"convoy_finished"},
	})

	supplier2Id := getSomeFormationByName(c, d, "Supply F4").Id
	supplier2 := swapi.MakeFormationTasker(supplier2Id)
	// change links to have different superiors e.g. a 'nominal' and a 'current'
	err = client.LogisticsChangeLinks(automat.Id, []uint32{supplierId, supplier2Id})
	c.Assert(err, IsNil)
	err = client.LogisticsSupplyChangeQuotas(supplierId, provider, quotas)
	c.Assert(err, IsNil)
	err = client.LogisticsSupplyChangeQuotas(supplier2Id, provider, quotas)
	c.Assert(err, IsNil)
	removeElectrogen_3 := func() {
		err := client.RecoverStocks(unit.Id,
			map[uint32]*swapi.ResourceDotation{
				uint32(electrogen_3): &swapi.ResourceDotation{
					Quantity:  0,
					Threshold: 50,
				}})
		c.Assert(err, IsNil)
	}
	// the 'current' superior is searched first
	checkSupplyUpdates(c, client, unit, supplier2, provider, removeElectrogen_3, []SupplyUpdateChecker{
		{"convoy_waiting_for_transporters"},
		{"convoy_setup"},
		{"convoy_moving_to_loading_point"},
		{"convoy_loading"},
		{"convoy_moving_to_unloading_point"},
		{"convoy_unloading"},
		{"convoy_moving_back_to_loading_point"},
		{"convoy_finished"},
	})
	// the 'nominal' superior is searched if the 'current' superior cannot handle the request
	checkSupplyUpdates(c, client, unit, supplier, provider, removeElectrogen_1, []SupplyUpdateChecker{
		{"convoy_waiting_for_transporters"},
		{"convoy_setup"},
		{"convoy_moving_to_loading_point"},
		{"convoy_loading"},
		{"convoy_moving_to_unloading_point"},
		{"convoy_unloading"},
		{"convoy_moving_back_to_loading_point"},
		{"convoy_finished"},
	})
}
