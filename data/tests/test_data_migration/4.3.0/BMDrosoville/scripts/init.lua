brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/config.lua"

scriptName = "init"



function Start()

   local eventTable =
   {
      {
      events.sim:ClientConnected(),
      { },
        function( client, profile )
      --[[  SetDock( { client = client, hide = docks.ALL } ) -- hide all docks
        SetDock( { show = { "orbat" } } )
        SetDock( { show = { "actions" } } )
        SetDock( { show = { "info" } } )
        SetDock( { show = { "clock" } } )
        ChangeOptions( {   TacticalLines     = options.fourstate.on,
                  OldPaths       = options.fourstate.on,
                  Paths             = options.fourstate.on,
                  MissionParameters   = options.fourstate.on } )]]
        Zoom( { width = 8000 } )
        Center( { coordinates = coord:UtmPosition( config.positions.center_All[1] ) } )
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
