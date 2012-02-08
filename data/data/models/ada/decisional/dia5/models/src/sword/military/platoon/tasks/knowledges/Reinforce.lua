local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyEngineeringSupport
    end,

    fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif )
        return { objective = companyTask:getEntityToReinforce( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Reinforce"] = t

return result
