local result = 
{
    fillParameters = function( self, companyTask, params )
        return { objectives = companyTask:getReachable( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.WaitOn"] = t

return result
