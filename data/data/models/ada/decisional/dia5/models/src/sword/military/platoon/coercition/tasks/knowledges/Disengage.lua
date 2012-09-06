local result = 
{
    fillParameters = function( self, companyTask, params )
        return { objective = companyTask:getReachable( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Disengage"] = t

return result
