-- -------------------------------------------------------------------------------- 
-- Predicates
-- --------------------------------------------------------------------------------
predicate "isFar"
{ 
    dependencies = "none",
    method = function( self )
        return self:proximityLevel() <= 50 and self:proximityLevel() > 0
    end
}
predicate "isAccessible"
{ 
    dependencies = "none",
    method = function( self )
        return self:accessibilityLevel() ~= 0
    end
}

return
{
    -- -------------------------------------------------------------------------------- 
    -- Decisionnal methods
    -- --------------------------------------------------------------------------------
    reachPriority = function( self )
        return ( math.max( self:proximityLevel(), 1 ) ) / 100
    end,
    isNearby = function( self )
        return self:proximityLevel() > 50
    end,
    isDistant = function( self )
        return self:proximityLevel() == 0
    end,
    isReached = function( self )
        return self:proximityLevel() == 100
    end,
    proximityLevel = function( self )
        return self:getProximity( meKnowledge )
    end,
    accessibilityLevel = function( self )
        return meKnowledge:computeMovementCapability( self ) * 100
    end,
}
