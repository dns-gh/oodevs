brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

scriptName = "Automate ABC Donner Coup Arret"

local tick = 0

functionTick =
{

[5] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[1] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Automate ABC Donner Coup Arret C1" )
end,
[5+2000] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[2] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Automate ABC Donner Coup Arret C2" )
end,

}

local function runStep( )
	if functionTick[tick] then
        functionTick[tick]()
	end
    tick = tick + 1
end

function Start()

    local eventTable =
    {
		{
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "Automate ABC Donner Coup Arret" then return end
				tick = 0
				local text = "Changer de Case ?"
				Dialog( { id = "Case", message = text, buttons = { "Case 1", "Case 2", "Case 3", "Case 4" } } )
				ChangeState( "Missions_Init" )
            end
        },

		{
		events.sim:TickEnded( ),
		{ "Missions_Init" },
			function()
				if profile ~= "Automate ABC Donner Coup Arret" then
					runStep()
				end
			end
		},

		{
			events.client:UserChose(),
			{ },
			function( dialog, answer )
				if dialog == "Case" then
					if answer == "Case 1" then
						tick = 0
					elseif answer == "Case 2" then
						tick = 2000
					elseif answer == "Case 3" then
						tick = 4000
					elseif answer == "Case 4" then
						tick = 6000
					end
				end
			end
		},
	}
    DeclareEvents( eventTable )

end
