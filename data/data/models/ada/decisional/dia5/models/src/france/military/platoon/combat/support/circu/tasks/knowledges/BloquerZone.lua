local result = 
{
    fillParameters = function( self, companyTask, params )
        local zone = companyTask:getZone( params )
         return { zone = zone }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.circu.tasks.knowledges.BloquerZone"] = t

return result
