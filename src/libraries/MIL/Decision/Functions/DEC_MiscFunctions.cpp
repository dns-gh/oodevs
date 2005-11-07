// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_MiscFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 16:18 $
// $Revision: 7 $
// $Workfile: DEC_MiscFunctions.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_MiscFunctions.h"

#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Orders/Lima/MIL_Lima.h"
#include "Decision/DEC_Tools.h"

#include "TER/TER_World.h"

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::SetCurrentSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::SetCurrentSpeedModificator( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    MT_Float rFactor = call.GetParameter( 0 ).ToFloat();
    callerAgent.GetRole< PHY_RoleAction_Moving >().SetSpeedModificator( rFactor );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::SetMaxSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::SetMaxSpeedModificator( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    MT_Float rFactor = call.GetParameter( 0 ).ToFloat();
    callerAgent.GetRole< PHY_RoleAction_Moving >().SetMaxSpeedModificator( rFactor );
}

// =============================================================================
// REINFORCEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetReinforcements
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::GetReinforcements( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    T_ObjectVector out;
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = callerAgent.GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        out.push_back( &(**it).GetRole< DEC_RolePion_Decision >() );

    call.GetResult().SetValue( out );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::Reinforce
// Created: NLD 2003-03-12
//-----------------------------------------------------------------------------
void DEC_MiscFunctions::Reinforce( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pTarget = call.GetParameter( 0 ).ToUserObject( pTarget );
    if( !pTarget )
    {
        assert( false );
        call.GetResult().SetValue( false );
        return;
    }
    bool bResult = callerAgent.GetRole< PHY_RolePion_Reinforcement >().Reinforce( pTarget->GetPion() );
    call.GetResult().SetValue( bResult );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::CancelReinforcement
// Created: NLD 2003-03-12
//-----------------------------------------------------------------------------
void DEC_MiscFunctions::CancelReinforcement( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Reinforcement >().CancelReinforcement();
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::IsReinforcing
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void DEC_MiscFunctions::IsReinforcing( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_Reinforcement >().IsReinforcing() );
}

