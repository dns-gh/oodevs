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

require "io"
require "table"

--------------------------------------------------------------------------------
-- Indicator recorder
--------------------------------------------------------------------------------

-- Recorder singleton
Recorder = {
	ticks = 0,
	data = {},
}

-- Add indicator to be tracked
function Recorder.Track( name )
	Recorder.data[name] = {}
end

-- Start recorder
function Recorder.Start()
	Recorder.ticks = 0
	ChangeState( "recorder_start" )
end

-- Reset recorder
function Recorder.Reset()
	Recorder.ticks = 0
	Recorder.data = {}
end

-- Create fsm recording states:
--  * records tracked indicators values
--  * goto "next_state" when "ticks" simulation ticks have been processed
function Recorder.Record( ticks, next_state )
	return {
		{
			events.indicators:IndicatorChanged(),
			{ "recorder_start" },
			function( name, value )
				Recorder.RecordValue( name, value )
			end
		},
		{
			events.sim:TickEnded(),
			{ "recorder_start" },
			function()
				Recorder.ticks = Recorder.ticks + 1
				if Recorder.ticks >= ticks then
					ChangeState( next_state )
				end
			end
		}
	}
end

-- Utility function to record an indicator value
function Recorder.RecordValue( name, value )
	if Recorder.data[name] ~= nil then
		Recorder.data[name][Recorder.ticks] = value
	end
end

-- Save data to specific file (csv format with headers)
function Recorder.Save( filename )
	local output = io.open( filename, "w+" )
	Recorder._SaveHeader( output )
	local last = {}
	for i = 1, Recorder.ticks, 1 do
		local line = {}
		for k, values in pairs( Recorder.data ) do
			if values[i] then
				last[k] = values[i]
			end
			table.insert( line, last[k] or 0 )
		end
		Recorder._SaveLine( output, line )
	end
	output:close()
end

function Recorder._SaveHeader( output )
	local headers = {}
	for k, _ in pairs( Recorder.data ) do
		table.insert( headers, k )
	end
	Recorder._SaveLine( output, headers )
end

function Recorder._SaveLine( output, line )
	output:write( table.concat( line, "," ) .. ";\n" )
end

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
	return self
end

-- Activate recording
function Indicator:Record()
	Recorder.Track( self.name )
	return self
end
