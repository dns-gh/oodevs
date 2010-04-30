brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

scriptName = "Pion ABC Couvrir"

local tick = 0

functionTick = {
[5] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[1] ) } )
	Zoom( { width = 35000 } )
    sim:Engage(formation[1].automat[3].id,true) --61
    actions:IssueOrder( "Pion ABC Couvrir C1" )
end,
[5+2000] = function()
    plugin:Later( function() sim:Engage(91,true) end )
    plugin:Later( function() sim:Engage(67,true) end )
end,
[6+2000] = function()
    sim:Engage(formation[6].automat[1].id,true) --91
    sim:Engage(formation[2].automat[3].id,true) --67
    Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[2] ) } )
	actions:IssueOrder( "Pion ABC Couvrir C2" )
end,
[5+4000] = function()
    Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[3] ) } )
    sim:Engage(formation[7].automat[1].id,true) --97
    sim:Engage(formation[3].automat[3].id,true) --73
    actions:IssueOrder( "Pion ABC Couvrir C3" )
end,
[5+6000] = function()
    actions:IssueOrder( "Pion ABC Couvrir C3 ami" )
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
                if profile ~= "Pion ABC Couvrir" then return end
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
				if profile ~= "Pion ABC Couvrir" then
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
