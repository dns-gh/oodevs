-- --------------------------------------------------------------------------------
-- Information
-- --------------------------------------------------------------------------------
method "getPosition" (
    function( self )
        return integration.getAreaPositionSecu( self )
    end )

method "getPositions" (
    function( self )
        return integration.getAreaPositionsSecu( self )
    end )

return {}