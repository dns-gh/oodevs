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

func IsParentEntity(actual *sword.ParentEntity, expected *sword.Tasker) bool {
	return swapi.GetParentEntityId(actual) == swapi.GetTaskerId(expected)
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
	c.Check(IsParentEntity(m.GetSupplier(), cc.supplier), Equals, true)
	c.Check(IsParentEntity(m.GetTransportersProvider(), cc.provider), Equals, true)
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

func checkSupply(c *C, client *swapi.Client, unit *swapi.Unit, offset int, resource ResourceType, checkers ...SupplyChecker) {
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
	err := client.ChangeDotation(unit.Id,
		map[uint32]*swapi.ResourceDotation{
			uint32(resource): &swapi.ResourceDotation{
				Quantity:  0,
				Threshold: 50,
			}})
	c.Assert(err, IsNil)
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

func checkSupplyUpdates(c *C, client *swapi.Client, unit *swapi.Unit, provider *sword.Tasker, resource ResourceType, updates []SupplyUpdateChecker) {
	checkers := []SupplyChecker{&SupplyCreateChecker{provider, provider}}
	for i := range updates {
		checkers = append(checkers, &updates[i])
	}
	checkers = append(checkers, SupplyDeleteChecker{})
	checkSupply(c, client, unit, -1, resource, checkers...)
}

func (s *TestSuite) TestSupplyHandlingsBase(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	supplyId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	supply := swapi.MakeAutomatTasker(supplyId)
	checkSupplyUpdates(c, client, unit, supply, electrogen_1, []SupplyUpdateChecker{
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
	supply2Id := getSomeAutomatByName(c, d, "Supply Log Automat 1d").Id
	supply2 := swapi.MakeAutomatTasker(supply2Id)
	// change links to have different superiors e.g. a 'current' and a 'nominal'
	err := client.LogisticsChangeLinks(automat.Id, []uint32{supplyId, supply2Id})
	c.Assert(err, IsNil)
	// the 'current' superior is searched first
	checkSupplyUpdates(c, client, unit, supply2, electrogen_2, []SupplyUpdateChecker{
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
	checkSupplyUpdates(c, client, unit, supply, electrogen_1, []SupplyUpdateChecker{
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
