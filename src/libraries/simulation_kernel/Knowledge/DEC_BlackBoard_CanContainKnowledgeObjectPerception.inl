// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectPerception.inl $
// $Author: Jvt $
// $Modtime: 6/04/05 18:52 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObjectPerception.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::GetKnowledgesObjectPerception
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_ObjectPerception* DEC_BlackBoard_CanContainKnowledgeObjectPerception::GetKnowledgeObjectPerception( const MIL_RealObject_ABC& associatedObject ) const
{
    CIT_KnowledgeObjectPerceptionMap itKnowledge = knowledgeObjectPerceptionMap_.find( &associatedObject );
    if( itKnowledge != knowledgeObjectPerceptionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

