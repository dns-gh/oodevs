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
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "MT_Tools/MT_ScipioException.h"

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
    unsigned int nNbr;
    file >> nNbr;
    while ( nNbr-- )
    {
        DEC_Knowledge_UrbanPerception* knowledge;
        file >> knowledge;
        knowledgeUrbanPerceptionMap_[ knowledge->GetUrbanPerceivedId() ].reset( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::save
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrbanPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const T_KnowledgeUrbanPerceptionMap::size_type size = knowledgeUrbanPerceptionMap_.size();
    file << size;
    for ( CIT_KnowledgeUrbanPerceptionMap it = knowledgeUrbanPerceptionMap_.begin(); it != knowledgeUrbanPerceptionMap_.end(); ++it )
    {
        DEC_Knowledge_UrbanPerception* knowledge = it->second.get();
        file << knowledge;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::CreateKnowledgeUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_UrbanPerception > DEC_BlackBoard_CanContainKnowledgeUrbanPerception::CreateKnowledgeUrbanPerception( const MIL_Agent_ABC& agentPerceiving, const UrbanObjectWrapper& objectPerceived )
{
    boost::shared_ptr< DEC_Knowledge_UrbanPerception > knowledge ( new DEC_Knowledge_UrbanPerception( agentPerceiving, objectPerceived.GetID() ) );
    if( ! knowledgeUrbanPerceptionMap_.insert( std::make_pair( objectPerceived.GetID(), knowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::DestroyKnowledgeUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrbanPerception::DestroyKnowledgeUrbanPerception( DEC_Knowledge_UrbanPerception& knowledge )
{
    if( knowledgeUrbanPerceptionMap_.erase( knowledge.GetUrbanPerceivedId() ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrbanPerception::GetKnowledgesUrbanPerception
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_UrbanPerception > DEC_BlackBoard_CanContainKnowledgeUrbanPerception::GetKnowledgeUrbanPerception( const UrbanObjectWrapper& associatedUrban ) const
{
    CIT_KnowledgeUrbanPerceptionMap itKnowledge = knowledgeUrbanPerceptionMap_.find( associatedUrban.GetID() );
    if( itKnowledge != knowledgeUrbanPerceptionMap_.end() )
        return itKnowledge->second;
    else
        return boost::shared_ptr< DEC_Knowledge_UrbanPerception >();
}
