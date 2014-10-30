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
	"masa/sword/swapi"
	"masa/sword/swapi/simu"
	"masa/sword/sword"
	"masa/sword/swtest"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"
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
	swtest.DeepEquals(c, strings.Join(lines, "\n"), strings.Join(prefixes, "\n"))
}

func initLogisticEvents(c *C, client *swapi.Client) {
	data := client.Model.GetData()
	unit := getSomeUnitByName(c, data, "Maintenance Mobile Infantry 2")
	supply := getSomeUnitByName(c, data, "Maintenance Log Unit 1").Id
	supplied := getSomeUnitByName(c, data, "Supply Mobile Infantry")
	wounded := getSomeUnitByName(c, data, "Medical Mobile Infantry")

	phydb := loadPhysicalData(c, "test")
	citroen, err := phydb.Components.Find("Citroën 2CV")
	c.Assert(err, IsNil)
	breakdown, err := phydb.Breakdowns.Find("breakdown")
	c.Assert(err, IsNil)
	electrogen1Id, err := phydb.Resources.GetIdFromName("electrogen 1")
	c.Assert(err, IsNil)

	// Waiting for deployment
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		u := data.Units[supply]
		return u.IsMaintenanceEnabled && u.IsMedicalEnabled && u.IsSupplyEnabled
	})

	// Generate maintenance activity
	dotation := swapi.Equipment{
		Available: 5,
	}
	swtest.DeepEquals(c, dotation, *unit.Equipments[citroen.Id])
	equipment := swapi.Equipment{
		Available:  4,
		Repairable: 1,
		Breakdowns: []int32{int32(breakdown.Id)},
	}
	err = client.ChangeEquipmentState(unit.Id, map[uint32]*swapi.Equipment{
		citroen.Id: &equipment})
	c.Assert(err, IsNil)

	// Generate medical and funeral activity
	c.Assert(CheckHumanQuantity(wounded.Humans,
		map[int32]int32{eOfficer: 1, eWarrantOfficer: 1, eTrooper: 3}), Equals, true)
	injured := swapi.Human{
		Quantity:     1,
		Rank:         eTrooper,
		State:        eInjured,
		Injury:       eInjuryU2,
		Psyop:        true,
		Contaminated: true,
	}
	dead := swapi.Human{
		Quantity: 1,
		Rank:     eTrooper,
		State:    eDead,
	}
	err = client.ChangeHumanState(wounded.Id, []*swapi.Human{&injured, &dead})
	c.Assert(err, IsNil)

	// Generate supply activity
	c.Assert(unit.Resources, NotNil)
	swtest.DeepEquals(c, supplied.Resources[electrogen1Id],
		swapi.Resource{
			Quantity:      10,
			LowThreshold:  50,
			HighThreshold: 100,
		})
	resource := swapi.Resource{
		Quantity:      1,
		LowThreshold:  50,
		HighThreshold: 100,
	}
	err = client.ChangeResource(supplied.Id, map[uint32]*swapi.Resource{electrogen1Id: &resource})
	c.Assert(err, IsNil)

	client.Model.WaitTicks(2)
}

func (s *TestSuite) TestLogisticPlugin(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	initLogisticEvents(c, client)

	// Expected log lines below are the minimum subset of events which must have
	// been logged in the tested tick range. More can be logged in practice.
	client.Model.WaitTicks(3)
	logs := readAndRewriteLogFiles(c, sim.Opts)
	assertIsPrefixed(c, logs["funeral"], ""+
		`request id ; tick ; GDH ; unit ; handling unit ; conveying unit ; rank ; packaging resource ; state ; state end tick
 *** ; *** ; *** ; Medical Mobile Infantry ;  ;  ; Trooper ;  ;  ; *** 
 *** ; *** ; *** ; Medical Mobile Infantry ; Medical Log Automat 1 ;  ; Trooper ;  ; transporting unpackaged ; *** `)
	assertIsPrefixed(c, logs["maintenance"], ""+
		`request id ; tick ; GDH ; unit ; provider ; equipment ; breakdown ; state ; state end tick
 *** ; *** ; *** ; Maintenance Mobile Infantry 2 ;  ; Citroën 2CV ; breakdown ;  ; *** 
 *** ; *** ; *** ; Maintenance Mobile Infantry 2 ; Maintenance Log Unit 1 ; Citroën 2CV ; breakdown ; moving toward logistics system ; *** `)
	assertIsPrefixed(c, logs["medical"], ""+
		`request id ; tick ; GDH ; unit ; provider ; rank ; wound ; nbc ; mental ; state ; state end tick
 *** ; *** ; *** ; Medical Mobile Infantry ;  ; Trooper ; injury severity level 2 ; yes ; yes ;  ; *** 
 *** ; *** ; *** ; Medical Mobile Infantry ; Medical Log Unit 1 ; Trooper ; injury severity level 2 ; yes ; yes ; waiting for evacuation ambulance ; *** 
 *** ; *** ; *** ; Medical Mobile Infantry ; Medical Log Unit 1 ; Trooper ; injury severity level 2 ; yes ; yes ; evacuation ambulance coming ; *** `)
	assertIsPrefixed(c, logs["supply"], ""+
		`request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed
 *** ; *** ; *** ;  ; Supply Log Automat 1c ; Supply Log Automat 1c ;  ;  ; *** `)
}

func hashLogEntry(c *C, e *sword.LogHistoryEntry) string {
	data, err := proto.Marshal(e)
	c.Assert(err, IsNil)
	h := sha1.New()
	h.Write(data)
	return fmt.Sprintf("%x", h.Sum(nil))
}

func (s *TestSuite) TestLogisticHistory(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	initLogisticEvents(c, client)
	// Wait for some basic activity
	client.Model.WaitTicks(3)

	data := client.Model.GetData()
	handlingId := uint32(0)
	for id := range data.MedicalHandlings {
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
	for id := range data.Units {
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
	defer replay.Stop()
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

func CheckDeployTime(c *C, start, end *sword.DateTime, duration time.Duration) {
	startTime, _ := swapi.GetTime(start.GetData())
	endTime, _ := swapi.GetTime(end.GetData())
	c.Assert(endTime.Sub(startTime), Equals, duration)
}

func (s *TestSuite) TestLogisticDeployment(c *C) {
	phydb := loadPhysical(c, "test")
	opts := NewAdminOpts(ExCrossroadLog)
	opts.Paused = true
	sim, client := connectAndWaitModel(c, opts)
	defer stopSimAndClient(c, sim, client)
	model := client.Model
	tc2 := getSomeAutomatByName(c, model.GetData(), "Maintenance Automat 1")
	unit := getSomeUnitByName(c, model.GetData(), "Maintenance Log Unit 1")
	startingReportsTypes := []string{
		"Starting deploy",
		"Starting undeploy",
	}
	endingReportsTypes := []string{
		"section deployed",
		"Section undeployed",
	}
	startingReporter, _ := newReporter(c, unit.Id, phydb, startingReportsTypes...)
	startingReporter.Start(client.Model)
	_ = startingReportsTypes
	endingReporter, _ := newReporter(c, unit.Id, phydb, endingReportsTypes...)
	endingReporter.Start(client.Model)
	client.Resume(0)

	// Waiting for auto deployment
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit.Id].Installation == 100
	})

	// Assume all starting reports have been emitted at that point
	starting := startingReporter.Count()

	// Deploy TC2 to another position to force undeployment and deployment
	MissionLogDeploy := uint32(8)
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

	// Wait until the TC2 has deployed again
	ok := endingReporter.WaitCount(starting + 1)
	c.Assert(ok, Equals, true)

	// Check reports
	startingReports := startingReporter.Stop()
	endingReports := endingReporter.Stop()
	// Check that reports are always by pairs "Starting deploy" -> "section deployed"
	c.Assert(len(startingReports), Equals, len(endingReports))
	// Check we have at least auto deployment and undeployment following mission
	c.Assert(len(endingReports) >= 2, Equals, true)
	// Check there is 1mn delay in deployment
	for i := 0; i < len(startingReports); i++ {
		CheckDeployTime(c, startingReports[i].Time, endingReports[i].Time, 1*time.Minute)
	}
}

func (s *TestSuite) TestLogisticUpdates(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	admin := loginAndWaitModel(c, sim, NewAdminOpts(ExCrossroadLog))
	defer admin.Close()

	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Mobile Infantry")
	tc2Id := getSomeAutomatByName(c, d, "Maintenance Automat 1").Id
	tc2 := swapi.MakeAutomatTasker(tc2Id)
	SetManualMaintenance(c, client, tc2Id)
	handlingId := uint32(0)
	checkMaintenance(c, admin, client, unit, 0, mobility_2,
		MaintenanceCreateChecker{},
		checkUpdateThenApply("waiting_for_transporter_selection", tc2, true,
			func(ctx *MaintenanceCheckContext) error {
				handlingId = ctx.handlingId
				return client.SelectNewLogisticState(ctx.handlingId)
			}),
		checkUpdate("waiting_for_transporter", tc2),
		checkUpdate("transporter_moving_to_supply", tc2),
		checkUpdate("transporter_loading", tc2),
		checkUpdate("transporter_moving_back", tc2),
		checkUpdate("transporter_unloading", tc2),
		&MaintenanceStrictUpdateChecker{
			MaintenanceUpdateChecker{"waiting_for_diagnosis_team_selection", tc2},
		},
	)
	entries, err := client.GetLogisticHistory(handlingId)
	c.Assert(err, IsNil)
	// unloading entry has current state end tick defined
	unloading := entries[len(entries)-2].GetMaintenance().GetUpdate()
	c.Assert(unloading.GetState(), Equals, sword.LogMaintenanceHandlingUpdate_transporter_unloading)
	c.Assert(unloading.CurrentStateEndTick, NotNil)
	// waiting entry has current state end tick undefined again
	waiting := entries[len(entries)-1].GetMaintenance().GetUpdate()
	c.Assert(waiting.GetState(), Equals, sword.LogMaintenanceHandlingUpdate_waiting_for_diagnosis_team_selection)
	c.Assert(waiting.CurrentStateEndTick, IsNil)
}
