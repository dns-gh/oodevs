local result = 
{
    fillParameters = function( self, companyTask, params, entity )
        return { crossingSite = companyTask:getCrossingSite( params ), 
                 meetingPoint = companyTask:getReachable( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.Cross" ] = t

return result