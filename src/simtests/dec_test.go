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
	"bytes"
	. "launchpad.net/gocheck"
	"regexp"
	"swapi"
	"text/template"
)

func getRandomUnit(c *C, client *swapi.Client) *swapi.Unit {
	data := client.Model.GetData()
	return getSomeUnit(c, data)
}

func (s *TestSuite) TestExecScript(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer sim.Stop()
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

func checkScript(c *C, client *swapi.Client, script string, keys map[string]interface{},
	expectedPattern, errorPattern string) {

	w := &bytes.Buffer{}
	t, err := template.New("test").Parse(script)
	c.Assert(err, IsNil)
	err = t.Execute(w, keys)
	c.Assert(err, IsNil)
	text := string(w.Bytes())

	unit := getRandomUnit(c, client)
	output, err := client.ExecScript(unit.Id, "TestFunction", text)
	if len(errorPattern) == 0 {
		c.Assert(err, IsNil)
		c.Assert(output, Matches, expectedPattern)
	} else {
		c.Assert(err, ErrorMatches, errorPattern)
	}
}

func (s *TestSuite) TestGenericLuaErrors(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer sim.Stop()

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
	defer sim.Stop()
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

const geometryHelpers = `
function CreatePolygonFromXY(coords)
    if not coords then
        return nil
    end
    local points = {}
    for i = 1, #coords/2 do
        points[i] = DEC_Geometrie_CreerPointXY(coords[2*i-1], coords[2*i])
    end
    return DEC_Geometrie_CreerLocalisationPolygone(points)
end

function CreatePolygonFromLatLong(coords)
    if not coords then
        return nil
    end
    local points = {}
    for i = 1, #coords/2 do
        points[i] = DEC_Geometrie_CreerPointLatLong(coords[2*i-1], coords[2*i])
    end
    return DEC_Geometrie_CreerLocalisationPolygone(points)
end

function PointToString(point)
    return string.format("(%.2f, %.2f)", point:DEC_Geometrie_X(), point:DEC_Geometrie_Y())
end

function PolygonToString(poly)
    if not poly then
        return "nil"
    end
    output = "["
    if poly then
        local points = DEC_Geometrie_ListePointsLocalisation(poly)
        for i = 1, #points do
        output = output .. PointToString(points[i]) .. ", "
        end
    end
    output = output .. "]"
    return output
end
`

func testDecGeometryPoints(c *C, client *swapi.Client) {
	script := geometryHelpers + `
function TestFunction()
    points = {
        DEC_Geometrie_CreerPoint(),
        DEC_Geometrie_CreerPointLatLong(-28.3456, -15.8193),
        DEC_Geometrie_CreerPointLatLong(0, 0),
        DEC_Geometrie_CreerPointXY(10000, 20000),
    }
    result = ""
    for i = 1, #points do
        p = points[i]
        result = result .. PointToString(p) .. "\n"
    end
    return result
end
`
	expected := regexp.QuoteMeta(`(0.00, 0.00)
(11829.77, -5316878.77)
(1750510.29, -2899797.56)
(10000.00, 20000.00)
`)
	checkScript(c, client, script, nil, expected, "")
}

func testDecGeometryPolygon(c *C, client *swapi.Client) {
	script := geometryHelpers + `
function TestFunction()
    local tests = {
        { nil, {
            {1, 2},
        }},
        { {}, {
            {1, 2},
        }},
        { { 5, 5 }, {
            {1, 2},
        }},
        { { 5, 5, 6, 6 }, {
            {1, 2},
        }},
        { { 0, 0, 5, 0, 5, 5, 0, 0 }, {
            {2, 1},
            -- Stupid precision
            {6, 6},
            -- This one is really outside
            {10, 10},
        }},
    }
    local output = ""
    for i = 1, #tests do
        local poly = CreatePolygonFromXY(tests[i][1])
        local points = tests[i][2]
        for j = 1, #points do
            local p = DEC_Geometrie_CreerPointXY(points[j][1], points[j][2])
            local result = DEC_Geometrie_EstPointDansLocalisation(p, poly)
            local relation = "is in"
            if not result then
                relation = "is not in"
            end
            output = output .. string.format("%s %s %s\n", PointToString(p),
                relation, PolygonToString(poly))
        end
    end
    return output
end
`
	expected := regexp.QuoteMeta("" +
		`(1.00, 2.00) is not in nil
(1.00, 2.00) is in []
(1.00, 2.00) is in [(5.00, 5.00), ]
(1.00, 2.00) is in [(5.00, 5.00), (6.00, 6.00), ]
(2.00, 1.00) is in [(0.00, 0.00), (5.00, 0.00), (5.00, 5.00), (0.00, 0.00), ]
(6.00, 6.00) is in [(0.00, 0.00), (5.00, 0.00), (5.00, 5.00), (0.00, 0.00), ]
(10.00, 10.00) is not in [(0.00, 0.00), (5.00, 0.00), (5.00, 5.00), (0.00, 0.00), ]
`)
	checkScript(c, client, script, nil, expected, "")
}

func (s *TestSuite) TestDecGeometry(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer sim.Stop()
	testDecGeometryPoints(c, client)
	testDecGeometryPolygon(c, client)
}
