--- Computes the aggressiveness of the given agent knowledge towards this entity 
--- and returns an integer from 0 to 100, 100 meaning the agent knowledge is aggressive,
--- and 0 meaning it is not.
-- The agent knowledge is deemed aggressive if and only if it is engaging this entity.
-- @see integration.getKnowledgesUnitsEngaging
-- @param target Directia agent knowledge
-- @return Integer between 0 and 100
integration.computeAggressiveness = function( target )
    local firers = integration.getKnowledgesUnitsEngaging()
    if exists( firers, target.source ) then
        return 100
    end
    return 0
end

--- Returns this entity's force ratio
-- This method can only be called by an agent.
-- @return Float between 0 and 1, 0 meaning very unfavourable, and 1 meaning very favourable
integration.getForceRatio = function()
    return DEC_RapportDeForceLocal() / 5
end

--- Returns the given agent knowledge's force ratio
-- @param kAgent Directia agent knowledge
-- @return Float between 0 and 1, 0 meaning very unfavourable, and 1 meaning very favourable
integration.getAgentKnowledgeForceRatio = function( kAgent )
    local agent = DEC_ConnaissanceAgent_EnAgent( kAgent.source )
    return DEC_Agent_RapportDeForceLocal( agent ) / 5
end

--- Returns the given agent's force ratio
-- @param agent Directia agent
-- @return Float between 0 and 1, 0 meaning very unfavourable, and 1 meaning very favourable
integration.getForceRatioAgent = function( agent )
    return DEC_Agent_RapportDeForceLocal( agent.source ) / 5
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
integration.getEstimateAttrition = function( self, position )
    return 0
end

--- Deprecated
integration.getEstimateDangerosity = function( platoon )
    return ( DEC_ConnaissanceAgent_Dangerosite( platoon.source ) - 1 ) * 100
end