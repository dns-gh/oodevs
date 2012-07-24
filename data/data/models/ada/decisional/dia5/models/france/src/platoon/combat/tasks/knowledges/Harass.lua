local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params )
         local objective = companyTask:getReachable( params )
         return { objective = objective, 
                  destroyingPositions = companyTask:getObserving( params ), 
                  entities = companyTask:getEntities( params ),
                  obstacles = companyTask:getObstaclesPlanForMelee( params ),
                  withImprovement = companyTask:getWithImprovement( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.tasks.knowledges.Harass"] = t

return result
