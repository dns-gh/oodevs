-- MASA Life Brain Neutralizable knowledge interface
-- <br/>This file registers the modelling level implementations of the Neutralizable interface methods.

--------------------------------------------------------
-- @name unit_operationalLevel
-- @class function
-- @description Return the operational Level of a knowledge.
-- @param self The knowledge.
-- @return The value of the operationLevel, between 0 and 100.
-- deprecated
integration.unit_operationalLevel = function( self )
    if self.getOperationalCapacity then
        return self:getOperationalCapacity()
    else
        return 100
    end
end

--------------------------------------------------------
-- @name generic_isOperational
-- @class function
-- @description Return true if the knowledge is operational.
-- @param self The knowledge.
-- @return Return true if the knowledge is operational, false otherwise.
-- deprecated
integration.generic_isOperationalPredicate = {
    dependencies = { self = { "operationalLevel_" } },
    method = function( self )
        return self:operationalLevel() > 70
    end
}

--------------------------------------------------------
-- @name generic_neutralizationPriority
-- @class function
-- @description Return the priority of a knowledge to neutralize it.
-- @param self The knowledge to neutralize.
-- @return The value of the priority, between 0 and 1.
-- deprecated
integration.generic_neutralizationPriority = function( self )
    return math.max( self:proximityLevel(), 1 ) / 100
end

