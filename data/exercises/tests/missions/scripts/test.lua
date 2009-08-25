brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/actions.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/indicators.lua"
include "resources/model.lua"
include "resources/config.lua"

function Start()

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
				Indicator.create( "opstate", "select( operational-state(), $Unit )" )
							:With( "Unit", "unit", entity:GetIdentifier() )
							:Compute()
							:Record()
				Indicator.create( "distance_to_objective", "distance( select( position(), $Unit ), $Destination )" )
							:With( "Unit", "unit", entity:GetIdentifier() )
							:With( "Destination", "position", config.positions.destination[1] )
							:Compute()
							:Record()
				Trace( "running" )
				Recorder.Start()
            end
        },

		Recorder.Record( 10, "test_end" ),

		AtState( "test_end",
            function()
				Trace( "end" )
				Recorder.Save( "output_test.csv" )
                Deactivate()
            end
        )

    }

    DeclareEvents( eventTable )
end
