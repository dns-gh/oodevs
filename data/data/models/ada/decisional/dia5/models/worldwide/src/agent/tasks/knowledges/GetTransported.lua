local result = 
{
    fillParameters = function( self, companyTask, params )
        return { position = companyTask:getEmbarkationPosition( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.GetTransported" ] = t

return result