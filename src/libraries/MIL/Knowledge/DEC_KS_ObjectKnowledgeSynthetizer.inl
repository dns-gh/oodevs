// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.inl $
// $Author: Nld $
// $Modtime: 26/10/04 16:18 $
// $Revision: 2 $
// $Workfile: DEC_KS_ObjectKnowledgeSynthetizer.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::AddEphemeralObjectKnowledge
// Created: NLD 2004-03-30
// -----------------------------------------------------------------------------
inline
void DEC_KS_ObjectKnowledgeSynthetizer::AddEphemeralObjectKnowledge( MIL_RealObject_ABC& objectKnown )
{
    ephemeralKnowledges_.push_back( &objectKnown );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::AddObjectKnowledgeToForget
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
inline
void DEC_KS_ObjectKnowledgeSynthetizer::AddObjectKnowledgeToForget( MIL_RealObject_ABC& objectKnown )
{
    objectsToForget_.push_back( &objectKnown );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectKnowledgeSynthetizer::AddObjectKnowledgeToForget
// Created: NLD 2004-06-08
// -----------------------------------------------------------------------------
inline
void DEC_KS_ObjectKnowledgeSynthetizer::AddObjectKnowledgeToForget( DEC_Knowledge_Object& knowledge )
{
    knowledgesObjectToForget_.push_back( &knowledge );
}
