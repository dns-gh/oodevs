brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/actions.lua"
dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/indicators.lua"
include "resources/model.lua"
include "resources/config.lua"

function Start()

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "mounted" then return end
                ChangeState( "mounted_startup" )
            end
        },

        {
            events.sim:TickEnded(),
            { "mounted_startup" },
            function()
                Trace( "mounted_startup" )
                ChangeState( "mounted_test_begin" )
            end
        },

        {
            events:Once(),
            { "mounted_test_begin" },
            function()
                Trace( "mounted_begin" )
                sim:CreateUnit( coord:UtmPosition( config.positions.start[1] )
                              , model.types.units["MBT platoon"]
                              , model.entities.automats["ABC"] )
                ChangeState( "mounted_test_mission" )
            end
        },

        {
            events.agents:AgentCreated(),
            { "mounted_test_mission" },
            function( entity )
                Trace( "mounted_mission" )
                Mission.create( entity:GetIdentifier(), model.types.missions["move"] )
                        :With( { name = "Danger direction", type = "Direction", value = 0 } )
                        :With( Path.create( "Route" ):AddPoint( "Destination", config.positions.destination[1] ) )
                        :Issue()
                ChangeState( "mounted_test_mountedstatechanged" )
            end
        },
        
        {
            events.agents:MountedStateChanged(),
            { "mounted_test_mountedstatechanged" },
            function( entity )
                if entity:GetIdentifier() == config.id.redUnit then
                    Display( { message = "Entity " .. tostring( entity:GetIdentifier() ) .. " mounting state is now: " .. tostring( entity:IsMounted() ) } )
                    ChangeState( "mounted_test_end" )
                end
            end
        },

        AtState( "mounted_test_end",
            function()
                Trace( "mounted_end" )
                Deactivate()
            end
        )
        
    }

    DeclareEvents( eventTable )
end
