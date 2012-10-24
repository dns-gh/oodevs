local result = 
{
     fillParameters = function( self, companyTask, params, entity, retrogradeContext, objectif, taskName )
        local obstacles = companyTask:getObstaclesPlan( params, entity, taskName )
        local index = next( obstacles )
        local meetingPoint = companyTask:getFinalMeetingPoint( params )
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
        return  integration.canBuildBypassingObject( entity.source, obstacle.source )
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.engineer.tasks.knowledges.ContournerObstacles"] = t

return result