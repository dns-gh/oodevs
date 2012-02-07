local result = 
{
    fillParameters = function( self, companyTask, params, entity)
        return { zone = companyTask:getReachable( params, entity ),position = companyTask:getFirePositions( params, entity ), typeDeploiement = 0 }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.art.tasks.knowledges.Defendre"] = t

return result
