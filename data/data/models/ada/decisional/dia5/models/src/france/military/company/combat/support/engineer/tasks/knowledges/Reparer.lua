return
{
    getObstaclesPlan = function( self, params, entity, taskKnowledge )
        local result = {}
        for task, taskWorkMap in pairs( myself.leadData.workMap ) do
            if task == taskKnowledge then
                for obstacle, builders in pairs( taskWorkMap ) do
                    if builders[1] == entity.source then
                        result[ #result + 1] = obstacle
                    end
                end
            end
        end
        return result
    end,

    getObstaclesParameter = function( self, params )
        return params.objectives
    end,

    getWithImprovement = function( self, params )
        return false
    end,

    getPositions = function( self, params )
        local positions = {}
        for _, entity in pairs( params.objectives ) do
            positions[ #positions + 1] = entity:getMyPosition()
            for _, position in pairs (entity:getPositions()) do
                positions[ #positions + 1] = CreateKnowledge( sword.military.world.Point, position )
            end
        end
        return positions
    end,
    
    getFinalMeetingPoint = function ( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return integration.randomPositionOnCircle( params.meetingPoint, 100 )
        end
        return NIL
    end,
    
    getMeetingPoint = function ( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { integration.randomPositionOnCircle( params.meetingPoint, 100 ) }
        end
        return NIL
    end,
}