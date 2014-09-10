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
    Clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::load
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrbanPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> perceptions_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::save
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrbanPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << perceptions_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::CreateKnowledgeUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
DEC_Knowledge_UrbanPerception* DEC_BlackBoard_CanContainKnowledgeUrbanPerception::CreateKnowledgeUrbanPerception( const MIL_Agent_ABC& agentPerceiving, const MIL_UrbanObject_ABC& objectPerceived )
{
    DEC_Knowledge_UrbanPerception*& knowledge = perceptions_[ objectPerceived.GetID() ];
    if( knowledge )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    else
        knowledge = new DEC_Knowledge_UrbanPerception( agentPerceiving, objectPerceived.GetID() );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::Clear
// Created: LDC 2014-09-08
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrbanPerception::Clear()
{
    for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
        delete it->second;
    perceptions_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::GetKnowledgesUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
DEC_Knowledge_UrbanPerception* DEC_BlackBoard_CanContainKnowledgeUrbanPerception::GetKnowledgeUrbanPerception( const MIL_UrbanObject_ABC& associatedUrban ) const
{
    auto it = perceptions_.find( associatedUrban.GetID() );
    if( it != perceptions_.end() )
        return it->second;
    return 0;
}
