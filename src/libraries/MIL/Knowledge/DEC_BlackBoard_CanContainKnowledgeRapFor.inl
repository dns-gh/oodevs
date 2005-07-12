// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeRapFor.inl $
// $Author: Jvt $
// $Modtime: 16/03/05 16:38 $
// $Revision: 2 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeRapFor.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeRapFor::GetKnowledgeRapForLocal
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_RapForLocal& DEC_BlackBoard_CanContainKnowledgeRapFor::GetKnowledgeRapForLocal()
{
    return knowledgeRapForLocal_;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeRapFor::GetKnowledgeRapForGlobal
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_RapForGlobal& DEC_BlackBoard_CanContainKnowledgeRapFor::GetKnowledgeRapForGlobal()
{
    return knowledgeRapForGlobal_;   
}
