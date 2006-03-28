// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationCollision.inl $
// $Author: Jvt $
// $Modtime: 6/04/05 17:38 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulationCollision.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_PopulationCollision* DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgePopulationCollision( const MIL_Population& associatedPopulation ) const
{
    CIT_KnowledgePopulationCollisionMap itKnowledge = knowledgePopulationCollisionMap_.find( &associatedPopulation );
    if( itKnowledge != knowledgePopulationCollisionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision( T_KnowledgePopulationCollisionVector& container ) const
{
    container.clear(); container.reserve( knowledgePopulationCollisionMap_.size() );
    for( CIT_KnowledgePopulationCollisionMap itKnowledge = knowledgePopulationCollisionMap_.begin(); itKnowledge != knowledgePopulationCollisionMap_.end(); ++itKnowledge )
        container.push_back( itKnowledge->second );
}

