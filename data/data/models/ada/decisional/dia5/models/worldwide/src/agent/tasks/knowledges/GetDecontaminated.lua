local result = 
{
    fillParameters = function( self, companyTask, params )
        local decontaminationSite = companyTask:getObject( params )
        local meetingPoint = companyTask:getDestination( params )
        return { decontaminationPlot = decontaminationSite, meetingPoint = meetingPoint }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.GetDecontaminated" ] = t

return result