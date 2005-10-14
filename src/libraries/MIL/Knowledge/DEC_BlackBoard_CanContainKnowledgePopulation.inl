// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulation.inl $
// $Author: Jvt $
// $Modtime: 6/04/05 18:52 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulation.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgesPopulation
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Population* DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulation( const MIL_Population& associatedPopulation ) const
{
    CIT_KnowledgePopulationMap itKnowledge = knowledgePopulationMap_.find( &associatedPopulation );
    if( itKnowledge != knowledgePopulationMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

