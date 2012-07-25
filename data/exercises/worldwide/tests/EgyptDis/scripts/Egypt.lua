brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

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
                actions:IssueOrder( "Red - ARMOR - interdict" )
                actions:IssueOrder( "Blue - ART - operate" )
                actions:IssueOrder( "Blue - ARMOR - seize" )
                -- actions:IssueOrder( "Blue - Engineer - perform work" )
                -- sim:ChangeTimeFactor(100)
                -- Display( { message = "XXXX" } )
                plugin:Later( function() fsm:ChangeState( "End" ) end )
            end
        },
        {
            events.sim:AtDateTime("20091126T172000"),
            { },
            function( time )
                actions:IssueOrder( "Blue - ART - Fire 1" )
            end
        },
        {
            events.sim:AtDateTime("20091126T172200"),
            { },
            function( time )
                actions:IssueOrder( "Blue - ART - Fire 2" )
            end
        },
        {
            events.sim:AtDateTime("20091126T172400"),
            { },
            function( time )
                actions:IssueOrder( "Blue - ART - Fire 3" )
            end
        },

    }
    DeclareEvents( eventTable )


end












