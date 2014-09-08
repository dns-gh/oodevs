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
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/DisasterCapacity.h"
#include "simulation_terrain/TER_World.h"
#include <boost/serialization/utility.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_ObjectInteraction )

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectInteraction::DEC_KS_ObjectInteraction( DEC_KnowledgeBlackBoard_AgentPion& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction constructor
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
DEC_KS_ObjectInteraction::DEC_KS_ObjectInteraction()
    : pBlackBoard_( 0 )
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
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this );
    archive & pBlackBoard_;
    archive & objectInteractions_;
    archive & objectCollisions_;
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
    for( auto it = objectCollisions_.begin(); it != objectCollisions_.end(); )
    {
        MIL_Object_ABC& object = *it->first;
        if( object.IsMarkedForDestruction() )
            it = objectCollisions_.erase( it );
        else
            ++it;
    }
    for( auto it = objectInteractions_.begin(); it != objectInteractions_.end(); )
    {
        MIL_Object_ABC& object = **it;
        if( object.IsMarkedForDestruction() )
            it = objectInteractions_.erase( it );
        else
            ++it;
    }
    if( !pBlackBoard_->GetPion().RetrieveRole< PHY_RoleInterface_Perceiver >() )
        return;
    pBlackBoard_->GetPion().GetRole< PHY_RoleInterface_Perceiver >().ExecuteCollisions();

    // Interaction with objects
    for( auto itInteraction = objectInteractions_.begin(); itInteraction != objectInteractions_.end(); ++itInteraction )
    {
        MIL_Object_ABC& object = **itInteraction;
        if( object.IsMarkedForDestruction() )
            continue;
        DEC_Knowledge_ObjectPerception& knowledge = GetKnowledgeObjectPerception( object );
        knowledge.Update( PHY_PerceptionLevel::identified_ );
    }
    objectInteractions_.clear();

    // Collision with objects
    for( auto itCollision = objectCollisions_.begin(); itCollision != objectCollisions_.end(); ++itCollision )
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
    auto method = boost::bind( &DEC_KS_ObjectInteraction::CleanKnowledgeObjectCollision, this, _1 );
    pBlackBoard_->GetKnowledgeObjectCollisionContainer().ApplyOnKnowledgesObjectCollision( method );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::CleanDeletedAgentKnowledges()
{
    // NOTHING
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
// Name: DEC_KS_ObjectInteraction::NotifyDisasterCollision
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::NotifyDisasterCollision( MIL_Object_ABC& object, const MT_Vector2D& vPosition, const MT_Vector2D& vDirection )
{
    // Detect disaster with a sensor
    NotifyCollision( object, vPosition, vDirection );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::NotifyObjectCollision
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::NotifyObjectCollision( MIL_Object_ABC& object, const MT_Vector2D& vPosition, const MT_Vector2D& vDirection )
{
    // Detect object but no disaster without sensors
    if( object.CanBeSeen() )
        NotifyCollision( object, vPosition, vDirection );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::NotifyCollision
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::NotifyCollision( MIL_Object_ABC& object, const MT_Vector2D& vPosition, const MT_Vector2D& vDirection )
{
    static const double epsilon = 1e-8;
    static const double lineFactor = 2 * TER_World::GetWorld().GetWeldValue();
    const TER_Localisation& objectLocation = object.GetLocalisation();
    MT_Vector2D vThrough( vPosition + lineFactor * vDirection );
    MT_Line orientedLine( vPosition, vThrough );
    TER_DistanceLess collisionCmp( vPosition );
    T_PointSet collisions( collisionCmp );
    if( objectLocation.Intersect2D( orientedLine, collisions, epsilon ) && !collisions.empty() )
    {
        if( !objectCollisions_.empty() && objectCollisions_.back().first == &object )
            return;
        MT_Vector2D collisionPosition( *collisions.begin() );
        objectCollisions_.push_back( std::make_pair( &object, collisionPosition ) );
    }
}
