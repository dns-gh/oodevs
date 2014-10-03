// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionRecoUrbanBlock.h"
#include "DetectionComputer.h"
#include "MIL_Random.h"
#include "PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Entities/MIL_Army_ABC.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlockReco constructor
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
PHY_PerceptionRecoUrbanBlockReco::PHY_PerceptionRecoUrbanBlockReco( const MIL_UrbanObject_ABC* pUrbanBlock, const boost::shared_ptr< DEC_Knowledge_Urban > pKnowledgeUrbanBlock )
    : pUrbanBlock_         ( pUrbanBlock )
    , pKnowledgeUrbanBlock_( pKnowledgeUrbanBlock )
{
    if( pUrbanBlock == 0 )
        throw MASA_EXCEPTION( "urban block invalid" );
    localisation_.Reset( pUrbanBlock->GetLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlockReco::IsInside
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoUrbanBlockReco::IsInside( const PHY_RoleInterface_Perceiver& /*perceiver*/, const MT_Vector2D& vPoint ) const
{
    return localisation_.IsInside( vPoint );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlockReco::GetAgentsInside
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoUrbanBlockReco::GetAgentsInside( const PHY_RoleInterface_Perceiver& /*perceiver*/, TER_Agent_ABC::T_AgentPtrVector& result ) const
{
    result.clear();
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( localisation_, result );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlockReco::CanSeeIt
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoUrbanBlockReco::CanSeeIt() const
{
    if( pKnowledgeUrbanBlock_ )
        return pKnowledgeUrbanBlock_->GetCurrentRecceProgress() >= ( 1. - MIL_Random::rand_ii( MIL_Random::ePerception ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlockReco::GeturbanBlock
// Created: LDC 2013-10-07
// -----------------------------------------------------------------------------
const MIL_UrbanObject_ABC* PHY_PerceptionRecoUrbanBlockReco::GeturbanBlock() const
{
    return pUrbanBlock_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock constructor
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
PHY_PerceptionRecoUrbanBlock::PHY_PerceptionRecoUrbanBlock( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_PerceptionWithLocation< PHY_PerceptionRecoUrbanBlockReco >( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::~PHY_PerceptionRecoUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
PHY_PerceptionRecoUrbanBlock::~PHY_PerceptionRecoUrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::AddUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoUrbanBlock::AddUrbanBlock( const MIL_UrbanObject_ABC* pUrbanBlock )
{
    return Add( new PHY_PerceptionRecoUrbanBlockReco(
        pUrbanBlock,
        perceiver_.GetPion().GetArmy().GetKnowledge().GetKnowledgeUrbanContainer().GetKnowledgeUrban( *pUrbanBlock ) ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::RemoveUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoUrbanBlock::RemoveUrbanBlock( int id )
{
    Remove( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::Compute
// Created: JVT 2004-10-21
// Modified: MGD 2010-02-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoUrbanBlock::Compute( const MT_Vector2D& vPoint ) const
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        if( it->IsInside( perceiver_, vPoint ) )
            return PHY_PerceptionLevel::recognized_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::Execute
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoUrbanBlock::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    for( auto itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableAgents.clear();
        itReco->GetAgentsInside( perceiver_, perceivableAgents );
        for( auto it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
            if( !DEC_Knowledge_Agent::detectDestroyedUnits_ && target.IsDead() )
                continue;
            detection::DetectionComputer detectionComputer( target );
            perceiver_.GetPion().Execute( detectionComputer );
            target.Execute( detectionComputer );
            if( detectionComputer.CanBeSeen() && itReco->CanSeeIt() )
                perceiver_.NotifyPerception( target, GetMaxHostilePerceptionLevel( perceiver_.GetPion(), target, PHY_PerceptionLevel::recognized_ ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::Compute
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoUrbanBlock::Compute( const MIL_Agent_ABC& agent ) const
{
    return Compute( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::HasLocalisationToHandle
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoUrbanBlock::HasLocalisationToHandle() const
{
    return !recos_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::IsReconnoitering
// Created: LDC 2013-10-07
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoUrbanBlock::IsReconnoitering( const MIL_UrbanObject_ABC* urbanBlock ) const
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        if ( it->GeturbanBlock() == urbanBlock )
            return true;
    return false;
}
