local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCommon
    end,

    fillParameters = function( self, companyTask, params, entity )
        local meetingPoint = companyTask:getMeetingPoint( params )
        local objective = companyTask:getObjective( params )
        return { meetingPoint = meetingPoint, objective = objective }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.tasks.knowledges.ReparerReseau"] = t

return result
