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

type RequestCheckContext struct {
	request   uint32
	requester uint32
	supplier  uint32
	verbose   bool
	deleted   bool
	last      sword.LogSupplyRequestUpdate_EnumLogSupplyRequestStatus
	group     sync.WaitGroup
	errors    chan error
}

type RequestChecker interface {
	Check(c *C, ctx *RequestCheckContext, msg *sword.SimToClient_Content) bool
}

type RequestCreateChecker struct {
	resource  uint32
	requester uint32
}

func (cc *RequestCreateChecker) Check(c *C, ctx *RequestCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogSupplyRequestUpdate, IsNil)
	c.Check(msg.LogSupplyRequestDestruction, IsNil)
	m := msg.LogSupplyRequestCreation
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("+ %+v\n", m)
	}
	c.Check(ctx.request, Equals, uint32(0))
	ctx.request = m.GetRequest().GetId()
	c.Check(m.GetResource().GetId(), Equals, cc.resource)
	c.Check(m.GetRequester().GetId(), Equals, cc.requester)
	return true
}

type RequestUpdateChecker struct {
	state     string
	requested int32
	granted   int32
	convoyed  int32
	delivered int32
}

func (cc *RequestUpdateChecker) Check(c *C, ctx *RequestCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogSupplyRequestCreation, IsNil)
	c.Check(msg.LogSupplyRequestDestruction, IsNil)
	m := msg.LogSupplyRequestUpdate
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("* %+v\n", m)
	}
	c.Check(ctx.request, Not(Equals), uint32(0))
	c.Check(m.GetRequest().GetId(), Equals, ctx.request)
	c.Check(m.GetRecipient().GetId(), Equals, ctx.requester)
	c.Check(m.GetSupplier().GetId(), Equals, ctx.supplier)
	value, ok := sword.LogSupplyRequestUpdate_EnumLogSupplyRequestStatus_value[cc.state]
	next := sword.LogSupplyRequestUpdate_EnumLogSupplyRequestStatus(value)
	c.Check(ok, Equals, true)
	c.Check(m.GetState(), Equals, next)
	c.Check(m.GetRequested(), Equals, cc.requested)
	c.Check(m.GetGranted(), Equals, cc.granted)
	c.Check(m.GetConvoyed(), Equals, cc.convoyed)
	c.Check(m.GetDelivered(), Equals, cc.delivered)
	ctx.last = next
	return true
}

type RequestDeleteChecker struct{}

func (RequestDeleteChecker) Check(c *C, ctx *RequestCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogSupplyRequestCreation, IsNil)
	c.Check(msg.LogSupplyRequestUpdate, IsNil)
	m := msg.LogSupplyRequestDestruction
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("- %+v\n", m)
	}
	c.Check(ctx.request, Not(Equals), uint32(0))
	c.Check(m.GetRequest().GetId(), Equals, ctx.request)
	ctx.deleted = true
	return true
}

type RequestCallback func()

func checkRequest(c *C, client *swapi.Client, offset int, requester, supplier uint32, callback RequestCallback, checkers ...RequestChecker) {
	client.Pause()
	check := RequestCheckContext{
		requester: requester,
		supplier:  supplier,
		errors:    make(chan error),
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

func checkRequestUpdates(c *C, client *swapi.Client, requester, supplier, resource uint32, callback RequestCallback) {
	checkers := []RequestChecker{&RequestCreateChecker{resource, requester},
		&RequestUpdateChecker{"request_granted", 10, 10, 0, 0},
		&RequestUpdateChecker{"request_conveyed", 10, 10, 10, 0},
		&RequestUpdateChecker{"request_delivered", 10, 10, 0, 10},
		&RequestDeleteChecker{},
	}
	checkRequest(c, client, -1, requester, supplier, callback, checkers...)
}

func (s *TestSuite) TestSupplyRequests(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	supplierId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	removeElectrogen_1 := func() {
		err := client.ChangeResource(unit.Id,
			map[uint32]*swapi.Resource{
				uint32(electrogen_1): &swapi.Resource{
					Quantity:  0,
					Threshold: 50,
				}})
		c.Assert(err, IsNil)
	}
	checkRequestUpdates(c, client, unit.AutomatId, supplierId, uint32(electrogen_1), removeElectrogen_1)
}
