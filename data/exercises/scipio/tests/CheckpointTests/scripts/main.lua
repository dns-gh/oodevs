brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
dofile "resources/scripts/actions.lua"

scriptName = "main"

function Start()

    local eventTable =
    {
        {
            events.sim:TickEnded(),
            { "start" },
            function( tick, duration )
                if tick == 10 then
                    plugin:Later( function() fsm:ChangeState( "orders" ) end )
                end
            end
        },

        {
            AtState( "orders",
                function()
                    actions:IssueOrder( "ASA Defendre Site" )
                    actions:IssueOrder( "Stationner" )
                    actions:IssueOrder( "Suivre" )
                    actions:IssueOrder( "Faire Mouvement" )
                    actions:IssueOrder( "NBC Reconnaitre Une Zone" )
                    actions:IssueOrder( "INF Eclairer" )
                    actions:IssueOrder( "LOG Se Deployer" )
                    actions:IssueOrder( "MELEE S'emparer De" )
                    actions:IssueOrder( "MELEE Reconnaitre" )
                    actions:IssueOrder( "GENIE Realiser Travaux" )
                    actions:IssueOrder( "ASY Commettre un Attentat Suicide" )
                    actions:IssueOrder( "ASY Commettre un Attentat" )
                    actions:IssueOrder( "ASS Assurer Mise En Oeuvre Sur Position" )
                end
            )
        }
    }
    DeclareEvents( eventTable )

end
