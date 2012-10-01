return
{
    getMeetingPoint = function ( self, params )
         if not params.meetingPoint then
            return NIL
        end
        return integration.randomPositionOnCircle( params.meetingPoint, 100 ) 
    end,
    
    getObstacles = function( self, params )
        if not params.obstacles then
            return {}
        end
        if not myself.leadData.obstaclesIndex then
            myself.leadData.obstaclesIndex = 0 
        end

        myself.leadData.obstaclesIndex = myself.leadData.obstaclesIndex + 1
        if myself.leadData.obstaclesIndex > #params.obstacles then
            return {}
        end   
        return { params.obstacles[myself.leadData.obstaclesIndex] }
    end,
    
    getNbrFront = function( self )
      local nbrEchelon = myself.taskParams.echelonNumber or 0
      if nbrEchelon == NIL or nbrEchelon == 0 then
        nbrEchelon = 1 -- default value
      end
      return integration.query.getNbrFront( nbrEchelon )
    end,
}
