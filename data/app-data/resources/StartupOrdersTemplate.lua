brain "brain" {}

dofile "resources/scripts/events.lua"

function Start()

    local eventTable =
    {
        {
            events.sim:TickEnded(),
            {},
            function()
                ChangeState( "setup" )
            end
        },
        {
            events:Once(),
            { "setup" },
            function()
$$$$
            end
        }
    }

    DeclareEvents( eventTable )
end
