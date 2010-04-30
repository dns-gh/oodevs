brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

scriptName = "Pion ABC Surveiller"

local tick = 0

functionTick = {

[5] = function()
	Select( { target = "id://agent/"..formation[1].automat[2].agent[2].id } )
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[1] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Pion ABC Surveiller 1 - C1" )
end,

[30] = function()
	actions:IssueOrder( "Pion ABC Surveiller 2 - C1" )
end,

[5+2000] = function()
	Select( { target = "id://agent/"..formation[2].automat[2].agent[2].id } )
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[2] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Pion ABC Reconnaitre Axe - C2" )
end,

[5+4000] = function()
	Select( { target = "id://agent/"..formation[3].automat[2].agent[2].id } )
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[3] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Pion ABC Reconnaitre Axe - C3" )
end,

[5+6000] = function()
	Select( { target = "id://agent/"..formation[4].automat[2].agent[2].id } )
	Center( { coordinates = coord:UtmPosition( config.positions.center_ABC[4] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Pion ABC Reconnaitre Axe - C4" )
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
                if profile ~= "Pion ABC Surveiller" then return end
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
				if profile ~= "Pion ABC Surveiller" then
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
