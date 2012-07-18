local result = 
{
    fillParameters = function( self, companyTask, objectives )
        return { entity = companyTask:getEntity( params ) }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Interrogate"] = t

return result
