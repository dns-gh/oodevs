--- Returns the efficiency of the given entity to observe the given objective.
-- This method can only be called by an entity defining a "computePerceptionCapability" method returning a float between 0 and 100
-- @param entity Knowledge defining a "proximityLevel" method returning a float between 0 and 100
-- @param objective Knowledge, the objective to observe
-- @return Float between 0 and 1, 0 meaning the lowest efficiency and 1 meaning the highest efficiency
integration.observationEfficiency = function( entity, objective )
    return ( ( masalife.brain.knowledge.me.body:computePerceptionCapability( objective, entity ) * math.max( entity:proximityLevel(), 1 ) ) / 100 ) / 100
end

--- Returns the efficiency of the given entity to approach the given objective.
-- @param entity Knowledge defining a "proximityLevel" method returning a float between 0 and 100
-- @param objective Knowledge, the objective to approach
-- @return Float between 0 and 1, 0 meaning the lowest efficiency and 1 meaning the highest efficiency
integration.approachEfficiency = function( entity, objective )
    return math.max( entity:proximityLevel(), 1 ) / 100
end

--- Returns the efficiency of the given entity to identify the given objective.
-- This method can only be called by an entity defining a "computeIdentificationCapability" method returning a float between 0 and 100
-- @param entity Knowledge defining a "proximityLevel" method returning a float between 0 and 100
-- @param objective Knowledge, the objective to identify
-- @return Float between 0 and 1, 0 meaning the lowest efficiency and 1 meaning the highest efficiency
integration.identificationEfficiency = function( entity, objective )
    return ( entity:proximityLevel() / 100 + masalife.brain.knowledge.me.body:computeIdentificationCapability( objective, entity ) ) / 101
end

--- Returns the efficiency of the given entity to flee the given objective.
-- @param entity Knowledge defining a "getProximity" method returning a float between 0 and 100
-- @param objective Knowledge, the objective to flee from
-- @return Float between 0 and 1, 0 meaning the lowest efficiency and 1 meaning the highest efficiency
integration.fleeEfficiency = function( entity, objective )
    return ( 100 - entity:getProximity( objective ) ) / 100
end

local estimateReconnaissanceLevel = function( self, objective )
    estimateReconnaissanceLevels = estimateReconnaissanceLevels or setmetatable( {}, { __mode = "kv" } )
    if not estimateReconnaissanceLevels[ objective ] then
        local values = {}
        values[ self ] = meknowledge.computeReconnaissanceCapability 
                        and meknowledge:computeReconnaissanceCapability( objective, self ) or 100
        estimateReconnaissanceLevels[ objective ] = values
    else
        if not estimateReconnaissanceLevels[ objective ][ self ] then
            estimateReconnaissanceLevels[ objective ][ self ] = meknowledge.computeReconnaissanceCapability 
                                                                and meknowledge:computeReconnaissanceCapability( objective, self ) or 100
        end
    end
    return estimateReconnaissanceLevels[ objective ][ self ]
end

--- Returns the efficiency of the given entity to reconnoiter the given objective.
-- @param entity Knowledge defining a "proximityLevel" method returning a float between 0 and 100
-- @param objective Knowledge, the objective to reconnoiter
-- @return Float between 0 and 1, 0 meaning the lowest efficiency and 1 meaning the highest efficiency
integration.reconnaissanceEfficiency = function( entity, objective )
    return ( entity:proximityLevel() / 100 + estimateReconnaissanceLevel( entity, objective ) ) / 101
end

local isInNearestNeighbors = function( position, objective )
    return objective:getProximity( position ) > 0 and masalife.brain.knowledge.me.body:computeMovementCapability( position ) > 0
end
------------------------------------------------------------------
-- @name isReachingFor
-- @class function
-- @description boolean knowledge class method that returns true whether the indexed knowledge instance is a good position to
-- reach a given objective ( i.e. a close enough position ).If used, both "me" and the indexed knowledge instance must have a
-- terrainAnalysisPosition_ attribute.
-- @return true whether the indexed knowledge is a good position, or false otherwise.
-- Deprecated
integration.isReachingForPredicate = {
    dependencies = { me = { "location" }, objective = { "terrainAnalysisPosition_" },  },
    method = function( self, objective )

                    if self == objective and masalife.brain.knowledge.me.body:computeMovementCapability( objective ) > 0 then
                        return true
                    else
                        return isInNearestNeighbors( self, objective )
                    end
            end
}

--- Returns the efficiency of the given entity to reach the given objective.
-- @param entity Knowledge defining "proximityLevel" and "getProximity" methods,
-- both returning a float between 0 and 100
-- @param target Knowledge, the objective to reach
-- @return Float between 0 and 1, 0 meaning the lowest efficiency and 1 meaning the highest efficiency
integration.reachEfficiency = function( entity, target )
    return ( ( entity:proximityLevel() / 100 + entity:getProximity( target ) ) ) / 101
end

------------------------------------------------------------------
-- @name isIdentifyingFor
-- @class function
-- @description Method which returns if a knowledge is a good position to identify a set of objectives. From a good position,
-- a potential identification of the objective is possible thanks to the identification capability of the agent.
-- @param self the position to be used for identification
-- @param objective the objective to observe
-- @return true, if the position respects the previous contraints, false otherwise.
-- Deprecated
integration.isIdentifyingForPredicate = {
    dependencies = { me = { "terrainAnalysisPosition_" }, objective = { "terrainAnalysisPosition_" } },
    method = function( self, objective )
                return masalife.brain.knowledge.me.body:computeMovementCapability( self ) ~= 0 and masalife.brain.knowledge.me.body:computeIdentificationCapability( objective, self ) > 66
             end
}