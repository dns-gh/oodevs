local result = 
{
    fillParameters = function( self, companyTask )
        return { entity = companyTask:getEntity( params ) }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.CollectInformation" ] = t

return result