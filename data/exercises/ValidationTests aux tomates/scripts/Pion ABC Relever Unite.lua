brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

scriptName = "Pion ABC Relever unit�"

local tick = 0

functionTick = {
[5] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[1] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Mission ReleverUnite ABC Attaquer C1" )
end,
[50] = function()
	actions:IssueOrder( "Mission ReleverUnite ABC Relever Unite C1" )
end,
[2000+5] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[2] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Mission ReleverUnite ABC Reconnaitre Axe C2" )
end,
[2000+50+5] = function()
	actions:IssueOrder( "Mission ReleverUnite ABC Relever Unite C2" )
end,
[4000+5] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[3] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Mission ReleverUnite ABC Semparer Zone C3" )
end,
[4000+50+5] = function()
	actions:IssueOrder( "Mission ReleverUnite ABC Relever Unite C3" )
end,
[6000+5] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[4] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Mission ReleverUnite ABC Controler Zone C4" )
end,
[6000+50+5] = function()
	actions:IssueOrder( "Mission ReleverUnite ABC Relever Unite C4" )
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
                if profile ~= "Pion ABC Relever unit�" then return end
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
				if profile ~= "Pion ABC Relever unit�" then
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
