return
{ 
    getObstaclesPlan = function( self, params, entity, taskKnowledge )
        local result = {}
        for task, taskWorkMap in pairs( myself.leadData.workMap ) do
            if task == taskKnowledge then
                for obstacle, builders in pairs( taskWorkMap ) do
                    if builders[ 1 ] == entity.source then
                        result[ #result + 1 ] = obstacle
                    end
                end
            end
        end
        return result
    end,

    getObstaclesParameter = function( self, params )
        return params.entities
    end,

    getFinalMeetingPoint = function ( self, params ) -- Need to send a single element for build knowledge
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return integration.randomPositionInCircle( params.meetingPoint, 100 )
        end
        return NIL
    end,

    getPositions = function( self, params )
        local positions = {}
        for _, entity in pairs ( params.entities ) do
            for _, position in pairs ( entity:getPositions() ) do
                positions[ #positions + 1 ] = CreateKnowledge( world.Point, position )
            end
        end
        return positions
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end
}