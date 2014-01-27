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

func getSomeEquipment(c *C, unit *swapi.Unit) (uint32, *swapi.EquipmentDotation) {
	for k, v := range unit.EquipmentDotations {
		return k, v
	}
	c.Fatal("unable to find any equipment")
	return 0, nil
}

type BreakdownType int32

const (
	// replace with swapi/phy
	electronic_1 BreakdownType = 108
	electronic_2               = 109
	electronic_3               = 110
	mobility_1                 = 111
	mobility_2                 = 112
	mobility_3                 = 113
)

func checkMaintenance(c *C, client *swapi.Client, unit *swapi.Unit, breakdown BreakdownType, names ...string) {
	client.Pause()
	states := []sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus{}
	for _, name := range names {
		value, ok := sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus_value[name]
		c.Assert(ok, Equals, true)
		states = append(states, sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus(value))
	}
	verbose := false
	eqid, eq := getSomeEquipment(c, unit)
	hid := uint32(0)
	done := false
	idx := 0
	diagnosed := false
	mutex := &sync.Mutex{}
	ctx := client.Register(func(msg *swapi.SwordMessage, id, ctx int32, err error) bool {
		if err != nil {
			return true
		}
		if msg == nil ||
			msg.SimulationToClient == nil {
			return false
		}
		mutex.Lock()
		defer mutex.Unlock()
		mm := msg.SimulationToClient.GetMessage()
		if pkt := mm.LogMaintenanceHandlingCreation; pkt != nil {
			if verbose {
				fmt.Printf("+ %+v\n", pkt)
			}
			c.Check(hid, Equals, uint32(0))
			c.Check(pkt.GetUnit().GetId(), Equals, unit.Id)
			c.Check(pkt.GetEquipement().GetId(), Equals, eqid)
			c.Check(pkt.GetBreakdown().GetId(), Equals, uint32(breakdown))
			hid = pkt.GetRequest().GetId()
		}
		if pkt := mm.LogMaintenanceHandlingUpdate; pkt != nil {
			if verbose {
				fmt.Printf("* %+v\n", pkt)
			}
			c.Check(hid, Not(Equals), uint32(0))
			c.Check(pkt.GetRequest().GetId(), Equals, hid)
			c.Check(idx, Lesser, len(states))
			if idx < len(states) {
				c.Check(states[idx], Equals, pkt.GetState())
			}
			if diagnosed != pkt.GetDiagnosed() {
				c.Check(idx, Greater, 1)
				if idx > 1 {
					c.Check(states[idx-1], Equals, sword.LogMaintenanceHandlingUpdate_diagnosing)
				}
			}
			diagnosed = diagnosed || pkt.GetDiagnosed()
			idx++
		}
		if pkt := mm.LogMaintenanceHandlingDestruction; pkt != nil {
			if verbose {
				fmt.Printf("- %+v\n", pkt)
			}
			c.Check(hid, Not(Equals), uint32(0))
			c.Check(pkt.GetRequest().GetId(), Equals, hid)
			c.Check(pkt.GetUnit().GetId(), Equals, unit.Id)
			done = true
		}
		return false
	})
	defer client.Unregister(ctx)
	err := client.ChangeEquipmentState(unit.Id, makeBreakdown(c, eqid, eq, 1, breakdown))
	c.Assert(err, IsNil)
	client.Resume(0)
	waitCondition(c, client.Model, func(d *swapi.ModelData) bool {
		mutex.Lock()
		defer mutex.Unlock()
		return c.Failed() || done
	})
	c.Assert(idx, Equals, len(states))
}

func (s *TestSuite) TestMaintenanceHandlings(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadBreakdown))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Mobile Infantry")
	c.Assert(unit, NotNil)
	checkMaintenance(c, client, unit, electronic_1,
		"moving_to_supply",
		"diagnosing",
		"waiting_for_repairer",
		"repairing",
		"moving_back",
	)
	checkMaintenance(c, client, unit, mobility_1,
		"transporter_moving_to_supply",
		"transporter_loading",
		"transporter_moving_back",
		"transporter_unloading",
		"diagnosing",
		"waiting_for_repairer",
		"repairing",
		"moving_back",
	)
	checkMaintenance(c, client, unit, electronic_2,
		"moving_to_supply",
		"diagnosing",
		"searching_upper_levels",
		"moving_to_supply",
		"waiting_for_repairer",
		"repairing",
		"moving_back",
	)
	checkMaintenance(c, client, unit, mobility_2,
		"transporter_moving_to_supply",
		"transporter_loading",
		"transporter_moving_back",
		"transporter_unloading",
		"diagnosing",
		"searching_upper_levels",
		"waiting_for_transporter",
		"transporter_moving_to_supply",
		"transporter_loading",
		"transporter_moving_back",
		"transporter_unloading",
		"waiting_for_repairer",
		"repairing",
		"moving_back",
	)
	checkMaintenance(c, client, unit, electronic_3,
		"moving_to_supply",
		"diagnosing",
		"searching_upper_levels",
		"moving_to_supply",
		"searching_upper_levels",
		"moving_to_supply",
		"waiting_for_repairer",
		"repairing",
		"moving_back",
	)
	checkMaintenance(c, client, unit, mobility_3,
		"transporter_moving_to_supply",
		"transporter_loading",
		"transporter_moving_back",
		"transporter_unloading",
		"diagnosing",
		"searching_upper_levels",
		"waiting_for_transporter",
		"transporter_moving_to_supply",
		"transporter_loading",
		"transporter_moving_back",
		"transporter_unloading",
		"searching_upper_levels",
		"waiting_for_transporter",
		"transporter_moving_to_supply",
		"transporter_loading",
		"transporter_moving_back",
		"transporter_unloading",
		"waiting_for_repairer",
		"repairing",
		"moving_back",
	)
}
