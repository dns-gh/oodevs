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
#include "Checkpoints/SerializationTools.h"
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
    while( !knowledgeUrbanPerceptionMap_.empty() )
        DestroyKnowledgeUrbanPerception( *knowledgeUrbanPerceptionMap_.begin()->second );
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
boost::shared_ptr< DEC_Knowledge_UrbanPerception > DEC_BlackBoard_CanContainKnowledgeUrbanPerception::CreateKnowledgeUrbanPerception( const MIL_Agent_ABC& agentPerceiving, const MIL_UrbanObject_ABC& objectPerceived )
{
    boost::shared_ptr< DEC_Knowledge_UrbanPerception > knowledge ( new DEC_Knowledge_UrbanPerception( agentPerceiving, objectPerceived.GetID() ) );
    if( ! knowledgeUrbanPerceptionMap_.insert( std::make_pair( objectPerceived.GetID(), knowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ FILE__ << " : Insert failed" );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::DestroyKnowledgeUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrbanPerception::DestroyKnowledgeUrbanPerception( DEC_Knowledge_UrbanPerception& knowledge )
{
    if( knowledgeUrbanPerceptionMap_.erase( knowledge.GetUrbanPerceivedId() ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ FILE__ << " : Erase failed" );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::GetKnowledgesUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_UrbanPerception > DEC_BlackBoard_CanContainKnowledgeUrbanPerception::GetKnowledgeUrbanPerception( const MIL_UrbanObject_ABC& associatedUrban ) const
{
    CIT_KnowledgeUrbanPerceptionMap itKnowledge = knowledgeUrbanPerceptionMap_.find( associatedUrban.GetID() );
    if( itKnowledge != knowledgeUrbanPerceptionMap_.end() )
        return itKnowledge->second;
    else
        return boost::shared_ptr< DEC_Knowledge_UrbanPerception >();
}
