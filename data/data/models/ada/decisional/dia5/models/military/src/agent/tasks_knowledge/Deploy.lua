local result = 
{
    fillParameters = function( self, companyTask, params )
        return { objective = companyTask:getObjective( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Deploy"] = t

return result
