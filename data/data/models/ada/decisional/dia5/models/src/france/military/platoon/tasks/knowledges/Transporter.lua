local result = 
{
    fillParameters = function( self, companyTask, params, entity ) 
        local units = companyTask:getUnitsToTransport( params )
        local loadingPoint = companyTask:getLoadingPoint( params )
        local destination = companyTask:getDestination( params )
        local meetingPoint = companyTask:getMeetingPoint( params )
        local withEquipment = companyTask:getEquipmentTransportation( params )
        return { units = units, loadingPoint = loadingPoint, destination = destination, meetingPoint = meetingPoint[1], withEquipment = withEquipment }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.tasks.knowledges.Transporter"] = t

return result
