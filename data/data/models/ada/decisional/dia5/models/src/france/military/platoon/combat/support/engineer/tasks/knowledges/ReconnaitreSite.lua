local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params, entity )
        local objective = companyTask:getSite( params, entity )
        return { objective = objective }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.engineer.tasks.knowledges.ReconnaitreSite"] = t

return result
