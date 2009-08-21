brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/actions.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/indicators.lua"
include "resources/model.lua"
include "resources/config.lua"

function Start()

	local output = {}
    local eventTable =
    {
        {
            events:Once(),
            {},
            function()
                ChangeState( "startup" )
            end
        },

        {
            events.sim:TickEnded(),
            { "startup" },
            function()
				Trace( "startup" )
                ChangeState( "test_begin" )
            end
        },

        {
            events:Once(),
            { "test_begin" },
            function()
				Trace( "begin" )
                sim:CreateUnit( coord:UtmPosition( config.positions.start[1] )
                              , model.types.units["MBT platoon"]
                              , model.entities.automats["ABC"] )
                ChangeState( "test_mission" )
            end
        },

        {
            events.agents:AgentCreated(),
            { "test_mission" },
            function( entity )
				Trace( "mission" )
				Mission.create( entity:GetIdentifier(), model.types.missions["move"] )
						:With( { name = "Danger direction", type = "Direction", value = 0 } )
						:With( Path.create( "Route" ):AddPoint( "Destination", config.positions.destination[1] ) )
						:Issue()
				Trace( "indicator" )
				--Indicator.create( "opstate", "select( operational-state(), $Unit )" )
				--			:With( "Unit", "unit", entity:GetIdentifier() )
				--			:Compute()
				Indicator.create( "distance_to_objective", "distance( select( position(), $Unit ), $Destination )" )
							:With( "Unit", "unit", entity:GetIdentifier() )
							:With( "Destination", "position", config.positions.destination[1] )
							:Compute()
				Trace( "running" )
				ChangeState( "test_run" )
            end
        },

		{
			events.indicators:IndicatorChanged(),
			{ "test_run" },
			function( name, value )
				Trace( "indicator updated: " .. name .. " = " .. value )
				if output[name] == nil then
					output[name] = {}
				end
				table.insert( output[name], value )
				if #output[name] > 10 then
					ChangeState( "test_end" )
				end
			end
		},

		AtState( "test_end",
            function()
				Trace( "end" )
				-- output results to file
				local results = io.open( "result.csv", "w+" )
				for k, values in pairs( output ) do
					results:write( k )
					for _, v in ipairs( values ) do
						results:write( ",", v )
					end
					results:write( "\n" )
				end
				results:close()

                Deactivate()
            end
        )

    }

    DeclareEvents( eventTable )
end
