------------------------------------------------------------------
-- Teammate methods
------------------------------------------------------------------
integration.ensureVictimsEvacuation = function( victimsUnit, medicalUnit )
    reportFunction(eRC_VictimsEvacuated )
    DEC_EvacuerBlessesVersTC2( victimsUnit.source, medicalUnit.source )
    return true
end

integration.AgentIsMoving = function( agent )
    return agent.source:DEC_Agent_EstEnMouvement( )
end

integration.AgentIsInTown = function( agent )
    return agent.source:DEC_Agent_EnVille()
end