------------------------------------------------------------------
--- OBSERVING INTERFACE IMPLEMENTATION SPECIFIC TO A KEYPOINT
------------------------------------------------------------------
integration.observationEfficiency = function( self, objective )
    -- fonction des capacités de visibilité et de l'accessibilityLevel normalement
    return ( ( masalife.brain.knowledge.me.body:computePerceptionCapability( objective, self ) * math.max( self:proximityLevel(), 1 ) ) / 100 ) / 100
end

-------------------------------------------------------------------
--- APPROACHING INTERFACE IMPLEMENTATION SPECIFIC TO A KEYPOINT
-------------------------------------------------------------------
integration.approachEfficiency = function( self, objective )
    return math.max( self:proximityLevel(), 1 ) / 100
end

------------------------------------------------------------------
--- IDENTIFYING INTERFACE IMPLEMENTATION SPECIFIC TO A KEYPOINT
-------------------------------------------------------------------
integration.identificationEfficiency = function( self, objective )
    return ( self:proximityLevel() / 100 + masalife.brain.knowledge.me.body:computeIdentificationCapability( objective, self ) ) / 101
end

------------------------------------------------------------------
--- FLEEING INTERFACE IMPLEMENTATION SPECIFIC TO A KEYPOINT
-------------------------------------------------------------------
integration.fleeEfficiency = function( self, objective )
    return ( 100 - self:getProximity( objective ) ) / 100
end


-------------------------------------------------------------------
--- RECONNOITRING INTERFACE IMPLEMENTATION SPECIFIC TO A KEYPOINT
-- deprecated
------------------------------------------------------------------
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

integration.reconnaissanceEfficiency = function( self, objective )
    return ( self:proximityLevel() / 100 + estimateReconnaissanceLevel( self, objective ) ) / 101
end

---
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

--
-- @name reachEfficiency
-- @class function
-- @description knowledge class method that returns the efficiency of a position considering the issue
-- of reaching an given objective. The indexed knowledge and the objective must have a
-- terrainAnalysisPosition_ attribute.
-- @param source The knowledge of the source to compute efficiency from.
-- @param target The knowledge of the target to compute efficiency to.
-- @return The efficiency between the knowledges, a [0..100] value where 100 means high efficiency and 0 poor efficiency.
--
integration.reachEfficiency = function( source, target )
    return ( ( source:proximityLevel() / 100 + source:getProximity( target ) ) ) / 100
end

------------------------------------------------------------------
-- @name isIdentifyingFor
-- @class function
-- @description Method which returns if a knowledge is a good position to identify a set of objectives. From a good position,
-- a potential identification of the objective is possible thanks to the identification capability of the agent.
-- @param self the position to be used for identification
-- @param objective the objective to observe
-- @return true, if the position respects the previous contraints, false otherwise.
integration.isIdentifyingForPredicate = {
    dependencies = { me = { "terrainAnalysisPosition_" }, objective = { "terrainAnalysisPosition_" } },
    method = function( self, objective )
                return masalife.brain.knowledge.me.body:computeMovementCapability( self ) ~= 0 and masalife.brain.knowledge.me.body:computeIdentificationCapability( objective, self ) > 66
             end
}