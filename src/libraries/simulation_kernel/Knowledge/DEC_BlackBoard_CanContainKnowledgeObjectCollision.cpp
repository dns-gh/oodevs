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
#include "MT_Tools/MT_ScipioException.h"
#include <boost/serialization/split_free.hpp>

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
    while( !knowledgeObjectCollisionMap_.empty() )
        DestroyKnowledgeObjectCollision( *knowledgeObjectCollisionMap_.begin()->second );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        void serialize( Archive& file, DEC_BlackBoard_CanContainKnowledgeObjectCollision::T_KnowledgeObjectCollisionMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const DEC_BlackBoard_CanContainKnowledgeObjectCollision::T_KnowledgeObjectCollisionMap& map, const unsigned int )
        {
            const std::size_t size = map.size();
            file << size;
            for( DEC_BlackBoard_CanContainKnowledgeObjectCollision::CIT_KnowledgeObjectCollisionMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, DEC_BlackBoard_CanContainKnowledgeObjectCollision::T_KnowledgeObjectCollisionMap& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while( nNbr-- )
            {
                MIL_Object_ABC* pObject;

                file >> pObject;
                file >> map[ pObject ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> knowledgeObjectCollisionMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << knowledgeObjectCollisionMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::CreateKnowledgeObjectCollision
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision& DEC_BlackBoard_CanContainKnowledgeObjectCollision::CreateKnowledgeObjectCollision( const MIL_Agent_ABC& agentPerceiving, MIL_Object_ABC& objectPerceived )
{
    DEC_Knowledge_ObjectCollision* pKnowledge = new DEC_Knowledge_ObjectCollision( agentPerceiving, objectPerceived );//$$ RAM
    if( ! knowledgeObjectCollisionMap_.insert( std::make_pair( &objectPerceived, pKnowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::DestroyKnowledgeObjectCollision
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::DestroyKnowledgeObjectCollision( DEC_Knowledge_ObjectCollision& knowledge )
{
    if( knowledgeObjectCollisionMap_.erase( &knowledge.GetObject() ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision* DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgeObjectCollision( const MIL_Object_ABC& associatedObject ) const
{
    CIT_KnowledgeObjectCollisionMap itKnowledge = knowledgeObjectCollisionMap_.find( &associatedObject );
    if( itKnowledge != knowledgeObjectCollisionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::GetKnowledgesObjectCollision( T_KnowledgeObjectCollisionVector& container ) const
{
    container.clear(); container.reserve( knowledgeObjectCollisionMap_.size() );
    for( CIT_KnowledgeObjectCollisionMap itKnowledge = knowledgeObjectCollisionMap_.begin(); itKnowledge != knowledgeObjectCollisionMap_.end(); ++itKnowledge )
        container.push_back( itKnowledge->second );
}
