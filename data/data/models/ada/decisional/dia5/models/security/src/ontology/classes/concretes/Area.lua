-- --------------------------------------------------------------------------------
-- Actions
-- --------------------------------------------------------------------------------
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

method "decontaminateIt" ( 
    function( self )
        return integration.decontaminateArea( self )
    end )

-- --------------------------------------------------------------------------------
-- Information
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self )
        return integration.getAreaPosition( self )
    end )

method "getPositions" ( 
    function( self )
        return integration.getAreaPositions( self )
    end )

return {}