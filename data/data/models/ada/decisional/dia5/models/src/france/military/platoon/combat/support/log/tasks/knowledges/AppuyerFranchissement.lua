local result = 
{
    fillParameters = function( self, companyTask, params )
        local objectif = companyTask:getPosition( params )
        return { objectif = objectif }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.log.tasks.knowledges.AppuyerFranchissement"] = t

return result