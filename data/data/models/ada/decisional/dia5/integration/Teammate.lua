------------------------------------------------------------------
-- Teammate methods
------------------------------------------------------------------
integration.ensureVictimsEvacuation = function( victimsUnit, medicalUnit )
    meKnowledge:RC( eRC_VictimsEvacuated )
    DEC_EvacuerBlessesVersTC2( victimsUnit.source, medicalUnit.source )
    return true
end