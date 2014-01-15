-- MASA Life Brain Observable knowledge interface
-- <br/>This file registers the modelling level implementations of the Observable interface methods.

--------------------------------------------------------
-- @name element_perceptionLevel
-- @class function
-- @description Return the perception level of the Element knowledge by the agent.
-- @param self The Element knowledge to observe.
-- @return Return the value of the perception level, between 0 and 100.
--
--- Deprecated
integration.element_perceptionLevel = function( self )
    return self:getPerception()
end

--------------------------------------------------------
-- @name generic_isFriend
-- @class function
-- @description Return true if the knowledge is considered as a friend by the agent. A knowledge is friendly if the hostility level value is
-- smaller than 33.
-- @param self The knowledge to observe.
-- @return Return true if the knowledge is friendly, false otherwise.
--
--- Deprecated
integration.generic_isFriend = function( self )
    return self:hostilityLevel() < 33
end

--------------------------------------------------------
-- @name generic_isNeutral
-- @class function
-- @description Return true if the knowledge is considered as neutral for the agent. A knowledge is neutral if the hostility level value is
-- between 33 and 66 included.
-- @param self The knowledge to observe.
-- @return Return true if the knowledge is neutral, false otherwise.
--
--- Deprecated
integration.generic_isNeutral = function( self )
    return self:hostilityLevel() >= 33 and self:hostilityLevel() <= 66
end

--------------------------------------------------------
-- @name generic_isHostile
-- @class function
-- @description Return true if the knowledge is considered as hostile for the agent. A knowledge is hostile if the hostility level value is upper
-- than 66.
-- @param self The knowledge to observe.
-- @return Return true if the knowledge is hostile, false otherwise.
--
--- Deprecated
integration.generic_isHostile = {
    dependencies = {},
    method = function( self )
                return self:hostilityLevel() > 66
             end
}

--------------------------------------------------------
-- @name generic_observationPriority
-- @class function
-- @description Return the priority for observe a knowledge. The priority depends on the perception level and the proximity level.
-- @param self The knowledge to observe.
-- @return The value of the observation priority, between 0 and 1.
--
--- Deprecated
integration.generic_observationPriority = function( self )
    return math.max( self:proximityLevel(), 1 ) / 100
end
