return
{
    -- OBSERVABLE
    perceptionLevel = function( self )
        return self:getPerception()
    end,
    observationPriority = function( self )
        return math.max( self:proximityLevel(), 1 ) / 100
    end,
    hostilityLevel = function( self )
        return ( 100 - self:computeRelation() )
    end,
    predicate "isHostile"
    {
        dependencies = "none",
        method = function( self )
            return self:hostilityLevel() > 66
        end
    },
    isNeutral = function( self )
        return self:hostilityLevel() >= 33 and self:hostilityLevel() <= 66
    end,
    isFriend = function( self )
        return self:hostilityLevel() < 33
    end,
    predicate "isPerceived"
    {
        dependencies = "none",
        method = function( self )
            return self:perceptionLevel() > 0
        end
    },
    isValid = function( self )
        return true
    end
}