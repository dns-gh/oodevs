brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"

scriptName = "init"


function Start()

     local eventTable =
     {
          {
            events.sim:ClientConnected(),
            { },
                function( client, profile )
                Zoom( { width = 10000 } )
                Center( { coordinates = coord:UtmPosition( "30UYV1564467060" ) } )
                sim:ChangeTimeFactor(30)
            end
          },

          {
            events.sim:ClientLeft(),
            { },
            function( client )
                plugin:Reset()
            end
        }
    }
    DeclareEvents( eventTable )

end
