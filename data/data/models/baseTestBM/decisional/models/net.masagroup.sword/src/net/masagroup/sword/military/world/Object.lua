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

}
return
{
    -- OBSERVABLE
    computeRelation = function( self )
        return integration.computeRelationObject( self )
    end,
    
    getPerception = function( self )
      BreakForDebug( "call getPerception object" )
      return integration.getObjectPerception( self )
    end,
      
    observeIt = function( self )
        --TODO
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
    moveToIt = function( self )
        return integration.moveToIt( self )
    end,
    -- OBSERVABLE
    observeIt = function( self )
        --TODO
    end,
}
