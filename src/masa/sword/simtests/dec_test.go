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
	"bytes"
	"fmt"
	"io"
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/swapi/simu"
	"masa/sword/swtest"
	"os"
	"path/filepath"
	"reflect"
	"regexp"
	"strconv"
	"strings"
	"time"
)

func DisableLuaChecks() *simu.SessionErrorsOpts {
	return &simu.SessionErrorsOpts{
		IgnoreLua: true,
	}
}

func (s *TestSuite) TestExecScript(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	unit := getRandomUnit(c, client)

	function := "TestFunc"
	script := `function TestFunc() return "foo" end`
	result, err := client.ExecScript(unit.Id, function, script)
	c.Assert(err, IsNil)
	c.Assert(result, Equals, "foo")

	// Invalid tasker
	_, err = client.ExecScript(1234, function, script)
	c.Assert(err, ErrorMatches, "error_invalid_unit.*")

	// Invalid function
	_, err = client.ExecScript(unit.Id, "IAmNotAFunction", script)
	c.Assert(err, ErrorMatches, "error_invalid_parameter: attempt to call a nil value")

	// Invalid script
	_, err = client.ExecScript(unit.Id, function, "do you really believe this will work?")
	c.Assert(err, ErrorMatches, "error_invalid_parameter:.*")

	// Function does not return a string
	script = `function TestFunc() return 1 end`
	result, err = client.ExecScript(unit.Id, function, script)
	c.Assert(err, IsNil)
	c.Assert(result, Equals, "1")

	// Function returns nil
	script = `function TestFunc() return nil end`
	_, err = client.ExecScript(unit.Id, function, script)
	c.Assert(err, ErrorMatches, "error_invalid_parameter:.*string expected, got nil.*")
}

func execScript(c *C, client *swapi.Client, script string, keys map[string]interface{}) (
	string, error) {

	unit := getRandomUnit(c, client)
	return client.ExecTemplate(unit.Id, "TestFunction", script, keys)
}

func checkScript(c *C, client *swapi.Client, script string, keys map[string]interface{},
	expectedPattern, errorPattern string) {

	output, err := execScript(c, client, script, keys)
	if len(errorPattern) == 0 {
		c.Assert(err, IsNil)
		c.Assert(output, Matches, expectedPattern)
	} else {
		c.Assert(err, ErrorMatches, errorPattern)
	}
}

// Evaluate supplied script as a Go template with supplied values, execute it.
// Returned string is expected to be like:
//
//   RESULT
//   "-- EXPECTED --"
//   EXPECTED
//
// Parts are split and diffed.
func diffScript(c *C, client *swapi.Client, script string, keys map[string]interface{}) {
	output, err := execScript(c, client, script, keys)
	c.Assert(err, IsNil)
	parts := strings.Split(output, "-- EXPECTED --\n")
	c.Assert(parts, HasLen, 2)
	swtest.DeepEquals(c, parts[0], parts[1])
}

func (s *TestSuite) TestGenericLuaErrors(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, DisableLuaChecks())
	defer client.Close()

	// Floating point division by zero used to trigger exceptions in the sim
	checkScript(c, client, `function TestFunction() return 1.0/0.0 end`, nil, "(?i).*inf.*", "")

	// Interpolating with minFrom == maxFrom triggers an error()
	checkScript(c, client, `
function TestFunction()
    return LinearInterpolation(1, 2, 3, 3, 0, 1.5)
end
`, nil, "", "error_invalid_parameter:.*Can't interpolate.*")

	// Splitting a string with an empty separator triggers an error()
	checkScript(c, client, `
function TestFunction()
    return explode( "", "abc" )
end
`, nil, "", "error_invalid_parameter:.*Can't split.*")
}

func (s *TestSuite) TestDecUnit(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, DisableLuaChecks())
	defer client.Close()
	automat := createAutomat(c, client)
	unit := CreateUnit(c, client, automat.Id)

	script := `
function TestFunction()
    return integration.getMaxSpeed({source=DEC_GetUnitById({{.unitid}})})
end`
	checkScript(c, client, script, map[string]interface{}{"unitid": 123456},
		"", ".*null pointer.*")
	// Result is in m/s
	checkScript(c, client, script, map[string]interface{}{"unitid": unit.Id},
		`19\.4\d*`, "")
}

func loadLuaScriptAndIncludes(rootDir, name string, included map[string]bool,
	loading []string, w io.Writer) error {

	// Check for cycles
	for _, n := range loading {
		if n == name {
			loading = append(loading, name)
			return fmt.Errorf("cyclic dependency: %s", strings.Join(loading, " -> "))
		}
	}
	loading = append(loading, name)

	// Resolve and open the script
	path := filepath.Join(rootDir, name+".lua")
	fp, err := os.Open(path)
	if err != nil {
		return fmt.Errorf("cannot find module %s: %s", name, err)
	}
	defer fp.Close()

	// Append it to the global script while resolving includes
	reCmt := regexp.MustCompile(`^\s*--`)
	reRequire := regexp.MustCompile(`^\s*require\s+"([^"]+)"\s*$`)
	scanner := bufio.NewScanner(fp)
	for scanner.Scan() {
		line := scanner.Text()
		if reCmt.MatchString(line) {
			line = ""
		}
		m := reRequire.FindStringSubmatch(line)
		if m != nil {
			if included[m[0]] {
				// We have it already
				continue
			}
			err = loadLuaScriptAndIncludes(rootDir, m[1], included, loading, w)
			if err != nil {
				return err
			}
			continue
		}
		_, err := io.WriteString(w, line+"\n")
		if err != nil {
			return err
		}
	}
	if err := scanner.Err(); err != nil {
		return err
	}
	included[name] = true
	return nil
}

// Loads lua script "name".
//
// Scripts are resolved relatively to "${TESTDIR}/testdata/dec" and a ".lua"
// extension is appended. "require" statements are expanded inlined, only once
// and cycles are detected.
func loadLuaScript(name string) (string, error) {
	rootDir, err := os.Getwd()
	if err != nil {
		return "", err
	}
	rootDir = filepath.Join(rootDir, "testdata/dec")
	included := map[string]bool{}
	loading := []string{}
	w := bytes.Buffer{}
	err = loadLuaScriptAndIncludes(rootDir, name, included, loading, &w)
	return w.String(), err
}

func (s *TestSuite) TestLoadLuaScripts(c *C) {
	// cyclic includes
	_, err := loadLuaScript("recurse_test")
	c.Assert(err, ErrorMatches, ".*cyclic.*")

	// missing includes
	_, err = loadLuaScript("missing_test")
	c.Assert(err, ErrorMatches, ".*cannot find module.*")
}

func (s *TestSuite) TestDecGeometry(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	runScript := func(name string) {
		script, err := loadLuaScript(name)
		c.Assert(err, IsNil)
		diffScript(c, client, script, nil)
	}

	runScript("point_test")
	runScript("polygon_test")
}

func DecCreateBreakdown(c *C, client *swapi.Client, unitId, equipmentType uint32,
	breakdownType int32, result string) {
	script := `function TestFunction()
		return tostring(DEC_CreateBreakdown(DEC_GetEquipmentFromID({{.equipmentType}}),
			{{.breakdownType}}))
	end`
	output, err := client.ExecTemplate(unitId, "TestFunction", script,
		map[string]interface{}{
			"equipmentType": equipmentType,
			"breakdownType": breakdownType,
		})
	c.Assert(err, IsNil)
	c.Assert(output, Equals, result)
}

func (s *TestSuite) TestDecCreateBreakdown(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallTest))
	defer stopSim(c, sim, DisableLuaChecks())
	defer client.Close()
	phydb := loadPhysicalData(c, "test")

	party := &swtest.Party{
		Name: "party1",
		Formations: []*swtest.Formation{
			&swtest.Formation{
				Name: "formation",
				Automats: []*swtest.Automat{
					&swtest.Automat{
						Name: "automat",
						Type: "VW Combi Rally",
						Units: []*swtest.Unit{
							&swtest.Unit{
								Name: "unit",
								Type: "VW Combi",
							},
						},
					},
				},
			},
		},
	}
	err := swtest.FindOrCreateEntities(client, phydb, party)
	c.Assert(err, IsNil)
	unit := party.Formations[0].Automats[0].Units[0].Entity

	// VW Combi
	equipmentType := uint32(2)
	// breakdown
	breakdownType := int32(107)

	// Invalid equipment type
	DecCreateBreakdown(c, client, unit.Id, 12345, 12345, "false")
	// Invalid breakdown type
	DecCreateBreakdown(c, client, unit.Id, equipmentType, 12345, "false")

	// Check initial state
	equipment := swapi.Equipment{
		Available:     1,
		Unavailable:   0,
		Repairable:    0,
		OnSiteFixable: 0,
	}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[unit.Id].Equipments[equipmentType], &equipment)
	})

	DecCreateBreakdown(c, client, unit.Id, equipmentType, breakdownType, "true")

	// Check breakdown
	equipment = swapi.Equipment{
		Available:     0,
		Unavailable:   0,
		Repairable:    1,
		OnSiteFixable: 0,
		Breakdowns:    []int32{breakdownType},
	}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[unit.Id].Equipments[equipmentType], &equipment)
	})
}

func DecConsumeResources(c *C, client *swapi.Client, unit, dotation uint32, offset, duration float64) {
	script := strings.TrimSpace(`
function ConsumeResources()
    DEC_StartConsumingResources({{.dotation}}, {{.offset}}, {{.duration}})
    return "";
end
`)
	_, err := client.ExecTemplate(unit, "ConsumeResources", script,
		map[string]interface{}{
			"dotation": dotation,
			"offset":   offset,
			"duration": duration,
		})
	c.Assert(err, IsNil)
}

func testDecStartConsumingResources(c *C, client *swapi.Client, unitType uint32,
	initial int32, percentage, duration float64, quantities []int32) {

	d := client.Model.GetData()
	automat := getSomeAutomatByName(c, d, "Maintenance Log Automat 3")
	unit, err := client.CreateUnit(automat.Id, unitType, swapi.Point{X: -15.8219, Y: 28.2456})
	c.Assert(err, IsNil)
	const dotation = uint32(electrogen_1)
	resource := swapi.Resource{
		Quantity:      initial,
		LowThreshold:  0,
		HighThreshold: 100,
	}
	if unit.Resources[dotation] != resource {
		err = client.ChangeResource(unit.Id,
			map[uint32]*swapi.Resource{dotation: &resource})
		c.Assert(err, IsNil)
		waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
			return data.Units[unit.Id].Resources[dotation] == resource
		})
	}
	client.Pause()
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
		if m.GetControlEndTick() == nil {
			return false
		}
		dotations := client.Model.GetUnit(unit.Id).Resources[dotation]
		c.Check(dotations.Quantity, Equals, quantities[idx])
		idx++
		if c.Failed() || idx == len(quantities) {
			close(quit)
			return true
		}
		return false
	})
	defer client.Unregister(ctx)
	const parts = 5
	DecConsumeResources(c, client, unit.Id, parts, percentage, duration)
	client.Resume(0)
	select {
	case <-quit:
	case <-time.After(1 * time.Minute):
		c.Error("timeout")
	}
	c.Assert(idx, Equals, len(quantities))
}

func (s *TestSuite) TestDecStartConsumingResources(c *C) {
	// Find type of unit which resources will be consumed
	phydb := loadPhysical(c, "test")
	unitType := getUnitTypeFromName(c, phydb, "Maintenance Log Unit 3")

	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)

	// normal case
	testDecStartConsumingResources(c, client, unitType, 10, -100, 100,
		[]int32{9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0})
	// non integer percentage
	testDecStartConsumingResources(c, client, unitType, 10, -33.334, 100,
		[]int32{10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7})
	// non integer time
	testDecStartConsumingResources(c, client, unitType, 10, -100, 94.99,
		[]int32{9, 8, 7, 6, 4, 3, 2, 1, 0, 0, 0, 0})
	// null duration
	testDecStartConsumingResources(c, client, unitType, 10, -100, 0,
		[]int32{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
	// non full dotations
	testDecStartConsumingResources(c, client, unitType, 6, -100, 100,
		[]int32{5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0})
	// partly increasing dotations
	testDecStartConsumingResources(c, client, unitType, 2, 50, 100,
		[]int32{3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7})
	// capped percentage
	testDecStartConsumingResources(c, client, unitType, 10, -200, 100,
		[]int32{8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0})
}

func DecCanLoad(c *C, client *swapi.Client, unitId, targetId uint32, vehicles bool,
	result string) {
	script := `
function TestFunction()
	return tostring(integration.canLoad(
		DEC_GetUnitById({{.unitId}}),
		DEC_GetUnitById({{.targetId}}),
		{{.vehicles}}))
end`
	output, err := client.ExecTemplate(unitId, "TestFunction", script,
		map[string]interface{}{
			"unitId":   unitId,
			"targetId": targetId,
			"vehicles": vehicles,
		})
	c.Assert(err, IsNil)
	c.Assert(output, Equals, result)
}

func (s *TestSuite) TestDecCanLoad(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallTest))
	defer stopSim(c, sim, DisableLuaChecks())
	defer client.Close()
	phydb := loadPhysical(c, "test")
	d := client.Model.GetData()
	party := d.FindPartyByName("party1")
	CreateFormation(c, client, party.Id)
	automat := createSpecificAutomat(c, client, "party1",
		getAutomatTypeFromName(c, phydb, "Mobile Infantry Platoon"))
	from := swapi.Point{X: -15.8193, Y: 28.3456}
	carrier, err := client.CreateUnit(automat.Id,
		getUnitTypeFromName(c, phydb, "Carrier"), from)
	c.Assert(err, IsNil)
	heavy, err := client.CreateUnit(automat.Id,
		getUnitTypeFromName(c, phydb, "Log Convoy"), from)
	c.Assert(err, IsNil)
	light, err := client.CreateUnit(automat.Id,
		getUnitTypeFromName(c, phydb, "Mobile Infantry"), from)
	c.Assert(err, IsNil)

	// invalid target
	DecCanLoad(c, client, carrier.Id, 1000, false, "false")
	// carrier cannot load itself
	DecCanLoad(c, client, carrier.Id, carrier.Id, false, "false")
	// carrier can load heavy and light units
	DecCanLoad(c, client, carrier.Id, light.Id, false, "true")
	DecCanLoad(c, client, carrier.Id, heavy.Id, false, "true")

	// load a light unit
	err = client.LoadUnit(carrier.Id, light.Id)
	c.Assert(err, IsNil)
	// carrier can load heavy and light units
	DecCanLoad(c, client, carrier.Id, light.Id, false, "true")
	DecCanLoad(c, client, carrier.Id, heavy.Id, false, "true")

	// load a heavy unit
	err = client.LoadUnit(carrier.Id, heavy.Id)
	c.Assert(err, IsNil)
	// carrier cannot load heavy unit, only light unit
	DecCanLoad(c, client, carrier.Id, light.Id, false, "true")
	DecCanLoad(c, client, carrier.Id, heavy.Id, false, "false")
	// carrier cannot load light unit because it's not "loadable"
	DecCanLoad(c, client, carrier.Id, light.Id, true, "false")
}

// Check supplied unit can invoke "property()" or not.
func HasDecProperty(c *C, client *swapi.Client, unitId uint32, has bool,
	property string) {

	script := `
function TestFunction()
	return tostring({{.property}}())
end`
	output, err := client.ExecTemplate(unitId, "TestFunction", script,
		map[string]interface{}{"property": property})
	c.Assert(err, IsNil)
	res := output == "true"
	if res != has {
		c.Errorf("unexpected property: %s", property)
	}
}

// Check the lua symbol "function" is not nil in unitId context.
func HasDecFunction(c *C, client *swapi.Client, unitId uint32, has bool,
	function string) {

	script := `
function TestFunction()
	return tostring({{.function}} ~= nil)
end`
	output, err := client.ExecTemplate(unitId, "TestFunction", script,
		map[string]interface{}{"function": function})
	c.Assert(err, IsNil)
	res := output == "true"
	if res != has {
		c.Errorf("unexpected function %s", function)
	}
}

func HasDecPropertiesOrFunctions(c *C, client *swapi.Client, unitId, refUnitId uint32,
	properties, functions []string) {

	for i, unitId := range []uint32{unitId, refUnitId} {
		has := i == 0
		for _, property := range properties {
			HasDecProperty(c, client, unitId, has, property)
		}
		for _, fn := range functions {
			HasDecFunction(c, client, unitId, has, fn)
		}
	}
}

// Check units have expected DEC properties and functions depending on their type.
func (s *TestSuite) TestUnitTypes(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallTest))
	defer stopSim(c, sim, DisableLuaChecks())
	phydb := loadPhysical(c, "test")
	d := client.Model.GetData()
	party := d.FindPartyByName("party1")
	CreateFormation(c, client, party.Id)
	automat := createSpecificAutomat(c, client, "party1",
		getAutomatTypeFromName(c, phydb, "Mobile Infantry Platoon"))
	pos := swapi.Point{X: -15.8193, Y: 28.3456}

	unit, err := client.CreateUnit(automat.Id,
		getUnitTypeFromName(c, phydb, "VW Combi"), pos)
	c.Assert(err, IsNil)

	// NBC properties
	nbcUnit, err := client.CreateUnit(automat.Id,
		getUnitTypeFromName(c, phydb, "VW Combi Type NBC"), pos)
	HasDecPropertiesOrFunctions(c, client, nbcUnit.Id, unit.Id,
		[]string{"DEC_Agent_EstAgentNBC"},
		[]string{"DEC_DecontaminerZone", "DEC_Agent_SeDecontaminer"})
}

func DecLinearInterpolation(c *C, client *swapi.Client, unitId uint32,
	minTo, maxTo, minFrom, maxFrom float64, upSlope bool, value float64) (float64, error) {

	script := `function TestFunction()
    return LinearInterpolation({{.minTo}}, {{.maxTo}}, {{.minFrom}}, {{.maxFrom}},
        {{.upSlope}}, {{.value}})
end
`
	output, err := client.ExecTemplate(unitId, "TestFunction", script,
		map[string]interface{}{
			"minTo":   minTo,
			"maxTo":   maxTo,
			"minFrom": minFrom,
			"maxFrom": maxFrom,
			"upSlope": upSlope,
			"value":   value,
		})
	if err != nil {
		return 0.0, err
	}
	return strconv.ParseFloat(output, 64)
}

func (s *TestSuite) TestDecLinearInterpolation(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, DisableLuaChecks())
	unit := getRandomUnit(c, client)

	tests := []struct {
		MinTo, MaxTo, MinFrom, MaxFrom float64
		UpSlope                        bool
		Value                          float64
		Expected                       float64
	}{
		{0, 1, 0, 1, true, 0.5, 0.5},
		{0.3, 0.7, 1, 2, true, 0, 0.3},
		{0.3, 0.7, 1, 2, true, 3, 0.7},
		{0.3, 0.7, 1, 2, true, 1.25, 0.4},
		{0.3, 0.7, 1, 2, false, 1.25, 0.6},
	}
	for _, t := range tests {
		res, err := DecLinearInterpolation(c, client, unit.Id, t.MinTo, t.MaxTo,
			t.MinFrom, t.MaxFrom, t.UpSlope, t.Value)
		c.Assert(err, IsNil)
		c.Assert(res, Equals, t.Expected)
	}
}

func (s *TestSuite) TestDecGetTerrainData(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExLandOfStripesEmpty))
	defer stopSim(c, sim, DisableLuaChecks())
	model := client.Model.GetData()

	// create a unit to execute script function
	f := getSomeFormation(c, model)
	kg := getSomeKnowledgeGroup(c, model, f.PartyId)
	automat, err := client.CreateAutomat(f.Id, 2, kg.Id)
	c.Assert(err, IsNil)
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	unit, err := client.CreateUnit(automat.Id, 2, swapi.Point{X: -15.82, Y: 28.34})
	c.Assert(err, IsNil)

	// create a point in directia and call getTerrainData with it
	script := `
	function TestFunction()
		local point = DEC_Geometrie_CreerPointLatLong({{.lat}},{{.long}})
		local data = integration.getTerrainData(point)
		return tostring(data.first..","..data.second)
	end`

	tests := []struct {
		lat, long float64
		expected  string
	}{
		{28.44, -15.82, "1,0"},  // forest, no linear
		{28.34, -15.82, "16,0"}, // water area, no linear
		{28.23, -15.82, "0,0"},  // unknown, no linear
	}
	for _, t := range tests {
		output, err := client.ExecTemplate(unit.Id, "TestFunction", script,
			map[string]interface{}{
				"lat":  t.lat,
				"long": t.long,
			})
		c.Assert(err, IsNil)
		c.Assert(output, Equals, t.expected)
	}
}

func DecGetMaxRangeToFireOn(client *swapi.Client, requiredPh float64,
	followerId, followedKgId uint32) (float64, error) {

	script := `function TestFunction()
        local kg = DEC_GetAgentKnowledge({{.followerId}}, {{.followedKgId}})
        local range = DEC_Tir_PorteeMaxPourTirerSurUnite(kg, {{.requiredPh}})
        return range
	end`
	output, err := client.ExecTemplate(followerId, "TestFunction", script,
		map[string]interface{}{
			"followerId":   followerId,
			"followedKgId": followedKgId,
			"requiredPh":   requiredPh,
		})
	if err != nil {
		return 0, err
	}
	maxRange, err := strconv.ParseFloat(output, 64)
	return maxRange, err
}

func (s *TestSuite) TestDecGetMaxRangeToFireOn(c *C) {
	phydb := loadPhysical(c, "test")

	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallTest))
	defer stopSimAndClient(c, sim, client)

	// The VW Direct Fire has two weapon systems, one firing up to 500m, the
	// second up to 1000m
	pos := swapi.Point{X: -15.9268, Y: 28.3453}
	//ammo500m := getResourceTypeFromName(c, phydb, "direct fire ammo 500m")
	//ammo1km := getResourceTypeFromName(c, phydb, "direct fire ammo 1km")

	// Create following unit
	follower := CreateUnitInParty(c, client, phydb, "party1", "VW Combi Rally",
		"VW Direct Fire", pos)
	data := client.Model.GetData()
	kg := data.KnowledgeGroups[data.Automats[follower.AutomatId].KnowledgeGroupId]
	c.Assert(kg, NotNil)

	// Create followed unit
	followed := CreateUnitInParty(c, client, phydb, "party2", "VW Combi Rally",
		"VW Combi", pos)

	// Wait for follower to aquire followed knowledge
	followedKgId := uint32(0)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, k := range data.UnitKnowledges {
			if k.KnowledgeGroupId == kg.Id && k.UnitId == followed.Id {
				followedKgId = k.Id
				return true
			}
		}
		return false
	})

	// Checks max range to fire is 1km
	maxRange, err := DecGetMaxRangeToFireOn(client, 1, follower.Id, followedKgId)
	c.Assert(err, IsNil)
	c.Assert(maxRange, Equals, 1000.0)

	// Drops 1km ammunitions
	const dotation = 117
	resource := swapi.Resource{
		Quantity:      0,
		LowThreshold:  100,
		HighThreshold: 100,
	}
	err = client.ChangeResource(follower.Id, map[uint32]*swapi.Resource{dotation: &resource})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[follower.Id].Resources[dotation] == resource
	})

	// Checks max range to fire falls back to 500m
	maxRange, err = DecGetMaxRangeToFireOn(client, 1, follower.Id, followedKgId)
	c.Assert(err, IsNil)
	c.Assert(maxRange, Equals, 500.0)
}

func (s *TestSuite) TestDecGetClosestPath(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	automat := createAutomat(c, client)
	positions := []swapi.Point{
		{X: -15.9170, Y: 28.2645},
		{X: -15.9200, Y: 28.2645},
		{X: -15.9230, Y: 28.2645},
		{X: -15.9260, Y: 28.2645},
	}
	startPosition := swapi.Point{X: -15.9200, Y: 28.2650}
	unit, err := client.CreateUnit(automat.Id, UnitType, startPosition)
	c.Assert(err, IsNil)

	// Create pathfind
	pathfind, err := client.CreatePathfind(unit.Id, positions...)
	c.Assert(err, IsNil)

	// Send pathfind to unit
	heading := swapi.MakeHeading(0)
	params := swapi.MakeParameters(heading, nil, nil, nil, swapi.MakePathfind(pathfind))
	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	_, err = client.SendUnitOrder(unit.Id, MissionMoveAlongId, params)
	// wait for first move
	waitCondition(c, client.Model, func(m *swapi.ModelData) bool {
		return !isNearby(startPosition, m.Units[unit.Id].Position)
	})
	script := `function TestFunction()
        local beginPoint = DEC_Geometrie_CreerPointLatLong(28.2645,-15.9200)
        local endPoint = DEC_Geometrie_CreerPointLatLong(28.2645,-15.9230)
        local path = DEC_GetClosestPath(myself, beginPoint, endPoint)
        if DEC_Geometrie_DistanceBetweenPoints( path[1], beginPoint ) > 10 or
           DEC_Geometrie_DistanceBetweenPoints( path[#path], endPoint ) > 10
        then
        	return "false"
    	end
       	return "true"
	end`
	output, err := client.ExecTemplate(unit.Id, "TestFunction", script,
		map[string]interface{}{})
	c.Assert(err, IsNil)
	result, err := strconv.ParseBool(output)
	c.Assert(err, IsNil)
	c.Assert(result, Equals, true)
}
