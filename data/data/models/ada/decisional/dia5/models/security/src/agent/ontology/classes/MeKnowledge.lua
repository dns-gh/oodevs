-- --------------------------------------------------------------------------------
-- Distance for Crowd transportation
-- $$$ MIA TEMP: change with a new DEC method that return the nearby concentration 
-- of the issued crowd with the issued position
-- --------------------------------------------------------------------------------
local nearbyCrowdConcentrationDistance = 100 -- meters

-- --------------------------------------------------------------------------------
-- Communication treatment
-- --------------------------------------------------------------------------------
-- from  another agent
masalife.brain.communication.setMessageTreatment( "FollowMe",
    function( content, sender )
        integration.communication.FollowMe( content.missionName, content.entityToFollow )
    end )
-- frm a commander
masalife.brain.communication.setMessageTreatment( "Commander_MoveTo",
    function( content, sender )
         integration.communication.OrderMoveToFromCommander( content.missionName, content.waypoints )
    end )

-- --------------------------------------------------------------------------------
-- Movement capability
-- --------------------------------------------------------------------------------
method "moveTo" (
    function( self, destination, urgency, pathType )
        local etat = F_Pion_GeteEtatEchelon( self.source )
        integration.setDecisionalState( "Echelon", "eEtatEchelon_Premier" )
        integration.setPace( urgency ) -- determine pace depending on urgency.
        return destination:reachIt( pathType )   -- movement toward destination
    end )

method "stopMovement" (
    function( self, destination )
       integration.stopMoveToIt( destination )
    end )

-- --------------------------------------------------------------------------------
-- Objects creation capability
-- --------------------------------------------------------------------------------
method "create" (
    function( self, work )
        return work:createIt() -- build object instantaneously
    end )

method "build" (
    function( self, work )
        return work:buildIt()
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
-- Populated area capability
-- --------------------------------------------------------------------------------
method "alert" (
    function( self, populatedArea )
        return populatedArea:alertIt()
    end )

method "undoAlert" (
    function( self, populatedArea )
        return populatedArea:undoAlertIt()
    end )

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
-- Systems capability
-- --------------------------------------------------------------------------------
method "enable" (
    function( self, system )
         return system:enableIt()
    end )

method "disable" (
    function( self, system )
        return system:disableIt()
    end )

method "operate" (
    function( self, system )
        return system:operateIt()
    end )

-- --------------------------------------------------------------------------------
-- Resource nodes capabilities
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
        meKnowledge:RC( eRC_ResourceNodeProductionDamaged )
        return integration.decreaseNodeProduction( resourceNode, quantity )
    end,
    started = function( self )
        return true
    end,
} ) )

-- --------------------------------------------------------------------------------
-- Medical evacuation capability
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
-- Transport capability
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
-- Extinguish fires capability
-- --------------------------------------------------------------------------------
method "extinguish" (
    function( self, fire )
        return fire:extinguishIt()-- immplemention depends on object type
    end )

-- --------------------------------------------------------------------------------
-- Contamination capability
-- --------------------------------------------------------------------------------
method "decontaminate" (
    function( self, area )
        return area:decontaminateIt() -- immplemention depends on object type
    end )

-- --------------------------------------------------------------------------------
-- NBC protection capability
-- --------------------------------------------------------------------------------
method "equipWithNBCProtection" ( masalife.brain.integration.startStopAction(
{ 
    start = function( self )
        integration.equipNBCOutfitSecu()
    end,
    started = function( self )
        return true
    end,
    stop = function( self )
        integration.unequipNBCOutfitSecu()
    end
} ) )


-- --------------------------------------------------------------------------------
-- Specific Crowd transportation capability
-- --------------------------------------------------------------------------------
method "loadCrowdConcentration" ( masalife.brain.integration.startStopAction( 
{
    start = function( self, crowd, destination )
        local concentration = integration.getNearbyConcentration( crowd, destination, nearbyCrowdConcentrationDistance )
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
-- Heal capability
-- --------------------------------------------------------------------------------
method "heal" ( 
    function( self, wounded )
        return wounded:healIt()
    end )

-- --------------------------------------------------------------------------------
-- Hit target with projectile capability
-- --------------------------------------------------------------------------------
method "launchProjectile" ( 
    function( self, target, dotationType, quantity )
        return target:launchProjectileOnIt( dotationType, quantity )
    end )

-- --------------------------------------------------------------------------------
-- Life level
-- --------------------------------------------------------------------------------
method "selfDestruct" ( 
    function( self )
        return integration.commitSuicide()
    end )

method "isDead" ( 
    function( self )
        return integration.isDead()
    end )

-- --------------------------------------------------------------------------------
-- NBC decontamination capability
-- --------------------------------------------------------------------------------
method "selfDecontaminate" ( 
    function( self )
        return integration.selfDecontaminate()
    end )

-- --------------------------------------------------------------------------------
-- Resources capability
-- --------------------------------------------------------------------------------
method "createResourceLink" ( 
    function( self, resourceProducer, resourceNode )
        return resourceProducer:createLinkWith( resourceNode )
    end )

-- --------------------------------------------------------------------------------
-- Check point activation capability
-- --------------------------------------------------------------------------------
method "activateFitration" ( 
    function( self, checkpoint, intensity )
        return checkpoint:filter( intensity )
    end )

-- --------------------------------------------------------------------------------
-- Decontamination capability
-- --------------------------------------------------------------------------------
method "getDecontaminated" ( 
    function( self, decontaminationPlot )
        return decontaminationPlot:useToGetDecontaminated()
    end )

-- --------------------------------------------------------------------------------
-- Order to agents to follow me
-- --------------------------------------------------------------------------------
method "orderToFollow" ( masalife.brain.integration.startStopAction( 
{
    start = function( self, agent )
        meKnowledge:sendReport( eRC_EntityHasBeenOrderedToFollow )
        integration.SendMessage( "FollowMe", agent, { missionName = "behaviors.tasks.Follow", 
          entityToFollow = meKnowledge.source }, { type = "dynamic" } )
    end,
    started = function( self )
        return true
    end
} ) )

-- --------------------------------------------------------------------------------
-- Reinforcement capability
-- --------------------------------------------------------------------------------
method "reinforce" ( 
    function( self, unit )
        return unit:reinforceIt()
    end )

-- --------------------------------------------------------------------------------
-- Dismount capability
-- --------------------------------------------------------------------------------
method "dismount" ( masalife.brain.integration.startStopAction( 
{
    started = function( self )
        if not self:canDismount() then
            meKnowledge:sendReport( eRC_CannotDismount )
            return true -- action ended
        else
            return integration.dismountStarted( self )
        end
    end,
    stop  = function( self ) 
        integration.stopDismount( self )
    end
} ) )

method "canDismount" ( 
    function( self, unit )
        return integration.canDismount( self )
    end )

method "isInstalled" ( masalife.brain.integration.startStopAction( 
{
    start = function( self )
       integration.pionRC( eRC_DebutInstallation )
    end,
    started = function( self )
        return integration.isPosted()
    end,
    stop  = function( self ) 
        integration.pionRC( eRC_FinInstallation )
    end
} ) )

-- --------------------------------------------------------------------------------
-- Fly capability
-- --------------------------------------------------------------------------------
method "takeOff" ( masalife.brain.integration.startStopAction( 
{
    start = function( self, height )
        return integration.changeHeight( height )
    end,
    started = function( self )
        return true
    end,
} ) )

method "land" ( masalife.brain.integration.startStopAction( 
{
    start = function( self )
        return integration.changeHeight( 0 ) -- 0 meters to reach the ground
    end,
    started = function( self )
        return true
    end,
} ) )

method "isFlying" ( 
    function( self )
        return integration.isFlying()
    end )

-- --------------------------------------------------------------------------------
-- NBC alert
-- --------------------------------------------------------------------------------
method "alertNBC" ( masalife.brain.integration.startStopAction( 
{
    start = function( self )
        return integration.goOnNBCAlert( self )
    end,
} ) )
method "isContaminated" ( 
    function( self )
        return integration.isContaminated( self )
    end )

method "isPoisoned" ( 
    function( self )
        return integration.isPoisoned( self )
    end )

-- --------------------------------------------------------------------------------
-- Reports / Messages
-- --------------------------------------------------------------------------------
method "sendReport" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, ... )
        self:RC( ... )
    end
} ) )

method "sendMessage" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, message )
        integration.displayTrace( message )
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