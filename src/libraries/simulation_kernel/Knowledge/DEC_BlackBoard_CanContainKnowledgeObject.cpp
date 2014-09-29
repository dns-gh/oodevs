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
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_Object.h"
#include "MIL_AgentServer.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "KnowledgesVisitor_ABC.h"
#include "MIL_AgentServer.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/InteractWithSideCapacity.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Entities/Objects/CapacityRetriever.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"

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
// Name: DEC_BlackBoard_CanContainKnowledgeObject constructor
// Created: LDC 2010-04-06
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::DEC_BlackBoard_CanContainKnowledgeObject( MIL_KnowledgeGroup* pKnowledgeGroup )
    : pKnowledgeGroup_( pKnowledgeGroup )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::~DEC_BlackBoard_CanContainKnowledgeObject()
{
    while( !objectMap_.empty() )
        DestroyKnowledgeObject( *objectMap_.begin()->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> objectMap_;
    file >> previousObjectMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << objectMap_;
    file << previousObjectMap_;
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
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject( MIL_Object_ABC& objectKnown )
{
    auto it = objectMap_.find( objectKnown.GetID() );
    if( it != objectMap_.end() )
        return it->second;
    boost::shared_ptr< DEC_Knowledge_Object > knowledge;
    if( pKnowledgeGroup_ )
        knowledge = objectKnown.CreateKnowledge( *pKnowledgeGroup_ );
    if( knowledge )
        objectMap_.insert( std::make_pair( objectKnown.GetID(), knowledge ) );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject
// Created: MMC 2013-07-15
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject( boost::shared_ptr< DEC_Knowledge_Object >& object )
{
    auto it = objectMap_.find( object->GetObjectId() );
    if( it != objectMap_.end() )
        return it->second;
    boost::shared_ptr< DEC_Knowledge_Object > knowledge( new DEC_Knowledge_Object( *object, *pKnowledgeGroup_ ) );
    objectMap_.insert( std::make_pair( object->GetObjectId(), knowledge ) );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::DestroyKnowledgeObject
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::DestroyKnowledgeObject( DEC_Knowledge_Object& knowledge )
{
    if( knowledge.IsValid() )
        knowledge.Invalidate();
    knowledge.CleanObjectKnown();
    if( objectMap_.erase( knowledge.GetObjectId() ) < 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    auto it = objectMap_.find( objectKnown.GetID() );
    if( it == objectMap_.end() )
        return boost::shared_ptr< DEC_Knowledge_Object >();
    return it->second;
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
    auto it = objectMap_.find( objectKnown.GetID() );
    if( it != objectMap_.end() )
        return it->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject( T_KnowledgeObjectVector& outContainer ) const
{
    outContainer.reserve( objectMap_.size() );
    for( auto it = objectMap_.begin(); it != objectMap_.end(); ++it )
        outContainer.push_back( it->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID( unsigned int nMosID ) const
{
    for( auto it = objectMap_.begin(); it != objectMap_.end(); ++it )
        if( it->second->GetID() == nMosID )
            return it->second;
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromObjectID
// Created: JSR 2011-09-29
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromObjectID( unsigned int nID ) const
{
    auto it = objectMap_.find( nID );
    if( it != objectMap_.end() )
        return it->second;
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    return objectMap_.find( objectKnown.GetID() ) != objectMap_.end();
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
    auto it = obstacleCache_.find( rHeight );
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
    MIL_AgentServer::GetWorkspace().GetEntityManager().VisitUniversalObjects(
        boost::bind( &DEC_BlackBoard_CanContainKnowledgeObject::UpdateUniversalObject, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::UpdateUniversalObject
// Created: LDC 2012-02-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::UpdateUniversalObject( MIL_Object_ABC& object )
{
    if( !HasKnowledgeObject( object ) )
    {
        boost::shared_ptr< DEC_Knowledge_Object > knowledge = CreateKnowledgeObject( object );
        knowledge->Update( PHY_PerceptionLevel::identified_ );
        knowledge->SkipPreparation();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject
// Created: LGY 2013-03-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject( const DEC_Knowledge_Object& knowledge )
{
    if( !knowledge.GetObjectKnown() || !pKnowledgeGroup_ )
        return;
    boost::shared_ptr< DEC_Knowledge_Object > copy = knowledge.GetObjectKnown()->CreateKnowledge( *pKnowledgeGroup_, knowledge );
    objectMap_.insert( std::make_pair( copy->GetObjectKnown()->GetID(), copy ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::Merge
// Created: LDC 2012-04-30
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::Merge( const DEC_BlackBoard_CanContainKnowledgeObject& subGroup )
{
    for( auto itKnowledge = subGroup.objectMap_.begin(); itKnowledge != subGroup.objectMap_.end(); ++itKnowledge )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = GetKnowledgeObjectFromObjectID( itKnowledge->first );
        boost::shared_ptr< DEC_Knowledge_Object > pSubKnowledge = itKnowledge->second;
        if( !pKnowledge.get() )
            CreateKnowledgeObject( *pSubKnowledge );
        else if( pKnowledge->GetRelevance() < pSubKnowledge->GetRelevance() )
        {
            DestroyKnowledgeObject( *pKnowledge );
            CreateKnowledgeObject( *pSubKnowledge );
        }
    }
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
        sObjectKnowledgesFilteredHeightInserter( T_KnowledgeObjectVector& container, const MIL_Agent_ABC& agent, const MIL_ObjectFilter& filter, bool bCheckBypassed = true )
            : pContainer_       ( &container )
            , agent_            ( agent )
            , filter_           ( filter )
            , bCheckBypassed_   ( bCheckBypassed )
        {
            // NOTHING
        }

        void operator()( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( !knowledge->IsValid() )
                return;
            if( !knowledge->CanCollideWith( agent_, bCheckBypassed_ ) )
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
        bool bCheckBypassed_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsAtInteractionHeight
// Created: LGY 2013-01-18
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, const MIL_Agent_ABC& agent, const MIL_ObjectFilter& filter, bool bCheckBypassed /*= true*/ )
{
    bool useCache = ( dynamic_cast< const MIL_DangerousObjectFilter* >( &filter ) != 0 ) && bCheckBypassed;
    const double rHeight = agent.GetRole< PHY_RoleInterface_Location >().GetHeight();
    if( useCache && HasObjectsAtInteractionHeightCache( rHeight ) )
    {
        GetCachedObjectsAtInteractionHeight( container, rHeight );
        return ;
    }
    sObjectKnowledgesFilteredHeightInserter functor( container, agent, filter, bCheckBypassed );
    ApplyOnKnowledgesObject( functor );
    if( useCache )
        SetCachedObjectsAtInteractionHeight( container, rHeight );
}

namespace
{
    class sObjectKnowledgesInCircleFilteredInserter : boost::noncopyable
    {
    public:
        sObjectKnowledgesInCircleFilteredInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const MT_Vector2D& center, double rRadius, bool nonActivatedObstacles )
            : pContainer_( &container )
            , filter_    ( filter )
            , pCenter_   ( &center )
            , rRadius_   ( rRadius )
            , nonActivatedObstacles_( nonActivatedObstacles )
        {
            // NOTHING
        }

        void operator() ( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( knowledge->IsValid()
                && ( nonActivatedObstacles_ || ( !knowledge->IsActivableObstacle() || knowledge->IsObstacleActivated() ) )
                && filter_.Test( knowledge->GetType() )
                && knowledge->GetLocalisation().ComputeBarycenter().Distance( *pCenter_ ) <= rRadius_ )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
        const MT_Vector2D* pCenter_;
        double rRadius_;
        bool nonActivatedObstacles_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsInCircle
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsInCircle( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const MT_Vector2D& center, double rRadius, bool nonActivatedObstacles )
{
    sObjectKnowledgesInCircleFilteredInserter functor( container, filter, center, rRadius, nonActivatedObstacles );

    ApplyOnKnowledgesObject( functor );
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: DEC_KnowledgeBlackBoard_Army::GetObjectsInZone
    // Created: NLD 2006-05-05
    // -----------------------------------------------------------------------------
    template< typename T >
    class sObjectKnowledgesInZoneFilteredInserter : private boost::noncopyable
    {
    public:
        sObjectKnowledgesInZoneFilteredInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const T& zone )
            : pContainer_( &container )
            , filter_    ( filter )
            , pZone_     ( &zone )
        {
            // NOTHING
        }

        void operator()( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( filter_.Test( knowledge->GetType() )
                && ( !knowledge->IsActivableObstacle() || knowledge->IsObstacleActivated() )
                && pZone_->IsInside( knowledge->GetLocalisation().ComputeBarycenter() )
                && knowledge->IsValid() )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
        const T* pZone_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsInZone
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone )
{
    sObjectKnowledgesInZoneFilteredInserter< TER_Localisation > functor( container, filter, zone );
    ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsInZone
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Polygon& zone )
{
    sObjectKnowledgesInZoneFilteredInserter< TER_Polygon > functor( container, filter, zone );
    ApplyOnKnowledgesObject( functor );
}

namespace
{
    class sObjectKnowledgesDiaIDInserter : boost::noncopyable
    {
    public:
        sObjectKnowledgesDiaIDInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter )
            : pContainer_( &container )
            , filter_    ( filter )
        {
            // NOTHING
        }

        void operator()( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( filter_.Test( knowledge->GetType() ) )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjects
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter ) const
{
    sObjectKnowledgesDiaIDInserter functor( container, filter );
    ApplyOnKnowledgesObject( functor );
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: DEC_KnowledgeBlackBoard_Army::GetObjects
    // Created: NLD 2005-05-09
    // -----------------------------------------------------------------------------
    class sObjectKnowledgesInserter
    {
    public:
        sObjectKnowledgesInserter( T_KnowledgeObjectVector& container )
            : pContainer_( &container )
        {
            // NOTHING
        }

        void operator() ( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( ( !knowledge->IsActivableObstacle() || knowledge->IsObstacleActivated() ) && knowledge->IsValid() )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectVector* pContainer_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjects
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjects( T_KnowledgeObjectVector& container ) const
{
    sObjectKnowledgesInserter functor( container );
    ApplyOnKnowledgesObject( functor );
}

namespace
{
    class sClosestObjectInserter : boost::noncopyable
    {
    public:
        sClosestObjectInserter( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter )
            : rClosestDist_( std::numeric_limits< double >::max() )
            , pPos_        ( vPos )
            , filter_      ( filter )
        {
            // NOTHING
        }

        void operator()( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( !filter_.Test( knowledge->GetType() ) || !knowledge->IsValid() )
                return;

            const double rDist = knowledge->GetLocalisation().ComputeBarycenter().Distance( pPos_ );
            if( rDist > rClosestDist_ )
                return;
            rClosestDist_ = rDist;
            pResult_ = knowledge;
        }

    public:
        boost::shared_ptr< DEC_Knowledge_Object > pResult_;

    private:
        double rClosestDist_;
        const MT_Vector2D& pPos_;
        const MIL_ObjectFilter& filter_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetClosestObject
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetClosestObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter ) const
{
    sClosestObjectInserter functor( vPos, filter );

    ApplyOnKnowledgesObject( functor );
    return functor.pResult_;
}

namespace
{
    class sClosestObjectFriendInserter : boost::noncopyable
    {
    public:
        sClosestObjectFriendInserter( const MT_Vector2D& vPos, const MIL_Army_ABC& army, const MIL_ObjectFilter& filter )
            : pArmy_       ( &army )
            , rClosestDist_( std::numeric_limits< double >::max() )
            , pos_         ( vPos )
            , filter_      ( filter )
        {
            // NOTHING
        }

        void operator() ( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( !knowledge->IsValid() )
                return;

            if( !filter_.Test( knowledge->GetType() ) )
                return;

            if( !knowledge->GetArmy() || pArmy_->IsAFriend( *knowledge->GetArmy() ) != eTristate_True )
                return;

            const double rDist = knowledge->GetLocalisation().ComputeBarycenter().Distance( pos_ );
            if( rDist > rClosestDist_ )
                return;
            rClosestDist_ = rDist;
            pResult_ = knowledge;
        }

    public:
        boost::shared_ptr< DEC_Knowledge_Object > pResult_;

    private:
        double rClosestDist_;
        const MIL_Army_ABC* pArmy_;
        const MT_Vector2D& pos_;
        const MIL_ObjectFilter& filter_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetClosestFriendObject
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_BlackBoard_CanContainKnowledgeObject::GetClosestFriendObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter, const MIL_Army_ABC& army ) const
{
    sClosestObjectFriendInserter functor( vPos, army, filter );
    ApplyOnKnowledgesObject( functor );
    return functor.pResult_;
}

namespace
{

    void FindResourceNetworks( const MIL_Object_ABC& object, const TER_Localisation& zone, const std::string& type, T_ResourceNetworkVector& container )
    {
        const ResourceNetworkCapacity* capacity = object.Retrieve< ResourceNetworkCapacity >();
        if( capacity && zone.Contains( object.GetLocalisation() ) && ( type.empty() || capacity->ContainsType( type ) ) )
        {
            const T_ResourceNetworkVector& nodes = capacity->GetDECResourceNetworks( object.GetID() );
            container.insert( container.end(), nodes.begin(), nodes.end() );
        }
    }

    class sResourceNetworkInserter : private boost::noncopyable
    {
    public:
        sResourceNetworkInserter( T_ResourceNetworkVector& container, const TER_Localisation& zone, const std::string& type )
            : container_( container )
            , zone_      ( zone )
            , type_      ( type )
        {
            // NOTHING
        }

        void operator() ( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( knowledge->IsValid() )
                if( MIL_Object_ABC* obj = knowledge->GetObjectKnown() )
                    FindResourceNetworks( *obj, zone_, type_, container_ );
        }

    private:
        T_ResourceNetworkVector& container_;
        const TER_Localisation& zone_;
        const std::string& type_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetResourceNetworksInZone
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetResourceNetworksInZone( T_ResourceNetworkVector& container, const TER_Localisation& zone, const std::string& type /*= std::string()*/ )
{
    const std::vector< const MIL_UrbanObject_ABC* >& blocks = MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocks();
    for( std::size_t i = 0; i < blocks.size(); ++i )
        FindResourceNetworks( *blocks[ i ], zone, type, container );
    sResourceNetworkInserter functor( container, zone, type );
    ApplyOnKnowledgesObject( functor );
}

namespace
{
    class sObjectKnowledgesCapacityPositionInside : private boost::noncopyable
    {
    public:
        sObjectKnowledgesCapacityPositionInside( const std::string& capacity, const MT_Vector2D& loc )
            : capacity_( capacity )
            , loc_     ( loc )
            , result_  ( false )
        {
            // NOTHING
        }

        void operator()( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( result_ )
                return;
            result_ = knowledge && knowledge->IsValid() && CapacityRetriever::RetrieveCapacity( knowledge->GetType(), capacity_ ) != 0
                && knowledge->GetLocalisation().IsInside( loc_);
        }

        bool Result() const
        {
            return result_;
        }

    private:
        const std::string& capacity_;
        const MT_Vector2D& loc_;
        bool result_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::IsPositionInsideObjectOfType
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgeObject::IsPositionInsideObjectOfType( const std::string& capacity, const MT_Vector2D& loc )
{
    sObjectKnowledgesCapacityPositionInside functor( capacity, loc );

    ApplyOnKnowledgesObject( functor );
    return functor.Result();
}

namespace
{
    class sObjectKnowledgesInZoneCapacityInserter : private boost::noncopyable
    {
    public:
        sObjectKnowledgesInZoneCapacityInserter( T_KnowledgeObjectDiaIDVector& container, const std::string& capacity, const TER_Localisation& zone )
            : pContainer_( container )
            , capacity_  ( capacity )
            , pZone_     ( zone )
        {
            // NOTHING
        }

        void operator()( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( knowledge && knowledge->IsValid() && CapacityRetriever::RetrieveCapacity( knowledge->GetType(), capacity_ ) != 0
                && pZone_.IsInside( knowledge->GetLocalisation().ComputeBarycenter() ) )
                pContainer_.push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector& pContainer_;
        const std::string& capacity_;
        const TER_Localisation& pZone_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsWithCapacityInZone
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsWithCapacityInZone( T_KnowledgeObjectDiaIDVector& container, const std::string& capacity, const TER_Localisation& zone )
{
    sObjectKnowledgesInZoneCapacityInserter functor( container, capacity, zone );
    ApplyOnKnowledgesObject( functor );
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: DEC_KnowledgeBlackBoard_Army::GetObjectsInZone
    // Created: ABR 2012-18-01
    // -----------------------------------------------------------------------------
    template< typename T >
    class sObjectKnowledgesIntersectingInZoneFilteredInserter : private boost::noncopyable
    {
    public:
        sObjectKnowledgesIntersectingInZoneFilteredInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const T& zone )
            : pContainer_( &container )
            , filter_    ( filter )
            , pZone_     ( &zone )
        {
            // NOTHING
        }

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( filter_.Test( knowledge->GetType() ) 
                && ( !knowledge->IsActivableObstacle() || knowledge->IsObstacleActivated() ) 
                && pZone_->IsIntersecting( knowledge->GetLocalisation() )
                && knowledge->IsValid() )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
        const T* pZone_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsIntersectingInZone
// Created: LGY 2013-01-28
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetObjectsIntersectingInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone )
{
    sObjectKnowledgesIntersectingInZoneFilteredInserter< TER_Localisation > functor( container, filter, zone );
    ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::SaveAllCurrentKnowledgeObject
// Created: MMC 2013-07-03
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::SaveAllCurrentKnowledgeObject()
{
    previousObjectMap_ = objectMap_;
}

