return
{
    getUnitsToTransport = function( self, params )
        if not myself.leadData.unitsIndex then
            myself.leadData.unitsIndex = 0
        end
        if #params.units < params.maxNbrFront then
            myself.leadData.unitsIndex = myself.leadData.unitsIndex % #params.units + 1
            return { params.units[ myself.leadData.unitsIndex ] }
        end
        myself.leadData.unitsIndex = myself.leadData.unitsIndex + 1
        local index = 0
        while myself.leadData.unitsIndex + ( index + params.maxNbrFront ) <= #params.units do
            index = index + params.maxNbrFront
        end
        local unitsBuffer = {}
        for i = 0, index, params.maxNbrFront do
            unitsBuffer[ #unitsBuffer + 1 ] = params.units[ myself.leadData.unitsIndex + i ]
        end
        return unitsBuffer
    end,

    getLoadingPoint = function( self, params )
        return params.loadingPoint
    end,

    getDestination = function( self, params )
        return params.destination
    end,

    getMeetingPoint = function( self, params)
        return { params.meetingPoint }
    end,

    getNbrFront = function( self )
        return #myself.taskParams.units
    end,

    getEquipmentTransportation = function( self, params )
        if params.withEquipment then
            return params.withEquipment
        else
            return false
        end
    end,
}