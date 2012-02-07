return
{
   getPosition = function( self, params )
        myself.leadData.currentPositionForCheckpoint = myself.leadData.currentPositionForCheckpoint or 0
        myself.leadData.currentPositionForCheckpoint = myself.leadData.currentPositionForCheckpoint % #params.positions + 1
        
        return params.positions[ myself.leadData.currentPositionForCheckpoint ]
    end,
    
    getBodySearchStrength = function( self, params )
        return nil
    end,

    getBlockingStrength = function( self, params )
        return nil
    end,
}
