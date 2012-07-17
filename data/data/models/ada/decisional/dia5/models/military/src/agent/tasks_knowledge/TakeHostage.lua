local result = 
{
    fillParameters = function( self, companyTask, params )
      local hostage = companyTask:getHostage( params )
      local position = companyTask:getPosition( params )
      return { hostage = hostage, position = position }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.TakeHostage"] = t

return result
