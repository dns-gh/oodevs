brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/config.lua"

scriptName = "Pion Load"

local tick = 0

functionTick =
{

[5] = function()
  actions:IssueOrder( "Load" )
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
                if profile ~= "Load" then return end
                tick = 0
                plugin:Later( function() fsm:ChangeState( "Missions_Init" ) end )
              end
          },

      {
      events.sim:TickEnded( ),
      { "Missions_Init" },
        function()
          if profile ~= "Load" then
            runStep()
          end
        end
      },
    }
    DeclareEvents( eventTable )

end
