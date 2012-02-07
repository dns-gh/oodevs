return
{
    getReachable = function( self, params )
        return params.objective
    end,

    getObjectives = function( self, params )
        return { params.objective }
    end,

    getEntities = function( self, params )
        return params.entities
    end,

    getPeiObjectives = function( self, params )
        return { params.objective }
    end,

    getObserving = function( self, params )
        if not myself.leadData.observingIndex then
            myself.leadData.observingIndex = 0 
        end
      
        myself.leadData.observingIndex = myself.leadData.observingIndex % #params.meetingPoints + 1
        return { params.meetingPoints[myself.leadData.observingIndex] }
    end,
    
    getMeetingPoint = function( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { params.meetingPoint } 
        end
        return NIL
    end,
    
    getObstacles = function( self, params )
        if not params.obstacles then
            return NIL
        end
        if not myself.leadData.obstaclesIndex then
            myself.leadData.obstaclesIndex = 0 
        end
      
        myself.leadData.obstaclesIndex = myself.leadData.obstaclesIndex + 1
        if myself.leadData.obstaclesIndex > #params.obstacles then
            return NIL
        end   
        return { params.obstacles[myself.leadData.obstaclesIndex] }
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
}