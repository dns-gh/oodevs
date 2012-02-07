local result = 
{
    fillParameters = function( self, companyTask, params )
         return {}
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.rens.tasks.knowledges.ROIM_MettreEnOeuvre"] = t

return result
