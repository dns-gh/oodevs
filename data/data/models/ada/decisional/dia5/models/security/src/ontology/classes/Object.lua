-- --------------------------------------------------------------------------------
--  Work
-- --------------------------------------------------------------------------------
method "createIt" ( masalife.brain.integration.startStopAction( 
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

method "isBuilt" (
    function( self )
        return integration.isConstructed( self )
    end )

method "canBeCreated" (
    function( self )
        local result = integration.canBuildNowObjectType( DEC_ConnaissanceObjet_Type( self.source ) ) 
        and integration.hasEnoughtDotationForObjectType( DEC_ConnaissanceObjet_Type( self.source ) )
        if not result then -- $$$ MIA not the right place to do this report
            meKnowledge:sendReport( eRC_ConstructionObjetImpossible )
        end
        return result
    end )

method "canBeDeconstructed" (
    function( self, instantaneously )
        return integration.canRemoveItSecu( self ) or instantaneously
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
        return CreateKnowledge( ontology.classes.Position, simPosition )
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
        return true -- $$$ MIA TODO check physical capability
    end )

method "getPositionToExtinguish" ( 
    function( self )
        local simPosition = integration.getObjectNearestBorderPosition( self )
        return CreateKnowledge( ontology.classes.Position, simPosition )
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
method "disableIt" (
    function( self )
        return true -- $$$ MIA TODO
    end )

method "enableIt" (
    function( self )
        return true -- $$$ MIA TODO
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
method "decontaminateMe" ( 
    function( self )
        meKnowledge:sendReport( eRC_DemandeDecontamination )
        integration.requestForDecontamination( self )
        if not integration.isContaminated() then
            integration.pionRC( eRC_UniteDecontaminee )
            return true
        end
        return false
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