brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/config.lua"

scriptName = "actionOnEvent"

function Start()
    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "Evenement" then return end
                ChangeState( "Waiting" )
            end
        },
        {
            events:Once(),
            { "Waiting" },
            function()
                Center( { coordinates = coord:UtmPosition( config.positions.start[1] ) } )
                Zoom( { width = 30000 } )
                Select( { target = "id://agent/" .. config.id.blueUnit } )
                ChangeState( "ApplyMagic" )
            end
        },
        {
            events.agents:AgentEnters( drawings:Create( "Area1" ):GetCoordinates() ),
            { "ApplyMagic" },
            function( )
                Trace( "step ApplyMagic" )
                    actions:IssueOrder( "Magic" )
            end
        },
    }
    DeclareEvents( eventTable )
end