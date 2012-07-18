local result = 
{
    fillParameters = function( self, companyTask, params )
        return { objective = companyTask:getObjective( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Deploy"] = t

return result
