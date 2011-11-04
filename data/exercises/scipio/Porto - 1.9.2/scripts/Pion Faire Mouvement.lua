brain "brain" {}

dofile "resources/scripts/commands.lua"
dofile "resources/scripts/events.lua"
include "resources/config.lua"

scriptName = "Pion Faire Mouvement"

local tick = 0

functionTick =
{

[5] = function()
  actions:IssueOrder( "MoveToPoint" )
end,
[5+2000] = function()
  actions:IssueOrder( "MoveToUrbanBlock" )
end,
[5+4000] = function()
  actions:IssueOrder( "MoveToAgent" )
end,
[5+6000] = function()
  actions:IssueOrder( "MoveToAutomat" )
end,
[5+8000] = function()
  actions:IssueOrder( "MoveToPolygon" )
end,
--[[[5+10000] = function()
  actions:IssueOrder( "MoveToAgenKnowledge" )
end,
[5+12000] = function()
  actions:IssueOrder( "MoveToObjectKnowledge" )
end,]]
[5+14000] = function()
  actions:IssueOrder( "MoveToPointList" )
end,
[5+16000] = function()
  actions:IssueOrder( "MoveToPolygonList" )
end,
[5+18000] = function()
  actions:IssueOrder( "MoveToAgentList" )
end,
[5+20000] = function()
  actions:IssueOrder( "MoveToAutomatList" )
end,
--[[[5+22000] = function()
  actions:IssueOrder( "MoveToAgenKnowledgeList" )
end,
[5+24000] = function()
  actions:IssueOrder( "MoveToObjectKnowledgeList" )
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
                if profile ~= "Faire Mouvement" then return end
        tick = 0
        local text = "Changer de Mission ?"
        Dialog( { id = "Mission", message = text, buttons = { "MoveToUrbanBlock", "MoveToAgent", "MoveToAutomat", "MoveToPolygon", --[["MoveToAgenKnowledge", "MoveToObjectKnowledge", ]]"MoveToPointList", "MoveToPolygonList", "MoveToAgentList", "MoveToAutomatList", --[["MoveToAgenKnowledgeList", "MoveToObjectKnowledgeList"]]} } )
        --ChangeState( "Missions_Init" )
        plugin:Later( function() fsm:ChangeState( "Missions_Init" ) end )
            end
        },

    {
    events.sim:TickEnded( ),
    { "Missions_Init" },
      function()
        if profile ~= "Faire Mouvement" then
          runStep()
        end
      end
    },

    {
      events.client:UserChose(),
      { },
      function( dialog, answer )
        if dialog == "Mission" then
          if answer == "MoveToUrbanBlock" then
            tick = 2000
          elseif answer == "MoveToAgent" then
            tick = 4000
          elseif answer == "MoveToAutomat" then
            tick = 6000
          elseif answer == "MoveToPolygon" then
            tick = 8000
          --[[elseif answer == "MoveToAgenKnowledge" then
            tick = 10000
          elseif answer == "MoveToObjectKnowledge" then
            tick = 12000]]
          elseif answer == "MoveToPointList" then
            tick = 14000
          elseif answer == "MoveToPolygonList" then
            tick = 16000
          elseif answer == "MoveToAgentList" then
            tick = 18000
          elseif answer == "MoveToAutomatList" then
            tick = 20000
          --[[elseif answer == "MoveToAgenKnowledgeList" then
            tick = 22000
          elseif answer == "MoveToObjectKnowledgeList" then
            tick = 24000]]
          end
        end
      end
    },
  }
    DeclareEvents( eventTable )

end
