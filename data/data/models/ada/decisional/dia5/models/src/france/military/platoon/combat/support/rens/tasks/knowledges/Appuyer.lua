local result = 
{
    fillParameters = function( self, companyTask, params )
         local objective = companyTask:getObjective( params )
         local meetingPoint = companyTask:getMeetingPoint( params )
         local jammingTime = companyTask:getJammingTime( params )
         return { objective = objective, meetingPoint = meetingPoint, jammingTime = jammingTime }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.rens.tasks.knowledges.Appuyer"] = t

return result
