-- --------------------------------------------------------------------------------
-- Actions
-- --------------------------------------------------------------------------------
-- We introduce a local "reachIt" which is the same as moveToItGeneric but
-- the return of the result is missing in the latter.
-- We set the default value 0 for the pathType. The agent moves directly to the destination
method "reachIt" ( masalife.brain.integration.startStopAction( 
{ 
    start   = function( self )
                  return integration.startMoveToIt( self ) 
              end,
    started = function( self )
                  return integration.updateMoveToIt( self )
              end, 
    stop    = function( self )
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