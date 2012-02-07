local result = 
{
    fillParameters = function( self, companyTask, params )
         local objective = companyTask:getObjective( params )
         return { entity = objective }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.rens.tasks.knowledges.SExfiltrer"] = t

return result
