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
#include "DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_RealObjectTypeFilter.h"

BOOST_CLASS_EXPORT_GUID( DEC_KnowledgeBlackBoard_Army, "DEC_KnowledgeBlackBoard_Army" )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::DEC_KnowledgeBlackBoard_Army( MIL_Army& army )
    : DEC_KnowledgeBlackBoard_ABC   ()
    , pArmy_                        ( &army )
    , pKnowledgeObjectContainer_    ( new DEC_BlackBoard_CanContainKnowledgeObject() )
    , pKsObjectKnowledgeSynthetizer_( new DEC_KS_ObjectKnowledgeSynthetizer       ( *this ) )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::DEC_KnowledgeBlackBoard_Army()
    : DEC_KnowledgeBlackBoard_ABC   ()
    , pArmy_                        ( 0 )
    , pKnowledgeObjectContainer_    ( 0 )
    , pKsObjectKnowledgeSynthetizer_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::~DEC_KnowledgeBlackBoard_Army()
{
    delete pKnowledgeObjectContainer_;
    delete pKsObjectKnowledgeSynthetizer_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_Army::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KnowledgeBlackBoard_Army::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            & pArmy_
            & pKnowledgeObjectContainer_
            & pKsObjectKnowledgeSynthetizer_;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::SendFullState() const
{
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( std::mem_fun_ref( & DEC_Knowledge_Object::SendStateToNewClient ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::SendChangedState() const
{
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( std::mem_fun_ref( & DEC_Knowledge_Object::UpdateOnNetwork ) );
}

// =============================================================================
// QUERIES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgesObject
// Created: NLD 2004-05-06
// -----------------------------------------------------------------------------
class sObjectKnowledgesDiaIDInserter
{
public:
    sObjectKnowledgesDiaIDInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter )
        : pContainer_( &container )
        , pFilter_   ( &filter    )
    {
    }

    void operator() ( DEC_Knowledge_Object& knowledge )
    {
        if( pFilter_->Test( knowledge.GetType() ) )
            pContainer_->push_back( (void*)knowledge.GetID() );
    }

private:
          T_KnowledgeObjectDiaIDVector* pContainer_;
    const MIL_RealObjectTypeFilter*     pFilter_;
};

void DEC_KnowledgeBlackBoard_Army::GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter ) const
{
    sObjectKnowledgesDiaIDInserter functor( container, filter );
    
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgesObject
// Created: NLD 2004-05-06
// -----------------------------------------------------------------------------
class sObjectKnowledgesFilteredInserter
{
public:
    sObjectKnowledgesFilteredInserter( T_KnowledgeObjectVector& container, const MIL_RealObjectTypeFilter& filter )
        : pContainer_( &container )
        , pFilter_   ( &filter    )
    {
    }

    void operator() ( DEC_Knowledge_Object& knowledge )
    {
        if( pFilter_->Test( knowledge.GetType() ) && !knowledge.IsPrepared() ) //$$ ??
            pContainer_->push_back( &knowledge );
    }

private:
          T_KnowledgeObjectVector* pContainer_;
    const MIL_RealObjectTypeFilter*    pFilter_;
};

void DEC_KnowledgeBlackBoard_Army::GetObjects( T_KnowledgeObjectVector& container, const MIL_RealObjectTypeFilter& filter ) const
{
    sObjectKnowledgesFilteredInserter functor( container, filter );
    
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetObjectsInCircle
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
class sObjectKnowledgesInCircleFilteredInserter
{
public:
    sObjectKnowledgesInCircleFilteredInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter, const MT_Vector2D& center, MT_Float rRadius )
        : pContainer_( &container )
        , pFilter_   ( &filter    )
        , pCenter_   ( &center    )
        , rRadius_   ( rRadius    )
    {
    }

    void operator() ( DEC_Knowledge_Object& knowledge )
    {
        if( pFilter_->Test( knowledge.GetType() ) && !knowledge.IsPrepared() && knowledge.GetLocalisation().ComputeBarycenter().Distance( *pCenter_ ) <= rRadius_ )
            pContainer_->push_back( (void*)knowledge.GetID()  );
    }

private:
          T_KnowledgeObjectDiaIDVector* pContainer_;
    const MIL_RealObjectTypeFilter*     pFilter_;
    const MT_Vector2D*                  pCenter_;
          MT_Float                      rRadius_;
};

void DEC_KnowledgeBlackBoard_Army::GetObjectsInCircle( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter, const MT_Vector2D& center, MT_Float rRadius )
{
    sObjectKnowledgesInCircleFilteredInserter functor( container, filter, center, rRadius );
    
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetObjectsInZone
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T >
class sObjectKnowledgesInZoneFilteredInserter
{
public:
    sObjectKnowledgesInZoneFilteredInserter( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter, const T& zone )
        : pContainer_( &container )
        , pFilter_   ( &filter    )
        , pZone_     ( &zone      )
    {
    }

    void operator() ( DEC_Knowledge_Object& knowledge )
    {
        if( pFilter_->Test( knowledge.GetType() ) && !knowledge.IsPrepared() && pZone_->IsInside( knowledge.GetLocalisation().ComputeBarycenter() ) )
            pContainer_->push_back( (void*)knowledge.GetID()  );
    }

private:
          T_KnowledgeObjectDiaIDVector* pContainer_;
    const MIL_RealObjectTypeFilter*     pFilter_;
    const T*                            pZone_;
};

void DEC_KnowledgeBlackBoard_Army::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter, const TER_Localisation& zone )
{
    sObjectKnowledgesInZoneFilteredInserter< TER_Localisation > functor( container, filter, zone );
   
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );           
}

void DEC_KnowledgeBlackBoard_Army::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter, const TER_Polygon& zone )
{
    sObjectKnowledgesInZoneFilteredInserter< TER_Polygon > functor( container, filter, zone );
   
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::sObjectKnowledgesFilteredHeightInserter
// Created: NLD 2005-05-09
// -----------------------------------------------------------------------------
class sObjectKnowledgesFilteredHeightInserter
{
public:
    sObjectKnowledgesFilteredHeightInserter( T_KnowledgeObjectVector& container, MT_Float rHeight, const MIL_RealObjectTypeFilter& filter )
        : pContainer_( &container )
        , rHeight_   ( rHeight    )
        , pFilter_   ( &filter    )
    {
    }

    void operator() ( DEC_Knowledge_Object& knowledge )
    {
        if( pFilter_->Test( knowledge.GetType() ) && rHeight_ <= knowledge.GetMaxInteractionHeight() ) ///$$$ A ENCAPSULER DEC_Knowledge_Object::CanInteractWith()
            pContainer_->push_back( &knowledge );
    }

private:
          T_KnowledgeObjectVector*  pContainer_;
    const MT_Float                  rHeight_;
    const MIL_RealObjectTypeFilter* pFilter_;
};

void DEC_KnowledgeBlackBoard_Army::GetObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, MT_Float rHeight, const MIL_RealObjectTypeFilter& filter ) const
{
    sObjectKnowledgesFilteredHeightInserter functor( container, rHeight, filter );
    
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );           
}

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
    }

    void operator() ( DEC_Knowledge_Object& knowledge )
    {
        if( !knowledge.IsPrepared() ) //$$ ??
            pContainer_->push_back( &knowledge );
    }

private:
    T_KnowledgeObjectVector* pContainer_;
};

void DEC_KnowledgeBlackBoard_Army::GetObjects( T_KnowledgeObjectVector& container ) const
{
    sObjectKnowledgesInserter functor( container );
    
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetClosestObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
class sClosestObjectInserter
{
public:
    sClosestObjectInserter( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter )
        : pResult_     ( 0 )
        , rClosestDist_( std::numeric_limits< MT_Float >::max() )
        , pPos_        ( &vPos )
        , pFilter_     ( &filter    )
    {
    }

    void operator() ( DEC_Knowledge_Object& knowledge )
    {
        if( !pFilter_->Test( knowledge.GetType() ) )
            return;

        const MT_Float rDist = knowledge.GetLocalisation().ComputeBarycenter().Distance( *pPos_ );
        if( rDist > rClosestDist_ )
            return;
        rClosestDist_ = rDist;
        pResult_      = &knowledge;
    }

public:
    DEC_Knowledge_Object* pResult_;

private:
          MT_Float                  rClosestDist_;
    const MT_Vector2D*              pPos_;   
    const MIL_RealObjectTypeFilter* pFilter_;
};

DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_Army::GetClosestObject( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter  ) const
{
    sClosestObjectInserter functor( vPos, filter );
    
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( functor );
    return functor.pResult_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::sClosestObjectFriendInserter
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
class sClosestObjectFriendInserter
{
public:
    sClosestObjectFriendInserter( const MT_Vector2D& vPos, const MIL_Army& army, const MIL_RealObjectTypeFilter& filter )
        : pResult_     ( 0 )
        , pArmy_       ( &army )
        , rClosestDist_( std::numeric_limits< MT_Float >::max() )
        , pPos_        ( &vPos )
        , pFilter_     ( &filter    )
    {
    }

    void operator() ( DEC_Knowledge_Object& knowledge )
    {
        if( !pFilter_->Test( knowledge.GetType() ) )
            return;

        if( pArmy_->IsAFriend( knowledge.GetArmy() ) != eTristate_True )
            return;

        const MT_Float rDist = knowledge.GetLocalisation().ComputeBarycenter().Distance( *pPos_ );
        if( rDist > rClosestDist_ )
            return;
        rClosestDist_ = rDist;
        pResult_      = &knowledge;
    }

public:
    DEC_Knowledge_Object* pResult_;

private:
          MT_Float                  rClosestDist_;
    const MIL_Army*                 pArmy_;
    const MT_Vector2D*              pPos_;   
    const MIL_RealObjectTypeFilter* pFilter_;
};

DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_Army::GetClosestFriendObject( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter  ) const
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
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromID( uint nID ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject( MIL_RealObject_ABC& object ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->GetKnowledgeObject( object );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::IsKnown
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_Army::IsKnown( const MIL_RealObject_ABC& object ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->HasKnowledgeObject( object );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject( const DEC_Knowledge_ObjectCollision& collision ) const
{
    return GetKnowledgeObject( collision.GetObject() );
}
