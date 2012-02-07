local result = 
{
    fillParameters = function( self, companyTask, params )
        return { objectives = companyTask:getReachable( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.WaitOn"] = t

return result
