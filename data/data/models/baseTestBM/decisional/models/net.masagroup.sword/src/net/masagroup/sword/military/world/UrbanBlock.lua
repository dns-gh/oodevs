defaultMethods
{
    -- REACHABLE
    proximityLevel = function() return default_engine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return default_engine.methods.load( "accessibilityLevel") end,
    reachPriority = function() return default_engine.methods.load( "reachPriority") end,
    isReachingFor = function() return default_engine.methods.load( "isReachingFor" ) end,
    isDistant = function() return default_engine.methods.load( "isDistant" ) end,
    isNearby = function() return default_engine.methods.load( "isNearby" ) end,
    isFar = function() return default_engine.methods.load( "isFar" ) end,
    isReached = function() return default_engine.methods.load( "isReached" ) end,

   -- OBSERVABLE
    hostilityLevel = function() return default_engine.methods.load( "unit_hostilityLevel" ) end,
    isHostile = function () return default_engine.predicates.load( "isHostile") end,
    isNeutral = function () return default_engine.methods.load( "isNeutral") end,
    isFriend = function () return default_engine.methods.load( "isFriend" ) end,
    perceptionLevel = function() return default_engine.methods.load( "perceptionLevel") end,
    observationPriority = function() return default_engine.methods.load( "observationPriority" ) end,

    -- RECONNOITRABLE
    reconnaissancePriority = function() return default_engine.methods.load( "reconnaissancePriority" ) end,
    canReconnoitreIt = function() return default_engine.methods.load( "canReconnoitreIt" ) end,
    reconnaissanceLevel = function() return default_engine.methods.load( "reconnaissanceLevel" ) end,
    isRecognized = function() return default_engine.methods.load( "keypoint_isRecognized" ) end,
    reconnoitreIt = function() return default_engine.methods.load( "reconnoitreIt" ) end,
    
    -- POSITIONNABLE
    isInMyAOR = function() return default_engine.predicates.load( "isInMyAOR") end,
    canTakePosition = function() return default_engine.methods.load( "canTakePosition" ) end,
    
    -- REACHING
    reachEfficiency = function() return default_engine.methods.load( "reachEfficiency" ) end,
    
    -- RECONNOITRING
    reconnaissanceEfficiency = function() return default_engine.methods.load( "reconnaissanceEfficiency" ) end,
    isReconnoitringFor = function() return default_engine.predicates.load( "terrain_analysis_isReconnoitringFor" ) end,   
}
return
{
    -- $$$ MIA: temp, to move in default military implementation
    isPerceived = function( self )
        return self:perceptionLevel() > 0
    end,
    takeUpPosition = function( point )
        --NOTHING
    end,
    safeApproachIt = function( point )
        --TODO
    end,
    occupationLevel = function( self )
      --TODO
    end,
    isSafety = function( area )
        --TODO
    end,
    -- INTEGRATION METHODS
    -- reachable action
    moveToIt = function( self )
        return integration.moveToIt( self )
    end,
    -- observable action
    observeIt = function( self )
        --TODO
    end,
    -- Tactical analysis integration
    computeDistance = function( self, target )
        return integration.normalizedInversedDistance( self, target )
    end,
    computeRelation = function( self )
        return 100 --TODO
    end,
    computePerceptionCapability = function( self )
     return 0 -- TODO
    end,
    computeMovementCapability = function( self )
        return 100 -- TODO -- $$$ MIA: not used in skill yet... 
    end,
    getPerception = function( self )
        if timer then
            timer = timer + 1
        else
            timer = 0
        end
        return timer -- TODO MGD Replace by DEC_function
    end,
    getReconnaissanceState = function( self ) 
       return 0 -- TODO
    end,
    recce = function( self )
        return false -- TODO
    end,
    -- TEMP FUNCTIONS
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
}
