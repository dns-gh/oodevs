// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeBlackBoardObjects_ABC.h"
#include "DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_Knowledge_Object.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/CapacityRetriever.h"
#include "Entities/Objects/InteractWithSideCapacity.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC constructor
// Created: JSR 2013-06-26
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoardObjects_ABC::DEC_KnowledgeBlackBoardObjects_ABC( MIL_KnowledgeGroup* pKnowledgeGroup )
    : pKnowledgeGroup_( pKnowledgeGroup )
    , pKnowledgeObjectContainer_( new DEC_BlackBoard_CanContainKnowledgeObject() )
    , pKsObjectKnowledgeSynthetizer_( new DEC_KS_ObjectKnowledgeSynthetizer( *this ) )
{
    pKnowledgeObjectContainer_->SetKnowledgeGroup( pKnowledgeGroup );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC constructor
// Created: JSR 2013-06-24
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoardObjects_ABC::DEC_KnowledgeBlackBoardObjects_ABC()
    : pKnowledgeGroup_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC destructor
// Created: JSR 2013-06-24
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoardObjects_ABC::~DEC_KnowledgeBlackBoardObjects_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::load
// Created: JSR 2013-06-26
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::load( MIL_CheckPointInArchive& archive, const unsigned int )
{
    DEC_BlackBoard_CanContainKnowledgeObject* bb;
    DEC_KS_ObjectKnowledgeSynthetizer* ks;
    archive >> boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            >> bb
            >> ks;
    pKnowledgeObjectContainer_.reset( bb );
    pKsObjectKnowledgeSynthetizer_.reset( ks );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::save
// Created: JSR 2013-06-26
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
{
    DEC_BlackBoard_CanContainKnowledgeObject* bb = pKnowledgeObjectContainer_.get();
    DEC_KS_ObjectKnowledgeSynthetizer* ks = pKsObjectKnowledgeSynthetizer_.get();
    archive << boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            << bb
            << ks;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::Update
// Created: JSR 2013-06-24
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::Update( int currentTimeStep )
{
    pKnowledgeObjectContainer_->Prepare();
    DEC_KnowledgeBlackBoard_ABC::Update( currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::SendFullState
// Created: JSR 2013-06-24
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::SendFullState( unsigned int /*nCtx*/ ) const
{
    std::mem_fun_ref_t< void, DEC_Knowledge_Object > functorObject = std::mem_fun_ref( &DEC_Knowledge_Object::SendStateToNewClient );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObjectRef( functorObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::SendObjectChangedState
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::SendObjectChangedState() const
{
    boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) > objectFunctor = boost::bind( &DEC_Knowledge_Object::UpdateOnNetwork, _1 );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( objectFunctor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::Accept
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    pKnowledgeObjectContainer_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::UpdateUniversalObjects
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::UpdateUniversalObjects( const MIL_Army_ABC* army )
{
    if( !pKnowledgeGroup_ )
        pKnowledgeObjectContainer_->UpdateUniversalObjects( army );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::SetKnowledgeGroup
// Created: JSR 2013-06-26
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::SetKnowledgeGroup( MIL_KnowledgeGroup* group )
{
    pKnowledgeGroup_ = group;
    GetKnowledgeObjectContainer().SetKnowledgeGroup( group );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetKnowledgeGroup
// Created: JSR 2013-06-26
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > DEC_KnowledgeBlackBoardObjects_ABC::GetKnowledgeGroup() const
{
    if( pKnowledgeGroup_ )
        return pKnowledgeGroup_->shared_from_this();
    return boost::shared_ptr< MIL_KnowledgeGroup >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetKnowledgeObjectContainer
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject& DEC_KnowledgeBlackBoardObjects_ABC::GetKnowledgeObjectContainer() const
{
    return *pKnowledgeObjectContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetKsObjectKnowledgeSynthetizer
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
DEC_KS_ObjectKnowledgeSynthetizer& DEC_KnowledgeBlackBoardObjects_ABC::GetKsObjectKnowledgeSynthetizer()
{
    return *pKsObjectKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetKnowledgesObject
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetKnowledgesObject( T_KnowledgeObjectVector& container ) const
{
    pKnowledgeObjectContainer_->GetKnowledgesObject( container );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::ResolveKnowledgeObject
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoardObjects_ABC::ResolveKnowledgeObject( const sword::ObjectKnowledgeId& asn ) const
{
    return ResolveKnowledgeObject( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::ResolveKnowledgeObject
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoardObjects_ABC::ResolveKnowledgeObject( const MIL_Object_ABC& object ) const
{
    return pKnowledgeObjectContainer_->GetKnowledgeObject( object );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::ResolveKnowledgeObject
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoardObjects_ABC::ResolveKnowledgeObject( unsigned int nID ) const
{
    return pKnowledgeObjectContainer_->GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::ResolveKnowledgeObjectByObjectID
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoardObjects_ABC::ResolveKnowledgeObjectByObjectID( unsigned int nID ) const
{
    return pKnowledgeObjectContainer_->GetKnowledgeObjectFromObjectID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::IsKnown
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoardObjects_ABC::IsKnown( const MIL_Object_ABC& object ) const
{
    return pKnowledgeObjectContainer_->HasKnowledgeObject( object );
}

namespace
{
    class sObjectKnowledgesInserter : boost::noncopyable
    {
    public:
        sObjectKnowledgesInserter( T_KnowledgeObjectVector& container )
            : pContainer_( &container )
        {
            // NOTHING
        }

        void operator() ( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( ( !knowledge->IsReservedObstacle() || knowledge->IsReservedObstacleActivated() ) && knowledge->IsValid() )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectVector* pContainer_;
    };

    class sObjectKnowledgesDiaIDInserter : boost::noncopyable
    {
    public:
        sObjectKnowledgesDiaIDInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter )
            : pContainer_( &container )
            , filter_    ( filter )
        {
            // NOTHING
        }

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( filter_.Test( knowledge->GetType() ) )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
    };

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

        void operator() ( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( knowledge->IsValid()
                && ( nonActivatedObstacles_ || ( !knowledge->IsReservedObstacle() || knowledge->IsReservedObstacleActivated() ) )
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

    template< typename T >
    class sObjectKnowledgesInZoneFilteredInserter : boost::noncopyable
    {
    public:
        sObjectKnowledgesInZoneFilteredInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const T& zone )
            : pContainer_( &container )
            , filter_    ( filter )
            , pZone_     ( &zone )
        {
            // NOTHING
        }

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( filter_.Test( knowledge->GetType() ) 
                && ( !knowledge->IsReservedObstacle() || knowledge->IsReservedObstacleActivated() ) 
                && pZone_->IsInside( knowledge->GetLocalisation().ComputeBarycenter() ) 
                && knowledge->IsValid() )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
        const T* pZone_;
    };

    template< typename T >
    class sObjectKnowledgesIntersectingInZoneFilteredInserter : boost::noncopyable
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
                && ( !knowledge->IsReservedObstacle() || knowledge->IsReservedObstacleActivated() ) 
                && pZone_->IsIntersecting( knowledge->GetLocalisation() )
                && knowledge->IsValid() )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
        const T* pZone_;
    };

    class sObjectKnowledgesInZoneCapacityInserter : boost::noncopyable
    {
    public:
        sObjectKnowledgesInZoneCapacityInserter( T_KnowledgeObjectDiaIDVector& container, const std::string& capacity, const TER_Localisation& zone )
            : pContainer_( container )
            , capacity_  ( capacity )
            , pZone_     ( zone )
        {
            // NOTHING
        }

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
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

    class sObjectKnowledgesCapacityPositionInside : boost::noncopyable
    {
    public:
        sObjectKnowledgesCapacityPositionInside( const std::string& capacity, const MT_Vector2D& loc )
            : capacity_( capacity )
            , loc_     ( loc )
            , result_  ( false )
        {
            // NOTHING
        }

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
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

    class sObjectKnowledgesFilteredHeightInserter : boost::noncopyable
    {
    public:
        sObjectKnowledgesFilteredHeightInserter( T_KnowledgeObjectVector& container, const MIL_Agent_ABC& agent, const MIL_ObjectFilter& filter, const MIL_Army_ABC& army )
            : pContainer_( &container )
            , agent_     ( agent )
            , filter_    ( filter )
            , army_      ( army )
        {
            // NOTHING
        }

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( !knowledge->IsValid() )
                return;
            if( !knowledge->CanCollideWith( agent_ ) )
                return;
            const InteractWithSideCapacity* pSideInteraction = knowledge->GetType().GetCapacity< InteractWithSideCapacity >();
            if( pSideInteraction && &knowledge->GetArmy() )
            {
                if( pSideInteraction->IsPossible( knowledge->GetArmy(), army_ ) )
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
        const MIL_Army_ABC& army_;
    };

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

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
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

        void operator() ( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( !knowledge->IsValid() )
                return;

            if( !filter_.Test( knowledge->GetType() ) )
                return;

            if( pArmy_->IsAFriend( knowledge->GetArmy() ) != eTristate_True )
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
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetObjects
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetObjects( T_KnowledgeObjectVector& container ) const
{
    sObjectKnowledgesInserter functor( container );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetObjects
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter ) const
{
    sObjectKnowledgesDiaIDInserter functor( container, filter );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsInCircle
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsInCircle( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const MT_Vector2D& center, double rRadius, bool nonActivatedObstacles ) const
{
    sObjectKnowledgesInCircleFilteredInserter functor( container, filter, center, rRadius, nonActivatedObstacles );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsInZone
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone ) const
{
    sObjectKnowledgesInZoneFilteredInserter< TER_Localisation > functor( container, filter, zone );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsInZone
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Polygon& zone ) const
{
    sObjectKnowledgesInZoneFilteredInserter< TER_Polygon > functor( container, filter, zone );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsIntersectingInZone
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsIntersectingInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone ) const
{
    sObjectKnowledgesIntersectingInZoneFilteredInserter< TER_Localisation > functor( container, filter, zone );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsWithCapacityInZone
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsWithCapacityInZone( T_KnowledgeObjectDiaIDVector& container, const std::string& capacity, const TER_Localisation& zone ) const
{
    sObjectKnowledgesInZoneCapacityInserter functor( container, capacity, zone );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::IsPositionInsideObjectOfType
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoardObjects_ABC::IsPositionInsideObjectOfType( const std::string& capacity, const MT_Vector2D& loc ) const
{
    sObjectKnowledgesCapacityPositionInside functor( capacity, loc );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
    return functor.Result();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsAtInteractionHeight
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoardObjects_ABC::GetObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, const MIL_Agent_ABC& agent, const MIL_ObjectFilter& filter ) const
{
    bool useCache = ( dynamic_cast< const MIL_DangerousObjectFilter* >( &filter ) != 0 );
    const double rHeight = agent.GetRole< PHY_RoleInterface_Location >().GetHeight();
    if( useCache && pKnowledgeObjectContainer_->HasObjectsAtInteractionHeightCache( rHeight ) )
    {
        pKnowledgeObjectContainer_->GetCachedObjectsAtInteractionHeight( container, rHeight );
        return ;
    }
    sObjectKnowledgesFilteredHeightInserter functor( container, agent, filter, GetArmy() );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
    if( useCache )
        pKnowledgeObjectContainer_->SetCachedObjectsAtInteractionHeight( container, rHeight );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetClosestObject
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoardObjects_ABC::GetClosestObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter ) const
{
    sClosestObjectInserter functor( vPos, filter );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
    return functor.pResult_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoardObjects_ABC::GetClosestFriendObject
// Created: JSR 2013-06-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoardObjects_ABC::GetClosestFriendObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter ) const
{
    sClosestObjectFriendInserter functor( vPos, GetArmy(), filter );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
    return functor.pResult_;
}
