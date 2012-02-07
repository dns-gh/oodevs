local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getObjective( params )
        return { objective = objective , obstacles = companyTask:getObstaclesPlanForMelee( params ),
                withImprovement = companyTask:getWithImprovement( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Control"] = t

return result
