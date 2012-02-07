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