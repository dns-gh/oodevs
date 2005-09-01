// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeBlackBoard.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KnowledgeBlackBoard.h"

#include "DEC_KnowledgeSource_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_KnowledgeBlackBoard, "DEC_KnowledgeBlackBoard" )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard::DEC_KnowledgeBlackBoard()
{
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard::~DEC_KnowledgeBlackBoard()
{
    
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard::Initialize
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard::Initialize()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard::Terminate
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard::Terminate()
{
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeAgentPerception >( *this );
    file >> boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeAgent           >( *this );
    file >> boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeObjectCollision >( *this );
    file >> boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeObjectPerception>( *this );
    file >> boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeObject          >( *this );
    file >> boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeRapFor          >( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeAgentPerception >( *this );
    file << boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeAgent           >( *this );
    file << boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeObjectCollision >( *this );
    file << boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeObjectPerception>( *this );
    file << boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeObject          >( *this );
    file << boost::serialization::base_object< DEC_BlackBoard_CanContainKnowledgeRapFor          >( *this );
}


// =============================================================================
// MAIN OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard::Update
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard::Update()
{
    IT_KnowledgeSourceList itKS;

    for( itKS = talkingKnowledgeSources_.begin(); itKS != talkingKnowledgeSources_.end(); ++itKS )
        (**itKS).Prepare();
    
    for( itKS = talkingKnowledgeSources_.begin(); itKS != talkingKnowledgeSources_.end(); ++itKS )
        (**itKS).Talk();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard::Clean
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard::Clean()
{
    IT_KnowledgeSourceList itKS;

    for( IT_KnowledgeSourceList itKS = talkingKnowledgeSources_.begin(); itKS != talkingKnowledgeSources_.end(); ++itKS )
        (**itKS).Clean();
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard::AddToScheduler
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard::AddToScheduler( DEC_KnowledgeSource_ABC& ks, bool bHasPriority )
{
    if( bHasPriority )
        talkingKnowledgeSources_.insert( talkingKnowledgeSources_.begin(), &ks );
    else
        talkingKnowledgeSources_.push_back( &ks );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard::RemoveFromScheduler
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard::RemoveFromScheduler( DEC_KnowledgeSource_ABC& ks )
{
    talkingKnowledgeSources_.remove( &ks );
}
