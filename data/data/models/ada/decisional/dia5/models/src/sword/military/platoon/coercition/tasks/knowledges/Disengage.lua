local result = 
{
    fillParameters = function( self, companyTask, params )
        local reachable = companyTask:getReachable( params )
        if next(reachable) then
            return { objective = reachable[1] }
        else
            return { objective = reachable }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Disengage"] = t

return result
