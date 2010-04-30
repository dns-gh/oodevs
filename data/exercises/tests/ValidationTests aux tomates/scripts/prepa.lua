brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

scriptName = "prepa"

local Weapon = "Arme par défaut"
local Preparing = "Mission par défaut"

function Start()

    local eventTable =
    {
		{
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "Preparator" then return end
				sim:Pause()
                ChangeState( "Prepa_Exo" )
            end
        },

		AtState( "Prepa_Exo",
				function()
					 local text = "Sur quelle arme voulez-vous travailler ?"
					 Dialog( { id = "Weapon_choice", message = text, buttons = { "ABC", "INF", "Communes", "Toutes !" } } )
				end
		  ),

		  {
				events.client:UserChose(),
				{ },
				function( dialog, answer )
					if dialog == "Weapon_choice" then
						if answer == "ABC" then
							ChangeState( "Prepa_ABC")
						Weapon = "ABC"
						elseif answer == "INF" then
							ChangeState( "Prepa_INF")
						Weapon = "INF"
						elseif answer == "Communes" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[5] ) } )
							Zoom( { width = 35000 } )
							Weapon = "Communes"
							Display( { message = "Préparation de l'exercice" } )
						else
							ChangeState( "Center" )
						end
					end
				end
		  },

		  AtState( "Prepa_ABC",
				function()
					 local text = "Sur quelle case voulez-vous vous centrer ?"
					 Dialog( { id = "ABC_choice", message = text, buttons = { "Case 1", "Case 2", "Case 3", "Case 4", "Case 5", } } )
				end
		  ),

		  {
				events.client:UserChose(),
				{ },
				function( dialog, answer )
					if dialog == "ABC_choice" then
						if answer == "Case 1" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[1] ) } )
						elseif answer == "Case 2" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[2] ) } )
						elseif answer == "Case 3" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[3] ) } )
						elseif answer == "Case 4" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[4] ) } )
						else
							Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[5] ) } )
						end
						--sim:Resume()
						Zoom( { width = 35000 } )
						Preparing = Weapon.." en "..answer
						ChangeState( "Record" )
						--Display( { message = "Préparation de l'exercice pour "..Preparing } )
					end
				end
		  },

		  AtState( "Prepa_INF",
				function()
					 local text = "Sur quelle case voulez-vous vous centrer ?"
					 Dialog( { id = "INF_choice", message = text, buttons = { "Case 1", "Case 2", "Case 3", "Case 4", "Case 5", } } )
				end
		  ),

		  {
				events.client:UserChose(),
				{ },
				function( dialog, answer )
					if dialog == "INF_choice" then
						if answer == "Case 1" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_INF[1] ) } )
						elseif answer == "Case 2" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_INF[2] ) } )
						elseif answer == "Case 3" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_INF[3] ) } )
						elseif answer == "Case 4" then
							Center( { coordinates = coord:UtmPosition( config.positions.center_INF[4] ) } )
						else
							Center( { coordinates = coord:UtmPosition( config.positions.center_INF[5] ) } )
						end
						Zoom( { width = 35000 } )
						Preparing = Weapon.." en "..answer
						ChangeState( "Record")
					end
				end
		  },


			AtState( "Center",
				function()
					Display( { message = "Préparation de l'exercice" } )
				end
			),

			AtState( "Record",
				function()
					sim:Resume()
					local text = "Préparation de l'exercice pour "..Preparing.."\n".."Préparer un nouvel exercice ?"
					PromptNextPhase( { phase = "GoBack", message = text, buttons = { "Nouvel Exercice" } } )
					--Display( { message = "Préparation de l'exercice pour "..Preparing } )
				end
			),

			WaitForUserPhase( "GoBack" ),

			AtState( "GoBack",
				function( client )
					plugin:Reset()
					ChangeState( "Prepa_Exo" )
				end
			),
	}
    DeclareEvents( eventTable )

end
