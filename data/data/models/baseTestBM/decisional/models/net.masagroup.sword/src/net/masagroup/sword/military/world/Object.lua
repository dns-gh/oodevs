defaultMethods
{
    -- OBSERVABLE
    hostilityLevel = function() return masalife.brain.defaultengine.methods.load( "unit_hostilityLevel" ) end,
    isHostile = function () return masalife.brain.defaultengine.predicates.load( "generic_isHostile") end,
    isNeutral = function () return masalife.brain.defaultengine.methods.load( "generic_isNeutral") end,
    isFriend = function () return masalife.brain.defaultengine.methods.load( "generic_isFriend" ) end,
    perceptionLevel = function() return masalife.brain.defaultengine.methods.load( "element_perceptionLevel") end,
    observationPriority = function() return masalife.brain.defaultengine.methods.load( "generic_observationPriority" ) end,
    
    -- REACHABLE
    proximityLevel = function() return masalife.brain.defaultengine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return masalife.brain.defaultengine.methods.load( "accessibilityLevel") end,
    reachPriority = function() return masalife.brain.defaultengine.methods.load( "reachPriority") end,
    isDistant = function() return masalife.brain.defaultengine.methods.load( "generic_isDistant" ) end,
    isNearby = function() return masalife.brain.defaultengine.methods.load( "generic_isNearby" ) end,
    isFar = function() return masalife.brain.defaultengine.methods.load( "generic_isFar" ) end,
    isReached = function() return masalife.brain.defaultengine.methods.load( "generic_isReached" ) end,
    
    -- POSITIONNABLE
    isInMyAOR = function() return masalife.brain.defaultengine.predicates.load( "isInMyAOR") end,
    canTakePosition = function() return masalife.brain.defaultengine.methods.load( "canTakePosition" ) end,
    
    -- REACHING
    reachEfficiency = function() return masalife.brain.defaultengine.methods.load( "reachEfficiency" ) end,
    isReachingFor = function() return masalife.brain.defaultengine.predicates.load( "isReachingFor" ) end,    
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
    getPosition = function( self )
      return integration.getObjectPosition( self )
    end,
    computeDistance = function( self, target )
        return integration.normalizedInversedDistance( self, target )
    end,
    moveToIt = masalife.brain.core.integration.startStopAction( { start = integration.startMoveToIt, started = integration.updateMoveToIt, stop = integration.stopMoveToIt } ),
    -- OBSERVABLE
    observeIt = function( self )
        integration.observeIt( self )
    end,
    -- POSITIONNABLE
    takeUpPosition = function( self )
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
