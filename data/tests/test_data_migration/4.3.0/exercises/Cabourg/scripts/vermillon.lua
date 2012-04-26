brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/actions.lua"

scriptName = "vermillon"

function Start()

    local enemyCount = 0
    local requiredEnemyCount = 10;

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "" then return end
                plugin:Later( function() fsm:ChangeState( "P1 start" ) end )
            end
        },

        {
            events:Once(),
            { "P1 start" },
            function()
                actions:IssueOrder( "Vermillon P1 - Hold" )
                plugin:Later( function() fsm:ChangeState( "P1 wait delay" ) end )
            end
        },

        {
            events.agents:KnowledgeCreated(),
            { "P1 wait delay" },
            function( k, a )
              if a:GetTeam() == "AZUR" and k:GetOwnerTeam() == "VERMILLON" then
                enemyCount = enemyCount + 1
                if enemyCount >= requiredEnemyCount then
                  plugin:Later( function() fsm:ChangeState( "P2 delay" ) end )
                end
              end
            end
        },

        AtState( "P2 delay",
          function()
            actions:IssueOrder( "Vermillon P2 - Delay" )
            plugin:Later( function() fsm:Deactivate() end )
          end
        )

    }
    DeclareEvents( eventTable )

end












