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
       return( ( ( 100 - objective:getProximity( self ) )/100 ) + meKnowledge:computeProtectionCapability( objective, self ) ) / 100 
        -- $$$ MIA TODO: et si le point est dans un BU, récupérer la protection du BU?
    end,
    destructionEfficiency = function( self, objective )
         return ( self:proximityLevel() / 100 + meKnowledge:
                    computeDestructionCapability( objective, self ) ) / 100
    end,
    neutralizationEfficiency = function( self, objective )
         return ( self:proximityLevel() / 100 + meKnowledge:
                  computeNeutralisationCapability( objective, self ) ) / 100
    end,
    reachEfficiency = function( self, objective )
        return ( self:proximityLevel()/ 100 + self:getProximity( objective ) ) / 100
    end,
    reinforcementEfficiency = function( self, objective )
        return integration.normalizedInversedDistance( objective, self ) / 100
    end,
    supportEfficiency = function( self, objective ) -- here objective is the element that is supported
         return ( self:proximityLevel() / 100
            + meKnowledge:computeSupportCapability( objective, self ) ) / 100
    end,
    reconnaissanceEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computeReconnaissanceCapability( objective, self ) ) / 100
    end,
    observationEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computePerceptionCapability( objective, self ) ) / 100
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
    predicate "isReachingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeMovementCapability( objective, self ) > 0
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
                   computeProtectionCapability( objective, self ) > 0
        end
    },
    predicate "isFleeingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeProtectionCapability( objective, self ) > 0
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
            return meKnowledge:computeDestructionCapability( objective, self ) > 0
        end
    },
    predicate "isNeutralizingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeNeutralisationCapability( objective, self ) > 0
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
            return meKnowledge:computeSupportCapability( objective, self ) > 0
        end
    },
    predicate "isReconnoitringFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeReconnaissanceCapability( objective, self ) > 0
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
                    integration.capture( integration.getDestroyableInObjective( self ), eRC_TerroristCaptured )
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
        -- HYPOTHESES: pour un point, la protection est assimilée au fait d'être non vu sur ce point
        -- we suppose that ennemi's perception on me is equal to my perception on objective's position 
        if  integration.getPerception( self, objective ) > 0 then 
            return 0 -- point is not concealed, means that it has no protection capability.
        else 
            return 100
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
}
