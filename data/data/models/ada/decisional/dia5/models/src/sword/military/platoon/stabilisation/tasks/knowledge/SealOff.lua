local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getSealOffObjective( params )
        local position = companyTask:getSealOffPosition( params )
        if objective then
            return { objective = objective, position = position }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.stabilisation.tasks.knowledge.SealOff"] = t

return result
