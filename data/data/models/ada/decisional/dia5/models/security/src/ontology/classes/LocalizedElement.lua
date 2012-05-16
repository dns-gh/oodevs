-- --------------------------------------------------------------------------------
-- Movement
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, pathType )
        return integration.startMoveToIt( self, pathType ) 
    end,
    started = function( self, pathType )
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
        if not integration.isPositionInAOR( self ) then
            return false
        end
        return integration.isPositionInAOR( self )
        and ( integration.isPointTrafficable( self ) or integration.canDismount() )
    end )

method "isReached" (
    function( self )
        return self:getProximity( meKnowledge ) == 100
    end )

-- --------------------------------------------------------------------------------
--  Target default implementation
-- --------------------------------------------------------------------------------
method "canBeHit" (
    function( self, dotation )
        return integration.isOnRangeFor( self, dotation )
    end )

method "launchProjectileOnIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, dotation, quantity )
        return integration.startLaunchProjectile( self, dotation, quantity )
    end,
    started = function( self, dotation )
        return integration.updateLaunchProjectile( self, dotation )
    end,
    stop = function( self, dotation )
        return integration.stopLaunchProjectile( self, dotation )
    end
} ) )
return {}