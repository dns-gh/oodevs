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
	"os"
	"path/filepath"
	"reflect"
	"regexp"
	"strings"
	"swapi"
	"swapi/simu"
	"swtest"
	"text/template"
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

func Parse(c *C, script string, keys map[string]interface{}) string {
	w := &bytes.Buffer{}
	t, err := template.New("test").Parse(script)
	c.Assert(err, IsNil)
	err = t.Execute(w, keys)
	c.Assert(err, IsNil)
	return string(w.Bytes())
}

func execScript(c *C, client *swapi.Client, script string, keys map[string]interface{}) (
	string, error) {

	unit := getRandomUnit(c, client)
	output, err := client.ExecScript(unit.Id, "TestFunction",
		Parse(c, script, keys))
	return output, err
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
	swtest.AssertEqualOrDiff(c, parts[0], parts[1])
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
		return tostring(DEC_CreateBreakdown({{.equipmentType}},
			{{.breakdownType}}))
	end`
	output, err := client.ExecScript(unitId, "TestFunction", Parse(c, script,
		map[string]interface{}{"equipmentType": equipmentType,
			"breakdownType": breakdownType}))
	c.Assert(err, IsNil)
	c.Assert(output, Equals, result)
}

func (s *TestSuite) TestDecCreateBreakdown(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, DisableLuaChecks())
	defer client.Close()
	automat := createAutomat(c, client)
	// ARMOR.MBT platoon
	unit := CreateUnit(c, client, automat.Id)
	// ARMOR.Main Battle Tank
	equipmentType := uint32(11)
	// MOB Breakdown 3 EB Evac
	breakdownType := int32(15)

	// Invalid equipment type
	DecCreateBreakdown(c, client, unit.Id, 12345, 12345, "false")
	// Invalid breakdown type
	DecCreateBreakdown(c, client, unit.Id, equipmentType, 12345, "false")

	// Check initial state
	equipment := swapi.EquipmentDotation{
		Available:     4,
		Unavailable:   0,
		Repairable:    0,
		OnSiteFixable: 0,
	}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[unit.Id].EquipmentDotations[equipmentType], &equipment)
	})

	DecCreateBreakdown(c, client, unit.Id, equipmentType, breakdownType, "true")

	// Check breakdown
	equipment = swapi.EquipmentDotation{
		Available:     3,
		Unavailable:   0,
		Repairable:    1,
		OnSiteFixable: 0,
		Breakdowns:    []int32{breakdownType},
	}
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Units[unit.Id].EquipmentDotations[equipmentType], &equipment)
	})
}

func DecConsumeResources(c *C, client *swapi.Client, unit uint32, resource ResourceType, offset, duration float64) {
	script := strings.TrimSpace(`
function ConsumeResources()
    dotation = DEC_GetDotation({{.resource}})
    DEC_StartConsumingResources(dotation, {{.offset}}, {{.duration}})
    return "";
end
`)
	_, err := client.ExecScript(unit, "ConsumeResources", Parse(c, script,
		map[string]interface{}{
			"resource": resource,
			"offset":   offset,
			"duration": duration,
		}))
	c.Assert(err, IsNil)
}

func testDecStartConsumingResources(c *C, client *swapi.Client, unitType uint32,
	initial int32, percentage, duration float64, quantities []int32) {

	d := client.Model.GetData()
	automat := getSomeAutomatByName(c, d, "Maintenance Log Automat 3")
	unit, err := client.CreateUnit(automat.Id, unitType, swapi.Point{X: -15.8219, Y: 28.2456})
	c.Assert(err, IsNil)
	const dotation = uint32(electrogen_1)
	resource := swapi.ResourceDotation{
		Quantity:  initial,
		Threshold: 0,
	}
	if unit.ResourceDotations[dotation] != resource {
		err = client.ChangeDotation(unit.Id,
			map[uint32]*swapi.ResourceDotation{dotation: &resource})
		c.Assert(err, IsNil)
		waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
			return data.Units[unit.Id].ResourceDotations[dotation] == resource
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
		dotations := client.Model.GetUnit(unit.Id).ResourceDotations[dotation]
		c.Check(dotations.Quantity, Equals, quantities[idx])
		idx++
		if c.Failed() || idx == len(quantities) {
			close(quit)
			return true
		}
		return false
	})
	defer client.Unregister(ctx)
	DecConsumeResources(c, client, unit.Id, electrogen_1, percentage, duration)
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
	output, err := client.ExecScript(unitId, "TestFunction", Parse(c, script,
		map[string]interface{}{"unitId": unitId,
			"targetId": targetId,
			"vehicles": vehicles}))
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
