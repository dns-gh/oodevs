brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

scriptName = "init"



function Start()

	 local eventTable =
	 {
		  {
			events.sim:ClientConnected(),
			{ },
				function( client, profile )
				SetDock( { show = { "orbat" } } )
				SetDock( { show = { "actions" } } )
				SetDock( { show = { "info" } } )
				SetDock( { show = { "clock" } } )
				ChangeOptions( { 	TacticalLines 		= options.fourstate.on,
									OldPaths 			= options.fourstate.on,
									Paths           	= options.fourstate.on,
									MissionParameters 	= options.fourstate.on } )
				Zoom( { width = 175000 } )
				Center( { coordinates = coord:UtmPosition( config.positions.center_All[1] ) } )
				sim:ChangeTimeFactor(30)
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
