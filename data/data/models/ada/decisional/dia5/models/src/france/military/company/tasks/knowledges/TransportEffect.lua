return
{
    getTargets = function( self, params, entity, taskKnowledge )
        local result = {}
        for task, taskTargetMap in pairs( myself.leadData.workMap ) do
            if task == taskKnowledge then
                for target, units in pairs( taskTargetMap ) do
                    if units[1] == entity.source then
                        result[ #result + 1] = target
                    end
                end
            end
        end
        return result
    end,

    getLoadingPoint = function( self, params )
        return params.loadingPoint
    end,

    getDestination = function( self, params )
        return params.destination
    end,

    getMeetingPoint = function( self, params)
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { integration.randomPositionInCircle( params.meetingPoint, 100 ) }
        end
        return NIL
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
    
    getObstaclesParameter = function( self, params )
        return params.units
    end,
}