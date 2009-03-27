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
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Object* DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    CIT_KnowledgeObjectMap itKnowledge = objectMap_.find( &objectKnown );   
    if( itKnowledge == objectMap_.end() )
        return 0;
    return itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
void DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject( T_KnowledgeObjectVector& outContainer ) const
{
    outContainer.reserve( knowledgeObjectFromIDMap_.size() );
    for( CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromIDMap_.begin(); itKnowledge != knowledgeObjectFromIDMap_.end(); ++itKnowledge )
        outContainer.push_back( itKnowledge->second );
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Object* DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID( uint nMosID ) const
{
    CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromIDMap_.find( nMosID );
    return itKnowledge == knowledgeObjectFromIDMap_.end() ? 0 : itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    return objectMap_.find( &objectKnown ) != objectMap_.end();
}
