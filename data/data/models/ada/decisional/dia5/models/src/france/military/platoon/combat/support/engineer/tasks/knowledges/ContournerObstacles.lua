local result = 
{
     fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif, taskName )
        local obstacles = companyTask:getObstaclesPlan( params, entity, taskName )
        local index = next( obstacles )
        local meetingPoint = companyTask:getMeetingPoint( params )
        if index then
            return { obstacles = obstacles, meetingPoint = meetingPoint }
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
        return  DEC_Agent_AgentPeutConstruireContournementObjet( entity.source, obstacle.source )
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.engineer.tasks.knowledges.ContournerObstacles"] = t

return result