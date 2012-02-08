-- --------------------------------------------------------------------------------
-- Movement
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{
    start = function( self )
        return integration.startMoveToItArea( self, pathType ) 
    end,
    started = function( self )
        return integration.updateMoveToItArea( self, pathType )
    end, 
    stop = function( self )
        return integration.stopMoveToIt( self )
    end,
 } ) )

-- --------------------------------------------------------------------------------
-- Destination
-- --------------------------------------------------------------------------------
method "isAccessible" (
    function( self )
        if not integration.isPointTrafficable( self.proxy )
           and not integration.canDismount() then
            return false
        else
            return true
        end
    end )

method "getProximity" (
    function( self, element )
        return integration.normalizedInversedDistance( self.proxy, element )
    end )

-- -------------------------------------------------------------------------------- 
-- Specific classe methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self )
        return self.proxy:getPosition()
    end )

method "getPositions" (
    function( self )
        return integration.getPointPositions( self:getMyPosition() )
    end )

return {}