brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

local isExecuted = false

function Start()

 	local eventTable =
	{

        AtState( "start",
			function()
			end
		),

        {
            events.sim:TickEnded(),
            {},
            function( tick, duration )
				if not isExecuted then
					actions:StartScheduler("orders")
					isExecuted = true
				end
            end
        },
	}
	DeclareEvents( eventTable )
end