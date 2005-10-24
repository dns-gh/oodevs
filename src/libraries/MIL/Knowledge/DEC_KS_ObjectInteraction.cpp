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

#include "MIL_pch.h"
#include "DEC_KS_ObjectInteraction.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( DEC_KS_ObjectInteraction, "DEC_KS_ObjectInteraction" )

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectInteraction::DEC_KS_ObjectInteraction( DEC_KnowledgeBlackBoard& blackBoard, const MIL_AgentPion& agentInteracting )
    : DEC_KnowledgeSource_ABC( blackBoard )
    , pAgentInteracting_     ( &agentInteracting )
    , objectInteractions_    ()
    , objectCollisions_      ()
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction constructor
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
DEC_KS_ObjectInteraction::DEC_KS_ObjectInteraction()
    : DEC_KnowledgeSource_ABC()
    , pAgentInteracting_     ( 0 )
    , objectInteractions_    ()
    , objectCollisions_      ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_ObjectInteraction::~DEC_KS_ObjectInteraction()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );    
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::Prepare()
{
    pBlackBoard_->ApplyOnKnowledgesObjectCollision( std::mem_fun_ref( &DEC_Knowledge_ObjectCollision::Prepare ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::UpdateObjectPerceptionKnowledge
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception& DEC_KS_ObjectInteraction::GetKnowledgeObjectPerception( MIL_RealObject_ABC& object ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_ObjectPerception* pKnowledge = pBlackBoard_->GetKnowledgeObjectPerception( object );
    if( !pKnowledge )
        pKnowledge = &pBlackBoard_->CreateKnowledgeObjectPerception( GetAgentInteracting(), object );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::UpdateObjectCollisionKnowledge
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision& DEC_KS_ObjectInteraction::GetKnowledgeObjectCollision( MIL_RealObject_ABC& object ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_ObjectCollision* pKnowledge = pBlackBoard_->GetKnowledgeObjectCollision( object );
    if( !pKnowledge )
        pKnowledge = &pBlackBoard_->CreateKnowledgeObjectCollision( GetAgentInteracting(), object );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::Talk()
{
    // Interaction with objects
    for( CIT_ObjectVector itInteraction = objectInteractions_.begin(); itInteraction != objectInteractions_.end(); ++itInteraction )
    {
        MIL_RealObject_ABC& object = **itInteraction;
        if( object.IsMarkedForDestruction() )
            continue;
        DEC_Knowledge_ObjectPerception& knowledge = GetKnowledgeObjectPerception( object );
        knowledge.Update( PHY_PerceptionLevel::identified_ ); 
    }
    objectInteractions_.clear();
        
    // Collision with objects
    for( CIT_ObjectCollisionVector itCollision = objectCollisions_.begin(); itCollision != objectCollisions_.end(); ++itCollision )
    {
		MIL_RealObject_ABC& object = *itCollision->first;
        if( object.IsMarkedForDestruction() )
            continue;
        DEC_Knowledge_ObjectCollision& knowledge = GetKnowledgeObjectCollision( object );
        knowledge.Update( itCollision->second ); 
    }
    objectCollisions_.clear();
}

// =============================================================================
// CLEAN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::CleanKnowledgeObjectCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::CleanKnowledgeObjectCollision( DEC_Knowledge_ObjectCollision& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->DestroyKnowledgeObjectCollision( knowledge ); // NB - The knowledge will be deleted
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
    class_mem_fun_void_t< DEC_KS_ObjectInteraction, DEC_Knowledge_ObjectCollision> method( DEC_KS_ObjectInteraction::CleanKnowledgeObjectCollision, *this );        
    pBlackBoard_->ApplyOnKnowledgesObjectCollision( method );    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::load
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::load( boost::archive::binary_iarchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
         >> const_cast< MIL_AgentPion*& >( pAgentInteracting_ )
         >> objectInteractions_
         >> objectCollisions_; 
}
    
// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::save
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void DEC_KS_ObjectInteraction::save( boost::archive::binary_oarchive& file, const uint ) const
{
    file << boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
         << pAgentInteracting_
         << objectInteractions_
         << objectCollisions_;
}
