local result =  
{
    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getEntityToEscort( params )
        return { objective = objective }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Escort"] = t

return result
