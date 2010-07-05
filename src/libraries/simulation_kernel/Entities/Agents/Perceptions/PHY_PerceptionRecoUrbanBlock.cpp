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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "simulation_terrain/TER_World.h"
#include "urban/TerrainObject_ABC.h"
#include "simulation_kernel/DetectionComputer_ABC.h"
#include "simulation_kernel/DetectionComputerFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlockReco constructor
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
PHY_PerceptionRecoUrbanBlockReco::PHY_PerceptionRecoUrbanBlockReco( const boost::shared_ptr< DEC_Knowledge_Urban > urbanBlock )
    : urbanBlock_( urbanBlock )
{
    T_PointVector points;
    const geometry::Polygon2f::T_Vertices footprint = urbanBlock_->GetTerrainObjectKnown().GetFootprint()->Vertices();
    for( geometry::Polygon2f::T_Vertices::const_iterator it = footprint.begin(); it != footprint.end(); ++it )
        points.push_back( MT_Vector2D( it->X(), it->Y() ) );
    localisation_.Reset( TER_Localisation::ePolygon, points );
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
    return urbanBlock_->GetProgress() >= MIL_Random::rand_ii( MIL_Random::ePerception );
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
    for( IT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
        delete *it;
    recos_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::AddUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoUrbanBlock::AddUrbanBlock( const boost::shared_ptr< DEC_Knowledge_Urban > urbanBlock )
{
    PHY_PerceptionRecoUrbanBlockReco* pNewReco = new PHY_PerceptionRecoUrbanBlockReco( urbanBlock );
    assert( pNewReco );
    return Add( pNewReco );
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
    for ( CIT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
    {
        if( (*it)->IsInside( perceiver_, vPoint ) )
            return PHY_PerceptionLevel::recognized_;
    }

    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::Execute
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoUrbanBlock::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/, const detection::DetectionComputerFactory_ABC& detectionComputerFactory )
{
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;

    for ( CIT_RecoVector itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        (*itReco)->GetAgentsInside( perceiver_, perceivableAgents );

        for ( TER_Agent_ABC::CIT_AgentPtrVector it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >(**it).GetAgent();

            std::auto_ptr< detection::DetectionComputer_ABC > detectionComputer( detectionComputerFactory.Create( target ) );
            perceiver_.GetPion().Execute( *detectionComputer );
            target.Execute( *detectionComputer );

            if( detectionComputer->CanBeSeen() )
            {
                if( (*itReco)->CanSeeIt() )
                    perceiver_.NotifyPerception( target, PHY_PerceptionLevel::recognized_ );
            }
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
// Name: PHY_PerceptionRecoUrbanBlock::Compute
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoUrbanBlock::Compute( const DEC_Knowledge_Agent & knowledge ) const
{
    return Compute( knowledge.GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoUrbanBlock::HasLocalisationToHandle
// Created: JMGD 2010-02-11
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoUrbanBlock::HasLocalisationToHandle() const
{
    return !recos_.empty();
}
