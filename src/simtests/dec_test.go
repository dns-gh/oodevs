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
	"strconv"
	"strings"
	"swapi"
	"swapi/simu"
	"swtest"
	"text/template"
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

	// Interpolating with start == stop triggers an error()
	checkScript(c, client, `
function TestFunction()
    return LinearInterpolation(1, 2, 3, 3, 0, 1.5)
end
`, nil, "", "error_invalid_parameter:.*Can't interpolate.*")
}

func (s *TestSuite) TestDecUnit(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, DisableLuaChecks())
	defer client.Close()
	automat := createAutomat(c, client)
	unit := CreateUnit(c, client, automat.Id)

	script := `function TestFunction()
    return DEC_Agent_MaxSpeed(DEC_GetUnitById({{.unitid}}))
end
`
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

func DecConsumeResources(c *C, client *swapi.Client, unit uint32, resource ResourceType, offset, duration int) uint32 {
	script := strings.TrimSpace(`
function ConsumeResources()
    dotation = DEC_GetDotation({{.resource}})
    action = DEC_StartConsumingResources(dotation, {{.offset}}, {{.duration}})
    return tostring(action)
end
`)
	output, err := client.ExecScript(unit, "ConsumeResources", Parse(c, script,
		map[string]interface{}{
			"resource": resource,
			"offset":   offset,
			"duration": duration,
		}))
	c.Assert(err, IsNil)
	id, err := strconv.ParseUint(output, 10, 32)
	c.Assert(err, IsNil)
	return uint32(id)
}

func DecApplyAction(c *C, client *swapi.Client, name string, unit, action uint32) {
	names := map[string]string{
		"suspend": "Pause",
		"resume":  "Reprend",
		"stop":    "_Stop",
	}
	script := strings.TrimSpace(`
function ApplyAction()
    DEC_{{.name}}Action({{.action}})
    return ""
end
`)
	_, err := client.ExecScript(unit, "ApplyAction", Parse(c, script,
		map[string]interface{}{
			"name":   names[name],
			"action": action,
		}))
	c.Assert(err, IsNil)
}

func DecSuspendAction(c *C, client *swapi.Client, unit, action uint32) {
	DecApplyAction(c, client, "suspend", unit, action)
}

func DecResumeAction(c *C, client *swapi.Client, unit, action uint32) {
	DecApplyAction(c, client, "resume", unit, action)
}

func DecStopAction(c *C, client *swapi.Client, unit, action uint32) {
	DecApplyAction(c, client, "stop", unit, action)
}

func TickOnce(c *C, client *swapi.Client) {
	tick := client.Model.GetTick()
	err := client.Resume(1)
	c.Assert(err, IsNil)
	done := client.Model.WaitUntilTickEnds(tick)
	c.Assert(done, Equals, true)
}

func testDecStartConsumingResources(c *C, initial int32, percentage int, callback func(client *swapi.Client, unit, action, dotation uint32)) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadLog))
	defer stopSimAndClient(c, sim, client)
	d := client.Model.GetData()
	unit := getSomeUnitByName(c, d, "Maintenance Log Unit 3")
	const dotation = uint32(electrogen_1)
	err := client.ChangeDotation(unit.Id,
		map[uint32]*swapi.ResourceDotation{
			dotation: &swapi.ResourceDotation{
				Quantity:  initial,
				Threshold: 0,
			},
		})
	c.Assert(err, IsNil)
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Units[unit.Id].ResourceDotations[dotation].Quantity == initial
	})
	client.Pause()
	action := DecConsumeResources(c, client, unit.Id, electrogen_1, percentage, 10*10)
	TickOnce(c, client) // need one full tick to apply dec command
	callback(client, unit.Id, action, dotation)
}

func (s *TestSuite) TestDecStartConsumingResources(c *C) {
	// normal case
	testDecStartConsumingResources(c, 10, -100, func(client *swapi.Client, unit, action, dotation uint32) {
		quantities := []int32{9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0}
		for _, qty := range quantities {
			TickOnce(c, client)
			d := client.Model.GetData()
			c.Assert(d.Units[unit].ResourceDotations[dotation].Quantity, Equals, qty)
		}
	})
	// non full dotations
	testDecStartConsumingResources(c, 6, -100, func(client *swapi.Client, unit, action, dotation uint32) {
		quantities := []int32{5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0}
		for _, qty := range quantities {
			TickOnce(c, client)
			d := client.Model.GetData()
			c.Assert(d.Units[unit].ResourceDotations[dotation].Quantity, Equals, qty)
		}
	})
	// capped percentage
	testDecStartConsumingResources(c, 10, -200, func(client *swapi.Client, unit, action, dotation uint32) {
		quantities := []int32{8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0}
		for _, qty := range quantities {
			TickOnce(c, client)
			d := client.Model.GetData()
			c.Assert(d.Units[unit].ResourceDotations[dotation].Quantity, Equals, qty)
		}
	})
	// stop action before end
	testDecStartConsumingResources(c, 10, -100, func(client *swapi.Client, unit, action, dotation uint32) {
		quantities := []int32{9, 8, 7, 6, 5, 5, 5}
		for i, qty := range quantities {
			TickOnce(c, client)
			d := client.Model.GetData()
			if i == 3 {
				DecStopAction(c, client, unit, action)
			}
			c.Assert(d.Units[unit].ResourceDotations[dotation].Quantity, Equals, qty)
		}
	})
	// suspend action in the middle
	testDecStartConsumingResources(c, 10, -100, func(client *swapi.Client, unit, action, dotation uint32) {
		quantities := []int32{9, 8, 7, 6, 5, 5, 5, 4, 3, 2, 1, 0, 0, 0}
		for i, qty := range quantities {
			TickOnce(c, client)
			d := client.Model.GetData()
			if i == 3 {
				DecSuspendAction(c, client, unit, action)
			}
			if i == 5 {
				DecResumeAction(c, client, unit, action)
			}
			c.Assert(d.Units[unit].ResourceDotations[dotation].Quantity, Equals, qty)
		}
	})
}
