local result = 
{
    fillParameters = function( self, companyTask, params )
         local objective = companyTask:getObjective( params )
         local meetingPoint = companyTask:getMeetingPoint( params )
         return { objective = objective, meetingPoint = meetingPoint }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.rens.tasks.knowledges.Appuyer"] = t

return result
