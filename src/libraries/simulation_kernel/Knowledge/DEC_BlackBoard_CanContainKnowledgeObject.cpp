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
#include "MIL_AgentServer.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/InteractWithSideCapacity.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"
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

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject constructor
// Created: LDC 2010-04-06
// Copies army's object blackboard.
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::DEC_BlackBoard_CanContainKnowledgeObject( MIL_Army_ABC& army, MIL_KnowledgeGroup* pKnowledgeGroup )
: pKnowledgeGroup_( pKnowledgeGroup )
{
    DEC_BlackBoard_CanContainKnowledgeObject& copy = army.GetKnowledge().GetKnowledgeObjectContainer();
    for( auto it = copy.objectMap_.begin(); it != copy.objectMap_.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object( *(it->second), pKnowledgeGroup_->shared_from_this() ) );
        if( ! objectMap_.insert( std::make_pair( it->first, knowledge ) ).second )
            MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
        if( ! knowledgeObjectFromIDMap_.insert( std::make_pair( knowledge->GetID(), knowledge ) ).second )
            MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    }
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
            for( auto it = map.begin(); it != map.end(); ++it )
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
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject( const MIL_Army_ABC& teamKnowing, MIL_Object_ABC& objectKnown )
{
    boost::shared_ptr< DEC_Knowledge_Object > knowledge;
    if( pKnowledgeGroup_ )
        knowledge = objectKnown.CreateKnowledge( pKnowledgeGroup_->shared_from_this() );
    else
        knowledge = objectKnown.CreateKnowledge( teamKnowing );

    if( knowledge )
    {
        if( ! objectMap_.insert( std::make_pair( &objectKnown, knowledge ) ).second )
            MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
        if( ! knowledgeObjectFromIDMap_.insert( std::make_pair( knowledge->GetID(), knowledge ) ).second )
            MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
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
            if( objectMap_.erase( knowledge.GetObjectKnown() ) < 1 )
                MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
            knowledge.CleanObjectKnown();
        }
        if( knowledgeObjectFromIDMap_.erase( knowledge.GetID() ) < 1 )
            MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::NotifyKnowledgeObjectDissociatedFromRealObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::NotifyKnowledgeObjectDissociatedFromRealObject( const MIL_Object_ABC& objectKnown, DEC_Knowledge_Object& /*knowledge*/ )
{
    if( objectMap_.erase( &objectKnown ) < 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    auto itKnowledge = objectMap_.find( &objectKnown );
    if( itKnowledge == objectMap_.end() )
        return boost::shared_ptr< DEC_Knowledge_Object >();
    return itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjects
// Created: NPT 2012-08-09
// -----------------------------------------------------------------------------
const DEC_BlackBoard_CanContainKnowledgeObject::T_KnowledgeObjectMap& DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjects() const
{
    return objectMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::RetrieveKnowledgeObject
// Created: LDC 2011-08-24
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_BlackBoard_CanContainKnowledgeObject::RetrieveKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    auto itKnowledge = objectMap_.find( &objectKnown );
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
    for( auto itKnowledge = knowledgeObjectFromIDMap_.begin(); itKnowledge != knowledgeObjectFromIDMap_.end(); ++itKnowledge )
        outContainer.push_back( itKnowledge->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID( unsigned int nMosID ) const
{
    auto itKnowledge = knowledgeObjectFromIDMap_.find( nMosID );
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
void DEC_BlackBoard_CanContainKnowledgeObject::GetCachedObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, double rHeight )
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
void DEC_BlackBoard_CanContainKnowledgeObject::UpdateUniversalObjects( const MIL_Army_ABC& team )
{
    const std::set< MIL_Object_ABC* >& universalObjects = MIL_AgentServer::GetWorkspace().GetEntityManager().GetUniversalObjects();
    for( std::set< MIL_Object_ABC* >::const_iterator it = universalObjects.begin(); it != universalObjects.end(); ++it )
    {
        if( !HasKnowledgeObject( **it ) && !(*it)->IsMarkedForDestruction() )
        {
            boost::shared_ptr< DEC_Knowledge_Object > knowledge = CreateKnowledgeObject( team, **it );
            knowledge->Update( PHY_PerceptionLevel::identified_ );
            knowledge->SkipPreparation();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::Merge
// Created: LDC 2012-04-30
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::Merge( const DEC_BlackBoard_CanContainKnowledgeObject* /*subGroup*/ )
{
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: DEC_KnowledgeBlackBoard_Army::sObjectKnowledgesFilteredHeightInserter
    // Created: NLD 2005-05-09
    // -----------------------------------------------------------------------------
    class sObjectKnowledgesFilteredHeightInserter : private boost::noncopyable
    {
    public:
        sObjectKnowledgesFilteredHeightInserter( T_KnowledgeObjectVector& container, const MIL_Agent_ABC& agent, const MIL_ObjectFilter& filter )
            : pContainer_( &container )
            , agent_     ( agent )
            , filter_    ( filter )
        {
            // NOTHING
        }

        void operator()( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( !knowledge->IsValid() )
                return;
            if( !knowledge->CanCollideWith( agent_ ) )
                return;
            const InteractWithSideCapacity* pSideInteraction = knowledge->GetType().GetCapacity< InteractWithSideCapacity >();
            if( pSideInteraction && knowledge->GetArmy() )
            {
                if( pSideInteraction->IsPossible( *knowledge->GetArmy(), agent_ ) )
                    pContainer_->push_back( knowledge );
                return;
            }
            if( filter_.Test( knowledge->GetType() ) )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectVector* pContainer_;
        const MIL_Agent_ABC& agent_;
        const MIL_ObjectFilter& filter_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsAtInteractionHeight
// Created: LGY 2013-01-18
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, const MIL_Agent_ABC& agent, const MIL_ObjectFilter& filter )
{
    bool useCache = ( dynamic_cast< const MIL_DangerousObjectFilter* >( &filter ) != 0 );
    const double rHeight = agent.GetRole< PHY_RoleInterface_Location >().GetHeight();
    if( useCache && HasObjectsAtInteractionHeightCache( rHeight ) )
    {
        GetCachedObjectsAtInteractionHeight( container, rHeight );
        return ;
    }
    sObjectKnowledgesFilteredHeightInserter functor( container, agent, filter );
    ApplyOnKnowledgesObject( functor );
    if( useCache )
        SetCachedObjectsAtInteractionHeight( container, rHeight );
}
