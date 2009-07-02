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

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgeObjectCollision, "DEC_BlackBoard_CanContainKnowledgeObjectCollision" )

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
    while ( !knowledgeObjectCollisionMap_.empty() )
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
        inline
        void serialize( Archive& file, DEC_BlackBoard_CanContainKnowledgeObjectCollision::T_KnowledgeObjectCollisionMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_BlackBoard_CanContainKnowledgeObjectCollision::T_KnowledgeObjectCollisionMap& map, const uint )
        {
            const DEC_BlackBoard_CanContainKnowledgeObjectCollision::T_KnowledgeObjectCollisionMap::size_type size = map.size();
            file << size;
            for ( DEC_BlackBoard_CanContainKnowledgeObjectCollision::CIT_KnowledgeObjectCollisionMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_BlackBoard_CanContainKnowledgeObjectCollision::T_KnowledgeObjectCollisionMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
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
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> knowledgeObjectCollisionMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << knowledgeObjectCollisionMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::CreateKnowledgeObjectCollision
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision& DEC_BlackBoard_CanContainKnowledgeObjectCollision::CreateKnowledgeObjectCollision( const MIL_AgentPion& agentPerceiving, MIL_Object_ABC& objectPerceived )
{
    DEC_Knowledge_ObjectCollision* pKnowledge = new DEC_Knowledge_ObjectCollision( agentPerceiving, objectPerceived );//$$ RAM   
    bool bOut = knowledgeObjectCollisionMap_.insert( std::make_pair( &objectPerceived, pKnowledge ) ).second;
    assert( bOut );
    return *pKnowledge;
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectCollision::DestroyKnowledgeObjectCollision
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectCollision::DestroyKnowledgeObjectCollision( DEC_Knowledge_ObjectCollision& knowledge )
{
    int nOut = knowledgeObjectCollisionMap_.erase( &knowledge.GetObject() );
    assert( nOut == 1 );
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
