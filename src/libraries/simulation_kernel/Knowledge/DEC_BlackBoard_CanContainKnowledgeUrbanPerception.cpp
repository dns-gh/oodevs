// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrbanPerception.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_UrbanPerception.h"
#include "CheckPoints/SerializationTools.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeUrbanPerception )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception constructor
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrbanPerception::DEC_BlackBoard_CanContainKnowledgeUrbanPerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception destructor
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrbanPerception::~DEC_BlackBoard_CanContainKnowledgeUrbanPerception()
{
    for( auto it = knowledgeUrbanPerceptionMap_.begin(); it != knowledgeUrbanPerceptionMap_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::load
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrbanPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> knowledgeUrbanPerceptionMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::save
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrbanPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << knowledgeUrbanPerceptionMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::CreateKnowledgeUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
DEC_Knowledge_UrbanPerception* DEC_BlackBoard_CanContainKnowledgeUrbanPerception::CreateKnowledgeUrbanPerception( const MIL_Agent_ABC& agentPerceiving, const MIL_UrbanObject_ABC& objectPerceived )
{
    DEC_Knowledge_UrbanPerception*& knowledge = knowledgeUrbanPerceptionMap_[ objectPerceived.GetID() ];
    if( knowledge )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    else
        knowledge = new DEC_Knowledge_UrbanPerception( agentPerceiving, objectPerceived.GetID() );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::GetKnowledgesUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
DEC_Knowledge_UrbanPerception* DEC_BlackBoard_CanContainKnowledgeUrbanPerception::GetKnowledgeUrbanPerception( const MIL_UrbanObject_ABC& associatedUrban ) const
{
    auto itKnowledge = knowledgeUrbanPerceptionMap_.find( associatedUrban.GetID() );
    if( itKnowledge != knowledgeUrbanPerceptionMap_.end() )
        return itKnowledge->second;
    return 0;
}
