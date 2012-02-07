local result = 
{
    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getReachable( params )
        local crossingSite = companyTask:getCrossingSite( params )
        return { meetingPoint = objective, crossingSite = crossingSite }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.PassThrough"] = t

return result
