// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectCollision.inl $
// $Author: Jvt $
// $Modtime: 6/04/05 17:38 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObjectCollision.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_ObjectCollision* DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgeObjectCollision( const MIL_RealObject_ABC& associatedObject ) const
{
    CIT_KnowledgeObjectCollisionMap itKnowledge = knowledgeObjectCollisionMap_.find( &associatedObject );
    if( itKnowledge != knowledgeObjectCollisionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision( T_KnowledgeObjectCollisionVector& container ) const
{
    container.clear(); container.reserve( knowledgeObjectCollisionMap_.size() );
    for( CIT_KnowledgeObjectCollisionMap itKnowledge = knowledgeObjectCollisionMap_.begin(); itKnowledge != knowledgeObjectCollisionMap_.end(); ++itKnowledge )
        container.push_back( itKnowledge->second );
}

