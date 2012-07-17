local result = 
{
    fillParameters = function( self, companyTask, params )
      local objectives = companyTask:getObjectivesGen( params )
      return { objectives = objectives }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.OpenRoute"] = t

return result
