// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.inl $
// $Author: Jvt $
// $Modtime: 6/04/05 18:34 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObject.inl $
//
// *****************************************************************************

// =============================================================================
// QUERIES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject( const MIL_RealObject_ABC& objectKnown, T_KnowledgeObjectVector& outContainer ) const
{
    outContainer.clear();

    CIT_KnowledgeObjectMap itKnowledge = knowledgeObjectMap_.find( &objectKnown );   
    if( itKnowledge != knowledgeObjectMap_.end() )
        outContainer.push_back( itKnowledge->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
void DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject( T_KnowledgeObjectVector& outContainer ) const
{
    for( CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromMosIDMap_.begin(); itKnowledge != knowledgeObjectFromMosIDMap_.end(); ++itKnowledge )
        outContainer.push_back( itKnowledge->second );
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromMosID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Object* DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromMosID( uint nMosID ) const
{
    CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromMosIDMap_.find( nMosID );
    return itKnowledge == knowledgeObjectFromMosIDMap_.end() ? 0 : itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromDiaID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Object* DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromDiaID( uint nDiaID ) const
{
    CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromDiaIDMap_.find( nDiaID );
    return itKnowledge == knowledgeObjectFromDiaIDMap_.end() ? 0 : itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject( const MIL_RealObject_ABC& objectKnown ) const
{
    return knowledgeObjectMap_.find( &objectKnown ) != knowledgeObjectMap_.end();
}
