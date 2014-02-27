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
