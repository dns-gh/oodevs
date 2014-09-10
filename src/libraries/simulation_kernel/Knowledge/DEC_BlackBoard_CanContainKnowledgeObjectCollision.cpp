// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectCollision.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:28 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObjectCollision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectCollision.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeObjectCollision )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObjectCollision::DEC_BlackBoard_CanContainKnowledgeObjectCollision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObjectCollision::~DEC_BlackBoard_CanContainKnowledgeObjectCollision()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> collisions_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << collisions_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::CreateKnowledgeObjectCollision
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision& DEC_BlackBoard_CanContainKnowledgeObjectCollision::CreateKnowledgeObjectCollision( const MIL_Agent_ABC& agentPerceiving, MIL_Object_ABC& objectPerceived )
{
    DEC_Knowledge_ObjectCollision* pKnowledge = new DEC_Knowledge_ObjectCollision( agentPerceiving, objectPerceived );//$$ RAM
    if( ! collisions_.insert( std::make_pair( &objectPerceived, pKnowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::DestroyKnowledgeObjectCollision
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::DestroyKnowledgeObjectCollision( DEC_Knowledge_ObjectCollision& knowledge )
{
    if( collisions_.erase( &knowledge.GetObject() ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::Clear
// Created: LDC 2014-09-08
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::Clear()
{
    while( !collisions_.empty() )
        DestroyKnowledgeObjectCollision( *collisions_.begin()->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision* DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgeObjectCollision( const MIL_Object_ABC& associatedObject ) const
{
    auto it = collisions_.find( &associatedObject );
    if( it != collisions_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision( T_KnowledgeObjectCollisionVector& container ) const
{
    container.clear(); container.reserve( collisions_.size() );
    for( auto it = collisions_.begin(); it != collisions_.end(); ++it )
        container.push_back( it->second );
}
