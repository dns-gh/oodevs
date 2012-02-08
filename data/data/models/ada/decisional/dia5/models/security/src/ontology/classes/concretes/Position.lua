-- --------------------------------------------------------------------------------
-- Movement
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        return integration.startMoveToIt( self, pathType ) 
    end,
    started = function( self )
        return integration.updateMoveToIt( self, pathType )
    end, 
    stop = function( self )
        return integration.deselectMoveToIt( self )
    end,
} ) )

-- --------------------------------------------------------------------------------
-- Specific classe methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self ) 
        return integration.getPointPosition( self )
    end )

method "getPositions" (
    function( self ) 
         return integration.getPointPositions( self )
    end )

return {}