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
-- Destination
-- --------------------------------------------------------------------------------
method "getProximity" (
    function( self, element )
        return integration.normalizedInversedDistance( self, element )
    end )

method "isAccessible" (
    function( self )
        return integration.isPointTrafficable( self ) or integration.canDismount()
    end )

method "isReached" (
    function( self )
        return self:getProximity( meKnowledge ) == 100
    end )

return {}