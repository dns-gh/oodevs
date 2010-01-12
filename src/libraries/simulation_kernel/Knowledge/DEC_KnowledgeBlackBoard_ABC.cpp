// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_ABC.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeBlackBoard_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeBlackBoard_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_ABC constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_ABC::DEC_KnowledgeBlackBoard_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_ABC destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_ABC::~DEC_KnowledgeBlackBoard_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_ABC::Update
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_ABC::Update( int currentTimeStep )
{
    IT_KnowledgeSourceList itKS;

    for( itKS = talkingKnowledgeSources_.begin(); itKS != talkingKnowledgeSources_.end(); ++itKS )
        (**itKS).Prepare();
    
    for( itKS = talkingKnowledgeSources_.begin(); itKS != talkingKnowledgeSources_.end(); ++itKS )
        (**itKS).Talk( currentTimeStep );

    SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_ABC::Clean
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_ABC::Clean()
{
    IT_KnowledgeSourceList itKS;

    for( IT_KnowledgeSourceList itKS = talkingKnowledgeSources_.begin(); itKS != talkingKnowledgeSources_.end(); ++itKS )
        (**itKS).Clean();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_ABC::AddToScheduler
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_ABC::AddToScheduler( DEC_KnowledgeSource_ABC& newKs )
{
    for( IT_KnowledgeSourceList it = talkingKnowledgeSources_.begin(); it != talkingKnowledgeSources_.end(); ++it )
    {
        const DEC_KnowledgeSource_ABC& curKs = (**it);
        if( curKs.GetPriority() > newKs.GetPriority() )
        {
            talkingKnowledgeSources_.insert( it, &newKs );
            return;
        }
    }       
    talkingKnowledgeSources_.push_back( &newKs );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_ABC::RemoveFromScheduler
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_ABC::RemoveFromScheduler( DEC_KnowledgeSource_ABC& ks )
{
    talkingKnowledgeSources_.remove( &ks );
}
