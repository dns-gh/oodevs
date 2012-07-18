local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getReachable( params )
        if objective then
            return { objective = objective }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Seize"] = t

return result
