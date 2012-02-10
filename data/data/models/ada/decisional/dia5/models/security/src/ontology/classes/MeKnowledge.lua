-- --------------------------------------------------------------------------------
-- Movement
-- --------------------------------------------------------------------------------
method "moveTo" (
    function( self, destination, urgency, pathType )
        integration.setPace( urgency ) -- determine pace depending on urgency.
        return destination:reachIt( pathType )   -- movement toward destination
    end )

method "stopMovement" (
    function( self, destination )
       integration.stopMoveToIt( destination )
    end )

-- --------------------------------------------------------------------------------
-- Objects creation
-- --------------------------------------------------------------------------------
method "create" (
    function( self, work )
        return work:createIt()
    end )

method "deconstruct" (
    function( self, work )
        return work:deconstructIt()
    end )

method "remove" (
    function( self, work )
        return work:removeIt() -- remove object instantaneously
    end )

-- --------------------------------------------------------------------------------
-- Populated area
-- --------------------------------------------------------------------------------
method "alert" (
    function( self, populatedArea )
        return populatedArea:alertIt()
    end )

method "undoAlert" (
    function( self, populatedArea )
        return populatedArea:undoAlertIt()
    end
)

method "confine" (
    function( self, populatedArea )
        return populatedArea:confineIt()
    end )

method "undoConfine" (
    function( self, populatedArea )
        return populatedArea:undoConfineIt()
    end )

method "evacuate" (
    function( self, populatedArea )
        return populatedArea:evacuateIt()
    end )

method "undoEvacuate" (
    function( self, populatedArea )
        return populatedArea:undoEvacuateIt()
    end )

-- --------------------------------------------------------------------------------
-- Systems operations (objects, 
-- --------------------------------------------------------------------------------
method "enable" (
    function( self, system )
         return system:enableIt()
    end )

method "disable" (
    function( self, system )
        return system:disableIt()
    end )

-- --------------------------------------------------------------------------------
-- Resource nodes
-- --------------------------------------------------------------------------------
method "increaseResourceProduction" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, resourceNode, quantity )
        meKnowledge:sendReport( eRC_StartingToProduceResource )
            return integration.increaseNodeProduction( resourceNode, quantity )
        end,
    stop  = function( self, resourceNode, quantity )
        meKnowledge:sendReport( eRC_ResourceProductionStopped )
        return integration.decreaseNodeProduction( resourceNode, quantity )
    end
} ) )

method "decreaseResourceProduction" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, resourceNode, quantity )
        meKnowledge:sendReport( eRC_StartingToReduceResourceProduction )
        return integration.decreaseNodeProduction( resourceNode, quantity )
    end,
    stop  = function( self, resourceNode, quantity )
        meKnowledge:sendReport( eRC_ResourceReductionStopped )
        return integration.increaseNodeProduction( resourceNode, quantity )
    end
} ) )

method "enhanceResourceProductionCapability" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, resourceNode, quantity )
         meKnowledge:RC( eRC_ResourceNodeProductionEnhanced )
        return integration.increaseNodeProduction( resourceNode, quantity )
    end,
    started = function( self )
        return true
    end,
} ) )

method "damageResourceProductionCapability" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, resourceNode, quantity )
        meKnowledge:RC( eRC_ResourceNodeProductionEnhanced )
        return integration.decreaseNodeProduction( resourceNode, quantity )
    end,
    started = function( self )
        return true
    end,
} ) )

-- --------------------------------------------------------------------------------
-- Medical evacuation
-- --------------------------------------------------------------------------------
method "ensureVictimsEvacuation" (
    function( self, victimsUnit, medicalUnit )
        return medicalUnit:evacuateVictims( victimsUnit )
    end )

method "extractVictims" (
    function( self, victimsUnit )
        return victimsUnit:extractVictimsFromIt()
    end )

-- --------------------------------------------------------------------------------
-- Transport
-- --------------------------------------------------------------------------------
method "load" (
    function( self, entity )
        return entity:loadIt()
    end )

method "unload" (
    function( self, entity )
        return entity:unloadIt()
    end )

-- --------------------------------------------------------------------------------
-- Extinguish fires
-- --------------------------------------------------------------------------------
method "extinguish" (
    function( self, fire )
        return fire:extinguishIt()-- immplemention depends on object type
    end )

-- --------------------------------------------------------------------------------
-- Contamination areas
-- --------------------------------------------------------------------------------
method "decontaminate" (
    function( self, area )
        return area:decontaminateIt() -- immplemention depends on object type
    end )

-- --------------------------------------------------------------------------------
-- Specific methods on body
-- --------------------------------------------------------------------------------
method "equipWithNBCProtection" ( masalife.brain.integration.startStopAction(
{ 
    start = function( self )
        integration.equipNBCOutfit()
    end,
    started = function( self )
        return true
    end,
    stop = function( self )
        integration.unequipNBCOutfit()
    end
} ) )


-- --------------------------------------------------------------------------------
-- Specific methods for crowd transportation
-- --------------------------------------------------------------------------------
method "loadCrowdConcentration" ( masalife.brain.integration.startStopAction( 
{
    start = function( self, crowd, destination )
        local concentration = integration.getNearbyConcentration( crowd, destination, 10 )
        if concentration ~= nil then
            return integration.startLoadCrowd( crowd, concentration )
        else
            return false
        end
    end,
    started = function( self )
        return integration.startedLoadCrowd( self )
    end,
    stop = function( self )
        return integration.stopLoadCrowd( self )
    end,
} ) )

method "unloadCrowdConcentration" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, crowd, destination )
        return integration.startUnloadCrowd( crowd, destination )
    end,
    started = function( self )
        return integration.startedUnloadCrowd( self )
    end, 
    stop = function( self )
        return integration.stopUnloadCrowd( self )
    end,
} ) )

method "canTransportCrowd" ( 
    function( self, crowd )
        return integration.canTransportCrowd()
    end )


-- --------------------------------------------------------------------------------
-- Reports
-- --------------------------------------------------------------------------------
method "sendReport" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, ... )
        self:RC( ... )
    end
} ) )

method "RC" ( 
    function ( self, ... ) -- $$$ MIA:To remove
        integration.pionRC( ... )
    end )

-- --------------------------------------------------------------------------------
-- Specific integration methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self )
        return integration.getBodyPosition()
    end )

return {}