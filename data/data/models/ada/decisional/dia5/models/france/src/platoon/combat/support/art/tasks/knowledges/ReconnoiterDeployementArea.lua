local result = 
{
    fillParameters = function( self, companyTask, params, entity)
        return { objectives = companyTask:getObjective( params, entity ), meetingPoint = companyTask:getMeetingPoint( params )[1] }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.art.tasks.knowledges.ReconnoiterDeployementArea"] = t

return result
