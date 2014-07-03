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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_kernel/DetectionComputer.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisationReco constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoLocalisationReco::PHY_PerceptionRecoLocalisationReco( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& callerAgent )
    : rCurrentRadius_( 0. )
    , rGrowthSpeed_( rGrowthSpeed )
    , callerAgent_( callerAgent )
    , localisation_( localisation )
    , bShouldUseRadius_( rGrowthSpeed > 0. ? true : false )
{
    const MT_Rect& boundingBox = localisation_.GetBoundingBox();
    rRadius_ = static_cast< float >( boundingBox.GetCenter().Distance( boundingBox.GetPointUpLeft() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisationReco constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoLocalisationReco::PHY_PerceptionRecoLocalisationReco( const TER_Localisation& localisation, bool bUseDefaultRadius, DEC_Decision_ABC& callerAgent )
    : rRadius_( -1. )
    , rCurrentRadius_( 0. )
    , rGrowthSpeed_( -1. )
    , callerAgent_( callerAgent )
    , localisation_( localisation )
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
    if( bShouldUseRadius_ )
    {
        const float rRadius = rRadius_ < 0. ? static_cast< float >( perceiver.GetMaxAgentPerceptionDistance() ) : rRadius_;
        if( perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition().SquareDistance( vPoint ) > rRadius * rRadius )
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
        const float rRadius = rRadius_ < 0. ? static_cast< float >( perceiver.GetMaxAgentPerceptionDistance() ) : rRadius_;
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), rRadius, result );
        for( auto it = result.begin(); it != result.end(); )
            if( localisation_.IsInside( (*it)->GetPosition() ) )
                ++it;
            else
                it = result.erase( it );
    }
    else
        TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( localisation_, result );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisationReco::GetRadius
// Created: MGD 2010-06-28
// -----------------------------------------------------------------------------
float PHY_PerceptionRecoLocalisationReco::GetRadius() const
{
    return rRadius_;
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoLocalisation::AddLocalisationWithGrowthSpeed( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& callerAgent )
{
    for( auto it = pendingLocalisations_.begin(); it != pendingLocalisations_.end(); ++it )
        if( it->callerAgent_.GetID() == callerAgent.GetID() && it->rGrowthSpeed_ == rGrowthSpeed && it->localisation_ == localisation )
            return Add( pendingLocalisations_.release( it ).release() );
    return Add( new PHY_PerceptionRecoLocalisationReco( localisation, rGrowthSpeed, callerAgent ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoLocalisation::AddLocalisationWithDefaultGrowthSpeed( const TER_Localisation& localisation, DEC_Decision_ABC& callerAgent )
{
    for( auto it = pendingLocalisations_.begin(); it != pendingLocalisations_.end(); ++it )
        if( it->callerAgent_.GetID() == callerAgent.GetID() && it->localisation_ == localisation && it->rGrowthSpeed_ == -1 )
            return Add( pendingLocalisations_.release( it ).release() );
    return Add( new PHY_PerceptionRecoLocalisationReco( localisation, true, callerAgent ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::RemoveLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoLocalisation::RemoveLocalisation( int id )
{
    auto pending = Remove( id );
    if( pending )
        pendingLocalisations_.push_back( pending.release() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoLocalisation::Compute( const MT_Vector2D& vPoint ) const
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        if( it->IsInside( perceiver_, vPoint ) )
            return PHY_PerceptionLevel::recognized_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoLocalisation::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    for( auto itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableAgents.clear();
        itReco->GetAgentsInside( perceiver_, perceivableAgents );
        for( auto it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >(**it).GetAgent();
            if( !DEC_Knowledge_Agent::detectDestroyedUnits_ && target.IsDead() )
                continue;
            detection::DetectionComputer detectionComputer( target );
            perceiver_.GetPion().Execute( detectionComputer );
            target.Execute( detectionComputer );
            if( detectionComputer.CanBeSeen() )
                perceiver_.NotifyPerception( target, GetMaxHostilePerceptionLevel( perceiver_.GetPion(), target, PHY_PerceptionLevel::recognized_ ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoLocalisation::Compute( const MIL_Agent_ABC& agent ) const
{
    return Compute( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoLocalisation::HasLocalisationToHandle() const
{
    return !recos_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::Update
// Created: MGD 2010-06-28
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoLocalisation::Update()
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
    {
        it->rCurrentRadius_ = std::min( it->rRadius_, it->rCurrentRadius_ + it->rGrowthSpeed_ );
        // Agrandissement de la zone de reconnaissance
        if( it->rCurrentRadius_ == it->rRadius_ )
            it->callerAgent_.CallbackPerception( it->Id() );
    }
    pendingLocalisations_.erase_if( []( PHY_PerceptionRecoLocalisationReco& reco ) -> bool
        {
            reco.rCurrentRadius_ = std::max< float >( 0, reco.rCurrentRadius_ - reco.rGrowthSpeed_ );
            return reco.rCurrentRadius_ == 0;
        } );
}
