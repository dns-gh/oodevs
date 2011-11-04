brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/config.lua"

scriptName = "Pion Follow"

local tick = 0

functionTick =
{

[5] = function()
  actions:IssueOrder( "Follow1" )
end,
[5+10] = function()
  actions:IssueOrder( "Follow2" )
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
                if profile ~= "Follow" then return end
                tick = 0
                plugin:Later( function() fsm:ChangeState( "Missions_Init" ) end )
              end
          },

      {
      events.sim:TickEnded( ),
      { "Missions_Init" },
        function()
          if profile ~= "Follow" then
            runStep()
          end
        end
      },
    }
    DeclareEvents( eventTable )

end
