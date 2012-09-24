local result = 
{
    fillParameters = function( self, companyTask, params, entity )
        local crowd = companyTask:getCrowdToTransport( params )
        local loadingPoint = companyTask:getLoadingPoint( params )
        local destination = companyTask:getDestination( params )
        local meetingPoint = companyTask:getMeetingPoint( params )
        return { crowd = crowd, loadingPoint = loadingPoint, destination = destination, meetingPoint = meetingPoint[ 1 ] }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.TransportACrowd" ] = t

return result