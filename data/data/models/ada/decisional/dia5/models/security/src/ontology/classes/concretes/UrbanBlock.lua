-- --------------------------------------------------------------------------------
-- Delays for magic actions in MINUTES
-- --------------------------------------------------------------------------------
local decontaminationDelays = 2 -- min

-- --------------------------------------------------------------------------------
-- Movement
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        return integration.startMoveToIt( self, pathType ) 
    end,
    started = function( self )
        return integration.updateMoveToIt( self, pathType )
    end, 
    stop = function( self )
        return integration.deselectMoveToIt( self )
    end,
} ) )


-- --------------------------------------------------------------------------------
-- Populated area
-- --------------------------------------------------------------------------------
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
-- Contamination area
-- --------------------------------------------------------------------------------
method "decontaminateIt" ( masalife.brain.integration.startStopAction( 
{ 
    started = function( self )
        if waitInMin( self, decontaminationDelays ) then -- $$$ temp
            meKnowledge:sendReport( eRC_DecontaminationDone )
            return integration.decontaminateUrbanBlock( self )
        end
        meKnowledge:sendReport( eRC_StartingToDecontaminate )
        return false
    end
} ) )


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