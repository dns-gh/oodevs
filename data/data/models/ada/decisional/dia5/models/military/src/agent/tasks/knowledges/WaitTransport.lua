local result = 
{
    fillParameters = function( self, companyTask, params )
        return { position = companyTask:getEmbarkPoint( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.WaitTransport"] = t

return result
