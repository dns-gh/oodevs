local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params, entity )
        local position = companyTask:getPosition( params )
           return { position = position }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.tasks.knowledges.OrganiserAccueilColonneRefugies"] = t

return result
