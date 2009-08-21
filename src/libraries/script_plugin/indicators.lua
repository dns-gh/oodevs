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
-- Indicators
--
-- Provides tools for creating score indicators
--
--------------------------------------------------------------------------------

require "table"

--------------------------------------------------------------------------------
-- Indicator
--------------------------------------------------------------------------------
Indicator = {}
Indicator.__index = Indicator

function Indicator.create( name, formula )
	local new = {}
	setmetatable( new, Indicator )
	new.name = name
	new.formula = formula
	new.variables = {}
	return new
end

-- Define Variable
function Indicator:With( name, type, value )
	table.insert( self.variables, indicators:CreateVariable( name, type, value ) )
	return self
end

-- Start computation
function Indicator:Compute()
	indicators:Compute( self.name, self.formula, self.variables )
end
