return
{
    -- $$$ MIA TODO rapatrier les implémentations dans ce fichier
    reconnaissancePriority = integration.generic_reconnaissancePriority,
    approachEfficiency = integration.approachEfficiency,
    approachPriority = integration.approachPriority,
    identificationEfficiency = integration.identificationEfficiency,
    fleeEfficiency = integration.fleeEfficiency,

    -- -------------------------------------------------------------------------------- 
    -- High level methods 
    -- --------------------------------------------------------------------------------
    canApproachIt = function( self )
        return meKnowledge:isSupported()
    end,
    canReconnoitreIt = function( self ) 
        return self:isReached()
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
    safetyEfficiency = function( self, objective )
       return( ( 100 - objective:getProximity( self ) ) + meKnowledge:computeProtectionCapability( objective, self ) ) / 200 
        -- $$$ MIA TODO: et si le point est dans un BU, récupérer la protection du BU?
    end,
    destructionEfficiency = function( self, objective )
         return self:proximityLevel() / 100
    end,
    neutralizationEfficiency = function( self, objective )
         return self:proximityLevel() / 100
    end,
    reachEfficiency = function( self, objective )
        return ( self:proximityLevel()/ 100 + self:getProximity( objective ) ) / 100
    end,
    reinforcementEfficiency = function( self, objective )
        return integration.normalizedInversedDistance( objective, self ) / 100
    end,
    supportEfficiency = function( self, objective ) -- here objective is the element that is supported
        local position = integration.getPositionToSupportFriend( objective ) 
        if position then
            return integration.normalizedInversedDistance( position, self ) / 100
        else
            return 0
        end
    end,
    reconnaissanceEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computeReconnaissanceCapability( objective, self ) ) / 100
    end,
    observationEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computePerceptionCapability( objective, self ) ) / 100
    end,
    bypassingEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + self:getProximity( objective ) ) / 100
    end,
    removalEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + self:getProximity( objective ) ) / 100
    end,
    elementIsReconnoitered = function( self )
        integration.AddReconnoiteredPointBy( self )
    end,
    activationEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computeBuildCapability( objective, self ) ) / 100  -- same hypothesis that build efficiency
    end,
    isBeingReconnoiteredByMeOrNoOne = function( self )
        return integration.pointIsReconnoitedByMeOrNoOne( self )
    end,

    -- -------------------------------------------------------------------------------- 
    -- Predicates
    -- --------------------------------------------------------------------------------
    predicate "isBypassingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return true
        end
    },
    predicate "isRemovingFor"
    {
        dependencies = "none",
        method = function( self, objective )
           return true
        end
    },
    predicate "isReachingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeMovementCapability( objective, self ) > 0 and integration.isElementInAOR( self )
        end
    },
    predicate "isObservingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computePerceptionCapability( objective, self ) > 0
        end
    },
    predicate "isIdentifyingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeIdentificationCapability( objective, self ) > 66 -- $$$ MIA pourquoi 66?
        end
    },
    predicate "isSafeguardingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeProtectionCapability( objective, self ) > 0 and integration.isElementInAOR( self )
        end
    },
    predicate "isFleeingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeProtectionCapability( objective, self ) > 0 and integration.isElementInAOR( self )
        end
    },
    predicate "isApproachingFor"
    {
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
        -- A good destroying position is a position where agenbt can perceive the objective 
        -- and can make importante attrition on it.
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeDestructionCapability( objective, self ) > 0 and integration.isElementInAOR( self )
        end
    },
    predicate "isNeutralizingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeNeutralisationCapability( objective, self ) > 0 and integration.isElementInAOR( self )
        end
    },
    predicate "isReinforcingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return true
        end
    },
    
    predicate "isSupportingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return ( meKnowledge:computeSupportCapability( objective, self ) > 0 ) and integration.isElementInAOR( self ) and self:supportEfficiency( objective ) > 0.8 
        end
    },
    predicate "isReconnoitringFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeReconnaissanceCapability( objective, self ) > 0 and integration.isElementInAOR( self )
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

    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    takeUpPosition = function( self )
        return integration.takeUpPosition( self )
    end,
    -- reachable action
    moveToIt = function( self, pathType )
        return meKnowledge:moveToItIntegration( self, pathType )
    end,
    -- observable action
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end,
    getProximity = function( self, reachable )
        return integration.normalizedInversedDistance( self, reachable )
    end,
    computeRelation = function( self )
        return 50 --Always neutral
    end,
    getPerception = function( self )
        -- perception d'un point: on estime qu'un point est perçu quand on est proche de lui
        return self:proximityLevel() -- $$$ MIA TODO faire un integration.getPerception ?
    end,
    getReconnaissanceState = function( self )
        return integration.getPointReconnaissanceState( self )
    end,
    getSearchState = function( self )
        return integration.getPointSearchState( self )
    end,
    reconnoitreIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startReccePoint, 
        stop = integration.stopReccePoint
    } ),
    isReconnoitred = function( self )
        return self:reconnaissanceLevel() == 100 
    end,
    searchIt = masalife.brain.integration.startStopAction( 
    {
          start = function( self )
            integration.startSearchPoint( self )
            myself.CRCaptureSomeone = {}
        end,
        started = function( self)
                    integration.capture( integration.getTerroristsInObjective( self ), eRC_TerroristCaptured )
                  end,
        stop = integration.stopSearchPoint
    } ),
    getNBCState = function( self )
        return integration.getPointNBC( self )
    end,
    RecoNBC = masalife.brain.integration.startStopAction( 
    {
        start = function( self )
            integration.startNBCPoint( self )
        end,
        stop = integration.stopNBCPoint
    } ),
    applyFireOnIt = masalife.brain.integration.startStopAction(
    { 
        start = integration.startApplyFireOnPoint, 
        started = integration.updateApplyFireOnPoint, 
        stop = integration.stopApplyFireOnPoint
    } ),
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
    end,
    controlIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startControlPoint, 
        stop = integration.stopControlPoint
    } ),

    -- -------------------------------------------------------------------------------- 
    -- Specific class methods used by integration methods
    -- --------------------------------------------------------------------------------
    getPosition = function( self )
        return integration.getPointPosition( self )
    end,
    getMyPosition = function( self )
        return CreateKnowledge( sword.military.world.Point, self:getPosition() )
    end,
    getPositions = function( self )
        return integration.getPointPositions( self )
    end,
    isOnRangeFor = function( self, munition )
        return integration.isOnRangeFor( self, munition )
    end,
    getTraficabilityLevelFor = function( self, forLoadedState )
        -- Renvoi vrai si le point n'est pas dans un BU ou dans un BU trafficable
        return integration.isPointInUrbanBlockTrafficable( self, forLoadedState )
    end,
    getCoverAndConcealmentLevelFor = function( self, entity, objective )
        local scalaire = 0.50
        if DEC_HasMission( meKnowledge.source ) then
            local mission = DEC_GetRawMission( meKnowledge.source )
            local dirDanger = DEC_GetDirectionDanger( mission )
            local dirEni = DEC_Geometrie_CreerDirection(meKnowledge:getPosition(), self:getPosition())
            
            local xDanger = dirDanger:DEC_Geometrie_X()
            local yDanger = dirDanger:DEC_Geometrie_Y()
            local xEni = dirEni:DEC_Geometrie_X()
            local yEni = dirEni:DEC_Geometrie_Y()
            scalaire = ( xDanger * xEni ) + ( yDanger * yEni )            
        end

        local perception = 100
        if integration.getPerception( self, objective ) > 0 then -- l'ennemi me voit
            return (1 - scalaire) * 35 -- resultat en 0 et 70
        else
            if scalaire > 0 then
                return (1 - scalaire) * 50 -- resultat en 0 et 100
            else
                return (1 - scalaire) * 35 -- resultat en 0 et 70
            end
        end
    end,
    computeSupportCapabilityFor = function( self, platoon, objective )
        if not self.trafficableComputed then
          self.trafficableComputed = { trafficable = platoon:canDismount() 
          or 100 == integration.isPointInUrbanBlockTrafficable( self, true ) }
        end
        if self.trafficableComputed.trafficable then 
            return self:getProximity( objective ) 
        else 
            return 0 
        end
    end,
    computePerceptionCapabilityFor = function( self, platoon, objectif )
        if 0 == integration.isPointInUrbanBlockTrafficable( self, true ) then
            if integration.canDismount() then
                return self:getProximity( objectif ) -- hypothèse simplificatrice
            else
                return 0
            end
        else -- le point n'est pas dans un BU trafficable
            if integration.getPerception( self, objectif ) > 0 then
                return 100 else return 0
            end
        end
    end,
    getPositionsToReconnoiter = function( self )
        local res = {}
        local point
        res[ #res + 1 ] = self
        return res
    end,
    
    isVisible = function( self )
        return integration.isPointVisible( self )
    end,
    isInAttackRange = function( self, suicide, dotation )
        if suicide then return self:isReached() end
        return integration.distance( meKnowledge, self ) < integration.porteePourAttentat( dotation )
    end,
    attackIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startAttackIt, 
        started = integration.updateAttackIt, 
        stop = integration.stopAttackIt
    } ),
   firePermitted = function( self )
        return integration.firePermittedForPoint( self )
    end,
}
