local result =
{
    fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif, taskName )
        local obstacles = companyTask:getObstaclesPlan( params, entity, taskName )
        if next(obstacles) then
            return { obstacles = obstacles }
        else
            return nil
        end
    end,

    getObstaclesParameter = function( self, companyTask, params )
        return companyTask:getObstaclesParameter( params )
    end,

    hasDotation = function( self, entity, obstacle )
        return true
    end,

    canDoIt = function( self, entity, obstacle )
        return  DEC_Agent_AgentPeutDetruireObjet( entity.source, obstacle.source )
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.engineer.tasks.knowledges.DegagerObstacles"] = t

return result