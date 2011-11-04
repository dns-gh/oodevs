brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/config.lua"

scriptName = "Pion ObserveReconnaitre"

local tick = 0

functionTick =
{
  [5] = function()
    actions:IssueOrder( "MoveAndObserve" )
  end,
  [5+2000] = function()
    actions:IssueOrder( "ObservePoint" )
  end,
  [5+4000] = function()
    actions:IssueOrder( "ObserveUrbanBlock" )
  end,
  [5+6000] = function()
    actions:IssueOrder( "ObserveAndReconnoiterPoint" )
  end,
  [5+8000] = function()
    actions:IssueOrder( "ObserveAndReconnoiterUrbanBlock" )
  end,
  [5+10000] = function()
    actions:IssueOrder( "ReconnoiterPoint" )
  end,
  [5+12000] = function()
    actions:IssueOrder( "ReconnoiterUrbanBlock" )
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
            if profile ~= "Observer et Reconnaitre" then return end
            tick = 0
            local text = "Changer de Mission ?"
            Dialog( { id = "Mission", message = text, buttons = { "ObservePoint", "ObserveUrbanBlock", "ObserveAndReconnoiterPoint", "ObserveAndReconnoiterUrbanBlock", "ReconnoiterPoint", "ReconnoiterUrbanBlock",} } )
            plugin:Later( function() fsm:ChangeState( "Missions_Init" ) end )
          end
      },

      {
        events.sim:TickEnded( ),
        { "Missions_Init" },
        function()
          if profile ~= "Observer et Reconnaitre" then
            runStep()
          end
        end
      },

      {
        events.client:UserChose(),
        { },
        function( dialog, answer )
          if dialog == "Mission" then
            if answer == "ObservePoint" then
              tick = 2000
            elseif answer == "ObserveUrbanBlock" then
              tick = 4000
            elseif answer == "ObserveAndReconnoiterPoint" then
              tick = 6000
            elseif answer == "ObserveAndReconnoiterUrbanBlock" then
              tick = 8000
            elseif answer == "ReconnoiterPoint" then
              tick = 10000
            elseif answer == "ReconnoiterUrbanBlock" then
              tick = 12000
            end
          end
        end
      },
    }
    DeclareEvents( eventTable )

end
