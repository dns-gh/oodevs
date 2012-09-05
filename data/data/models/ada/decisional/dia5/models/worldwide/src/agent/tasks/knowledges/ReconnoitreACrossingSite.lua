local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params, entity )
        local objective = companyTask:getSite( params, entity )
        return { crossingSite = objective }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.ReconnoitreACrossingSite" ] = t

return result