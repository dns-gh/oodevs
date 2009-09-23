// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoLocalisation.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 17:18 $
// $Revision: 8 $
// $Workfile: PHY_PerceptionRecoLocalisation.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionRecoLocalisation.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "simulation_terrain/TER_World.h"

#include "simulation_kernel/DetectionComputer_ABC.h"
#include "simulation_kernel/DetectionComputerFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisationReco constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoLocalisationReco::PHY_PerceptionRecoLocalisationReco( const TER_Localisation& localisation, MT_Float rRadius )
    : localisation_    ( localisation )
    , rRadius_         ( rRadius )
    , bShouldUseRadius_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisationReco constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoLocalisationReco::PHY_PerceptionRecoLocalisationReco( const TER_Localisation& localisation, bool bUseDefaultRadius )
    : localisation_    ( localisation )
    , rRadius_         ( -1. )
    , bShouldUseRadius_( bUseDefaultRadius )
{
    // NOTHING
}
 
// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisationReco::IsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoLocalisationReco::IsInside( const PHY_RoleInterface_Perceiver& perceiver, const MT_Vector2D& vPoint ) const
{
    if ( bShouldUseRadius_ )
    {
        const MT_Float rRadius = rRadius_ < 0. ? perceiver.GetMaxAgentPerceptionDistance() : rRadius_;
        if ( perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition().SquareDistance( vPoint ) > rRadius * rRadius )
            return false;
    }
    return localisation_.IsInside( vPoint );    
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisationReco::GetAgentsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoLocalisationReco::GetAgentsInside( const PHY_RoleInterface_Perceiver& perceiver, TER_Agent_ABC::T_AgentPtrVector& result ) const
{
    result.clear();
    if( bShouldUseRadius_ )
    {
        const MT_Float rRadius = rRadius_ < 0. ? perceiver.GetMaxAgentPerceptionDistance() : rRadius_;
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), rRadius, result );
        
        for ( TER_Agent_ABC::IT_AgentPtrVector it = result.begin(); it != result.end(); )
            if ( localisation_.IsInside( (*it)->GetPosition() ) )
                ++it;
            else
                it = result.erase( it ); 
    }
    else
        TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( localisation_, result );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoLocalisation::PHY_PerceptionRecoLocalisation( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_PerceptionWithLocation< PHY_PerceptionRecoLocalisationReco >( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::~PHY_PerceptionRecoLocalisation
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoLocalisation::~PHY_PerceptionRecoLocalisation()
{
    for( IT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
        delete *it;
    recos_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoLocalisation::AddLocalisationWithRadius( const TER_Localisation& localisation, MT_Float rRadius )
{
    PHY_PerceptionRecoLocalisationReco* pNewReco = new PHY_PerceptionRecoLocalisationReco( localisation, rRadius );
    assert( pNewReco );
    return Add( pNewReco );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoLocalisation::AddLocalisationWithDefaultRadius( const TER_Localisation& localisation )
{
    PHY_PerceptionRecoLocalisationReco* pNewReco = new PHY_PerceptionRecoLocalisationReco( localisation, true );
    assert( pNewReco );
    return Add( pNewReco );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::AddLocalisationWithoutRadius
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoLocalisation::AddLocalisationWithoutRadius( const TER_Localisation& localisation )
{
    PHY_PerceptionRecoLocalisationReco* pNewReco = new PHY_PerceptionRecoLocalisationReco( localisation, false );
    assert( pNewReco );
    return Add( pNewReco ); 
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::RemoveLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoLocalisation::RemoveLocalisation( int id )
{
    Remove( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoLocalisation::Compute( const MT_Vector2D& vPoint ) const
{
    for ( CIT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
    {
        if ( (*it)->IsInside( perceiver_, vPoint ) )
            return PHY_PerceptionLevel::recognized_;
    }
    
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoLocalisation::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/, const detection::DetectionComputerFactory_ABC& detectionComputerFactory )
{
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    
    for ( CIT_RecoVector itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        (*itReco)->GetAgentsInside( perceiver_, perceivableAgents );
        
        for ( TER_Agent_ABC::CIT_AgentPtrVector it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >(**it).GetAgent();

            detection::DetectionComputer_ABC& detectionComputer = detectionComputerFactory.Create( target );
            perceiver_.GetPion().Execute( detectionComputer );
            target.Execute( detectionComputer );

            if( detectionComputer.CanBeSeen() )
                perceiver_.NotifyPerception( target, PHY_PerceptionLevel::recognized_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoLocalisation::Compute( const MIL_Agent_ABC& agent ) const
{//@TODO move to  private
    return Compute( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoLocalisation::Compute( const DEC_Knowledge_Agent & knowledge ) const
{
    return Compute( knowledge.GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoLocalisation::HasLocalisationToHandle() const
{
    return !recos_.empty();
}
