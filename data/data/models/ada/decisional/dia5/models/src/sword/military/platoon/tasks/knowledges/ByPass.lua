local result = 
{
    fillParameters = function( self, companyTask, params )
        local obstacles = companyTask:getObstaclesPlan( params )
        local index = next( obstacles )
        local meetingPoint = companyTask:getMeetingPoint( params )
        if index then
            return { obstacles = obstacles, meetingPoint = meetingPoint }
        else 
            error( "No objective for task "..tostring(self.name).." in company task "..tostring(companyTask.name) )
        return nil
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.ByPass"] = t

return result
