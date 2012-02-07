-- --------------------------------------------------------------------------------
-- Movement
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

-- --------------------------------------------------------------------------------
-- System activation/deactivation
-- --------------------------------------------------------------------------------
method "enableIt" ( 
    function( self ) 
        return integration.enable( self )
    end )

method "disableIt" ( 
    function( self ) 
        return integration.disable( self )
    end )

-- --------------------------------------------------------------------------------
-- Information
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self ) 
        return integration.getRessourceNodePosition( self )
    end )

method "getPositions" ( 
    function( self ) 
        return { self:getPosition() }
    end )



return{}