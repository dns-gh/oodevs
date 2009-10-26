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
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_AutomateMission.h"
#include "Entities/Orders/MIL_PionMission.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Representations.h"
#include "simulation_terrain/TER_World.h"

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::SetCurrentSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::SetCurrentSpeedModificator( MIL_AgentPion& callerAgent, MT_Float rFactor )
{
    callerAgent.GetRole< moving::PHY_RoleAction_Moving >().SetSpeedModificator( rFactor );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::SetMaxSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::SetMaxSpeedModificator( MIL_AgentPion& callerAgent, MT_Float rFactor )
{
    callerAgent.GetRole< moving::PHY_RoleAction_Moving >().SetMaxSpeedModificator( rFactor );
}

// =============================================================================
// REINFORCEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetReinforcements
// Created: NLD 2004-10-01
// Modified: RPD 2009-08-03
// -----------------------------------------------------------------------------
std::vector<DEC_Decision_ABC*> DEC_MiscFunctions::GetReinforcements( const MIL_AgentPion& callerAgent )
{
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = callerAgent.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    std::vector<DEC_Decision_ABC*> result;
    result.reserve( reinforcements.size() );
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        result.push_back( &(**it).GetRole< DEC_RolePion_Decision >() );
    return result;
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::Reinforce
// Created: NLD 2003-03-12
// Modified: RPD 2009-08-03
//-----------------------------------------------------------------------------
bool DEC_MiscFunctions::Reinforce( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pTarget )
{
    assert ( pTarget );
    return callerAgent.GetRole< PHY_RoleInterface_Reinforcement >().Reinforce( pTarget->GetPion() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::CancelReinforcement
// Created: NLD 2003-03-12
// Modified: RPD 2009-08-03
//-----------------------------------------------------------------------------
void DEC_MiscFunctions::CancelReinforcement( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Reinforcement >().CancelReinforcement();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
std::vector<MIL_FragOrder*> DEC_MiscFunctions::GetOrdersCategory( MIL_Entity_ABC& callerAgent )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    return role.GetOrdersCategory();
}
// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
std::vector<DEC_PathPoint*> DEC_MiscFunctions::GetPointsCategory( MIL_Entity_ABC& callerAgent )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    return role.GetPointsCategory();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::RemoveFromOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::RemoveFromOrdersCategory( MIL_Entity_ABC& callerAgent, MIL_FragOrder* pOrder )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.RemoveFromOrdersCategory( pOrder );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::RemoveFromPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::RemoveFromPointsCategory( MIL_Entity_ABC& callerAgent, DEC_PathPoint* pPoint )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.RemoveFromPointsCategory( pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DeleteOrderRepresentation
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::DeleteOrderRepresentation( MIL_Entity_ABC& callerAgent, MIL_FragOrder* pOrder )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.DeleteOrderRepresentation( pOrder );
}

namespace DEC_DecisionImpl
{
    void RegisterMissionParameters( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, MIL_Mission_ABC& mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::FillMissionParameters
// Created: LDC 2009-05-04
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::FillMissionParameters( directia::ScriptRef& initTaskFunction, const directia::ScriptRef& refMission, MIL_Mission_ABC* pMission )
{
    if( pMission )
        DEC_DecisionImpl::RegisterMissionParameters( initTaskFunction, refMission, *pMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_MiscFunctions::GetName( DEC_Decision_ABC* pEntity )
{
    return pEntity->GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetAutomate
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_MiscFunctions::GetAutomate( DEC_Decision_ABC* pAgent )
{
    return pAgent->GetDecAutomate();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetDirectionEnnemi
// Created: LDC 2009-04-20
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_MiscFunctions::GetDirectionEnnemi( MIL_Mission_ABC* pMission )
{
    boost::shared_ptr< MT_Vector2D > result( new MT_Vector2D( pMission->GetDirDanger() ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::CopyDirectionDanger
// Created: LDC 2009-04-21
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::CopyDirectionDanger( MT_Vector2D* pPosSource, MIL_Mission_ABC* pMission )
{
    assert( pPosSource );
    assert( !pPosSource->IsZero() );
    assert( MT_IsZero( pPosSource->SquareMagnitude() - 1. ) );    

    pMission->AffectDirection( *pPosSource );
}
