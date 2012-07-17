local result =
{
    fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif, taskName )
        local obstacles = companyTask:getObstaclesPlan( params, entity, taskName )
        local meetingPoint = companyTask:getFinalMeetingPoint( params )
        if next(obstacles) then
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
        return  DEC_Agent_AgentPeutDetruireObjet( entity.source, obstacle.source )
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.ClearObstacles"] = t

return result