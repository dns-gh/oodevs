// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Alzheimer.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 18:57 $
// $Revision: 3 $
// $Workfile: DEC_KS_Alzheimer.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_Alzheimer.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_Object.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_Alzheimer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Alzheimer::DEC_KS_Alzheimer( DEC_KnowledgeBlackBoard& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard )
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Alzheimer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Alzheimer::~DEC_KS_Alzheimer()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );    
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_Alzheimer::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Alzheimer::Prepare()
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Alzheimer::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Alzheimer::Talk()
{
    assert( pBlackBoard_ );
    
    class_mem_fun_void_t< DEC_KS_Alzheimer, DEC_Knowledge_Agent> methodUpdateKnowledgeAgent( DEC_KS_Alzheimer::UpdateKnowledgeAgent, *this );
    pBlackBoard_->ApplyOnKnowledgesAgent( methodUpdateKnowledgeAgent );

    class_mem_fun_void_t< DEC_KS_Alzheimer, DEC_Knowledge_Object> methodUpdateKnowledgeObject( DEC_KS_Alzheimer::UpdateKnowledgeObject, *this );
    pBlackBoard_->ApplyOnKnowledgesObject( methodUpdateKnowledgeObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Alzheimer::UpdateKnowledgeAgent
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
void DEC_KS_Alzheimer::UpdateKnowledgeAgent( DEC_Knowledge_Agent& knowledge )
{
    knowledge.UpdateRelevance();   
    if ( knowledge.GetRelevance() <= 0. )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->DestroyKnowledgeAgent( knowledge ); // NB - The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Alzheimer::UpdateKnowledgeObject
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
void DEC_KS_Alzheimer::UpdateKnowledgeObject( DEC_Knowledge_Object& knowledge )
{
    const MIL_RealObject_ABC* pObjectKnown = knowledge.GetObjectKnown();
    knowledge.UpdateRelevance();
    if ( pObjectKnown && knowledge.IsDissociatedFromRealObject() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->NotifyKnowledgeObjectDissociatedFromRealObject( *pObjectKnown, knowledge );
    }
    
    if ( knowledge.GetRelevance() <= 0. )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->DestroyKnowledgeObject( knowledge ); // NB - The knowledge will be deleted
    }
}