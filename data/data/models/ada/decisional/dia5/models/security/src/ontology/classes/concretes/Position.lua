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