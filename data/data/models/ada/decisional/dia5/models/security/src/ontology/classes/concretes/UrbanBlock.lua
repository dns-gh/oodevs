-- --------------------------------------------------------------------------------
-- Actions
-- ------------------------------------------------------------------------------------------
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

method "alertIt" ( 
    function( self )
        return integration.alertUrbanBlock( self )
    end )

method "undoAlertIt" ( 
    function( self )
        return integration.undoAlertUrbanBlock( self )
    end )

method "confineIt" (
    function( self )
        return integration.confineUrbanBlock( self )
    end )

method "undoConfineIt" (
    function( self )
        return integration.undoConfineUrbanBlock( self )
    end )

method "evacuateIt" (
    function( self )
        return integration.evacuateUrbanBlock( self )
    end )

method "undoEvacuateIt" (
    function( self )
        return integration.undoEvacuateUrbanBlock( self )
    end )

-- --------------------------------------------------------------------------------
-- Informations
-- --------------------------------------------------------------------------------
method "isAlerted" (
    function( self )
        return integration.isUrbanBlockAlerted( self )
    end )

method "isConfined" (
    function( self )
        return integration.isUrbanBlockConfined( self )
    end )

method "isEvacuated" (
    function( self )
        local result = integration.isUrbanBlockEvacuated( self )
        return result
    end )

-- --------------------------------------------------------------------------------
-- Specific classes methods
-- --------------------------------------------------------------------------------
method "getPosition" (
    function( self ) 
        return integration.getUrbanBlockPosition( self )
    end )

method "getPositions" (
    function( self ) 
        return integration.getUrbanBlockPositions( self )
    end )

return {}