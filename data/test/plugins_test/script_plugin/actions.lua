-- *****************************************************************************
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
--
-- *****************************************************************************

--------------------------------------------------------------------------------
--
-- Unit test suite for actions.lua
--
--------------------------------------------------------------------------------

brain "test_suite" {}

dofile "resources/scripts/actions.lua"

include "luaunit.lua"

TestActions = {}

function TestActions:setUp()
	-- nothing to set up
end

function TestActions:tearDown()
	-- nothing to tear down
end

function TestActions:test_Mission_HasDefaultParameters()
	local expectation =
		"<action id=\"51\" name=\"\" target=\"42\" type=\"mission\">\n" ..
			"<parameter name=\"Danger direction\" type=\"Direction\" value=\"360\"/>\n" ..
			"<parameter name=\"Phase lines\" type=\"PhaseLineList\"/>\n" ..
			"<parameter name=\"Boundary limit 1\" type=\"Limit\"/>\n" ..
			"<parameter name=\"Boundary limit 2\" type=\"Limit\"/>\n" ..
			"<parameter name=\"Intelligences\" type=\"IntelligenceList\"/>\n" ..
		"</action>\n";
	assertEquals( Mission.create( 42, 51 ):ToXml(), expectation )
end

function TestActions:test_Mission_WithAddsParameters()
	local expectation =
		"<action id=\"51\" name=\"\" target=\"42\" type=\"mission\">\n" ..
			"<parameter name=\"Danger direction\" type=\"Direction\" value=\"69\"/>\n" ..
			"<parameter name=\"Phase lines\" type=\"PhaseLineList\"/>\n" ..
			"<parameter name=\"Boundary limit 1\" type=\"Limit\"/>\n" ..
			"<parameter name=\"Boundary limit 2\" type=\"Limit\"/>\n" ..
			"<parameter name=\"Intelligences\" type=\"IntelligenceList\"/>\n" ..
			"<parameter name=\"Route\" type=\"Path\">\n" ..
				"<parameter name=\"Destination\" type=\"pathpoint\">\n" ..
					"<location type=\"point\">\n" ..
						"<point coordinates=\"30UYV1152545719\"/>\n" ..
					"</location>\n" ..
				"</parameter>\n" ..
			"</parameter>\n" ..
		"</action>\n";
	local order = Mission.create( 42, 51 )
						:With( { name = "Danger direction", type = "Direction", value = 69 } )
						:With( Path.create( "Route" ):AddPoint( "Destination", "30UYV1152545719" ) )
	assertEquals( order:ToXml(), expectation )
end

function TestActions:test_FragOrder_HasNoDefaultParameter()
	local expectation = "<action id=\"51\" name=\"\" target=\"42\" type=\"fragorder\"/>\n"
	assertEquals( FragOrder.create( 42, 51 ):ToXml(), expectation )
end

function TestActions:test_FragOrder_WithAddsParameters()
	local expectation =
		"<action id=\"51\" name=\"\" target=\"42\" type=\"fragorder\">\n" ..
			"<parameter name=\"Route\" type=\"Path\">\n" ..
				"<parameter name=\"Destination\" type=\"pathpoint\">\n" ..
					"<location type=\"point\">\n" ..
						"<point coordinates=\"30UYV1152545719\"/>\n" ..
					"</location>\n" ..
				"</parameter>\n" ..
			"</parameter>\n" ..
		"</action>\n";
	local order = FragOrder.create( 42, 51 ):With( Path.create( "Route" ):AddPoint( "Destination", "30UYV1152545719" ) )
	assertEquals( order:ToXml(), expectation )
end

function RunTest()
	LuaUnit:run()
end
