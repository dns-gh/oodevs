local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params, entity )
         local objectives = companyTask:getObjectives( params )
         local obstacles = companyTask:getObstaclesPlanForMelee( params )
         local withImprovement = companyTask:getWithImprovement( params )
         if next( obstacles ) then
            return { objectives =  objectives , obstacles = obstacles, withImprovement = withImprovement }
         else
            return { objectives =  objectives }
         end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Defend"] = t

return result
