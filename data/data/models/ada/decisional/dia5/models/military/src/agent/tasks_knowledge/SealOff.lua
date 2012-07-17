local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getSealOffObjective( params )
        if objective then
            return { objective = objective }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.SealOff"] = t

return result
