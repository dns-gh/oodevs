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

func checkRequest(c *C, admin, client *swapi.Client, offset int,
	requester, supplier uint32, callback RequestCallback, checkers ...RequestChecker) {

	_, err := admin.Pause()
	c.Assert(err, IsNil)
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
	_, _, err = admin.Resume(0)
	c.Assert(err, IsNil)
	select {
	case <-quit:
	case <-time.After(Cfg.Timeout):
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

func checkRequestUpdates(c *C, admin, client *swapi.Client, requester, supplier,
	resource uint32, callback RequestCallback) {

	checkers := []RequestChecker{&RequestCreateChecker{resource, requester},
		&RequestUpdateChecker{"request_granted", 10, 10, 0, 0},
		&RequestUpdateChecker{"request_conveyed", 10, 10, 10, 0},
		&RequestUpdateChecker{"request_delivered", 10, 10, 0, 10},
		&RequestDeleteChecker{},
	}
	checkRequest(c, admin, client, -1, requester, supplier, callback, checkers...)
}

func (s *TestSuite) TestSupplyRequests(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))

	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	supplierId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
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
	checkRequestUpdates(c, admin, client, unit.AutomatId, supplierId,
		uint32(electrogen_1), removeElectrogen_1)
}

func (s *TestSuite) TestSupplyRequestsDotations(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	resources := []swapi.Quantity{}

	// Invalid tasker identifier
	err := client.CreateBasicLoadSupplyRequests(InvalidIdentifier, InvalidIdentifier,
		InvalidIdentifier, resources)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Tasker should be an automaton or a logistics base
	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	err = client.CreateBasicLoadSupplyRequests(unit.Id, InvalidIdentifier,
		InvalidIdentifier, resources)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid supplier identifier
	automatId := getSomeAutomatByName(c, d, "Supply Mobile Infantry Platoon").Id
	err = client.CreateBasicLoadSupplyRequests(automatId, InvalidIdentifier,
		InvalidIdentifier, resources)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid recipent identifier
	supplierId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	SetManualSupply(c, client, supplierId, true)
	err = client.CreateBasicLoadSupplyRequests(automatId, supplierId, InvalidIdentifier, resources)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// No request is created if no resources are below the low threshold
	err = client.CreateBasicLoadSupplyRequests(automatId, supplierId, automatId, resources)
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)
	d = client.Model.GetData()
	c.Assert(d.SupplyRequests, HasLen, 0)

	resource := &swapi.Resource{
		Quantity:      0,
		LowThreshold:  50,
		HighThreshold: 100,
	}

	// Create two supply requests
	units := getUnitsFromAutomat(automatId, client.Model.GetData())
	c.Assert(len(units), Greater, 2)
	err = client.ChangeResource(units[0].Id, map[uint32]*swapi.Resource{
		uint32(electrogen_1): resource})
	c.Assert(err, IsNil)
	err = client.ChangeResource(units[1].Id, map[uint32]*swapi.Resource{
		uint32(electrogen_1): resource,
		uint32(electrogen_2): resource,
	})
	c.Assert(err, IsNil)
	err = client.ChangeResource(units[2].Id, map[uint32]*swapi.Resource{
		uint32(electrogen_3): resource})
	c.Assert(err, IsNil)

	resources = []swapi.Quantity{{Id: uint32(electrogen_1), Quantity: 10},
		{Id: uint32(electrogen_2), Quantity: 2500000}}
	err = client.CreateBasicLoadSupplyRequests(automatId, supplierId, automatId, resources)
	c.Assert(err, IsNil)

	// electrogen_1 : unit1 : 10, unit2 : 10 => tolal: 20
	// electrogen_2 : unit2 : 10  => tolal: 10
	request1 := swapi.SupplyRequest{
		ResourceId:  uint32(electrogen_1),
		RequesterId: automatId,
		SupplierId:  supplierId,
		RecipientId: automatId,
		Requested:   20,
	}
	request2 := swapi.SupplyRequest{
		ResourceId:  uint32(electrogen_2),
		RequesterId: automatId,
		SupplierId:  supplierId,
		RecipientId: automatId,
		Requested:   10,
	}

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		electrogen_1_request := false
		electrogen_2_request := false
		for _, request := range data.SupplyRequests {
			if *request == request1 {
				electrogen_1_request = true
			} else if *request == request2 {
				electrogen_2_request = true
			}
		}
		return electrogen_1_request && electrogen_2_request
	})

	// Only two requests is created(no one for electrogen_3).
	d = client.Model.GetData()
	c.Assert(d.SupplyRequests, HasLen, 2)
}

func (s *TestSuite) TestSupplyAutomatRequestsStocks(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	resources := []swapi.Quantity{}

	// Invalid tasker identifier
	err := client.CreateStockSupplyRequests(InvalidIdentifier, InvalidIdentifier,
		InvalidIdentifier, resources)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Tasker should be an automaton or a logistics base
	unit := getSomeUnitByName(c, d, "Supply Mobile Infantry")
	err = client.CreateStockSupplyRequests(unit.Id, InvalidIdentifier,
		InvalidIdentifier, resources)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid supplier identifier
	formationId := getSomeFormationByName(c, d, "Supply F3").Id
	err = client.CreateStockSupplyRequests(formationId, InvalidIdentifier,
		InvalidIdentifier, resources)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Invalid recipent identifier
	err = client.CreateStockSupplyRequests(formationId, formationId,
		InvalidIdentifier, resources)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	recipentId := getSomeAutomatByName(c, d, "Supply Log Automat 1c").Id
	SetManualSupply(c, client, recipentId, true)

	// No request is created if no resources are below the low threshold
	err = client.CreateStockSupplyRequests(formationId, formationId, recipentId, resources)
	c.Assert(err, IsNil)
	client.Model.WaitTicks(2)
	d = client.Model.GetData()
	c.Assert(d.SupplyRequests, HasLen, 0)

	unit1 := getSomeUnitByName(c, d, "Supply Log Unit 1c")
	err = client.RecoverStocks(unit1.Id,
		map[uint32]*swapi.Resource{
			uint32(electrogen_1): {
				Quantity:      0,
				LowThreshold:  50,
				HighThreshold: 100,
			},
			uint32(electrogen_3): {
				Quantity:      0,
				LowThreshold:  50,
				HighThreshold: 100,
			}})
	c.Assert(err, IsNil)

	resources = []swapi.Quantity{{Id: uint32(electrogen_1), Quantity: 25000}}
	err = client.CreateStockSupplyRequests(formationId, formationId, recipentId, resources)
	c.Assert(err, IsNil)

	request1 := swapi.SupplyRequest{
		ResourceId:  uint32(electrogen_1),
		RequesterId: formationId,
		SupplierId:  formationId,
		RecipientId: recipentId,
		Requested:   50,
	}

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, request := range data.SupplyRequests {
			if *request == request1 {
				return true
			}
		}
		return false
	})

	// Only one request is created(no one for electrogen_1).
	d = client.Model.GetData()
	c.Assert(d.SupplyRequests, HasLen, 1)
}

func (s *TestSuite) TestSupplyFormationRequestsStocks(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()

	bl1Id := getSomeFormationByName(c, d, "Supply F6").Id
	bl2Id := getSomeFormationByName(c, d, "Supply F3").Id
	tc21Id := getSomeAutomatByName(c, d, "Supply Log Automat 1h").Id
	tc22Id := getSomeAutomatByName(c, d, "Supply Log Automat 1g").Id
	SetManualSupply(c, client, tc21Id, true)
	SetManualSupply(c, client, tc22Id, true)

	err := client.RecoverStocks(tc21Id,
		map[uint32]*swapi.Resource{
			uint32(electrogen_2): {
				Quantity:      0,
				LowThreshold:  50,
				HighThreshold: 100,
			}})
	c.Assert(err, IsNil)

	err = client.RecoverStocks(tc22Id,
		map[uint32]*swapi.Resource{
			uint32(electrogen_1): {
				Quantity:      0,
				LowThreshold:  50,
				HighThreshold: 100,
			},
			uint32(electrogen_2): {
				Quantity:      0,
				LowThreshold:  50,
				HighThreshold: 100,
			}})
	c.Assert(err, IsNil)

	resources := []swapi.Quantity{{Id: uint32(electrogen_1), Quantity: 250},
		{Id: uint32(electrogen_2), Quantity: 10}}
	err = client.CreateStockSupplyRequests(bl2Id, bl2Id, bl1Id, resources)
	c.Assert(err, IsNil)

	// electrogen_1 : tc22Id : 50 => tolal: 50
	// electrogen_2 : tc21Id : 10,  tc22Id : 10 => tolal: 20
	request1 := swapi.SupplyRequest{
		ResourceId:  uint32(electrogen_1),
		RequesterId: bl2Id,
		SupplierId:  bl2Id,
		RecipientId: bl1Id,
		Requested:   50,
	}
	request2 := swapi.SupplyRequest{
		ResourceId:  uint32(electrogen_2),
		RequesterId: bl2Id,
		SupplierId:  bl2Id,
		RecipientId: bl1Id,
		Requested:   20,
	}

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		electrogen_1_request := false
		electrogen_2_request := false
		for _, request := range data.SupplyRequests {
			if *request == request1 {
				electrogen_1_request = true
			} else if *request == request2 {
				electrogen_2_request = true
			}
		}
		return electrogen_1_request && electrogen_2_request
	})
}
