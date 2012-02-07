local result = 
{
    fillParameters = function( self, companyTask, params )
        local positions = companyTask:getReachable( params )
        if next( positions ) then
            return { objective = positions[1] }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Disengage"] = t

return result
