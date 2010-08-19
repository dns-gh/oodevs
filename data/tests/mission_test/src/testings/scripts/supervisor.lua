brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/actions.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/indicators.lua"
include "resources/model.lua"
include "resources/config.lua"

function Start()

    local currentTick = 0

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                Display( { message = "in event client connected" } )
                if profile ~= "supervisor" then return end
                ChangeState( "supervisor_startup" )
            end
        },

        {
            events.sim:TickEnded(),
            { "supervisor_startup" },
            function()
                Display( { message = "In supervisor starup tick: " .. tostring( currentTick ) } )
                currentTick = currentTick + 1
                if currentTick > 19 then
                    ChangeState( "supervisor_test_end" )
                end
            end
        },
        
         AtState( "supervisor_test_end",
            function()
                Deactivate()
            end
        )

        
    }

    DeclareEvents( eventTable )
end
  