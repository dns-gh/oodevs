local result =
{
    fillParameters = function( self, companyTask, params, entity, retrogradeContext, objectif, taskName )
        local obstacles = companyTask:getObstaclesPlan( params, entity, taskName )
        local meetingPoint = companyTask:getFinalMeetingPoint( params )
        if next( obstacles ) then
            return { objects = obstacles, meetingPosition = meetingPoint }
        else
            return nil
        end
    end,

    getObstaclesParameter = function( self, companyTask, params )
        return companyTask:getObstaclesParameter( params )
    end,

    hasDotation = function( self, entity, obstacle )
        return true -- no dotation needed
    end,

    canDoIt = function( self, entity, obstacle )
        return  true -- all agents of the simulation can activate obstacles.
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.Activate" ] = t

return result