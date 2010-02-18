defaultMethods
{
    -- OBSERVABLE
    hostilityLevel = function() return default_engine.methods.load( "unit_hostilityLevel" ) end,
    isHostile = function () return default_engine.predicates.load( "generic_isHostile") end,
    isNeutral = function () return default_engine.methods.load( "generic_isNeutral") end,
    isFriend = function () return default_engine.methods.load( "generic_isFriend" ) end,
    perceptionLevel = function() return default_engine.methods.load( "element_perceptionLevel") end,
    observationPriority = function() return default_engine.methods.load( "generic_observationPriority" ) end,
    
    -- REACHABLE
    proximityLevel = function() return default_engine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return default_engine.methods.load( "accessibilityLevel") end,
    reachPriority = function() return default_engine.methods.load( "reachPriority") end,
    isDistant = function() return default_engine.methods.load( "generic_isDistant" ) end,
    isNearby = function() return default_engine.methods.load( "generic_isNearby" ) end,
    isFar = function() return default_engine.methods.load( "generic_isFar" ) end,
    isReached = function() return default_engine.methods.load( "generic_isReached" ) end,
    
    -- POSITIONNABLE
    isInMyAOR = function() return default_engine.predicates.load( "isInMyAOR") end,
    canTakePosition = function() return default_engine.methods.load( "canTakePosition" ) end,
    
    -- REACHING
    reachEfficiency = function() return default_engine.methods.load( "reachEfficiency" ) end,
    isReachingFor = function() return default_engine.predicates.load( "isReachingFor" ) end,    
}
return
{
    -- OBSERVABLE
    computeRelation = function( self )
        return integration.computeRelationObject( self )
    end,
    
    getPerception = function( self )
      return integration.getObjectPerception( self )
    end,
      
    observeIt = function( self )
        integration.observeIt( self )
    end,
    -- $$$ MIA: temp, to move in default military implementation
    isTotallyPerceived = function( self )
        return self:perceptionLevel() == 100
    end,
    isPartiallyPerceived = function( self )
        return self:perceptionLevel() > 25
    end,
    -- REACHABLE
    computeDistance = function( self, target )
        return integration.normalizedInversedDistance( self, target )
    end,
    moveToIt = behavior_model.integration.startStopAction( { start = integration.startMoveToIt, started = integration.updateMoveToIt, stop = integration.stopMoveToIt } ),
    -- OBSERVABLE
    observeIt = function( self )
        integration.observeIt( self )
    end,
    -- POSITIONNABLE
    takeUpPosition = function( self )
        BreakForDebug( "call take up position ")
        return integration.takeUpPositionObject( self )
    end,
    occupationLevel = function()
      return 0 --TODO MGD
    end,
    safeApproachIt = function()
    end,
    isOccupied = function( self )
      return true -- TODO MGD
    end,
    -- ANIMATABLE
    animationPriority = function( self )
        return self:proximityLevel()
    end,
    canAnimateIt = function( self )
        return integration.canAnimateIt( self )
    end,
    animateIt = function( self )
        return integration.animateIt( self )
    end,
    predicate "isFullyAnimated"
    {
        method = function( self )
                    return animationLevel() == 100
                 end,
    },
    predicate "isPartiallyAnimated"
    {
        method = function( self )
                    return animationLevel() > 0
                 end,
    },
    animationLevel = function( self )
        return integration.animationLevel()
    end,
    -- ANIMATING
    predicate "isAnimatingFor"
    {
        method = function( self )
                    return self == objective
                 end,
    },
    animationEfficiency = function( self, objective )
        return self == objective
    end
}
