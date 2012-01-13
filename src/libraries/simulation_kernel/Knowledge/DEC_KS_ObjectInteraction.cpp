// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_ObjectInteraction.cpp $
// $Author: Jvt $
// $Modtime: 5/04/05 13:34 $
// $Revision: 4 $
// $Workfile: DEC_KS_ObjectInteraction.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_ObjectInteraction.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectCollision.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectPerception.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "MT_Tools/MT_Stl.h"
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_ObjectInteraction )

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectInteraction::DEC_KS_ObjectInteraction( DEC_KnowledgeBlackBoard_AgentPion& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_           ( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction constructor
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
DEC_KS_ObjectInteraction::DEC_KS_ObjectInteraction()
    : DEC_KnowledgeSource_ABC()
    , pBlackBoard_           ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectInteraction::~DEC_KS_ObjectInteraction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_AgentPion::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_ObjectInteraction::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_
            & objectInteractions_
            & objectCollisions_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::Prepare()
{
    assert( pBlackBoard_ );
    std::mem_fun_ref_t< void, DEC_Knowledge_ObjectCollision > objectFunctor = std::mem_fun_ref( &DEC_Knowledge_ObjectCollision::Prepare );
    pBlackBoard_->GetKnowledgeObjectCollisionContainer().ApplyOnKnowledgesObjectCollision( objectFunctor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::UpdateObjectPerceptionKnowledge
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception& DEC_KS_ObjectInteraction::GetKnowledgeObjectPerception( MIL_Object_ABC& object ) const
{
    assert( pBlackBoard_ );
    DEC_Knowledge_ObjectPerception* pKnowledge = pBlackBoard_->GetKnowledgeObjectPerceptionContainer().GetKnowledgeObjectPerception( object );
    if( !pKnowledge )
        pKnowledge = &pBlackBoard_->GetKnowledgeObjectPerceptionContainer().CreateKnowledgeObjectPerception( pBlackBoard_->GetPion(), object );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::UpdateObjectCollisionKnowledge
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision& DEC_KS_ObjectInteraction::GetKnowledgeObjectCollision( MIL_Object_ABC& object ) const
{
    assert( pBlackBoard_ );
    DEC_Knowledge_ObjectCollision* pKnowledge = pBlackBoard_->GetKnowledgeObjectCollisionContainer().GetKnowledgeObjectCollision( object );
    if( !pKnowledge )
        pKnowledge = &pBlackBoard_->GetKnowledgeObjectCollisionContainer().CreateKnowledgeObjectCollision( pBlackBoard_->GetPion(), object );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::Talk( int /*currentTimeStep*/ )
{
    // Interaction with objects
    for( CIT_ObjectVector itInteraction = objectInteractions_.begin(); itInteraction != objectInteractions_.end(); ++itInteraction )
    {
        MIL_Object_ABC& object = **itInteraction;
        if( object.IsMarkedForDestruction() )
            continue;
        DEC_Knowledge_ObjectPerception& knowledge = GetKnowledgeObjectPerception( object );
        knowledge.Update( PHY_PerceptionLevel::identified_ );
    }
    objectInteractions_.clear();

    // Collision with objects
    for( CIT_ObjectCollisionVector itCollision = objectCollisions_.begin(); itCollision != objectCollisions_.end(); ++itCollision )
    {
        MIL_Object_ABC& object = *itCollision->first;
        if( object.IsMarkedForDestruction() )
            continue;
        DEC_Knowledge_ObjectCollision& knowledge = GetKnowledgeObjectCollision( object );
        knowledge.Update( itCollision->second );
    }
    objectCollisions_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::CleanKnowledgeObjectCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::CleanKnowledgeObjectCollision( DEC_Knowledge_ObjectCollision& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgeObjectCollisionContainer().DestroyKnowledgeObjectCollision( knowledge ); // NB - The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::Clean()
{
    // Remove all invalid knowledges
    assert( pBlackBoard_ );
    class_mem_fun_void_t< DEC_KS_ObjectInteraction, DEC_Knowledge_ObjectCollision> method( & DEC_KS_ObjectInteraction::CleanKnowledgeObjectCollision, *this );
    pBlackBoard_->GetKnowledgeObjectCollisionContainer().ApplyOnKnowledgesObjectCollision( method );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::NotifyObjectInteraction
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::NotifyObjectInteraction( MIL_Object_ABC& object )
{
    objectInteractions_.push_back( &object );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::NotifyObjectCollision
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::NotifyObjectCollision( MIL_Object_ABC& object, const MT_Vector2D& vPosition )
{
    const TER_Localisation& objectLocation = object.GetLocalisation();
    MT_Vector2D collisionPosition;
    objectLocation.ComputeNearestPoint( vPosition, collisionPosition );
    objectCollisions_.push_back( std::make_pair( &object, collisionPosition ) );
}
