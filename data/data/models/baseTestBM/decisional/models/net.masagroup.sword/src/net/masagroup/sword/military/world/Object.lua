defaultMethods
{
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
    -- OBSERVABLE
    computeRelation = function( self )
        return integration.computeRelationObject( self )
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
}
