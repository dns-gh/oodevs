brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

scriptName = "common"

function Start()

    local eventTable =
    {
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