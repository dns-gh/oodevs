defaultMethods
{
    -- DEFAULT METHODS NEEDED UNTIL SECTION INHERITE FROM 
    -- net.masagroup.military.generic.world.Unit
    -- REACHABLE
    proximityLevel = function() return default_engine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return default_engine.methods.load( "accessibilityLevel") end,
    reachPriority = function() return default_engine.methods.load( "reachPriority") end,
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

}
return
{
    -- $$$ MIA: temp, to move in default military implementation
    isPerceived = function( self )
        return self:perceptionLevel() > 0
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
        return 0 -- TODO
    end,
    computeReconnaissanceCapability = function( target )
    	return 42 --TODO
    end
}