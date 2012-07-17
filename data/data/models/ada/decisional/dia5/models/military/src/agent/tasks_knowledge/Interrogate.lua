local result = 
{
    fillParameters = function( self, companyTask, objectives )
        return { entity = companyTask:getEntity( params ) }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Interrogate"] = t

return result
