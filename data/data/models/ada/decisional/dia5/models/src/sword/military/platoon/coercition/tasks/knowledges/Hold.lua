local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params )
        local objectives = companyTask:getObjectives( params )
        local obstacles = companyTask:getObstaclesPlanForMelee( params )
        local withImprovement = companyTask:getWithImprovement( params )
        return { objectives = objectives, obstacles = obstacles, withImprovement = withImprovement }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Hold"] = t

return result
