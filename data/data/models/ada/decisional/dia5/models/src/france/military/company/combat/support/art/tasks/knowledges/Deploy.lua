return
{    
    getFirePositions = function( self, params )
        if not myself.leadData.firePositionIndex then
          myself.leadData.firePositionIndex = 0
        end
        if params.firePositions == NIL then
          return NIL
        else
          myself.leadData.firePositionIndex = myself.leadData.firePositionIndex % #params.firePositions + 1    
          
          return  { params.firePositions[ myself.leadData.firePositionIndex ] }
        end
    end,
    
    getMeetingPoint = function ( self, params )
        return params.retreatPosition
    end,
}
