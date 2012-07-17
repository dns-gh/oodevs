local result = 
{
    fillParameters = function( self, companyTask, params )
        return { position = companyTask:getReachable( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Park"] = t

return result
