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
-- Unit test suite for artillery.lua
--
--------------------------------------------------------------------------------

brain "test_suite" {}

dofile "resources/scripts/artillery.lua"

include "luaunit.lua"


TestArtillery = {}

function TestArtillery:setUp()
	-- nothing to set up
end

function TestArtillery:tearDown()
	-- nothing to tear down
end

function TestArtillery:test_SimpleFire()
	-- artillery.DeclareFires( { name = "FireArea", firers = { 529, 530, 531 }, targetTeams = { "Blue" } } )
end

function TestArtillery:test_MultipleFires()
end

function RunTest()
	LuaUnit:run()
end
