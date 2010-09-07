brain "brain" {}

require "io"
require "table"

dofile "resources/scripts/actions.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/indicators.lua"
include "resources/model.lua"
include "resources/config.lua"

function Start()

    local frequency = 1 -- second(s) between mission start
    local unit = nil

    local eventTable =
    {
        {
            events:Once(),
            { },
            function( client, profile )
                ChangeState( "init_startup" )
            end
        },

        {
            events.sim:TickEnded(),
            { "init_startup" },
            function()
                StartTimeSequence( "init_messages" )
            end
        },

        TimeSequence( "init_messages", frequency,
            function()
                Mission.create( 9, model.types.missions["Suivre"] )
                    :With( { name = "Danger direction", type = "Direction", value = 0 } )
                    :With( { name = "Unite a suivre", type = "Agent", value = config.id.redUnit } )
                    :Issue()
                Trace( "init : running Suivre" )
            end
        ),

        TimeSequence( "init_messages", frequency,
            function()
                ChangeState( "init_test_end" )
            end
        ),
     
        AtState( "init_test_end",
            function()
                Deactivate()
            end
        ),
    }

    DeclareEvents( eventTable )
end
