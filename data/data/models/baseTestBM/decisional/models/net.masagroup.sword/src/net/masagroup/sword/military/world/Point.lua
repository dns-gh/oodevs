defaultMethods
{
    -- REACHABLE
    proximityLevel = function() return default_engine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return default_engine.methods.load( "accessibilityLevel") end,
    reachPriority = function() return default_engine.methods.load( "reachPriority") end,
    isDistant = function() return default_engine.methods.load( "generic_isDistant" ) end,
    isNearby = function() return default_engine.methods.load( "generic_isNearby" ) end,
    isFar = function() return default_engine.methods.load( "generic_isFar" ) end,
    isReached = function() return default_engine.methods.load( "generic_isReached" ) end,

    -- OBSERVABLE
    hostilityLevel = function() return default_engine.methods.load( "unit_hostilityLevel" ) end,
    isHostile = function () return default_engine.predicates.load( "generic_isHostile") end,
    isNeutral = function () return default_engine.methods.load( "generic_isNeutral") end,
    isFriend = function () return default_engine.methods.load( "generic_isFriend" ) end,
    perceptionLevel = function() return default_engine.methods.load( "element_perceptionLevel") end,
    observationPriority = function() return default_engine.methods.load( "generic_observationPriority" ) end,

    -- RECONNOITRABLE
    reconnaissancePriority = function() return default_engine.methods.load( "reconnaissancePriority" ) end,
    canReconnoitreIt = function() return default_engine.methods.load( "canReconnoitreIt" ) end,

    -- POSITIONNABLE
    isInMyAOR = function() return default_engine.predicates.load( "isInMyAOR") end,
    canTakePosition = function() return default_engine.methods.load( "canTakePosition") end,

    -- REACHING
    reachEfficiency = function() return default_engine.methods.load( "reachEfficiency" ) end,
    isReachingFor = function() return default_engine.predicates.load( "isReachingFor" ) end,    
    
    -- RECONNOITRING
    reconnaissanceEfficiency = function() return default_engine.methods.load( "reconnaissanceEfficiency" ) end,
    isReconnoitringFor = function() return default_engine.predicates.load( "terrain_analysis_isReconnoitringFor" ) end,

    -- OBSERVING
    observationEfficiency = function() return default_engine.methods.load( "observationEfficiency" ) end,
    isObservingFor = function() return default_engine.predicates.load( "terrain_analysis_isObservingFor" ) end,

    -- DESTROYABLE
    destructionEfficiency = function() return default_engine.methods.load( "destructionEfficiency" ) end,
    
    -- IDENTIFYING
  isIdentifyingFor = function() return default_engine.predicates.load( "terrain_analysis_isIdentifyingFor" ) end,
  identificationEfficiency = function() return default_engine.methods.load( "identificationEfficiency" ) end,
}
return
{

    -- $$$ MIA: temp, to move in default military implementation
    isTotallyPerceived = function( self )
      return self:perceptionLevel() == 100
    end,
    isPartiallyPerceived = function( self )
      return self:perceptionLevel() > 25
    end,
    -- POSITIONNABLE
    takeUpPosition = function( self )
        return integration.takeUpPosition( self )
    end,
    --
    safeApproachIt = function( point )
        --TODO
    end,
    occupationLevel = function( self )
      return 0 --TODO MGD
    end,
    -- INTEGRATION METHODS
    -- reachable action
    moveToIt = behavior_model.integration.startStopAction( { start = integration.startMoveToIt, started = integration.updateMoveToIt, stop = integration.stopMoveToIt } ),
    -- observable action
    observeIt = function( self )
        integration.observeIt( self )
    end,
    -- Tactical analysis integration
    computeDistance = function( self, target )
        return integration.normalizedInversedDistance( self, target )
    end,
    computeRelation = function( self )
        return 50 --Always neutral
    end,
    computePerceptionCapability = function( self )
     return 0 -- TODO
    end,
    computeMovementCapability = function( self )
        return 100 -- TODO -- $$$ MIA: not used in skill yet...
    end,
    getPerception = function( self )
        return 0 -- TODO
    end,
     -- RECONNOITRABLE -- @TODO use default function when caro will commit changes
    reconnaissanceLevel = function( self )
        return self:getReconnaissanceState()
    end,
    getReconnaissanceState = function( self )
        return integration.getPointReconnaissanceState( self )
    end,
    reconnoitreIt = function( self )
        return self:recce()
    end,
    recce = behavior_model.integration.startStopAction( { start = integration.startReccePoint, stop = integration.startReccePoint } ),
    isRecognized = function( self ) 
        return self:reconnaissanceLevel() == 100 
    end,
    estimatedReconnaissanceLevel = function( self, objective )
        if not estimatedReconnaissanceLevels[ objective ] then
            local values = {}
            values[ self ] = kBase.me.body:computeReconnaissanceCapability( objective, self )
            estimatedReconnaissanceLevels[ objective ] = values
        else
            if not estimatedReconnaissanceLevels[ objective ][ self ] then
                estimatedReconnaissanceLevels[ objective ][ self ] = kBase.me.body:computeReconnaissanceCapability( objective, self )
            end
        end
        return estimatedReconnaissanceLevels[ objective ][ self ]
    end,
    
    isOccupied = function( self )
        return true -- TODO
    end,

    computeIdentificationCapability = function ( self, objective )
        return self:computeDistance( objective )
    end,
}
