brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/config.lua"

scriptName = "Pion Detruire"

local tick = 0

functionTick =
{

[5] = function()
  actions:IssueOrder( "SeekAndDestroy" )
end,
--[[[5+2000] = function()
  actions:IssueOrder( "ReconnoiterUrbanBlock" )
end,]]
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
                if profile ~= "Detruire" then return end
                tick = 0
                --[[local text = "Changer de Mission ?"
                Dialog( { id = "Mission", message = text, buttons = { "Destroy$$$", } } )
                ChangeState( "Missions_Init" )]]
                plugin:Later( function() fsm:ChangeState( "Missions_Init" ) end )

            end
      },

      {
      events.sim:TickEnded( ),
      { "Missions_Init" },
        function()
          if profile ~= "Detruire" then
            runStep()
          end
        end
      },

      --[[{
        events.client:UserChose(),
        { },
        function( dialog, answer )
          if dialog == "Mission" then
            if answer == "Destroy$$$" then
              tick = 0
            end
          end
        end
      },
      ]]
    }
    DeclareEvents( eventTable )

end
