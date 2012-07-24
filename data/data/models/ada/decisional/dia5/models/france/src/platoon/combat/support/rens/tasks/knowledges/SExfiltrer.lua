local result = 
{
    fillParameters = function( self, companyTask, params )
         local objective = companyTask:getObjective( params )
         local byNight = companyTask:getByNight( params )
         return { entity = objective, byNight = byNight }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.rens.tasks.knowledges.SExfiltrer"] = t

return result
