// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_IndirectFire.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_IndirectFire )

DEC_KS_IndirectFire::DEC_KS_IndirectFire( DEC_KnowledgeBlackBoard_AgentPion& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_( &blackBoard )
{
    // NOTHING
}

DEC_KS_IndirectFire::DEC_KS_IndirectFire()
    : pBlackBoard_( 0 )
{
    // NOTHING
}

DEC_KS_IndirectFire::~DEC_KS_IndirectFire()
{
    // NOTHING
}

void DEC_KS_IndirectFire::Prepare()
{
    // NOTHING
}

void DEC_KS_IndirectFire::Clean()
{
    // NOTHING
}

void DEC_KS_IndirectFire::CleanDeletedAgentKnowledges()
{
    for( auto it = pionsAttacking_.begin(); it != pionsAttacking_.end(); )
    {
        if( ( *it )->IsMarkedForDestruction() )
            it = pionsAttacking_.erase( it );
        else
            ++it;
    }
}

void DEC_KS_IndirectFire::Talk( int /*currentTimeStep*/ )
{
    const MIL_Agent_ABC& agent = pBlackBoard_->GetPion();
    for( auto it = pionsAttacking_.begin(); it != pionsAttacking_.end(); ++it )
    {
        DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerceptionContainer().GetKnowledgeAgentPerception( **it );
        if( !pKnowledge )
            pKnowledge = &pBlackBoard_->GetKnowledgeAgentPerceptionContainer().CreateKnowledgeAgentPerception( agent, **it );
        pKnowledge->Update( PHY_PerceptionLevel::detected_, false );
    }
    pionsAttacking_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_IndirectFire::Clear
// Created: LDC 2014-09-09
// -----------------------------------------------------------------------------
void DEC_KS_IndirectFire::Clear()
{
    pionsAttacking_.clear();
}

void DEC_KS_IndirectFire::NotifyAttackedBy( const MIL_Agent_ABC& attacker )
{
    pionsAttacking_.insert( &attacker );
}
