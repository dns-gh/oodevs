local result = 
{
    fillParameters = function( self, companyTask, params )
         local objective = companyTask:getObjective( params )
         return { unit = objective }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.rens.tasks.knowledges.OrienterGuider"] = t

return result
