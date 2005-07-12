//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ControlZone.cpp $
// $Author: Age $
// $Modtime: 31/01/05 12:50 $
// $Revision: 4 $
// $Workfile: MIL_ControlZone.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_ControlZone.h"

#include "MIL_VirtualObjectType_ControlZone.h"

#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Humans/PHY_RolePion_Humans.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h"
#include "TER/TER_Localisation.h"

MT_Random MIL_ControlZone::randomGenerator_;

// -----------------------------------------------------------------------------
// Name: MIL_ControlZone constructor
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_ControlZone::MIL_ControlZone( MIL_Army& army, const TER_Localisation& localisation, MT_Float rRadius )
    : MIL_VirtualObject_ABC( MIL_VirtualObjectType::controlZone_ )
    , rCircleSquareRadius_ ( rRadius * rRadius )
    , bMustUseCircle_      ( localisation.GetArea() > rCircleSquareRadius_ * MT_PI )
{
    Initialize( army, localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_ControlZone destructor
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_ControlZone::~MIL_ControlZone()
{

}

// -----------------------------------------------------------------------------
// Name: MIL_ControlZone::GetTargets
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void MIL_ControlZone::GetTargets( const MIL_AgentPion& firer, T_TargetVector& targets ) const
{
    const T_AgentSet&  agentsInside   = GetAgentsInside();
    const MT_Vector2D& vFirerPosition = firer.GetRole< PHY_RolePion_Location >().GetPosition();
    const MT_Float     rPHCoeff       = MT_IsZero( GetLocalisation().GetArea() ) ? 0. : firer.GetRole< PHY_RolePion_Humans >().GetNbrUsableHumans() / GetLocalisation().GetArea();

    targets.clear();
    for( CIT_AgentSet itAgent = agentsInside.begin(); itAgent != agentsInside.end(); ++itAgent )
    {
        MIL_Agent_ABC& agent = **itAgent;

        if ( GetArmy().IsAFriend( agent.GetArmy() ) == eTristate_True )
            continue;
        if ( bMustUseCircle_ && agent.GetRole< PHY_RoleInterface_Location >().GetPosition().SquareDistance( vFirerPosition ) > rCircleSquareRadius_ )
            continue;

        PHY_RoleInterface_Composantes::T_ComposanteVector compTargets;
        agent.GetRole< PHY_RoleInterface_Composantes >().GetComposantesAbleToBeFired( compTargets );
        for( PHY_RoleInterface_Composantes::CIT_ComposanteVector itCompTarget = compTargets.begin(); itCompTarget != compTargets.end(); ++itCompTarget )
        {
            PHY_Composante_ABC& compTarget = **itCompTarget;

            if ( randomGenerator_.rand_oi( 0., 1. ) <= rPHCoeff * MIL_VirtualObjectType::controlZone_.GetUnitDensityFirePercentage( compTarget.GetType().GetVolume() ) )
                targets.push_back( std::make_pair( &agent, &compTarget ) );
        }
    }
}

    