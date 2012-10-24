-- --------------------------------------------------------------------------------
--  Work
-- --------------------------------------------------------------------------------
method "buildIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.startBuildItKnowledge( self )
        return self
    end,
    started = function( self )
        integration.updateBuildItSecu( self )
        return self
    end, 
    stop = function( self )
        integration.stopBuildItSecu( self )
        return self
    end
} ) )

method "createIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.buildObjectInstantaneously( self )
        return self
    end,
    started = function( self )
        return self
    end, 
    stop = function( self )
        return self
    end
} ) )

method "isBuilt" (
    function( self )
        return integration.isConstructed( self )
    end )

method "canBeCreated" (
    function( self, instantaneously )
        if instantaneously then 
            return true -- magic action
        else
            local result = integration.canBuildNowObjectType( integration.getKnowledgeObjectType( self.source ) ) 
            and integration.hasEnoughtDotationForObjectType( integration.getKnowledgeObjectType( self.source ) )
            if not result then -- $$$ MIA not the right place to do this report
                meKnowledge:sendReport( eRC_ConstructionObjetImpossible )
            end
            return result
        end
    end )

method "canBeDeconstructed" (
    function( self, instantaneously )
        if instantaneously then 
            return true -- magic action
        else
            return integration.canRemoveItSecu( self )
        end
    end )

method "deconstructIt" ( masalife.brain.integration.startStopAction( 
{ 
    start   = integration.startRemoveIt, 
    started = integration.updateRemoveItSecu, 
    stop    = integration.stopRemoveItSecu
} ) )

method "removeIt" (
    function( self )
        return integration.removeObjectInstantaneously( self )
    end )
    
method "isDeconstructed" (
    function( self )
        return integration.isObjectRemoved( self ) or integration.buildLevel( self ) == 0
    end )

method "getDestinationForWork" ( 
    function( self ) 
        local simPosition = integration.getObjectNearestBorderPosition( self )
        return CreateKnowledge( agent.ontology.classes.Position, simPosition )
    end )

-- --------------------------------------------------------------------------------
--  Fire
-- --------------------------------------------------------------------------------
method "isExtinguished" ( 
    function( self )
        return integration.getBurningState( self ) == 0
    end )

method "extinguishIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        return integration.startExtinguish( self )
    end, 
    started = function( self )
        return integration.updateExtinguish( self )
    end, 
    stop = function( self )
        return integration.stopExtinguish( self )
    end,
} ) )

method "canBeExtinguished" ( 
    function( self )
        return integration.canBeExtinguished( self )
    end )

method "getPositionToExtinguish" ( 
    function( self )
        local simPosition = integration.getObjectNearestBorderPosition( self )
        return CreateKnowledge( agent.ontology.classes.Position, simPosition )
    end )

-- --------------------------------------------------------------------------------
-- ResourceReceiver
-- --------------------------------------------------------------------------------
method "createLinkWith" (
    function( self, resourceNode ) 
        return integration.createResourceLinkWithObject( self, resourceNode )
    end )

-- --------------------------------------------------------------------------------
-- System
-- --------------------------------------------------------------------------------
method "canBeDisabled"(
    function( self )
        return false -- an object cannot be disabled once it has been enabled.
    end )

method "canBeEnabled"(
    function( self )
        return integration.canActivateIt( self )
    end )

method "disableIt" (
    function( self )
        return false -- an object cannot be disabled
    end )

method "enableIt" (
    function( self )
        return integration.activateItSecu( self )
    end )

method "canBeOperated"(
    function( self )
        return integration.canAnimateIt( self )
    end )

method "operateIt" ( masalife.brain.integration.startStopAction( 
{
    start = function( self )
        integration.startAnimateIt( self )
        return true
    end,
    started = function( self )
        return true
    end,
    stop = function( self )
        return integration.stopAnimateIt( self )
    end,
} ) )

-- --------------------------------------------------------------------------------
-- CheckPoint
-- --------------------------------------------------------------------------------
method "canBeUsedToFilter"(
    function( self )
        return integration.canFilter( self )
    end )

method "filter" ( masalife.brain.integration.startStopAction( 
{
    start = function( self, intensity )
        integration.startFilterCrowds( intensity, self.source )
        return true
    end,
    started = function( self )
        return true
    end,
    stop = function( self )
        integration.stopFilterCrowds( self.source )
        return false
    end,
} ) )

-- --------------------------------------------------------------------------------
-- Decontamination plot
-- --------------------------------------------------------------------------------
method "useToGetDecontaminated" ( 
    function( self )
        meKnowledge:sendReport( eRC_DemandeDecontamination )
        integration.requestForDecontamination( self )
        if not integration.isContaminated() then
            integration.pionRC( eRC_UniteDecontaminee )
            return true
        end
        return false
    end )

method "canBeUsedToGetDecontaminated" ( 
    function( self )
        return integration.canDecontaminateAgents( self )
    end )

-- --------------------------------------------------------------------------------
-- Specific classe methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self ) 
        return integration.getObjectPosition( self )
    end )

method "getPositions" ( 
    function( self ) 
        return integration.getObjectPositions( self )
    end )

return {}