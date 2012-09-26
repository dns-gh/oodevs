local result = 
{
    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getObject( params )
        return { object = objective }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.Operate" ] = t

return result