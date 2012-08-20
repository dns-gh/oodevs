local result = 
{
    fillParameters = function( self, companyTask, params )
        return { position = companyTask:getEmbarkPoint( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "platoon.tasks.knowledges.WaitTransport" ] = t

return result
