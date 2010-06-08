brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

scriptName = "init"

local tick = 0

function Start()

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                Zoom( { width = 10000 } )
                Center( { coordinates = coord:UtmPosition( "31UEQ0943923538" ) } )
            end
        },
    }
    DeclareEvents( eventTable )

end