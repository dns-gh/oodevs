-- --------------------------------------------------------------------------------
-- Movement [overrided from "Position"]
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{
    start = function( self, pathType )
        return integration.startMoveToItArea( self, pathType ) 
    end,
    started = function( self, pathType )
        return integration.updateMoveToItArea( self, pathType )
    end, 
    stop = function( self )
        return integration.stopMoveToIt( self )
    end,
 } ) )

method "isReached" (
    function( self )
        DEC_Trace( "self.distanceMin = "..tostring( self.distanceMin ) )
        return integration.distance( self.proxy, meKnowledge ) < self.distanceMin
    end )

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