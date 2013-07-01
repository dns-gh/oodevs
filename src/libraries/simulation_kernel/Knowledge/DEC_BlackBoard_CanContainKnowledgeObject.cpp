// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:38 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_Object.h"
#include "KnowledgesVisitor_ABC.h"
#include "Checkpoints/SerializationTools.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_ScipioException.h"
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeObject )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::DEC_BlackBoard_CanContainKnowledgeObject()
    : pKnowledgeGroup_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::~DEC_BlackBoard_CanContainKnowledgeObject()
{
    while( !knowledgeObjectFromIDMap_.empty() )
        DestroyKnowledgeObject( *knowledgeObjectFromIDMap_.begin()->second );
}

// =============================================================================
// CHECKPOINT
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_BlackBoard_CanContainKnowledgeObject::T_KnowledgeObjectMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const DEC_BlackBoard_CanContainKnowledgeObject::T_KnowledgeObjectMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for ( DEC_BlackBoard_CanContainKnowledgeObject::CIT_KnowledgeObjectMap it = map.begin(); it != map.end(); ++it )
            {
                boost::shared_ptr< DEC_Knowledge_Object > tmp = it->second;
                file << it->first;
                file << tmp;
            }
        }

        template< typename Archive >
        void load( Archive& file, DEC_BlackBoard_CanContainKnowledgeObject::T_KnowledgeObjectMap& map, const unsigned int )
        {
            std::size_t nNbr;
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
// Name: DEC_BlackBoard_CanContainKnowledgeObject::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> objectMap_;
    for( auto it = objectMap_.begin(); it != objectMap_.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Object > knowledge = it->second;
        knowledgeObjectFromIDMap_.insert( std::make_pair( knowledge->GetID(), knowledge ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << objectMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::SetKnowledgeGroup
// Created: LDC 2012-09-04
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::SetKnowledgeGroup( MIL_KnowledgeGroup* group )
{
    pKnowledgeGroup_ = group;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject( const MIL_Army_ABC* army, MIL_Object_ABC& objectKnown )
{
    auto it = objectMap_.find( &objectKnown );
    if( it != objectMap_.end() )
        return it->second;
    boost::shared_ptr< DEC_Knowledge_Object > knowledge;
    if( pKnowledgeGroup_ )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup = pKnowledgeGroup_->shared_from_this();
        knowledge = objectKnown.CreateKnowledge( knowledgeGroup );
    }
    else if ( army == objectKnown.GetArmy() )
        knowledge = objectKnown.CreateKnowledge( *objectKnown.GetArmy() );

    if( knowledge )
    {
        objectMap_.insert( std::make_pair( &objectKnown, knowledge ) );
        knowledgeObjectFromIDMap_.insert( std::make_pair( knowledge->GetID(), knowledge ) );
    }

    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::DestroyKnowledgeObject
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::DestroyKnowledgeObject( DEC_Knowledge_Object& knowledge )
{
    if( knowledge.IsValid() )
    {
        knowledge.Invalidate();
        if( knowledge.GetObjectKnown() )
        {
            objectMap_.erase( knowledge.GetObjectKnown() );
            knowledge.CleanObjectKnown();
        }
        knowledgeObjectFromIDMap_.erase( knowledge.GetID() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::NotifyKnowledgeObjectDissociatedFromRealObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::NotifyKnowledgeObjectDissociatedFromRealObject( const MIL_Object_ABC& objectKnown, DEC_Knowledge_Object& knowledge )
{
    assert( knowledge.GetObjectKnown() == 0 );
    if( objectMap_.erase( &objectKnown ) < 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    CIT_KnowledgeObjectMap itKnowledge = objectMap_.find( &objectKnown );
    if( itKnowledge == objectMap_.end() )
        return boost::shared_ptr< DEC_Knowledge_Object >();
    return itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::RetrieveKnowledgeObject
// Created: LDC 2011-08-24
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_BlackBoard_CanContainKnowledgeObject::RetrieveKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    CIT_KnowledgeObjectMap itKnowledge = objectMap_.find( &objectKnown );
    if( itKnowledge != objectMap_.end() )
        return itKnowledge->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject( T_KnowledgeObjectVector& outContainer ) const
{
    outContainer.reserve( knowledgeObjectFromIDMap_.size() );
    for( CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromIDMap_.begin(); itKnowledge != knowledgeObjectFromIDMap_.end(); ++itKnowledge )
        outContainer.push_back( itKnowledge->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID( unsigned int nMosID ) const
{
    CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromIDMap_.find( nMosID );
    return itKnowledge == knowledgeObjectFromIDMap_.end() ? boost::shared_ptr< DEC_Knowledge_Object >() : itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromObjectID
// Created: JSR 2011-09-29
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromObjectID( unsigned int nID ) const
{
    for( auto it = knowledgeObjectFromIDMap_.begin(); it != knowledgeObjectFromIDMap_.end(); ++it )
        if( it->second->GetObjectId() == nID )
            return it->second;
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    return objectMap_.find( &objectKnown ) != objectMap_.end();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::Accept
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    visitor.VisitKnowledgesObject( objectMap_.size() );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::Prepare
// Created: LDC 2012-02-02
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::Prepare()
{
    obstacleCache_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::HasObjectsAtInteractionHeightCache
// Created: LDC 2012-02-02
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgeObject::HasObjectsAtInteractionHeightCache( double rHeight ) const
{
    return obstacleCache_.find( rHeight ) != obstacleCache_.end();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetCachedObjectsAtInteractionHeight
// Created: LDC 2012-02-02
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetCachedObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, double rHeight ) const
{
    std::map< double, T_KnowledgeObjectVector >::const_iterator it = obstacleCache_.find( rHeight );
    if( it != obstacleCache_.end() )
        container = it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::SetCachedObjectsAtInteractionHeight
// Created: LDC 2012-02-02
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::SetCachedObjectsAtInteractionHeight( const T_KnowledgeObjectVector& container, double rHeight )
{
    obstacleCache_[ rHeight ] = container;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::UpdateUniversalObjects
// Created: LDC 2012-02-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::UpdateUniversalObjects()
{
    const std::set< MIL_Object_ABC* >& universalObjects = MIL_EntityManager_ABC::GetSingleton().GetUniversalObjects();
    for( std::set< MIL_Object_ABC* >::const_iterator it = universalObjects.begin(); it != universalObjects.end(); ++it )
    {
        if( !HasKnowledgeObject( **it ) && !(*it)->IsMarkedForDestruction() )
        {
            boost::shared_ptr< DEC_Knowledge_Object > knowledge = CreateKnowledgeObject( ( *it )->GetArmy(), **it );
            knowledge->Update( PHY_PerceptionLevel::identified_ );
            knowledge->SkipPreparation();
        }
    }
}
