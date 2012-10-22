return
{
    getFirePositions = function( self, params )
        if not myself.leadData.firePositionIndex then
          myself.leadData.firePositionIndex = 0
        end
        if params.deploymentPositions == NIL then
          return NIL
        else
          myself.leadData.firePositionIndex = myself.leadData.firePositionIndex % #params.deploymentPositions + 1
          return  { params.deploymentPositions[ myself.leadData.firePositionIndex ] }
        end
    end,

    getMeetingPoint = function ( self, params )
        return params.retreatPosition
    end,

    getChangeAutomatically = function ( self, params )
        return params.changeAutomatically
    end
}