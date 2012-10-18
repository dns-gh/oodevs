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
        return params.deploymentPositions
    end,

    getFinalMeetingPoint = function ( self, params ) -- Need to send a single element for build knowledge
        if params.meetingPosition and params.meetingPosition ~= NIL then
            return integration.randomPositionInCircle( params.meetingPosition, 20 )
        end
        return NIL
    end
}
