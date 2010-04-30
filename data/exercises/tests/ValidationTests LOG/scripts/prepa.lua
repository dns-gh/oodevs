brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

scriptName = "prepa"

local Weapon = "Arme par d�faut"
local Preparing = "Mission par d�faut"

function Start()

    local eventTable =
    {
		{
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "Preparator" then return end
                Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[5] ) } )
				Zoom( { width = 50000 } )
				ChangeState( "Center" )
            end
        },

			AtState( "Center",
				function()
					Display( { message = "Pr�paration de l'exercice" } )
				end
			),

	}
    DeclareEvents( eventTable )

end
