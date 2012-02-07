local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCommon
    end,

    fillParameters = function( self, companyTask, params )
        local objectives = companyTask:getEntitiesToSupport( params )
        if next( objectives ) then
             return { objective = objectives[next( objectives )] }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Follow"] = t

return result
