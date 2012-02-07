local result = 
{
    fillParameters = function( self, companyTask, params )
        return { position = companyTask:getReachable( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Park"] = t

return result
