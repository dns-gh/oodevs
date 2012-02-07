local result = 
{
    fillParameters = function( self, companyTask, params )
      local hostage = companyTask:getHostage( params )
      local position = companyTask:getPosition( params )
      return { hostage = hostage, position = position }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.individual.tasks.knowledges.TakeHostage"] = t

return result
