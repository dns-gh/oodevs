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
        DEC_Trace( "objects = "..tostring( params.objects ) )
        return params.objects
    end,

    getPositions = function( self, params )
        local positions = {}
        for _, object in pairs ( params.objects ) do
            for _, position in pairs ( object:getPositions() ) do
                positions[ #positions + 1 ] = CreateKnowledge( world.Point, position )
            end
        end
        return positions
    end,

    getMeetingPoint = function( self, params )
        if not params.meetingPoint then
            return {}
        end
        return params.meetingPoint 
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,

    getWithReinforcement = function( self, params )
        return params.waitForReinforcement or false
    end
}