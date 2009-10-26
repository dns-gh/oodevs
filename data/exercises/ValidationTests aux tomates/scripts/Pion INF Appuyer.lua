brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/model.lua"
include "resources/config.lua"

scriptName = "Pion INF Appuyer"

local tick = 0

functionTick =
{

[5] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_INF[1] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Mission Appuyer INF Eclairer C1" )
	actions:IssueOrder( "Mission Appuyer INF Appuyer C1" )
end,
[5+2000] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_INF[2] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Mission Appuyer INF Eclairer C2" )
	actions:IssueOrder( "Mission Appuyer INF Appuyer C2" )
end,

[5+4000] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_INF[3] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Mission Appuyer INF Eclairer C3" )
	actions:IssueOrder( "Mission Appuyer INF Appuyer C3" )
end,

[5+6000] = function()
	Center( { coordinates = coord:UtmPosition( config.positions.center_INF[4] ) } )
	Zoom( { width = 35000 } )
	actions:IssueOrder( "Mission Appuyer INF Eclairer C4" )
	actions:IssueOrder( "Mission Appuyer INF Appuyer C4" )
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
                if profile ~= "Pion INF Appuyer" then return end
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
				if profile ~= "Pion INF Appuyer" then
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
