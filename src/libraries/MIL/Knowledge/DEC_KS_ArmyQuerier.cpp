// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_ArmyQuerier.cpp $
// $Author: Nld $
// $Modtime: 9/05/05 14:46 $
// $Revision: 8 $
// $Workfile: DEC_KS_ArmyQuerier.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_ArmyQuerier.h"

#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_ObjectCollision.h"

#include "Entities/Objects/MIL_RealObjectTypeFilter.h"

#include "Entities/MIL_Army.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ArmyQuerier::DEC_KS_ArmyQuerier( DEC_KnowledgeBlackBoard& blackBoard, const MIL_Army& army )
    : DEC_KnowledgeSource_ABC( blackBoard )
    , pArmy_                 ( &army )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ArmyQuerier::~DEC_KS_ArmyQuerier()
{
    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::GetKnowledgesObject
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

void DEC_KS_ArmyQuerier::GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter ) const
{
    sObjectKnowledgesDiaIDInserter functor( container, filter );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesObject( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::GetKnowledgesObject
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
        if( pFilter_->Test( knowledge.GetType() ) && !knowledge.IsPrepared() )
            pContainer_->push_back( &knowledge );
    }

private:
          T_KnowledgeObjectVector* pContainer_;
    const MIL_RealObjectTypeFilter*    pFilter_;
};

void DEC_KS_ArmyQuerier::GetObjects( T_KnowledgeObjectVector& container, const MIL_RealObjectTypeFilter& filter ) const
{
    sObjectKnowledgesFilteredInserter functor( container, filter );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesObject( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::GetObjects
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
        if( !knowledge.IsPrepared() )
            pContainer_->push_back( &knowledge );
    }

private:
    T_KnowledgeObjectVector* pContainer_;
};

void DEC_KS_ArmyQuerier::GetObjects( T_KnowledgeObjectVector& container ) const
{
    sObjectKnowledgesInserter functor( container );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesObject( functor );           
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::GetClosestObject
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

DEC_Knowledge_Object* DEC_KS_ArmyQuerier::GetClosestObject( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter  ) const
{
    sClosestObjectInserter functor( vPos, filter );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesObject( functor );
    return functor.pResult_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ArmyQuerier::GetKnowledgeObject
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KS_ArmyQuerier::GetKnowledgeObject( const DEC_Knowledge_ObjectCollision& collision ) const
{
    T_KnowledgeObjectVector knowledges;
    
    assert( pBlackBoard_ );
    pBlackBoard_->GetKnowledgesObject( collision.GetObject(), knowledges );
    
    return knowledges.empty() ? 0 : knowledges.front();    
}
