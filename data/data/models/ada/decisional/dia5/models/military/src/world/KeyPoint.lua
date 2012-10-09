-- -------------------------------------------------------------------------------- 
-- Predicates
-- --------------------------------------------------------------------------------
predicate "isInMyAOR"
{
    dependencies = { "none" },
    method = function( self )
        return true
    end,
}
predicate "isLoadingFor"
{
    dependencies = { "none" },
    method = function( self, element )
        return meKnowledge:computeLoadCapability( element, self ) > 0
    end,
}
predicate "isUnloadingFor"
{
    dependencies = { "none" },
    method = function( self, element )
        return meKnowledge:computeUnloadCapability( element, self ) > 0
    end,
}
predicate "isBuildingFor"
{
    dependencies = ( "none" ),
    method = function( self, element )
        return meKnowledge:computeBuildCapability( element, self ) > 0
    end,
}

return
{
    -- -------------------------------------------------------------------------------- 
    -- Decisionnal methods
    -- --------------------------------------------------------------------------------
    canTakePosition = function( self )
        return self:isReached()
    end,
    occupationLevel = function( self )
        return 0
    end,
    isOccupied = function( self )
        return self:occupationLevel() == 100
    end,
    loadEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computeLoadCapability( objective, self ) ) / 100
    end,
    unloadEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computeLoadCapability( objective, self ) ) / 100
    end,
    buildEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 + meKnowledge:
                   computeBuildCapability( objective, self ) ) / 100
    end,
    isContaminated = function( self )
        return integration.isPositionContaminated( self )
    end
}