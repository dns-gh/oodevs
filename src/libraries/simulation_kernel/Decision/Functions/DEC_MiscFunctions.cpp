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

#include "simulation_kernel_pch.h"
#include "DEC_MiscFunctions.h"

#include "MIL_AgentServer.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Representations.h"
#include "simulation_terrain/TER_World.h"

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
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = callerAgent.GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    T_ObjectVector out; out.reserve( reinforcements.size() );
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

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DEC_MiscFunctions::GetCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::GetCategory( DIA_Call_ABC& call, MIL_Entity_ABC& callerAgent )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    const T_ObjectVector& category = role.GetCategory( call.GetParameter( 0 ).ToString() );
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );
    diaObjectList.SetValue( category );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DEC_MiscFunctions::AddToCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::AddToCategory( DIA_Call_ABC& call, MIL_Entity_ABC& callerAgent )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.AddToCategory( call.GetParameter( 0 ).ToString(), call.GetParameter( 1 ).ToObject() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DEC_MiscFunctions::RemoveFromCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::RemoveFromCategory( DIA_Call_ABC& call, MIL_Entity_ABC& callerAgent )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.RemoveFromCategory( call.GetParameter( 0 ).ToString(), call.GetParameter( 1 ).ToObject() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DEC_MiscFunctions::DeleteRepresentation
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::DeleteRepresentation( DIA_Call_ABC& call, MIL_Entity_ABC& callerAgent )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.DeleteRepresentation( call.GetParameter( 0 ).ToObject() );
}

