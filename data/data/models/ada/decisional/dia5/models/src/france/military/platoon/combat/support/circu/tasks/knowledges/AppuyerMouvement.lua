local result = 
{
    fillParameters = function( self, companyTask, params )
        local entity = companyTask:getEntity( params )
         return { entity = entity }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.circu.tasks.knowledges.AppuyerMouvement"] = t

return result