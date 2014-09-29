// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "DEC_Knowledge_Urban.h"
#include "MIL_AgentServer.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Urban/MIL_UrbanCache.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeUrban )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban constructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrban::DEC_BlackBoard_CanContainKnowledgeUrban( const MIL_Army_ABC& army )
    : army_( army )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban destructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrban::~DEC_BlackBoard_CanContainKnowledgeUrban()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::load
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> urbanMapFromConcrete_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::save
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << urbanMapFromConcrete_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::Finalize()
{
    const std::vector< const MIL_UrbanObject_ABC* >& blocks = MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocks();
    for( std::size_t i = 0; i < blocks.size(); ++i )
        urbanMapFromConcrete_[ blocks[ i ]->GetID() ].reset( new DEC_Knowledge_Urban( army_, *blocks[ i ] ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::CreateKnowledgeUrban
// Created: MGD 2009-12-04
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_BlackBoard_CanContainKnowledgeUrban::CreateKnowledgeUrban( const MIL_Army_ABC& army, const MIL_UrbanObject_ABC& object )
{
    boost::shared_ptr< DEC_Knowledge_Urban > knowledge ( new DEC_Knowledge_Urban( army, object ) );
    if( !urbanMapFromConcrete_.insert( std::make_pair( object.GetID(), knowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::DestroyKnowledgeUrban
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::DestroyKnowledgeUrban( DEC_Knowledge_Urban& knowledge )
{
    if( urbanMapFromConcrete_.erase( knowledge.GetObjectKnownId() ) < 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::GetKnowledgeUrban
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_BlackBoard_CanContainKnowledgeUrban::GetKnowledgeUrban( const MIL_UrbanObject_ABC& object ) const
{
    CIT_KnowledgeUrbanMap it = urbanMapFromConcrete_.find( object.GetID() );
    if( it == urbanMapFromConcrete_.end() )
        return boost::shared_ptr< DEC_Knowledge_Urban >();
    return it->second;
}
