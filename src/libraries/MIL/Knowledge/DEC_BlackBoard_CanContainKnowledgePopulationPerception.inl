// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationPerception.inl $
// $Author: Jvt $
// $Modtime: 6/04/05 18:52 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulationPerception.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::GetKnowledgesPopulationPerception
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_PopulationPerception* DEC_BlackBoard_CanContainKnowledgePopulationPerception::GetKnowledgePopulationPerception( const MIL_Population& associatedPopulation ) const
{
    CIT_KnowledgePopulationPerceptionMap itKnowledge = knowledgePopulationPerceptionMap_.find( &associatedPopulation );
    if( itKnowledge != knowledgePopulationPerceptionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

