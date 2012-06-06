-- --------------------------------------------------------------------------------
-- Sim position methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self )
        -- The agent moves to the nearest position (not necessary the head)
        return integration.getCrowdPosition( self )
    end )

method "getPositions" ( 
    function( self )
        return { self:getPosition() }
    end )

return {}