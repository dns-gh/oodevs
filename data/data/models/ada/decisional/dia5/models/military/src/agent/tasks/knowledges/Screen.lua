local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,
    
    fillParameters = function( self, companyTask, params, entity )
        local positions = companyTask:getScreenPositions( params, entity )
        if next( positions ) then   
            return { positions = positions }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Screen"] = t

return result
