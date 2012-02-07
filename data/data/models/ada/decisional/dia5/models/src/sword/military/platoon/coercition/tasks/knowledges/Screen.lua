local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,
    
    fillParameters = function( self, companyTask, params, entity )
        local positions, fuseau = companyTask:getScreenPositions( params, entity )
        if next( positions ) then   
            return {positions = positions, fuseau = fuseau }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Screen"] = t

return result
