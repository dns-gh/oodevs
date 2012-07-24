local result = 
{
    fillParameters = function( self, companyTask, params, entity)
        return { position = companyTask:getDeployPositions( params, entity ), typeDeploiement = 0 }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.art.tasks.knowledges.Surveiller"] = t

return result
