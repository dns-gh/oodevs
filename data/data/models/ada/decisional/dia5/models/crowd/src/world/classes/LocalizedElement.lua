-- --------------------------------------------------------------------------------
-- Movement
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, pathType )
        return integration.startMoveToItCrowd( self, pathType ) 
    end,
    started = function( self, pathType )
        return integration.updateMoveToItCrowd( self, pathType )
    end, 
    stop = function( self )
        return integration.deselectMoveToIt( self )
    end,
} ) )

-- --------------------------------------------------------------------------------
-- Destination
-- --------------------------------------------------------------------------------
method "getProximity" (
    function( self, element )
        return integration.normalizedInversedDistance( self, element )
    end )

method "isAccessible" (
    function( self )
        return true -- $$$ crowds can go everywhere.
    end )

method "isReached" (
    function( self )
        return integration.hasEntirelyReachedDestination( self )
    end )

return{}