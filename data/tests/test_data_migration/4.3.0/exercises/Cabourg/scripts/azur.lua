brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/actions.lua"

scriptName = "azur"

function Start()

    local enemyCount = 0
    local requiredEnemyCount = 3;

    local unitEnteredCountBeforeSupport = 5
    local unitEntered = {}

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "" then return end
                sim:Pause()
                Briefing( { id = "briefing", file = "resources/briefing.html" } )
                plugin:Later( function() fsm:ChangeState( "P1 start" ) end )
            end
        },

        {
            events:Once(),
            { "P1 start" },
            function()
                sim:Resume()
                actions:IssueOrder( "Azur P1 - Scout" )
                Display( { message = "Phase 1, temps 1 : le 110 EEI mène une mission d'éclairage." } )
                plugin:Later( function() fsm:ChangeState( "P1 scout" ) end )
            end
        },

        {
            events.agents:KnowledgeCreated(),
            { "P1 scout" },
            function( k, a )
              if a:GetTeam() == "VERMILLON" and k:GetOwnerTeam() == "AZUR" then
                enemyCount = enemyCount + 1
                if enemyCount >= requiredEnemyCount then
                  plugin:Later( function() fsm:ChangeState( "P1 raid choice" ) end )
                end
              end
            end
        },

        AtState( "P1 raid choice",
            function()
                local text = "Phase 1, temps 2 : le 111 RB et le 112 RC effectuent un raid.\n"
                          .. "Souhaitez-vous automatiser la gestion du 111 RB ?";
                sim:Pause()
                Dialog( { id = "P1_raid_choice", message = text, buttons = { "Oui", "Non" } } )
            end
        ),

        {
            events.client:UserChose(),
            { },
            function( dialog, answer )
                if dialog == "P1_raid_choice" then
                    sim:Resume()
                    if answer == "Oui" then
                        actions:IssueOrder( "Azur P1 - Raid - 111 auto" )
                        Display( { message = "Phase 1, temps 2 : le 111 RB et le 112 RC effectuent un raid." } )
                    else
                        Display( { message = "Phase 1, temps 2 : le 112 RC effectue un raid." } )
                    end
                    actions:IssueOrder( "Azur P1 - Raid" )
                    plugin:Later( function() fsm:ChangeState( "P1 wait support" ) end )
                end
            end
        },

        {
            events.agents:AgentEnters( drawings:Create( "Zone P1 - support trigger" ):GetCoordinates() ),
            { "P1 wait support" },
            function( a )
                if a:GetTeam() == "AZUR" then
                    unitEntered[ a:GetIdentifier() ] = true
                    local i = 0; for k,v in pairs( unitEntered ) do i = i + 1 end
                    if i >= unitEnteredCountBeforeSupport then
                        plugin:Later( function() fsm:ChangeState( "P1 support" ) end )
                    end
                end
            end
        },

        AtState( "P1 support",
          function()
            unitEntered = {}
            unitEnteredCountBeforeSupport = 20
            actions:IssueOrder( "Azur P1 - Support" )
            Display( { message = "Phase 1, temps 3 : le 113 RIMECA et le 114 RIMOTO appuient le 111 RB et le 112 RC." } )
            plugin:Later( function() fsm:ChangeState( "P1 wait P2" ) end )
          end
        ),

        {
            events.agents:AgentEnters( drawings:Create( "Zone P1 - P2 trigger" ):GetCoordinates() ),
            { "P1 wait P2" },
            function( a )
                if a:GetTeam() == "AZUR" then
                    unitEntered[ a:GetIdentifier() ] = true
                    local i = 0; for k,v in pairs( unitEntered ) do i = i + 1 end
                    if i >= unitEnteredCountBeforeSupport then
                        plugin:Later( function() fsm:ChangeState( "P2 start" ) end )
                    end
                end
            end
        },

        AtState( "P2 start",
          function()
            enemyCount = 0
            actions:IssueOrder( "Azur P2 - Scout" )
            Display( { message = "Phase 2, temps 1 : le 110 EEI mènent une nouvelle mission d'éclairage." } )
            plugin:Later( function() fsm:ChangeState( "P2 scout" ) end )
          end
        ),

        {
            events.agents:KnowledgeCreated(),
            { "P2 scout" },
            function( k, a )
              if a:GetTeam() == "VERMILLON" and k:GetOwnerTeam() == "AZUR" then
                enemyCount = enemyCount + 1
                if enemyCount >= requiredEnemyCount then
                  plugin:Later( function() fsm:ChangeState( "P2 attack choice" ) end )
                end
              end
            end
        },

        AtState( "P2 attack choice",
            function()
                local text = "Phase 2, temps 2 : le 111 RB et le 112 RC effectuent une attaque.\n"
                          .. "Souhaitez-vous automatiser la gestion du 111 RB ?";
                sim:Pause()
                Dialog( { id = "P2_attack_choice", message = text, buttons = { "Oui", "Non" } } )
            end
        ),

        {
            events.client:UserChose(),
            { },
            function( dialog, answer )
                if dialog == "P2_attack_choice" then
                    sim:Resume()
                    if answer == "Oui" then
                        actions:IssueOrder( "Azur P2 - Attack - 111 auto" )
                        Display( { message = "Phase 2, temps 2 : le 111 RB et le 112 RC mènent une attaque." } )
                    else
                        Display( { message = "Phase 2, temps 2 : le 112 RC mène une attaque." } )
                    end

                    actions:IssueOrder( "Azur P2 - Attack" )
                    plugin:Later( function() fsm:Deactivate() end )
                end
            end
        }

    }
    DeclareEvents( eventTable )

end

