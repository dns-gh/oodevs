local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getReachable( params )
        local elements = companyTask:getElements( params )
        if next( objective ) and next( elements ) then
            return { position = objective[ 1 ], entities = elements }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.tasks.knowledges.EscorterPrisonniersVersPosition"] = t

return result