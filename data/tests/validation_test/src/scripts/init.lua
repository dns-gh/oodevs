brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

function Start()

    local eventTable =
    {
        {
            events.sim:ClientConnected(),
            { },
            function( client, profile )
                if profile ~= "test1" then return end
                ResetOptions( { client = client } )
                SetDock( { client = client, hide = docks.ALL } )
                SetDock( { show = { "orbat" } } )
                SetDock( { show = { "clock" } } )
                sim:ChangeTimeFactor(75)
                ChangeState( "Q1" )
            end
        },

        AtState( "Q1",
            function()
                Dialog( { id      = "Q1_choice", 
                          message = "Selectionnez un mode de jeu", 
                          buttons = { "Automatique", "manuel" } } )
            end
        ),

        {
            events.client:UserChose(),
            { },
            function( dialog, answer )
                if dialog == "Q1_choice" then
                    if answer == "manuel" then
                        ChangeState( "Q2" )
                    else
                        ChangeState( "Q2" )
                    end

                end
            end
        },

        AtState( "Q2",
            function()
                Dialog( { id      = "Q2_choice",
                          message = "", 
                          buttons = { "Réinitialisation de l'exercice" } } )
            end
        ),
    }
    DeclareEvents( eventTable )


end






    



    
    
