local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params, entity )
        local objectives = companyTask:getObjectives( params )
        local objective = objectives[ next( objectives ) ]
        local positions = companyTask:getPositions( params, entity, objective )
        local obstacles = companyTask:getObstacles( params )
        local withImprovement = companyTask:getWithImprovement( params )
        if next( obstacles ) then
           return { objectives = { objective } , positions = positions, obstacles = obstacles, withImprovement = withImprovement }
        else
           return { objectives = { objective } , positions = positions }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Block"] = t

return result
