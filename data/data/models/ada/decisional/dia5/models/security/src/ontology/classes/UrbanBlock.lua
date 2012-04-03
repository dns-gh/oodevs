-- --------------------------------------------------------------------------------
-- Delays for magic actions in MINUTES
-- --------------------------------------------------------------------------------
local decontaminationDelays = 2 -- min

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
--  Work
-- --------------------------------------------------------------------------------
method "createIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.startBuildItUrbanBlock( self )
        return self
    end,
    started = function( self )
        integration.updateBuildItSecu( self )
        return self
    end, 
    stop = function( self )
        integration.stopBuildItUrbanBlock( self )
        return self
    end
} ) )

method "isBuilt" (
    function( self )
        return integration.buildLevelUrbanBlock( self ) == 1
    end )

method "canBeCreated" (
    function( self )
        local urbanBlockType = integration.getTypeUrbanBlock( self )
        local result = integration.canBuildNowObjectType( urbanBlockType ) 
        and integration.hasEnoughtDotationForObjectType( urbanBlockType )
        if not result then -- $$$ MIA not the right place to do this report
            meKnowledge:sendReport( eRC_ConstructionObjetImpossible )
        end
        return result
    end )

method "canBeDeconstructed" ( 
    function( self, instantaneously )
        if instantaneously then -- impossible to remove magically a urban block
            return false 
        end
        return false -- $$$ MIA TODO
    end )

method "deconstructIt" ( 
    function( self )
        return false -- $$$ MIA TODO
    end )

method "removeIt" (
    function( self )
        return false -- A urban block cannot be removed
    end )

method "isDeconstructed" (
    function( self )
        return integration.buildLevelUrbanBlock( self ) == 0
    end )

-- --------------------------------------------------------------------------------
-- Contamination area
-- --------------------------------------------------------------------------------
method "decontaminateIt" ( masalife.brain.integration.startStopAction( 
{ 
    started = function( self )
        if waitInMin( self, decontaminationDelays ) then -- $$$ temp
            return integration.decontaminateUrbanBlock( self )
        end
        meKnowledge:sendReport( eRC_StartingToDecontaminate )
        return false
    end
} ) )

-- --------------------------------------------------------------------------------
-- ResourceReceiver
-- --------------------------------------------------------------------------------
method "createLinkWith" (
    function( self, resourceNode )
        return integration.createResourceLinkWithUrbanBlock( self, resourceNode )
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