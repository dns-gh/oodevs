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
	"bufio"
	"code.google.com/p/goprotobuf/proto"
	"crypto/sha1"
	"fmt"
	. "launchpad.net/gocheck"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"
	"swapi"
	"swapi/simu"
	"sword"
	"swtest"
	"sync"
	"time"
)

// Parse the header and return a list of field matching regexps for fields
// which have to be censored, nil for the others.
func makeFieldMatchers(header string) []*regexp.Regexp {
	matchers := []*regexp.Regexp{}
	for _, field := range strings.Split(header, ";") {
		field = strings.TrimSpace(field)
		p := ""
		switch field {
		case "GDH":
			p = `(\d{8}T\d{6})`
		case "tick", "state end tick", "request id":
			p = `(\d*)`
		}
		if len(p) > 0 {
			p = `\s*` + p + `\s*`
			matchers = append(matchers, regexp.MustCompile(p))
		} else {
			matchers = append(matchers, nil)
		}
	}
	return matchers
}

// Read a logistic file and rewrite varying fields.
func readLogisticFile(c *C, path string) string {
	var fields []*regexp.Regexp
	fp, err := os.Open(path)
	c.Assert(err, IsNil)
	defer fp.Close()
	scanner := bufio.NewScanner(fp)
	output := []string{}
	for i := 0; scanner.Scan(); i++ {
		line := scanner.Text()
		if i == 0 {
			fields = makeFieldMatchers(line)
			output = append(output, line)
			continue
		}
		rewritten := []string{}
		parts := strings.Split(line, ";")
		for i, matcher := range fields {
			if i >= len(parts) {
				// Ignore missing field, supply lines have variable
				// fields, we cannot deduce mandatory ones...
				continue
			}
			part := parts[i]
			if matcher != nil && matcher.MatchString(part) {
				part = " *** "
			}
			rewritten = append(rewritten, part)
		}
		output = append(output, strings.Join(rewritten, ";"))
	}
	c.Assert(scanner.Err(), IsNil)
	return strings.Join(output, "\n")
}

// Returns a mapping of the concatenation of normalized content of each
// logistic chains files. Keys are chain names.
func readAndRewriteLogFiles(c *C, opts *simu.SimOpts) map[string]string {
	// List logistic files and sort them
	prefixes := map[string]string{
		"funeral":     "LogFuneral",
		"maintenance": "LogMaintenance",
		"medical":     "LogMedical",
		"supply":      "LogSupply",
	}
	files := map[string][]string{}
	sessionDir := opts.GetSessionDir()
	err := filepath.Walk(sessionDir, func(path string, fi os.FileInfo, err error) error {
		if err != nil || fi.IsDir() {
			return err
		}
		basename := filepath.Base(path)
		for name, prefix := range prefixes {
			if strings.HasPrefix(basename, prefix) {
				files[name] = append(files[name], path)
				break
			}
		}
		return nil
	})
	c.Assert(err, IsNil)

	// Concatenates rewritten contents for each chain
	logs := map[string]string{}
	for name, matched := range files {
		sort.Sort(sort.StringSlice(matched))
		data := []string{}
		for _, path := range matched {
			lines := readLogisticFile(c, path)
			data = append(data, lines)
		}
		logs[name] = strings.Join(data, "")
	}
	return logs
}

// Checks that s starts with the same lines than prefix.
func assertIsPrefixed(c *C, s, prefix string) {
	prefixes := strings.Split(prefix, "\n")
	lines := strings.Split(s, "\n")
	if len(lines) > len(prefixes) {
		lines = lines[:len(prefixes)]
	}
	swtest.AssertEqualOrDiff(c, strings.Join(lines, "\n"), strings.Join(prefixes, "\n"))
}

func initLogisticEvents(c *C, client *swapi.Client) {
	data := client.Model.GetData()
	unit := getSomeUnitByName(c, data, "ARMOR.MBT")
	supply := getSomeUnitByName(c, data, "Logistic CT").Id

	// Waiting for deployment
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[supply].Installation == 100
	})

	// Generate maintenance activity
	equipmentId := uint32(11)
	dotation := swapi.EquipmentDotation{
		Available: 4,
	}
	c.Assert(dotation, DeepEquals, *unit.EquipmentDotations[equipmentId])
	equipment := swapi.EquipmentDotation{
		Available:  3,
		Repairable: 1,
		Breakdowns: []int32{82},
	}
	err := client.ChangeEquipmentState(unit.Id, map[uint32]*swapi.EquipmentDotation{
		equipmentId: &equipment})
	c.Assert(err, IsNil)

	// Generate medical and funeral activity
	c.Assert(CheckHumanQuantity(unit.HumanDotations,
		map[int32]int32{eOfficer: 1, eWarrantOfficer: 4, eTrooper: 7}), Equals, true)
	injured := swapi.HumanDotation{
		Quantity:     1,
		Rank:         eTrooper,
		State:        eInjured,
		Injury:       eInjuryU2,
		Psyop:        true,
		Contaminated: true,
	}
	dead := swapi.HumanDotation{
		Quantity: 1,
		Rank:     eTrooper,
		State:    eDead,
	}
	err = client.ChangeHumanState(unit.Id, []*swapi.HumanDotation{&injured, &dead})
	c.Assert(err, IsNil)

	// Generate supply activity
	c.Assert(unit.ResourceDotations, NotNil)
	c.Assert(unit.ResourceDotations[1], DeepEquals,
		swapi.ResourceDotation{
			Quantity:  3200,
			Threshold: 10,
		})
	resource := swapi.ResourceDotation{
		Threshold: 10,
	}
	err = client.ChangeDotation(unit.Id, map[uint32]*swapi.ResourceDotation{1: &resource})
	c.Assert(err, IsNil)

	client.Model.WaitTicks(2)
}

func (s *TestSuite) TestLogisticPlugin(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	initLogisticEvents(c, client)

	// Expected log lines below are the minimum subset of events which must have
	// been logged in the tested tick range. More can be logged in practice.
	client.Model.WaitTicks(3)
	logs := readAndRewriteLogFiles(c, sim.Opts)
	assertIsPrefixed(c, logs["funeral"], ""+
		`request id ; tick ; GDH ; unit ; handling unit ; conveying unit ; rank ; packaging resource ; state ; state end tick
 *** ; *** ; *** ; ARMOR.MBT platoon ;  ;  ; Trooper ;  ;  ; *** 
 *** ; *** ; *** ; ARMOR.MBT platoon ; Logistic combat train [23] ;  ; Trooper ;  ; transporting unpackaged ; *** `)
	assertIsPrefixed(c, logs["maintenance"], ""+
		`request id ; tick ; GDH ; unit ; provider ; equipment ; breakdown ; state ; state end tick
 *** ; *** ; *** ; ARMOR.MBT platoon ;  ; ARMOR.Main Battle Tank ; COTAC Breakdown 10 ;  ; *** 
 *** ; *** ; *** ; ARMOR.MBT platoon ; Logistic CT ; ARMOR.Main Battle Tank ; COTAC Breakdown 10 ; moving toward logistics system ; *** `)
	assertIsPrefixed(c, logs["medical"], ""+
		`request id ; tick ; GDH ; unit ; provider ; rank ; wound ; nbc ; mental ; state ; state end tick
 *** ; *** ; *** ; ARMOR.MBT platoon ;  ; Trooper ; injury severity level 2 ; yes ; yes ;  ; *** 
 *** ; *** ; *** ; ARMOR.MBT platoon ; Logistic CT ; Trooper ; injury severity level 2 ; yes ; yes ; waiting for evacuation ambulance ; *** 
 *** ; *** ; *** ; ARMOR.MBT platoon ; Logistic CT ; Trooper ; injury severity level 2 ; yes ; yes ; evacuation ambulance coming ; *** `)
	assertIsPrefixed(c, logs["supply"], ""+
		`request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed
 *** ; *** ; *** ;  ; Logistic combat train [23] ; Logistic combat train [23] ;  ;  ; *** `)
}

func hashLogEntry(c *C, e *sword.LogHistoryEntry) string {
	data, err := proto.Marshal(e)
	c.Assert(err, IsNil)
	h := sha1.New()
	h.Write(data)
	return fmt.Sprintf("%x", h.Sum(nil))
}

func (s *TestSuite) TestLogisticHistory(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	initLogisticEvents(c, client)
	// Wait for some basic activity
	client.Model.WaitTicks(3)

	data := client.Model.GetData()
	handlingId := uint32(0)
	for id, _ := range data.MedicalHandlings {
		handlingId = id
		break
	}
	c.Assert(handlingId, Greater, uint32(0))

	// Valid handling, duplicate and invalid handling
	invalidId := uint32(123456)
	entries, err := client.GetLogisticHistory(handlingId, handlingId, invalidId)
	c.Assert(err, IsNil)
	c.Assert(len(entries), Greater, 0)
	uniqueKeys := map[string]struct{}{}
	for _, e := range entries {
		c.Assert(e.GetMedical(), NotNil)
		m := e.GetMedical()
		c.Assert(m.GetCreation().GetRequest().GetId(), Equals, handlingId)
		key := hashLogEntry(c, e)
		_, ok := uniqueKeys[key]
		c.Assert(ok, Equals, false)
		uniqueKeys[key] = struct{}{}
	}

	// No handling
	entries, err = client.GetLogisticHistory()
	c.Assert(err, IsNil)
	c.Assert(len(entries), Equals, 0)

	// List requests for invalid unit
	entries, err = client.ListLogisticRequests(-1, 12345)
	c.Assert(err, IsNil)
	c.Assert(entries, HasLen, 0)

	// List requests for a valid unit
	unitIds := []uint32{}
	for id, _ := range data.Units {
		unitIds = append(unitIds, id)
	}
	entries, err = client.ListLogisticRequests(-1, -1, unitIds...)
	c.Assert(err, IsNil)
	c.Assert(len(entries), Greater, 1)
	latestTick := *entries[0].Tick

	// List requests with maxcount
	entries, err = client.ListLogisticRequests(-1, 1, unitIds...)
	c.Assert(err, IsNil)
	c.Assert(entries, HasLen, 1)

	// Test current_tick, this is less reliable because we have no garantee there
	// are logistic events on other ticks than latestTick, so we can mistook
	// a "returns nothing" error for a success.
	entries, err = client.ListLogisticRequests(int(latestTick)-1, -1, unitIds...)
	c.Assert(err, IsNil)
	for _, e := range entries {
		c.Assert(*e.Tick, Lesser, latestTick)
	}

	// Stop the simulation, start a replay
	sim.Stop()
	client.Close()
	replay := startReplay(c, sim.Opts)
	defer replay.Kill()
	client = loginAndWaitModel(c, replay, NewAdminOpts(""))
	defer client.Close()

	// Check we can some request history
	entries, err = client.ReplayGetLogisticHistory(handlingId)
	c.Assert(err, IsNil)
	c.Assert(len(entries), Greater, 0)

	// Check we can list requests
	entries, err = client.ReplayListLogisticRequests(-1, -1, unitIds...)
	c.Assert(err, IsNil)
	c.Assert(len(entries), Greater, 0)
}

func makeBreakdown(c *C, id uint32, eq *swapi.EquipmentDotation, count int32, breakdown BreakdownType) map[uint32]*swapi.EquipmentDotation {
	c.Assert(eq.Available, Greater, count)
	return map[uint32]*swapi.EquipmentDotation{
		id: &swapi.EquipmentDotation{
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
			expected.Automat.GetId(): struct{}{},
		}
	} else if expected.Formation != nil {
		profile.ReadOnlyFormations = map[uint32]struct{}{
			expected.Formation.GetId(): struct{}{},
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

func checkMaintenance(c *C, client *swapi.Client, unit *swapi.Unit, offset int, breakdown BreakdownType, checkers ...MaintenanceChecker) {
	client.Pause()
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
	err := client.ChangeEquipmentState(unit.Id, makeBreakdown(c, eqid, eq, 1, breakdown))
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

func checkMaintenanceUpdates(c *C, client *swapi.Client, unit *swapi.Unit, breakdown BreakdownType, updates []MaintenanceUpdateChecker) {
	checkers := []MaintenanceChecker{MaintenanceCreateChecker{}}
	for i := range updates {
		checkers = append(checkers, &updates[i])
	}
	checkers = append(checkers, MaintenanceDeleteChecker{})
	checkMaintenance(c, client, unit, -1, breakdown, checkers...)
}

func setParts(client *swapi.Client, provider *sword.Tasker, qty int32, resources ...ResourceType) error {
	d := client.Model.GetData()
	for _, u := range d.Units {
		if IsProvider(d, u.Id, provider, false) {
			next := map[uint32]*swapi.ResourceDotation{}
			for _, id := range resources {
				next[uint32(id)] = &swapi.ResourceDotation{Quantity: qty}
			}
			err := client.ChangeDotation(u.Id, next)
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
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2 := swapi.MakeAutomatTasker(getSomeAutomatByName(c, d, "Maintenance Automat 1").Id)
	bld := swapi.MakeFormationTasker(getSomeFormationByName(c, d, "Maintenance BLD").Id)
	blt := swapi.MakeFormationTasker(getSomeFormationByName(c, d, "Maintenance BLT").Id)
	zero := &sword.Tasker{}
	checkMaintenanceUpdates(c, client, unit, electronic_1, []MaintenanceUpdateChecker{
		{"moving_to_supply", tc2},
		{"diagnosing", tc2},
		{"waiting_for_repairer", tc2},
		{"repairing", tc2},
		{"moving_back", tc2},
		{"finished", zero},
	})
	checkMaintenanceUpdates(c, client, unit, mobility_1, []MaintenanceUpdateChecker{
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
	checkMaintenanceUpdates(c, client, unit, electronic_2, []MaintenanceUpdateChecker{
		{"moving_to_supply", tc2},
		{"diagnosing", tc2},
		{"searching_upper_levels", tc2},
		{"moving_to_supply", bld},
		{"waiting_for_repairer", bld},
		{"repairing", bld},
		{"moving_back", bld},
		{"finished", zero},
	})
	checkMaintenanceUpdates(c, client, unit, mobility_2, []MaintenanceUpdateChecker{
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
	checkMaintenanceUpdates(c, client, unit, electronic_3, []MaintenanceUpdateChecker{
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
	checkMaintenanceUpdates(c, client, unit, mobility_3, []MaintenanceUpdateChecker{
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
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2 := swapi.MakeAutomatTasker(getSomeAutomatByName(c, d, "Maintenance Automat 1").Id)
	zero := &sword.Tasker{}
	err := setParts(client, tc2, 0, electrogen_1)
	c.Assert(err, IsNil)
	checkMaintenance(c, client, unit, 0, electronic_1,
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
	checkMaintenance(c, client, unit, 0, mobility_1,
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

func SetMaintenanceManualMode(c *C, client *swapi.Client, id uint32) {
	err := client.LogMaintenanceSetManual(id, true)
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
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bld := swapi.MakeFormationTasker(bldId)
	zero := &sword.Tasker{}

	SetMaintenanceManualMode(c, client, tc2Id)
	SetMaintenanceManualMode(c, client, bldId)

	checkMaintenance(c, client, unit, 0, mobility_2,
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

	SetMaintenanceManualMode(c, client, tc2Id)
	SetMaintenanceManualMode(c, client, bldId)

	checkMaintenance(c, client, unit, 0, mobility_2,
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

func (s *TestSuite) TestMaintenanceHandlingsWithBaseSwitchedBackToAutomatic(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	bld := swapi.MakeFormationTasker(bldId)
	zero := &sword.Tasker{}

	toAutomatic := func(id uint32) error {
		// set automat back to automatic mode
		err := client.LogMaintenanceSetManual(id, false)
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

	SetMaintenanceManualMode(c, client, tc2Id)

	checkMaintenance(c, client, unit, 0, mobility_2,
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

	SetMaintenanceManualMode(c, client, tc2Id)

	checkMaintenance(c, client, unit, 0, mobility_2,
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

	SetMaintenanceManualMode(c, client, bldId)

	checkMaintenance(c, client, unit, 0, mobility_2,
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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bld := swapi.MakeFormationTasker(getSomeFormationByName(c, d, "Maintenance BLD").Id)
	zero := &sword.Tasker{}

	SetMaintenanceManualMode(c, client, tc2Id)

	checkMaintenance(c, client, unit, 0, mobility_2,
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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bld := swapi.MakeFormationTasker(getSomeFormationByName(c, d, "Maintenance BLD").Id)
	zero := &sword.Tasker{}

	SetMaintenanceManualMode(c, client, tc2Id)

	checkMaintenance(c, client, unit, 0, mobility_2,
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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	bld := swapi.MakeFormationTasker(bldId)
	bltId := getSomeFormationByName(c, d, "Maintenance BLT").Id
	blt := swapi.MakeFormationTasker(bltId)
	zero := &sword.Tasker{}

	SetMaintenanceManualMode(c, client, tc2Id)
	SetMaintenanceManualMode(c, client, bldId)

	checkMaintenance(c, client, unit, 0, mobility_2,
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
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	bldId := getSomeFormationByName(c, d, "Maintenance BLD").Id
	bld := swapi.MakeFormationTasker(bldId)
	bltId := getSomeFormationByName(c, d, "Maintenance BLT").Id
	blt := swapi.MakeFormationTasker(bltId)

	SetMaintenanceManualMode(c, client, tc2Id)
	SetMaintenanceManualMode(c, client, bldId)
	SetMaintenanceManualMode(c, client, bltId)

	phydb := loadWWPhysical(c)
	reporter := newReporter(c, unit.Id, phydb, "Unable to repair")
	reporter.Start(client.Model)

	checkMaintenance(c, client, unit, 0, mobility_2,
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

func CheckDeployTime(c *C, start, end *sword.DateTime, duration time.Duration) {
	startTime, _ := swapi.GetTime(start)
	endTime, _ := swapi.GetTime(end)
	c.Assert(endTime.Sub(startTime), Equals, duration)
}

func (s *TestSuite) TestLogisticDeployment(c *C) {
	phydb := loadWWPhysical(c)
	opts := NewAdminOpts(ExCrossroadSmallLog)
	opts.Paused = true
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)
	model := client.Model
	tc2 := getSomeAutomatByName(c, model.GetData(), "LOG.Logistic combat train")
	unit := getSomeUnitByName(c, model.GetData(), "LOG.Logistic CT")
	startingReportsTypes := []string{
		"Starting deploy",
		"Starting undeploy"}
	endingReportsTypes := []string{
		"section deployed",
		"Section undeployed"}
	startingReporter := newReporter(c, unit.Id, phydb, startingReportsTypes...)
	endingReporter := newReporter(c, unit.Id, phydb, endingReportsTypes...)
	startingReporter.Start(client.Model)
	endingReporter.Start(client.Model)
	client.Resume(0)

	// Waiting for auto deployment
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit.Id].Installation == 100
	})

	// Deploy TC2 to another position to force undeployment and deployment
	MissionLogDeploy := uint32(44584)
	heading := swapi.MakeHeading(0)
	limit1 := swapi.MakeLimit(
		swapi.Point{X: -15.8302, Y: 28.3765},
		swapi.Point{X: -15.825, Y: 28.3413})
	limit2 := swapi.MakeLimit(
		swapi.Point{X: -15.7983, Y: 28.3765},
		swapi.Point{X: -15.7991, Y: 28.3413})
	destination := swapi.Point{X: -15.8164, Y: 28.3567}
	params := swapi.MakeParameters(heading, nil, limit1, limit2, swapi.MakePointParam(destination))
	_, err := client.SendAutomatOrder(tc2.Id, MissionLogDeploy, params)
	c.Assert(err, IsNil)
	model.WaitTicks(10)

	// Check reports
	startingReports := startingReporter.Stop()
	endingReports := endingReporter.Stop()
	// Check that reports are always by pairs "Starting deploy" -> "section deployed"
	c.Assert(len(startingReports), Equals, len(endingReports))
	// Check we have at least auto deployment and undeployment following mission
	c.Assert(len(startingReports) >= 2, Equals, true)
	// Check there is 1mn delay in deployment
	for i := 0; i < len(startingReports); i++ {
		CheckDeployTime(c, startingReports[i].Time, endingReports[i].Time, 1*time.Minute)
	}
}
