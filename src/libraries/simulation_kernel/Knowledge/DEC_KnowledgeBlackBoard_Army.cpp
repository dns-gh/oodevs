// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_Army.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeBlackBoard_Army.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "DEC_KS_UrbanKnowledgeSynthetizer.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "DEC_Knowledge_Urban.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/InteractWithEnemyCapacity.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KnowledgeBlackBoard_Army )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::DEC_KnowledgeBlackBoard_Army( MIL_Army_ABC& army )
    : DEC_KnowledgeBlackBoard_ABC()
    , pArmy_                        ( &army )
    , pKnowledgeObjectContainer_    ( new DEC_BlackBoard_CanContainKnowledgeObject() )
    , pKnowledgeUrbanContainer_     ( new DEC_BlackBoard_CanContainKnowledgeUrban( army ) )
    , pKsObjectKnowledgeSynthetizer_( new DEC_KS_ObjectKnowledgeSynthetizer( *this ) )
    , pKsUrbanKnowledgeSynthetizer_ ( new DEC_KS_UrbanKnowledgeSynthetizer( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::DEC_KnowledgeBlackBoard_Army()
    : DEC_KnowledgeBlackBoard_ABC()
    , pArmy_                        ( 0 )
    , pKnowledgeObjectContainer_    ( 0 )
    , pKnowledgeUrbanContainer_     ( 0 )
    , pKsObjectKnowledgeSynthetizer_( 0 )
    , pKsUrbanKnowledgeSynthetizer_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::~DEC_KnowledgeBlackBoard_Army()
{
    delete pKnowledgeObjectContainer_;
    delete pKnowledgeUrbanContainer_;
    delete pKsObjectKnowledgeSynthetizer_;
    delete pKsUrbanKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KnowledgeBlackBoard_Army::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            & pArmy_
            & pKnowledgeObjectContainer_
            & pKnowledgeUrbanContainer_
            & pKsObjectKnowledgeSynthetizer_;
            & pKsUrbanKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::SendFullState( unsigned int ) const
{
    std::mem_fun_ref_t< void, DEC_Knowledge_Object > functorObject = std::mem_fun_ref( &DEC_Knowledge_Object::SendStateToNewClient );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObjectRef( functorObject );
    std::mem_fun_ref_t< void, DEC_Knowledge_Urban > functorUrban = std::mem_fun_ref( &DEC_Knowledge_Urban::SendStateToNewClient );
    pKnowledgeUrbanContainer_->ApplyOnKnowledgesUrbanRef( functorUrban );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::SendChangedState() const
{
    std::mem_fun_ref_t< void, DEC_Knowledge_Object > functorObject = std::mem_fun_ref( &DEC_Knowledge_Object::UpdateOnNetwork );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObjectRef( functorObject );
    std::mem_fun_ref_t< void, DEC_Knowledge_Urban > functorUrban = std::mem_fun_ref( &DEC_Knowledge_Urban::UpdateOnNetwork );
    pKnowledgeUrbanContainer_->ApplyOnKnowledgesUrbanRef( functorUrban );
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

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( filter_.Test( knowledge->GetType() ) )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
    };
}

void DEC_KnowledgeBlackBoard_Army::GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter ) const
{
    sObjectKnowledgesDiaIDInserter functor( container, filter );

    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

namespace
{
    class sObjectKnowledgesInCircleFilteredInserter : boost::noncopyable
    {
    public:
        sObjectKnowledgesInCircleFilteredInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const MT_Vector2D& center, double rRadius )
            : pContainer_( &container )
            , filter_    ( filter )
            , pCenter_   ( &center )
            , rRadius_   ( rRadius )
        {
            // NOTHING
        }

        void operator() ( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( knowledge->IsValid()
            && ( !knowledge->IsReservedObstacle() || knowledge->IsReservedObstacleActivated() )
            && filter_.Test( knowledge->GetType() ) 
            && knowledge->GetLocalisation().ComputeBarycenter().Distance( *pCenter_ ) <= rRadius_ )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectDiaIDVector* pContainer_;
        const MIL_ObjectFilter& filter_;
        const MT_Vector2D* pCenter_;
        double rRadius_;
    };
}

void DEC_KnowledgeBlackBoard_Army::GetObjectsInCircle( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const MT_Vector2D& center, double rRadius )
{
    sObjectKnowledgesInCircleFilteredInserter functor( container, filter, center, rRadius );

    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: DEC_KnowledgeBlackBoard_Army::GetObjectsInZone
    // Created: NLD 2006-05-05
    // -----------------------------------------------------------------------------
    template< typename T >
    class sObjectKnowledgesInZoneFilteredInserter
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
}

void DEC_KnowledgeBlackBoard_Army::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone )
{
    sObjectKnowledgesInZoneFilteredInserter< TER_Localisation > functor( container, filter, zone );

    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

void DEC_KnowledgeBlackBoard_Army::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Polygon& zone )
{
    sObjectKnowledgesInZoneFilteredInserter< TER_Polygon > functor( container, filter, zone );

    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: DEC_KnowledgeBlackBoard_Army::sObjectKnowledgesFilteredHeightInserter
    // Created: NLD 2005-05-09
    // -----------------------------------------------------------------------------
    class sObjectKnowledgesFilteredHeightInserter
    {
    public:
        sObjectKnowledgesFilteredHeightInserter( T_KnowledgeObjectVector& container, double rHeight, const MIL_ObjectFilter& filter, const MIL_Army_ABC* army )
            : pContainer_( &container )
            , rHeight_   ( rHeight )
            , filter_    ( filter )
            , army_      ( army )
        {
            // NOTHING
        }

        void operator()( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( !knowledge->IsValid() )
                return;
            if( knowledge->GetType().GetCapacity< InteractWithEnemyCapacity >() != 0 && &knowledge->GetArmy() == army_ )
                return;
            if( filter_.Test( knowledge->GetType() )
                && rHeight_ <= knowledge->GetMaxInteractionHeight() )///$$$ A ENCAPSULER DEC_Knowledge_Object::CanInteractWith()
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectVector*  pContainer_;
        const double rHeight_;
        const MIL_ObjectFilter& filter_;
        const MIL_Army_ABC* army_;
    };
}

void DEC_KnowledgeBlackBoard_Army::GetObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, double rHeight, const MIL_ObjectFilter& filter ) const
{
    bool useCache = ( dynamic_cast< const MIL_DangerousObjectFilter* >( &filter ) != 0 );
    if( useCache && pKnowledgeObjectContainer_->HasObjectsAtInteractionHeightCache( rHeight ) )
    {
        pKnowledgeObjectContainer_->GetCachedObjectsAtInteractionHeight( container, rHeight );
        return ;
    }
    sObjectKnowledgesFilteredHeightInserter functor( container, rHeight, filter, pArmy_ );

    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
    if( useCache )
        pKnowledgeObjectContainer_->SetCachedObjectsAtInteractionHeight( container, rHeight );
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

        void operator() ( boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
        {
            if( ( !knowledge->IsReservedObstacle() || knowledge->IsReservedObstacleActivated() ) && knowledge->IsValid() )
                pContainer_->push_back( knowledge );
        }

    private:
        T_KnowledgeObjectVector* pContainer_;
    };
}

void DEC_KnowledgeBlackBoard_Army::GetObjects( T_KnowledgeObjectVector& container ) const
{
    sObjectKnowledgesInserter functor( container );

    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
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
}

boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetClosestObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter  ) const
{
    sClosestObjectInserter functor( vPos, filter );

    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
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
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetClosestFriendObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter  ) const
{
    assert( pArmy_ );
    sClosestObjectFriendInserter functor( vPos, *pArmy_, filter );

    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
    return functor.pResult_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgesObject
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::GetKnowledgesObject( T_KnowledgeObjectVector& container ) const
{
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->GetKnowledgesObject( container );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromID
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromID( unsigned int nID ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromObjectID
// Created: JSR 2011-09-29
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromObjectID( unsigned int nID ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->GetKnowledgeObjectFromObjectID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject( const MIL_Object_ABC& object ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->GetKnowledgeObject( object );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::Finalize()
{
    pKnowledgeUrbanContainer_->Finalize();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::IsKnown
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_Army::IsKnown( const MIL_Object_ABC& object ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->HasKnowledgeObject( object );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject( const DEC_Knowledge_ObjectCollision& collision ) const
{
    return GetKnowledgeObject( collision.GetObject() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent( const sword::UnitKnowledgeId& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent( const MIL_Agent_ABC& /*agent*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject( const sword::ObjectKnowledgeId& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject( const MIL_Object_ABC& /*object*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObjectByObjectID
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObjectByObjectID( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation( const sword::CrowdKnowledgeId& /*asn*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation( const MIL_Population& /*population*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation( unsigned int /*nID*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject& DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectContainer() const
{
    assert( pKnowledgeObjectContainer_ );
    return *pKnowledgeObjectContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeUrbanContainer
// Created: MGD 2009-12-02
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrban& DEC_KnowledgeBlackBoard_Army::GetKnowledgeUrbanContainer() const
{
    assert( pKnowledgeUrbanContainer_ );
    return *pKnowledgeUrbanContainer_;
}


namespace
{
    class sUrbanKnowledgesInserter
    {
    public:
        sUrbanKnowledgesInserter( T_UrbanObjectVector& container )
        : pContainer_( &container )
        {
        }

        void operator() ( UrbanObjectWrapper* pUrbanObject )
        {
            pContainer_->push_back( pUrbanObject );
        }

    private:
        T_UrbanObjectVector* pContainer_;
    };
}
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetObjects
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::GetUrbanObjects( T_UrbanObjectVector& container ) const
{
    sUrbanKnowledgesInserter functor( container );

    assert( pKnowledgeUrbanContainer_ );
    pKnowledgeUrbanContainer_->ApplyOnUrbanBlocks( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetArmy
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
MIL_Army_ABC& DEC_KnowledgeBlackBoard_Army::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKSObjectKnowledgeSynthetizer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_ObjectKnowledgeSynthetizer& DEC_KnowledgeBlackBoard_Army::GetKsObjectKnowledgeSynthetizer() const
{
    assert( pKsObjectKnowledgeSynthetizer_ );
    return *pKsObjectKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKSUrbanKnowledgeSynthetizer
// Created: MGD 2009-12-02
// -----------------------------------------------------------------------------
DEC_KS_UrbanKnowledgeSynthetizer& DEC_KnowledgeBlackBoard_Army::GetKsUrbanKnowledgeSynthetizer() const
{
    assert( pKsUrbanKnowledgeSynthetizer_ );
    return *pKsUrbanKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::Accept
// Created: LGY 2012-01-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    if( pKnowledgeObjectContainer_ )
        pKnowledgeObjectContainer_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::Update
// Created: LDC 2012-02-02
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::Update( int currentTimeStep )
{
    if( pArmy_ && pKnowledgeObjectContainer_ )
        pKnowledgeObjectContainer_->UpdateUniversalObjects( *pArmy_ );
    DEC_KnowledgeBlackBoard_ABC::Update( currentTimeStep );
    if( pKnowledgeObjectContainer_ )
        pKnowledgeObjectContainer_->Prepare();
}