local result = 
{
    fillParameters = function( self, companyTask, params )
      local objectives = companyTask:getObjectivesGen( params )
      return { objectives = objectives }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.OpenRoute"] = t

return result
