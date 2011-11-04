brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/data.lua"

scriptName = "Egypt"

function Start()

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "Supervisor" then return end
                plugin:Later( function() fsm:ChangeState( "Start" ) end )
            end
        },


         {
            events:Once(),
            { "Start" },
            function()
                actions:IssueOrder( "Change population ROE" )
                actions:IssueOrder( "Asymetric - attack population (in an area)" )
                actions:IssueOrder( "Population - move" )
                actions:IssueOrder( "Red - Armor - hold" )
                actions:IssueOrder( "Red - Infantry - hold" )
                actions:IssueOrder( "Blue - Armor - scout" )
                actions:IssueOrder( "Blue - Armor - attack" )
                actions:IssueOrder( "Blue - Armor - control" )
                -- actions:IssueOrder( "Blue - Engineer - perform work" )
                -- sim:ChangeTimeFactor(100)
                -- Display( { message = "XXXX" } )
                plugin:Later( function() fsm:ChangeState( "End" ) end )
            end
        },

    }
    DeclareEvents( eventTable )


end












