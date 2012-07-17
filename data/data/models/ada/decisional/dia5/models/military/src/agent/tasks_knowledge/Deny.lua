local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,
    
    fillParameters = function( self, companyTask, params, entity )
        local positions = companyTask:getPositions( params )
        local obstacles = companyTask:getObstaclesPlanForMelee( params )
        local withImprovement = companyTask:getWithImprovement( params )
        if next( obstacles ) then
            return { objectives = params.objectives, threats = params.threats, positions = positions, obstacles = obstacles, withImprovement = withImprovement }
        else
            return { objectives = params.objectives, threats = params.threats, positions = positions }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Deny"] = t

return result
