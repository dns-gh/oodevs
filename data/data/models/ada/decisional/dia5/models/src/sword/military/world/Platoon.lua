
-- -------------------------------------------------------------------------------- 
-- Predicates
-- --------------------------------------------------------------------------------
predicate( "isIdentified" )( integration.isIdentifiedPredicate )
predicate( "isThreateningOrHostile" )( integration.isThreateningOrHostilePredicate )

predicate "isDestroyed"
{
    method = function( self )
        return self:destructionLevel () == 100
    end
}
predicate "isDefinitelyIlluminated"
{
    method = function( self )
        return integration.isDefinitelyIlluminated( self )
    end
}
predicate "isThreatening"
{
    dependencies = "none",
    method = function( self )
        return self:threatLevel() > 0
    end
}
predicate "isOperational"
{
    method = function( self )
        return self:operationalLevel() ~= 0
    end
}
predicate "isDangerous"
{
    method = function( self )
        return self:destructionLevel() ~= 100 and self:dangerosityLevel() ~= 0
    end
}
predicate "isLoaded"
{
    dependencies = "none",
    method = function( self )
        return self:loadLevel() == 100
    end
}
predicate "isInCrowd"
{
    dependencies = "none",
    method = function( self )
        return integration.isKwoledgeInCrowd( self )
    end
}

return
{
    -- -------------------------------------------------------------------------------- 
    -- $$$ MIA TODO: rapatrier les méthodes dans ce fichier.
    -- --------------------------------------------------------------------------------
    approachPriority = integration.approachPriority,
    communicate = integration.unit_communicate,
    identificationLevel = function( self )
      return integration.getAgentIdentificationLevel( self )
    end,
    identificationPriority = integration.identificationPriority,
    canIdentifyIt = integration.canIdentifyIt,
    threatLevel = integration.threatLevel,
    fleePriority = integration.fleePriority,

    -- -------------------------------------------------------------------------------- 
    -- High level methods
    -- --------------------------------------------------------------------------------
    destructionPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    neutralizationPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    destructionLevel = function( self )
        return self:getDestructionState()
    end,
    operationalLevel = function( self )
        return self:getOperationalCapacity()
    end,
    dangerosityLevel = function( self )
        return masalife.brain.knowledge.me.body:computeEstimateAttritionOnMe( self )
    end,
    safetyPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100 
    end,
    illuminationPriority = function( self )
        return self:isDefinitelyIlluminated() and 0 or ( math.max( self:proximityLevel(), 1 ) ) / 100
    end,
    illuminationLevel = function( self )
        return integration.illuminationLevel( self )
    end,
    canDestroyIt = function( self )
        return self:firePermitted() 
        and self:isPerceived()
        and not self:isDestroyed()
        and meKnowledge:isOperational()

    end,
    canNeutralizeIt = function( self )
        return self:firePermitted()
        and self:isPerceived()
        and self:isOperational()
        and meKnowledge:isOperational()
    end,
    canApproachIt = function( self )
       return meKnowledge:isSupported()
    end, 
    canRespond = function( self )
        return self:fireNotForbbiden()
        and self:isThreatening()
        and self:isPerceived()
        and self:isOperational()
        and meKnowledge:computeNeutralisationCapability( self, meKnowledge ) > 0 -- utilisation de la capacité de neutralisation pour la riposte.
    end,
    canLoadIt = function( self )
        return true -- in skill load, load element if arrived on position
    end,
    isSurrendered = function( self )
        return integration.isKnowledgeAgentSurrendered( self )
    end,
    canUnloadIt = function( self )
        return true -- in skill unload, load element if arrived on position
    end,
    loadPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    unloadPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    loadLevel = function( self )
        return self:getLoadlevel()
    end,
    isMoving = function( self )
        return integration.isMoving( self )
    end,
    canBeFollowed = function( self )
        return self:isOperational()
    end,

    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    selfProtectFromIt = function( self )
        -- Nothing 
    end,
    moveToIt = function( self, pathType ) -- not used
        return meKnowledge:moveToItIntegration( self, pathType )
    end,
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
     end,
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end,
    getProximity = function( self, reachable )
        return integration.normalizedInversedDistance( self, reachable )
    end,
    computeRelation = function( self )
        return integration.computeRelationAgent( self )
    end,
    getTraficabilityLevelFor = function( self )
        return 100 -- $$$ MIA TODO regarder la position du platoon et voir si c'est traficable.'
    end,
    computeMovementCapability = function( self )
        return 100 -- TODO -- $$$ MIA: n @MGD URGENT clean all concrete knowledge
    end,
    computeEstimatePerceptionCapability = function( self, objective, position )
        return integration.getPerception( objective, position )
    end,
    getPerception = function( self )
        return integration.getAgentPerceptionLevel( self ) -- 0 pas vu || 100 parfaitement perçu.
    end,
    getIdentificationState = function( self )
      return integration.getAgentIdentificationLevel( self )
    end,
    isSafety = function( self )
      return true
    end,
    isCivilian = function( self )
        return integration.isCivilian( self )
    end,
    firePermitted = function( self )
        if self:isCivilian() then -- la cible est un civil 
            return meKnowledge:fireAllowedOnCrowd()
        elseif meKnowledge:isInCrowd() or self:isInCrowd() then -- la cible est dans une foule ou l'agent est dans une foule
            return meKnowledge:fireAllowedOnCrowd() and integration.firePermitted( self )
        else
            return integration.firePermitted( self )
        end
    end,
    fireNotForbbiden = function( self )
        if self:isCivilian() -- la cible est un civil
        then
            return meKnowledge:fireAllowedOnCrowd()
        else
            return integration.fireNotForbbiden( self )
        end
    end,
    getOperationalCapacity = function( self )
        if DEC_ConnaissanceAgent_EstDetruitTactique( self.source ) then
            if not meKnowledge.CRSended then 
                meKnowledge:RC( eRC_EnnemiTactiquementDetruit, self.source ) 
                meKnowledge.CRSended = true
            end
            return 0
        else
            return 100 -- unit is operational
        end
    end,
    getDestructionState = function( self ) -- destruction physique, indépendant des capacités opérationnelles.
        if DEC_ConnaissanceAgent_EtatOps( self.source ) == 0 then
            if not meKnowledge.CRSended then 
                meKnowledge:RC( eRC_EnnemiDetruit, self.source ) 
                meKnowledge.CRSended = true
            end
        end
        return ( 1 - DEC_ConnaissanceAgent_EtatOps( self.source ) ) * 100
    end,
    neutralizeIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startNeutralizingIt,
        started = integration.startedDestroyingIt,
        stop = integration.stopNeutralizingIt 
    } ),
    destroyIt = masalife.brain.integration.startStopAction(
    { 
        start = integration.startDestroyingIt,
        started = integration.startedDestroyingIt,
        stop = integration.stopDestroyingIt 
    } ),
    destroyItForASA = masalife.brain.integration.startStopAction(
    { 
        start = integration.startDestroyingItWithMissileAir,
        started = integration.startedDestroyingIt,
        stop = integration.stopDestroyingIt 
    } ),
    identifyIt = function ( self )
      integration.identifyIt( self )
    end,
    respondToIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startRespondIt, 
        started = function( self, ... ) end, 
        stop = integration.stopRespondIt
     } ),
    fleeFromIt = function( self )
         -- $$$$ MIA: nothing by default
    end,
    canIlluminateIt = function( self )
        return integration.canIlluminateIt( self )
    end,
    illuminateIt = masalife.brain.integration.startStopAction( 
    {
        start = integration.startIlluminateIt, 
        started = integration.startedIlluminateIt, 
        stop = integration.stopIlluminateIt
    } ),
    computeAggressiveness = function( self, target )
      return integration.computeAggressiveness( self, target )
    end,
    getEstimateAttrition = function( self, position )
      return integration.getEstimateAttrition(self, position)
    end,
    computePerceptionCapabilityFor = function( self, entity, position )
        return integration.getPerception( self, position )
    end,
    isOnRangeFor = function( self, munition )
        return integration.isOnRangeFor( self, munition )
    end,
    applyFireOnIt = masalife.brain.integration.startStopAction( 
    {
        start = integration.startApplyFireOnPlatoon,
        started = integration.updateApplyFireOnPlatoon, 
        stop = integration.stopApplyFireOnPlatoon
    } ),
    killOfficersWithin = function( self )
        return integration.killOfficers( self )
    end,

    -- -------------------------------------------------------------------------------- 
    -- Specific class methods used by integration methods
    -- --------------------------------------------------------------------------------
    hasDetectedMe = function( self )
        return DEC_ConnaissanceAgent_PercoitUnite( self.source )
    end,
    getPosition = function( self )
      return integration.getPlatoonPosition( self )
    end,
    getPositions = function( self )
      return { self:getPosition() }
    end,
    getMyPosition = function( self )
        return CreateProxyKnowledge( 
                    sword.military.world.ReachingArea, self, { distanceMin = 0 } )
    end,
    tasks =
    {
        adder = function( self, task ) 
            error( "unused method", 2 )
        end,
        accepter = function( self, visitor )
            local function roleVisitor( role )
                if role.body == self then return role.tasks:accept( visitor ) end
            end
            for group in masalife.brain.knowledge.executeQuery( { knowledgeType = { directia.core.model.Group } } ) do
                return group.roles:accept( roleVisitor )
            end
        end,
        remover = function( self, task )
            error( "unused method", 2 )
        end
     },
     
    isValid = function( self )
        return DEC_ConnaissanceAgent_EstValide( self.source )
    end,
    loadIt = function( self )
        return integration.loadFriendOrFoe( self )
    end,
    unloadIt = function( self )
        return integration.unloadFriendOrFoe( self )
    end,
    getLoadlevel = function ( self )
        if integration.isFriendOrFoeTransported( self ) then
            return 100
        else
            return 0
        end
    end,
    attackIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startAttackIt, 
        started = integration.updateAttackIt, 
        stop = integration.stopAttackIt
    } ),
    interrogateIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startInterrogatePion,
        started = integration.updateInterrogatePion,
        stop = integration.stopInterrogatePion
    } ),
    takeAsHostage = function( self )
        myself.CRCaptureSomeone = {}
        integration.capture( {self}, eRC_TakeHostage )
        return true
    end,
    takeAsPrisoner = masalife.brain.integration.startStopAction( 
    { 
        start = integration.takeAsPrisoner
    } ),
    takeAsRefugee = masalife.brain.integration.startStopAction( 
    { 
        start = integration.takeAsRefugee
    } ),
    addToTransport = function( self, onlyLoadable )
        integration.addKnowledgeInQueue( self, onlyLoadable )
    end,
    readyForBeingTransported = function( self )
        return integration.knowledgeReadyForLoad( self )
    end,
    canTransportIt = function( self, onlyLodable )
        return integration.canTransportKnowledge( self, onlyLodable )
    end,
    shareKnowledge = masalife.brain.integration.startStopAction(
    {
        start = integration.shareKnowledge,
        started = waitInMin,
        stop = function( self )
                self[myself].tempsDebut = nil
                meKnowledge:RC( eRC_FinLiaison )
                return true
            end
    } ),
    shareInstantlyKnowledge = masalife.brain.integration.startStopAction(
    {
        started = function( self )
           integration.shareKnowledge( self, 0)
        end
    } ),
    isInAttackRange = function( self, suicide, dotation )
        if suicide then return self:isReached() end
        return integration.distance( meKnowledge, self ) < integration.porteePourAttentat( dotation )
    end,    
    dropIt = function( self )
      return integration.dropUnit( self )
    end,
    isTransported = function( self )
      return integration.isTransported( self )
    end,
    dischargeItIn = function( self, camp )
        return integration.dischargeAgentKnowledge( self, camp )
    end,
    getName = function( self )
        return integration.getName( self )
    end,
    transportUnitRoundTrip = function( self, onlyLoadable )
        return integration.transportKnowledgeRoundTrip( self, onlyLoadable )
    end,
    enabledSharedPerception = function( self )
        integration.enabledSharedPerceptionWithKnowledge( self )
    end,
    disabledSharedPerception = function( self )
        integration.disableSharedPerceptionWithKnowledge( self )
    end,
    agentKnowledgeSharedPerception = function( self )
        integration.agentKnowledgeSharedPerception( self )
    end,
    agentKnowledgeStopSharedPerception = function( self )
        integration.agentKnowledgeStopSharedPerceptionWithKnowledge( self )
    end,
}
