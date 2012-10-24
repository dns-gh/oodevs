return
{
    -- -------------------------------------------------------------------------------- 
    -- High level decisional methods
    -- --------------------------------------------------------------------------------
    reconnaissancePriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    reachEfficiency = function( self, objective )
        return ( ( self:proximityLevel() / 100 + self:getProximity( objective ) ) ) / 100
    end,
    safetyEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 +  meKnowledge:
            computeProtectionCapability( objective, self ) ) / 100
    end,
    observationEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computePerceptionCapability( objective, self ) ) / 100
    end,
    destructionEfficiency = function( self, objective )
         return self:proximityLevel() / 100
    end,
    neutralizationEfficiency = function( self, objective )
         return self:proximityLevel() / 100
    end,
    supportEfficiency = function( self, objective ) -- here objective is the element that is supported
         return ( self:proximityLevel() / 100
            + meKnowledge:computeSupportCapability( objective, self ) ) / 100
    end,
    reconnaissanceEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 
            +  meKnowledge:computeReconnaissanceCapability( objective, self ) ) / 100
    end,
    fleeEfficiency = function( self, objective )
        return ( ( self:proximityLevel() / 100 ) + self:getProximity( objective ) ) / 100
    end,
    isReconnoitred = function( self )
        return self:reconnaissanceLevel() == 100
    end,
    canReconnoitreIt = function( self )
        return self:isReached()
    end,
    approachEfficiency = function( self, objective )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    approachPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    reconnaissancePriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    reconnaissanceLevel = function( self )
        return self:getReconnaissanceState()
    end,
    searchLevel = function( self )
        return self:getSearchState()
    end,
    NBCrecoLevel = function( self )
        return self:getNBCState()
    end,
    canBuildIt = function( self )
        local objectType = self:getType()
        return self:buildingCapacity( objectType ) and self:buildingDotation( objectType )
    end,
    buildingCapacity = function( self )
        return integration.canBuildObjectType( self:getType(), self:getLocalisation() )
    end,
    buildingDotation = function( self )
        return integration.hasEnoughtDotationForObjectType( self:getType() )
    end,
    buildPriority = function( self )
      return math.max( self:proximityLevel(), 1 ) / 100
    end,
    constructionLevel = function( self )
        return integration.buildLevelUrbanBlock( self )
    end,
    buildEfficiency = function( self, objective )
        return self == objective and 1 or 0
    end,
    canBeImproved = function( self )
        return false
    end,
    elementIsReconnoitered = function( self )
        -- Do nothing
    end,
    isBeingReconnoiteredByMeOrNoOne = function( self )
       return true
    end,

    -- -------------------------------------------------------------------------------- 
    -- Predicates
    -- --------------------------------------------------------------------------------
    predicate "isReachingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeMovementCapability( objective, self ) > 0
        end
    },
    predicate "isReconnoitringFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeReconnaissanceCapability( objective, self ) > 0 
        end
    },
    predicate "isSafeguardingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeProtectionCapability( objective, self ) > 0
        end
    },
    predicate "isFleeingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeProtectionCapability( self ) > 0
        end
    },
    predicate "isApproachingFor"
    {
        -- $$$ TODO DDA meKnowledge:computeProtectionCapability( self ) ~= 0 
        -- $$$ TODO DDA ajouter la protection sans ennemi
        dependencies = "none",
        method = function( self, objective )
            if myself.location ~= self and self == objective then
                return true
            else
                local distance = myself.location 
                and myself.location:getProximity( objective ) 
                or objective:proximityLevel()
                return myself.location ~= self
                       and self:getProximity( objective ) > distance
            end
        end
    },
    predicate "isDestroyingFor"
    {
        -- a good destroying position is a position where agent can perceive the objective 
        --  and can make importante attrition on it.
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeDestructionCapability( objective, self ) > 0
                   and meKnowledge:computePerceptionCapability( objective, self ) > 0 
        end
    },
    predicate "isNeutralizingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeDestructionCapability( objective, self ) > 0
                   and meKnowledge:computePerceptionCapability( objective, self ) > 0 
        end
    },
    predicate "isObservingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge: computePerceptionCapability( objective, self ) > 0
        end
    },
    predicate "isSupportingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeSupportCapability( objective, self ) > 0
        end
    },
    predicate "isSearched"
    {
        dependencies = "none",
        method = function( self )
            return self:searchLevel() == 100
        end
    },
    predicate "isRecoNBC"
    {
        dependencies = "none",
        method = function( self )
            return self:NBCrecoLevel() == 100
        end
    },
    predicate "isBuilt"
    {
        method = function( self )
            if self[myself] then
                return self[myself].actionBuildState == eActionObjetTerminee
            else
                return self:constructionLevel() == 1 -- renvoie une valeur entre 0 et 1
            end
        end,
    },

    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    computeRelation = function( self ) 
        return 0 -- $$$ MIA TODO: pour le momment un BU est toujours amis.
    end,
    takeUpPosition = function( self )
      return integration.takeUpPosition( self )
    end,
    canApproachIt = function( self )
       return meKnowledge:isSupported()
    end,
    moveToIt = function( self, pathType )
        return meKnowledge:moveToItIntegration( self, pathType )
    end,
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end,
    getProximity = function( self, element )
        return integration.normalizedInversedDistance( self, element )
    end,
    getPerception = function( self )
        -- $$$ MIA TODO il ne s'agit du niveau de reconnaissance
        return integration.getUrbanBlockCurrentReconnaissanceState( self.source ) * 100
    end,
    getReconnaissanceState = function( self )
        return integration.getUrbanBlockReconnaissanceState( self )
    end,
    getSearchState = function( self )
        return integration.getUrbanBlockSearchState( self )
    end,
    reconnoitreIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startRecceUrbanBlock,
        stop = integration.stopRecceUrbanBlock
    } ),
    controlIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startControlUrbanBlock, 
        stop = integration.stopControlUrbanBlock 
    } ),
    searchIt = masalife.brain.integration.startStopAction( 
    {
        start = function( self )
            integration.startSearchUrbanBlock( self )
            myself.CRCaptureSomeone = {}
        end,
        
        started = function( self )
            integration.startedSearchUrbanBlock( self )
        end,
        stop = integration.stopSearchUrbanBlock
    } ),
    getNBCState = function( self )
        return integration.getUrbanBlockNBC( self )
    end,
    RecoNBC = masalife.brain.integration.startStopAction( 
    {
        start = function( self )
            integration.startNBCUrbanBlock( self )
        end,
        stop = integration.stopNBCUrbanBlock
    } ),
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
    end,
    buildIt = masalife.brain.integration.startStopAction(
    { 
        start = integration.startBuildItUrbanBlock,
        started = integration.updateBuildIt,
        stop = integration.stopBuildItUrbanBlock
    } ),
    -- -------------------------------------------------------------------------------- 
    -- Specific class methods (used by integration methods)
    -- --------------------------------------------------------------------------------
    getPosition = function( self )
        return integration.getUrbanBlockPosition( self )
    end,
    getMyPosition = function( self )
        return CreateKnowledge( world.Point, self:getPosition() )
    end,
    getPositions = function( self )
        return integration.getUrbanBlockPositions( self )
    end,
    getLocalisation = function( self )
        return integration.polylineUrbanBlock( self.source )
    end,
    getCoverAndConcealmentLevelFor = function( self, entity, objective )
        return integration.getUrbanBlockCoverAndConcealmentLevelFor( self, entity, objective )
    end,
    getTraficabilityLevelFor = function( self, forLoadedState )
        return integration.urbanBlockTrafficabilityLevel( self, forLoadedState )
    end,
    computePerceptionCapabilityFor = function( self, platoon, objectif )
        if integration.canDismount() then
            return self:getProximity( objectif ) -- hypothèse simplificatrice
        else
            return 0
        end
    end,
    computeSupportCapabilityFor = function( self, platoon, objective )
        if not self.trafficableComputed then
              self.trafficableComputed = { trafficable = platoon:canDismount() 
              or integration.isUrbanBlockTrafficable( platoon, meKnowledge:isMounted() ) }
        end
        if self.trafficableComputed.trafficable and (
            integration.distance( objective:getMyPosition(), self:getMyPosition() ) < 300 ) then
            return 100 else return 0 end
    end,
    sealOffIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startSealOffUrbanBlock,
        started = integration.startedSealOffUrbanBlock,
        stop = integration.stopSealOffUrbanBlock 
    } ),
    -- commit attak
    attackIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startAttackIt, 
        started = integration.updateAttackIt, 
        stop = integration.stopAttackIt
    } ),
    getType = function( self )
        return integration.getTypeUrbanBlock( self )
    end,
    plannedworkToWork = function( self )
        return self
    end,
    isInAttackRange = function( self, suicide, dotation )
        if suicide then return self:isReached() end
        return integration.distance( meKnowledge, self ) < integration.porteePourAttentat( dotation )
    end,
    getPositionsToReconnoiter = function( self )
        local res = {}
        local point
        res[ #res + 1 ] = self:getMyPosition()
        return res
    end,
    isManeuverObstacle = function( self )
        return false
    end,
    isActifManeuverObstacle = function( self )
        return false
    end,
    canBuildItNow = function( self )
        local objectType = self:getType()
        return integration.canBuildNowObjectType( objectType, self:getLocalisation() ) and integration.hasEnoughtDotationForObjectType( objectType )
    end,
}
