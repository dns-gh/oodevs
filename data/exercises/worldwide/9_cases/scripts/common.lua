brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

scriptName = "common"

function Start()

    local eventTable =
    {

    {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
            actions:IssueOrder( "C5 - Red - Hold" )
            -- Briefing( { id = "briefing", file = "resources/briefing.html" } )
            sim:Pause()
            end
        },

        {
            events.sim:ClientLeft(),
            { },
            function( client )
                plugin:Reset()
            end
        }
    }

    DeclareEvents( eventTable )
end