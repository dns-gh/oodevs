-- --Safeguardable Implementation

integration.computeAggressiveness = function( target )
    local firers = integration.getKnowledgesUnitsEngaging()
    if exists(firers, target.source ) then
        return 100
    end
    return 0
end


integration.getEstimateAttrition = function(self, position)
  return 0
end

integration.getEstimateDangerosity = function( platoon )
    return ( DEC_ConnaissanceAgent_Dangerosite( platoon.source ) - 1 ) * 100
end

integration.getForceRatio = function( self )
    return DEC_RapportDeForceLocal() / 5
end

integration.getAgentKnowledgeForceRatio = function( kAgent )
    local agent = DEC_ConnaissanceAgent_EnAgent( kAgent.source )
    return DEC_Agent_RapportDeForceLocal( agent ) / 5
end

integration.getForceRatioAgent = function( pion )
    return DEC_Agent_RapportDeForceLocal( pion.source ) / 5
end

integration.getAgentKnowledgeForceRatio = function( kAgent )
    local agent = DEC_ConnaissanceAgent_EnAgent( kAgent.source )
    return DEC_Agent_RapportDeForceLocal( agent ) / 5
end