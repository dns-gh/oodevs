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
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Defend"] = t

return result
