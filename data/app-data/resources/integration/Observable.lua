--- Deprecated
integration.element_perceptionLevel = function( self )
    return self:getPerception()
end

--- Deprecated
integration.generic_isFriend = function( self )
    return self:hostilityLevel() < 33
end

--- Deprecated
integration.generic_isNeutral = function( self )
    return self:hostilityLevel() >= 33 and self:hostilityLevel() <= 66
end

--- Deprecated
integration.generic_isHostile = {
    dependencies = {},
    method = function( self )
                return self:hostilityLevel() > 66
             end
}

--- Deprecated
integration.generic_observationPriority = function( self )
    return math.max( self:proximityLevel(), 1 ) / 100
end
