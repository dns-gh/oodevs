-- --------------------------------------------------------------------------------
-- Delays for magic actions in MINUTES
-- --------------------------------------------------------------------------------
local decontaminationDelays = 10 -- min

-- --------------------------------------------------------------------------------
-- Populated area
-- --------------------------------------------------------------------------------
method "alertIt" ( 
    function( self )
        return integration.alertUrbanBlock( self )
    end )

method "canBeAlerted" ( 
    function( self )
        local isUrbanBlockAlerted = integration.isUrbanBlockAlerted( self )
        if isUrbanBlockAlerted then
            meKnowledge:sendMessage( "Urban block is already alerted" )
            return false
        else
            return true
        end 
    end )

method "undoAlertIt" ( 
    function( self )
        return integration.undoAlertUrbanBlock( self )
    end )

method "confineIt" (
    function( self )
        return integration.confineUrbanBlock( self )
    end )

method "canBeConfined" ( 
    function( self )
        local isUrbanBlockConfined = integration.isUrbanBlockConfined( self )
        if isUrbanBlockConfined then
            meKnowledge:sendMessage( "Urban block is already confined" )
            return false
        else
            return true
        end 
    end )

method "undoConfineIt" (
    function( self )
        return integration.undoConfineUrbanBlock( self )
    end )

method "evacuateIt" (
    function( self )
        return integration.evacuateUrbanBlock( self )
    end )

method "canBeEvacuated" ( 
    function( self )
        local isUrbanBlockEvacuated = integration.isUrbanBlockEvacuated( self )
        if isUrbanBlockEvacuated then
            meKnowledge:sendMessage( "Urban block is already evacuated" )
            return false
        else
            return true
        end 
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
        return integration.isUrbanBlockEvacuated( self )
    end )

-- --------------------------------------------------------------------------------
--  Work
-- --------------------------------------------------------------------------------
method "buildIt" ( masalife.brain.integration.startStopAction( 
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

method "createIt" ( 
    function( self )
        meKnowledge:sendMessage( "The agent cannot build a urban block instantaneously" )
        return false -- a urban block cannot be created instantaneously
    end )

method "isBuilt" (
    function( self )
        return integration.buildLevelUrbanBlock( self ) == 1
    end )

method "canBeCreated" (
    function( self, instantaneously )
        if instantaneously then
            meKnowledge:sendReport( eRC_ConstructionObjetImpossible ) -- $$$ MIA not the right place to do this report
            return false -- a urban block cannot be created instantaneously
        else
            local urbanBlockType = integration.getTypeUrbanBlock( self )
            local result = integration.canBuildNowObjectType( urbanBlockType ) 
            and integration.hasEnoughtDotationForObjectType( urbanBlockType )
            if not result then -- $$$ MIA not the right place to do this report
                meKnowledge:sendReport( eRC_ConstructionObjetImpossible )
            end
            return result
        end
    end )

method "canBeDeconstructed" ( 
    function( self, instantaneously )
        if instantaneously then 
            return false -- impossible to remove magically a urban block
        end
        return true -- a urban block can be decontructed
    end )

method "deconstructIt" ( masalife.brain.integration.startStopAction( 
{
    start = function( self )
        return integration.startDecontructItUrbanBlock( self )
    end,
    started = function( self )
        return integration.updateRemoveItSecu( self )
    end, 
    stop = function( self )
        return integration.stopRemoveItSecu( self )
    end
} ) )

method "removeIt" (
    function( self )
        meKnowledge:sendMessage( "The agent cannot remove a urban block instantaneously" )
        return false -- A urban block cannot be removed
    end )

method "isDeconstructed" (
    function( self )
        return integration.buildLevelUrbanBlock( self ) == 0
    end )

method "getDestinationForWork" ( 
    function( self ) 
        local simPosition = self:getPosition()
        return CreateKnowledge( ontology.classes.Position, simPosition )
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