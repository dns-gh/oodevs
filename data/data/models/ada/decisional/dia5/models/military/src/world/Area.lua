return
{
    -- $$$ MIA TODO rapatrier le code de ces méthodes.
    reconnaissancePriority = integration.generic_reconnaissancePriority,
    reachEfficiency = integration.reachEfficiency,
    approachPriority = integration.approachPriority,
    approachEfficiency = integration.approachEfficiency,
    reconnaissanceEfficiency = integration.reconnaissanceEfficiency,
    observationEfficiency = integration.observationEfficiency,

    -- -------------------------------------------------------------------------------- 
    -- High level methods
    -- --------------------------------------------------------------------------------
    takeUpPosition = function( self )
        return integration.takeUpPosition( self )
    end,
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
    destructionEfficiency = function( self, objective )
         return self:proximityLevel() / 100 
    end,
    neutralizationEfficiency = function( self, objective )
         return self:proximityLevel() / 100 
    end,
    isReconnoitred = function( self ) 
        return self:reconnaissanceLevel() >= 60
    end,
    elementIsReconnoitered = function( self )
        -- Do nothing
    end,
    isBeingReconnoiteredByMeOrNoOne = function( self )
       return true
    end,
    activateRadar = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startActivateRadar,
        stop = integration.stopActivateRadar
    } ),
    radarOn = function( self )
       return integration.activateRadar( self )
    end,
    radarOff = function( self )
       return integration.deactivateRadar( self )
    end,
    activateRadarTirIndirect = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startActivateRadarTirIndirect,
        stop = integration.stopActivateRadarTirIndirect
    } ),
    getIntelligence = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startRensRecceArea,
        stop = integration.stopRensRecceArea
    } ),
    getIntelligenceForEo = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startRecoEo,
        stop = integration.stopRecoEo
    } ),
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
    predicate "isApproachingFor"
    {
     -- $$ TODO DDA  meKnowledge:computeProtectionCapability( self ) ~= 0  
     -- !! ajouter la protection sans ennemi
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
    predicate "isReconnoitringFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return self == objective -- only THIS area allows to reconnoitre itself.
      end
    },
    predicate "isDestroyingFor"
    {
        -- a good destroying position is a position where agent can perceive the objective 
        --  and can apply attrition on it.
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
            return meKnowledge:computeNeutralisationCapability( objective, self ) > 0
                   and meKnowledge:computePerceptionCapability( objective, self ) > 0 
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
    -- INTEGRATION METHODS
    -- --------------------------------------------------------------------------------
    moveToIt = function( self, pathType )
        return meKnowledge:moveToItIntegration( self, pathType )
    end,
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end, 
    getTraficabilityLevelFor = function( self, forLoadedState )
        return integration.isPointInUrbanBlockTrafficable( self, forLoadedState )
    end,
    getPosition = function( self )
        return integration.getAreaPosition( self )
    end,
    getMyPosition = function( self )
        return CreateKnowledge( world.Point, self:getPosition() )
    end,
    getPositions = function( self )
        return integration.getAreaPositions( self )
    end,
    getBorderPosition = function( self )
        return CreateKnowledge( world.Point, integration.computePointNearLocationsInsideAOR( self.source ) )
    end,
    getProximity = function( self, reachable )
        return integration.normalizedInversedDistance( self, reachable )
    end,
    computeRelation = function( self )
        return 100 --TODO
    end,
    computeMovementCapability = function( self )
        return 100 -- TODO -- $$$ MIA: not used in skill yet... 
    end,
    getPerception = function( self )
        return 0 -- TODO
    end,
    getReconnaissanceState = function( self )
        return integration.getAreaReconnaissanceState( self )
    end,
    getSearchState = function( self )
        return integration.getAreaSearchState( self )
    end,
    getLocalisation = function ( self )
        return self.source
    end,
    affectMobility = masalife.brain.integration.startStopAction(
    {
        start = integration.startAffectMobility,
        started = integration.updateAffectMobility,
        stop = integration.stopAffectMobility
    } ),
    reconnoitreIt = masalife.brain.integration.startStopAction(
    {
        start = integration.startRecceArea,
        stop = integration.stopRecceArea
    } ),
    searchIt = masalife.brain.integration.startStopAction( 
    {
        start = function( self )
            integration.startSearchArea( self )
            myself.CRCaptureSomeone = {}
        end,
        started = function( self )
                    integration.capture( integration.getTerroristsInObjective( self ), eRC_TerroristCaptured )
                  end,
        stop = integration.stopSearchArea
    } ),
    getNBCState = function( self )
        return integration.getAreaNBC( self )
    end,
    RecoNBC = masalife.brain.integration.startStopAction( 
    {
        start = integration.startNBCArea,
        stop = integration.stopNBCArea
    } ),
    controlIt = masalife.brain.integration.startStopAction(
    {
        start = integration.startControlArea,
        stop = integration.stopControlArea
    } ),
    computePerceptionCapabilityFor = function( self, platoon, objectif )
        if 0 == integration.isPointInUrbanBlockTrafficable( self, true ) then
            return self:getProximity( objectif ) -- hypothèse simplificatrice
        else -- le point est en ZO
            if integration.getPerception( self, objectif ) > 0 then
                return 100 else return 0
            end
        end
    end,
    sealOffIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startSealOffArea,
        started = integration.startedSealOffArea,
        stop = integration.stopSealOffArea 
    } ),
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
    end,
    channelIt = masalife.brain.integration.startStopAction( 
    {
        start = function( self ) meKnowledge:RC( eRC_AnimationCanalisation ) end,
        started = integration.channelIt,
    } ),
    getPositionsToReconnoiter = function( self )
        local res = {}
        res[ #res + 1 ] = self:getMyPosition()
        return res
    end,
    decontaminateIt = function ( self )
        local zoneLocalisation = integration.createLocationPolyline( integration.locationInPoints( self.source ) )
        integration.decontaminateArea( zoneLocalisation )
        meKnowledge:RC( eRC_ZoneDecontaminee )
        return true
    end,
}
