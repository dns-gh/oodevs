-- --------------------------------------------------------------------------------
-- System activation/deactivation
-- --------------------------------------------------------------------------------
method "canBeDisabled"(
    function( self )
        return true -- ressource node can always be disabled (magic action)
    end )

method "canBeEnabled"(
    function( self )
        return true -- ressource node can always be enabled (magic action)
    end )

method "enableIt" ( 
    function( self ) 
        return integration.enable( self )
    end )

method "disableIt" ( 
    function( self ) 
        return integration.disable( self )
    end )

method "canBeOperated"(
    function( self )
        return false
    end )

method "operateIt" ( 
    function( self ) 
        return integration.enable( self )
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