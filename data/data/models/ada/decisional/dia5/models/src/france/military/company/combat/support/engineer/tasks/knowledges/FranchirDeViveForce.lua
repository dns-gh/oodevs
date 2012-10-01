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
        return params.obstacles
    end,

    getPositions = function( self, params )
        local positions = {}
        for _, entity in pairs (params.obstacles) do
            for _, position in pairs (entity:getPositions()) do
                positions[ #positions + 1] = CreateKnowledge( sword.military.world.Point, position )
            end
        end
        return positions
    end,

    getMeetingPoint = function( self, params )
        if not params.meetingPoint then
            return {}
        end
        return integration.randomPositionOnCircle( params.meetingPoint, 100 ) 
    end,
}