local result = 
{
    fillParameters = function( self, companyTask, params )
        return { objective = companyTask:getObjective( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "platoon.tasks.knowledges.Deploy" ] = t

return result
