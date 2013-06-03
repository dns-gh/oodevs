return
{
    -- -------------------------------------------------------------------------------- 
    -- High level methods 
    -- --------------------------------------------------------------------------------
    -- RECONNOITRABLE
    reconnaissancePriority = integration.generic_reconnaissancePriority,
    -- REACHING
    reachEfficiency = function( self, target )
        local position = self:getMyPosition()
        return ( ( target:proximityLevel() / 100 + position:getProximity( target ) ) ) / 100
    end,
    -- APPROCHABLE
    approachPriority = integration.approachPriority,

    reconnaissanceLevel = function( self )
        return self:getReconnaissanceState()
    end,
    NBCrecoLevel = function( self )
        return self:getNBCState()
    end,
    isReconnoitred = function( self ) 
        return self:reconnaissanceLevel() == 100 
    end,
    canReconnoitreIt = function( self ) 
        return self:isReached()
    end,
    canApproachIt = function( self )
       return meKnowledge:isSupported()
    end,
    animationPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    canAnimateIt = function( self )
        return self:isReached() and integration.canAnimateIt( self )
    end,
    animationLevel = function( self )
        return integration.animationLevel( self )
    end,
    animationEfficiency = function( self, objective )
        return self == objective and 1 or 0
    end,
    improvePriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    canImproveIt = function( self )
        return integration.canImproveIt( self )
    end,
    improvingDotation = function( self )
        return integration.hasEnoughtDotationForImprovement( self )
    end,
    isImproved = function( self )
        if self[myself] and self[myself].actionImprovementState then
            return self[myself].actionImprovementState == eActionObjetTerminee
        else
            return self:improvementLevel() == 100
        end
    end,
    removalLevel = function ( self )
        return integration.removalLevel( self )
    end,
    improvementLevel = function( self )
        return integration.improvementLevel( self )
    end,
    activationPriority = function( self )
     -- $$$ MIA: regarder si l'objet est contruit? Peut-on activer 
     -- un object si il n'est pas contruit?
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    activationLevel = function( self )
        return integration.activationLevel( self )
    end,
    removalPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    activationEfficiency = function( self, objective )
        return self == objective and 1 or 0
    end,
    bypassPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    bypassLevel = function( self )
        return integration.bypassLevel( self )
    end,
    buildPriority = function( self )
        local obstacleType = 0 -- First build preliminary obstacles,at the end build maneuver obstacles
        if DEC_ConnaissanceObjet_EstObstacleDeManoeuvre( self.source ) == false then
            obstacleType = 1
        end
        return ( ( self:proximityLevel() / 100 + obstacleType ) ) / 2
    end,
    constructionLevel = function( self )
        return integration.buildLevel( self )
    end,
    buildEfficiency = function( self, objective )
        return self == objective and 1 or 0
    end,
    reconnaissanceEfficiency = function( self, objective )
        return self == objective and 1 or 0
    end,
    elementIsReconnoitered = function( self )
        -- Do nothing
    end,
    isBeingReconnoiteredByMeOrNoOne = function( self )
       return true
    end,
    getNBCState = function( self )
        return integration.getObjectNBC( self )
    end,
    RecoNBC = masalife.brain.integration.startStopAction( 
    {
        start = function( self )
            integration.startNBCObject( self )
        end,
        stop = integration.stopNBCObject
    } ),
    isNRBCReconnoitred = function( self )
        return self.NRBCReconnoitred
    end,
    setNRBCReconnoitred = function( self, value )
        self.NRBCReconnoitred = value
    end,
    -- -------------------------------------------------------------------------------- 
    -- Predicates
    -- --------------------------------------------------------------------------------
    predicate "isReachingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeMovementCapability( objective, self ) > 0 and integration.isElementInAOR( self )
        end
    },
    predicate "isActivatingFor"
    {
        method = function( self, objective )
            -- $$$ MIA TODO à revoir, ne pas utlisé une efficiency
            return self:activationEfficiency( objective ) == 100 
        end,
    },
    predicate "isBuildingFor"
    {
       method = function( self, objective )
          -- $$$ MIA TODO à revoir, ne pas utlisé une efficiency
          return self:buildEfficiency( objective )
       end,
    },
    predicate "isFullyAnimated"
    {
        method = function( self )
            return self:animationLevel() == 100
        end,
    },
    predicate "isPartiallyAnimated"
    {
        method = function( self )
            return self:animationLevel() > 0
        end,
    },
    predicate "isAnimatingFor"
    {
        method = function( self, objective )
            return self == objective
        end,
    },
    predicate "isActivated"
    {
        method = function( self )
            return self:activationLevel() == 100
        end,
    },
    predicate "isDisactivated"
    {
        method = function( self )
            return self:activationLevel() == 0
        end,
    },
    predicate "isBypassed"
    {
        method = function( self )
            return self:bypassLevel() == 100
        end,
    },
    predicate "isRemoved"
    {
        method = function( self )
            return not DEC_IsValidKnowledgeObject(self.source)
        end,
    },
    predicate "isBuilt"
    {
       method = function( self )
        if self[myself] then
          return self[myself].actionBuildState == eActionObjetTerminee
        else
          return self:constructionLevel() == 100
        end
       end,
    },
    predicate "isReconnoitringFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                  computeReconnaissanceCapability( objective, self ) > 0 
        end
    },
    predicate "isRecoNBC"
    {
        dependencies = "none",
        method = function( self )
            return self:NBCrecoLevel() == 100
        end
    },

    -- -------------------------------------------------------------------------------- 
    -- Integrations methods 
    -- --------------------------------------------------------------------------------
    getPosition = function( self )
      return integration.getObjectPosition( self )
    end,
    getMyPosition = function( self )
      return CreateKnowledge( sword.military.world.Point, self:getPosition() )
    end,
    getPositions = function( self )
        return integration.getObjectPositions( self )
    end,
    getTraficabilityLevelFor = function( self, forLoadedState )
        -- Renvoi vrai si le point n'est pas dans un BU ou dans un BU trafficable
        return integration.isPointInUrbanBlockTrafficable( self, forLoadedState )
    end,
    computeRelation = function( self )
        return integration.computeRelationObject( self )
    end,
    getPerception = function( self )
        return integration.getObjectPerception( self )
    end,
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end,
    getProximity = function( self, target )
        if DEC_IsValidKnowledgeObject(self.source) then
            if masalife.brain.core.class.isOfType( target, sword.military.world.Direction ) then
                return 100 -- $$$$ PSN: TMP pour prendre en compte la direction, qui n'a pas de notion de position
            else
                return integration.normalizedInversedDistance( self, target )
            end
        else
            return 0
        end
    end,
    moveToIt = function( self, pathType )
        return meKnowledge:moveToItIntegration( self, pathType )
    end,
    getReconnaissanceState = function( self )
        return integration.getObjectReconnaissanceState( self )
    end,
    reconnoitreIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startRecceObject, 
        stop = integration.stopRecceObject 
    } ),
    takeUpPosition = function( self )
        return integration.takeUpPositionObject( self )
    end,
    animateIt = masalife.brain.integration.startStopAction( 
    {
        start = integration.startAnimateIt,
        stop = integration.stopAnimateIt
     } ),
    improveIt = masalife.brain.integration.startStopAction(
    { 
        start = integration.startImproveIt,
        started = integration.updateImproveIt,
        stop = integration.stopImproveIt
     } ),
    isManeuverObstacle = function( self )
         return integration.isManeuverObstacle( self )
    end,
    isActifManeuverObstacle = function( self )
         return integration.isActifManeuverObstacle( self )
    end,
    delayedActivationTime = function( self )
        return integration.delaiAvantActivationForObject( self )
    end,
    canActivateIt = function( self )
        return integration.canActivateIt( self )
    end,
    activateIt = function( self )
        return integration.activateIt( self )
    end,
    canRemoveIt = function( self )
        return integration.canRemoveIt( self ) or integration.isInEffectArea( self )
    end,
    removeIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startRemoveIt, 
        started = integration.updateRemoveIt, 
        stop = integration.stopRemoveIt
     } ),
    isMined = function( self )
        return integration.isMined( self )
    end,
    isAvoidable = function( self )
        return integration.isAvoidable( self )
    end,
    canDemineIt = function( self )
        return integration.canDemineIt( self )
    end,
    demineIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startDemineIt, 
        started = integration.updateDemineIt, 
        stop = integration.stopDemineIt
     } ),
    canBypassIt = function( self )
        return integration.canBypassIt( self ) or integration.isInEffectArea( self )
    end,
    canBeBypassed = function( self )
        return integration.canBeBypassed( self )
    end,
    bypassIt = masalife.brain.integration.startStopAction( 
    {
        start = integration.startBypassIt,
        started = integration.updateBypassIt,
        stop = integration.stopBypassIt
    } ),
    canBuildIt = function( self )
        local objectType = self:getType()
        return integration.canBuildObjectType( objectType, self:getLocalisation() ) and integration.hasEnoughtDotationForObjectType( objectType )
    end,
    canBuildItNow = function( self )
        local objectType = self:getType()
        return integration.canBuildNowObjectType( objectType, self:getLocalisation() ) and integration.hasEnoughtDotationForObjectType( objectType )
    end,
    buildingCapacity = function( self )
        return integration.canBuildObjectType( self:getType(), self:getLocalisation() )
    end,
    buildingCapacityWithOutReinforcement = function ( self )
        return integration.canBuildObjectTypeWithoutReinforcement( self:getType(), self:getLocalisation() ) 
    end,
    buildingDotation = function( self )
        return integration.hasEnoughtDotationForObjectType( self:getType() )
    end,
    buildingDotationWithOutReinforcement = function( self )
        return integration.hasEnoughtDotationForObjectTypeWithoutReinforcement( self:getType() )
    end,
    buildIt = masalife.brain.integration.startStopAction(
    { 
        start = integration.startBuildItKnowledge, 
        started = integration.updateBuildIt, 
        stop = integration.stopBuildIt
    } ),
    canBeImproved = function( self )
        return integration.canBeImproved( self )
    end,
    requestDecontamination = function( self )
        DEC_ConnaissanceObjet_DemandeDeDecontamination( self.source )
    end,
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
    end,
    isValid = function( self )
        return DEC_IsValidKnowledgeObject( self.source )
    end,
    getLocalisation = function( self )
        return DEC_ConnaissanceObjet_Localisation( self.source )
    end,
    getType = function( self )
        return DEC_ConnaissanceObjet_Type( self.source )
    end,
    -- commit attak
    attackIt = function( self, suicide, dotation  )
        return integration.attackObject( self, suicide, dotation )
    end,
    plannedworkToWork = function( self )
        return self
    end,
    isInAttackRange = function( self, suicide, dotation )
        if suicide then return self:isReached() end
        return integration.distance( meKnowledge, self ) < integration.porteePourAttentat( dotation )
    end,

    -- -------------------------------------------------------------------------------- 
    -- UndergroundNetwork methods 
    -- --------------------------------------------------------------------------------
    EnterInIt = function( self )
        return integration.EnterInUndergroundNetwork( self )
    end,
    PassThroughIt= masalife.brain.integration.startStopAction( 
    { 
        start = integration.StartPassThroughUndergroundNetwork, 
        started = integration.UpdatePassThroughUndergroundNetwork, 
        stop = integration.StopPassThroughUndergroundNetwork
    } ),
    EnableIt = function( self )
        return integration.EnableUndergroundNetwork( self )
    end,
    DisableIt = function( self )
        return integration.DisableUndergroundNetwork( self )
    end,

    -- -------------------------------------------------------------------------------- 
    -- Mobility affection
    -- --------------------------------------------------------------------------------
    affectMobility = masalife.brain.integration.startStopAction(
    {
        start = integration.startAffectMobility,
        started = integration.updateAffectMobility,
        stop = integration.stopAffectMobility
    } ),
    unaffectMobility = function( self )
        integration.unAffectMobility( self )
    end,
    canEquipIt = function( self, typePont )
        return DEC_Agent_PeutConstruireObjetAvecLocalisation( typePont, self:getLocalisation() )
    end,
    equipIt = masalife.brain.integration.startStopAction(
    { 
        start = integration.startEquipBridge, 
        started = integration.updateEquipBridge, 
        stop = integration.stopEquipBridge
    } ),
    unEquipIt = function( self )
        integration.unEquipSite( self )
    end,
    isReached = function( self )
        return integration.isPointInLocalisation( meKnowledge, self )
    end,
    getPositionsToReconnoiter = function( self )
        local CreateKnowledge = CreateKnowledge
        local res = {}
        local simPoints = self:getPositions()  -- Sim points, not reconnoitring elements
        for i = 1, #simPoints do
            res[ #res + 1 ] = CreateKnowledge( sword.military.world.Point, simPoints[i] ) -- create point as reconnoitring elements
        end
        return res
    end,
}
