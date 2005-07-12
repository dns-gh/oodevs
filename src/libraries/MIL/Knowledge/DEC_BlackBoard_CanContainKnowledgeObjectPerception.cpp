// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 19:05 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObjectPerception.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectPerception.h"

#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_ObjectPerception.h"

#include "Entities/Objects/MIL_RealObject_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObjectPerception::DEC_BlackBoard_CanContainKnowledgeObjectPerception()
{
    
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObjectPerception::~DEC_BlackBoard_CanContainKnowledgeObjectPerception()
{
    while( !knowledgeObjectPerceptionMap_.empty() )
        DestroyKnowledgeObjectPerception( *knowledgeObjectPerceptionMap_.begin()->second );        
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nNbr;
    file >> nNbr;
    while ( nNbr-- )
    {
        MIL_RealObject_ABC* pObject;
        file >> pObject;
        file >> knowledgeObjectPerceptionMap_[ pObject ];
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << knowledgeObjectPerceptionMap_.size();
    for ( CIT_KnowledgeObjectPerceptionMap it = knowledgeObjectPerceptionMap_.begin(); it != knowledgeObjectPerceptionMap_.end(); ++it )
    {
        file << it->first
             << it->second;
    }
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::CreateKnowledgeObjectPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception& DEC_BlackBoard_CanContainKnowledgeObjectPerception::CreateKnowledgeObjectPerception( const MIL_AgentPion& agentPerceiving, MIL_RealObject_ABC& objectPerceived )
{
    DEC_Knowledge_ObjectPerception* pKnowledge = new DEC_Knowledge_ObjectPerception( agentPerceiving, objectPerceived );//$$ RAM   
    bool bOut = knowledgeObjectPerceptionMap_.insert( std::make_pair( &objectPerceived, pKnowledge ) ).second;
    assert( bOut );
    return *pKnowledge;
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::DestroyKnowledgeObjectPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectPerception::DestroyKnowledgeObjectPerception( DEC_Knowledge_ObjectPerception& knowledge )
{
    int nOut = knowledgeObjectPerceptionMap_.erase( &knowledge.GetObjectPerceived() );
    assert( nOut == 1 );
    delete &knowledge;
}

