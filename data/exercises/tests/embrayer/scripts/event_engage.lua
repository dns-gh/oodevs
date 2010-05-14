brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

scriptName = "event_engage"

local tick = 0

functionTick =
{

[5] = function()
    sim:Engage( 12, true )
end,

[10] = function()
    actions:IssueOrder( "Faire Mouvement" )
end,

[25] = function()
    sim:Engage( 12, false )
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
                if profile ~= "embrayer_debrayer" then return end
                tick = 0
                ChangeState( "Missions_Init" )
            end
        },

        {
        events.sim:TickEnded( ),
        { "Missions_Init" },
            function()
                if profile ~= "embrayer_debrayer" then
                    runStep()
                end
            end
        },
    }
    DeclareEvents( eventTable )

end