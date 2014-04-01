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

type MedicalCheckContext struct {
	data         *swapi.ModelData
	unitId       uint32
	handlingId   uint32
	rank         sword.EnumHumanRank
	wound        sword.EnumHumanWound
	mental       bool
	contaminated bool
	verbose      bool
	diagnosed    bool
	deleted      bool
	last         sword.LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus
	group        sync.WaitGroup
	errors       chan error
}

type MedicalChecker interface {
	Check(c *C, ctx *MedicalCheckContext, msg *sword.SimToClient_Content) bool
}

type MedicalCreateChecker struct{}

func (MedicalCreateChecker) Check(c *C, ctx *MedicalCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogMedicalHandlingUpdate, IsNil)
	c.Check(msg.LogMedicalHandlingDestruction, IsNil)
	m := msg.LogMedicalHandlingCreation
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("+ %+v\n", m)
	}
	c.Check(ctx.handlingId, Equals, uint32(0))
	c.Check(m.GetUnit().GetId(), Equals, ctx.unitId)
	c.Check(m.GetRank(), Equals, ctx.rank)
	c.Check(m.GetWound(), Equals, ctx.wound)
	c.Check(m.GetMentalWound(), Equals, ctx.mental)
	c.Check(m.GetNbcContaminated(), Equals, ctx.contaminated)
	ctx.handlingId = m.GetRequest().GetId()
	return true
}

type MedicalUpdateChecker struct {
	name     string
	provider *sword.Tasker
}

func (cc *MedicalUpdateChecker) check(c *C, ctx *MedicalCheckContext, msg *sword.SimToClient_Content, strict bool) bool {
	c.Check(msg.LogMedicalHandlingCreation, IsNil)
	c.Check(msg.LogMedicalHandlingDestruction, IsNil)
	m := msg.LogMedicalHandlingUpdate
	if m == nil {
		return false
	}
	if ctx.verbose {
		fmt.Printf("* %+v\n", m)
	}
	c.Check(ctx.handlingId, Not(Equals), uint32(0))
	c.Check(m.GetRequest().GetId(), Equals, ctx.handlingId)
	c.Check(m.GetProvider().GetId(), Equals, swapi.GetTaskerId(cc.provider))
	value, ok := sword.LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus_value[cc.name]
	next := sword.LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus(value)
	c.Check(ok, Equals, true)
	c.Check(m.GetState(), Equals, next)
	if ctx.diagnosed != m.GetDiagnosed() {
		c.Check(ctx.last, Equals, sword.LogMedicalHandlingUpdate_diagnosing)
	}
	ctx.diagnosed = ctx.diagnosed || m.GetDiagnosed()
	ctx.last = next
	return true
}

func (cc *MedicalUpdateChecker) Check(c *C, ctx *MedicalCheckContext, msg *sword.SimToClient_Content) bool {
	return cc.check(c, ctx, msg, false)
}

type MedicalApplyChecker struct {
	MedicalChecker
	operand func(ctx *MedicalCheckContext) error
}

func (m *MedicalApplyChecker) Check(c *C, ctx *MedicalCheckContext, msg *sword.SimToClient_Content) bool {
	ok := m.MedicalChecker.Check(c, ctx, msg)
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

type MedicalDeleteChecker struct{}

func (MedicalDeleteChecker) Check(c *C, ctx *MedicalCheckContext, msg *sword.SimToClient_Content) bool {
	c.Check(msg.LogMedicalHandlingCreation, IsNil)
	c.Check(msg.LogMedicalHandlingUpdate, IsNil)
	m := msg.LogMedicalHandlingDestruction
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

func checkMedical(c *C, client *swapi.Client, unit *swapi.Unit, offset int,
	rank sword.EnumHumanRank, state int32, mental bool, checkers ...MedicalChecker) {

	client.Pause()
	check := MedicalCheckContext{
		data:         client.Model.GetData(),
		unitId:       unit.Id,
		rank:         rank,
		mental:       mental,
		contaminated: false,
		errors:       make(chan error),
	}
	if state == eHealthy {
		check.wound = sword.EnumHumanWound_unwounded
	} else {
		check.wound = sword.EnumHumanWound_wounded_urgency_1
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
	human := swapi.HumanDotation{
		Quantity:     1,
		Rank:         int32(check.rank),
		State:        state,
		Psyop:        mental,
		Contaminated: check.contaminated,
	}
	err := client.ChangeHumanState(unit.Id, []*swapi.HumanDotation{&human})
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

func (s *TestSuite) TestMedicalHandlings(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Medical Mobile Infantry")
	tc2 := swapi.MakeAutomatTasker(getSomeUnitByName(c, d, "Medical Log Unit 1").Id)
	bld := getSomeAutomatByName(c, d, "Medical Log Automat 2")
	triage := swapi.MakeUnitTasker(getSomeUnitByName(c, d, "Medical Log Unit 2").Id)
	none := &sword.Tasker{}
	// deploy for triage
	MissionLogDeploy := uint32(8)
	_, err := client.SendAutomatOrder(bld.Id, MissionLogDeploy,
		swapi.MakeParameters(swapi.MakeHeading(0), nil,
			swapi.MakeLimit(
				swapi.Point{X: -15.7568, Y: 28.4116},
				swapi.Point{X: -15.7688, Y: 28.3885}),
			swapi.MakeLimit(
				swapi.Point{X: -15.7413, Y: 28.4091},
				swapi.Point{X: -15.7522, Y: 28.3877}),
			nil))
	c.Assert(err, IsNil)
	// physical injury
	checkMedical(c, client, unit, 0, sword.EnumHumanRank_trooper, eInjured, false,
		MedicalCreateChecker{},
		&MedicalUpdateChecker{"waiting_for_evacuation", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_moving_in", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_loading", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_moving_out", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_unloading", tc2},
		&MedicalUpdateChecker{"diagnosing", tc2},
		&MedicalUpdateChecker{"waiting_for_collection", tc2},
		&MedicalUpdateChecker{"waiting_for_collection", tc2}, // http://jira.masagroup.net/browse/SWBUG-12119
		&MedicalUpdateChecker{"collection_ambulance_loading", tc2},
		&MedicalUpdateChecker{"looking_for_triage", tc2},
		&MedicalUpdateChecker{"collection_ambulance_moving_in", tc2},
		&MedicalUpdateChecker{"collection_ambulance_unloading", tc2},
		&MedicalUpdateChecker{"triaging", triage},
		&MedicalUpdateChecker{"looking_for_medical_attention", triage},
		&MedicalUpdateChecker{"receiving_medical_attention", triage},
		&MedicalUpdateChecker{"resting", triage},
		&MedicalUpdateChecker{"finished", none},
		// no deleter because a physical wound never heals
	)
	// physical and mental injury
	checkMedical(c, client, unit, 0, sword.EnumHumanRank_sub_officer, eInjured, true,
		MedicalCreateChecker{},
		&MedicalUpdateChecker{"waiting_for_evacuation", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_moving_in", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_loading", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_moving_out", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_unloading", tc2},
		&MedicalUpdateChecker{"diagnosing", tc2},
		&MedicalUpdateChecker{"waiting_for_collection", tc2},
		&MedicalUpdateChecker{"waiting_for_collection", tc2}, // http://jira.masagroup.net/browse/SWBUG-12119
		&MedicalUpdateChecker{"collection_ambulance_loading", tc2},
		&MedicalUpdateChecker{"looking_for_triage", tc2},
		&MedicalUpdateChecker{"collection_ambulance_moving_in", tc2},
		&MedicalUpdateChecker{"collection_ambulance_unloading", tc2},
		&MedicalUpdateChecker{"triaging", triage},
		&MedicalUpdateChecker{"looking_for_medical_attention", triage},
		&MedicalUpdateChecker{"receiving_medical_attention", triage},
		&MedicalUpdateChecker{"resting", triage},
		&MedicalUpdateChecker{"looking_for_medical_attention", triage},
		&MedicalUpdateChecker{"receiving_medical_attention", triage},
		&MedicalUpdateChecker{"resting", triage},
		&MedicalUpdateChecker{"finished", none},
		// no deleter because a physical wound never heals
	)
	// mental injury
	checkMedical(c, client, unit, 0, sword.EnumHumanRank_officer, eHealthy, true,
		MedicalCreateChecker{},
		&MedicalUpdateChecker{"waiting_for_evacuation", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_moving_in", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_loading", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_moving_out", tc2},
		&MedicalUpdateChecker{"evacuation_ambulance_unloading", tc2},
		&MedicalUpdateChecker{"diagnosing", tc2},
		&MedicalUpdateChecker{"waiting_for_collection", tc2},
		&MedicalUpdateChecker{"waiting_for_collection", tc2}, // http://jira.masagroup.net/browse/SWBUG-12119
		&MedicalUpdateChecker{"collection_ambulance_loading", tc2},
		&MedicalUpdateChecker{"looking_for_triage", tc2},
		&MedicalUpdateChecker{"collection_ambulance_moving_in", tc2},
		&MedicalUpdateChecker{"collection_ambulance_unloading", tc2},
		&MedicalUpdateChecker{"triaging", triage},
		&MedicalUpdateChecker{"looking_for_medical_attention", triage},
		&MedicalUpdateChecker{"receiving_medical_attention", triage},
		&MedicalUpdateChecker{"resting", triage},
		&MedicalUpdateChecker{"finished", none},
		MedicalDeleteChecker{},
	)
}
