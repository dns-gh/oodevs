// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_OrdersFunctions.cpp $
// $Author: Nld $
// $Modtime: 2/12/04 10:15 $
// $Revision: 4 $
// $Workfile: DEC_OrdersFunctions.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_OrdersFunctions.h"

#include "MIL_AgentServer.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Entities/Orders/Pion/MIL_PionMission_ABC.h"
#include "Entities/Orders/Automate/MIL_AutomateMRT.h"
#include "Decision/DEC_Tools.h"

//=============================================================================
// DIA MRT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_CreatePionMission
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_CreatePionMission( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );

    const MIL_PionMissionType* pMissionType = MIL_PionMissionType::FindPionMissionType( call.GetParameter( 1 ).ToId() );
    assert( pMissionType );

    MIL_PionMission_ABC* pPionMission = callerAutomate.GetOrderManager().MRT_CreatePionMission( pPion->GetPion(), *pMissionType );
    call.GetResult().SetValue( *pPionMission );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_Validate
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_Validate( DIA_Call_ABC& /*call*/, MIL_Automate& callerAutomate )
{
    callerAutomate.GetOrderManager().MRT_Validate();
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_AffectFuseaux
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_AffectFuseaux( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    T_ObjectVector pions = call.GetParameter( 0 ).ToSelection();

    //$$$ NAZE
    assert( callerAutomate.IsEmbraye() );
    MIL_AutomateOrderManager& automateOrderManager = callerAutomate.GetOrderManager();
    MIL_AutomateMRT*          pMRT                 = automateOrderManager.GetMRT();
    assert( pMRT );
    assert( !pMRT->IsActivated() );

    // Découpage
    MIL_Fuseau::T_FuseauPtrList subFuseaux;
    if( !callerAutomate.GetFuseau().SplitIntoSubFuseaux( pions.size(), subFuseaux ) )
        return;

    // Affectation des fuseaux
    for( CIT_ObjectVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        MIL_AgentPion& pion = static_cast< DEC_RolePion_Decision* >( *itPion )->GetPion();
        for( MIL_Fuseau::IT_FuseauPtrList itFuseau = subFuseaux.begin(); itFuseau != subFuseaux.end(); ++itFuseau )
        {
            MIL_Fuseau& fuseau = **itFuseau;
            if( fuseau.IsInside( pion.GetRole< PHY_RolePion_Location >().GetPosition() ) )
            {
                pMRT->SetFuseauForPion( pion, fuseau );
                subFuseaux.erase( itFuseau );
                break;
            }
        }

        // Pas de fuseau trouvé => prend le 1er
        if( itFuseau == subFuseaux.end() )
        {
            MIL_Fuseau& fuseau = **subFuseaux.begin();
            pMRT->SetFuseauForPion( pion, fuseau );
            subFuseaux.erase( subFuseaux.begin() );
        }
    }
    assert( subFuseaux.empty() );
}

//=============================================================================
// Conduite (CDT)
//=============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_IsPionWaitingForOrderConduite
// Created: NLD 2003-07-15
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_IsPionWaitingForOrderConduite( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    const MIL_Automate::T_PionVector& pions = callerAutomate.GetPions();
    for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        if( (**itPion).GetOrderManager().IsWaitingForOrderConduite() )
        {
            call.GetResult().SetValue( true );
            return;
        }
    }
    call.GetResult().SetValue( false );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GetPionsWaitingForOrderConduite
// Created: NLD 2003-07-15
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GetPionsWaitingForOrderConduite( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    T_ObjectVector pionVector;
    const MIL_Automate::T_PionVector& pions = callerAutomate.GetPions();
    for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        MIL_AgentPion& pion = **itPion;
        if( pion.GetOrderManager().IsWaitingForOrderConduite() )
            pionVector.push_back( &pion.GetDecision() );
    }
    call.GetResult().SetValue( pionVector );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GetListOrderConduiteWaitedByPion
// Created: NLD 2003-07-15
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GetListOrderConduiteWaitedByPion( DIA_Call_ABC& call, MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DIA_Parameters& params = call.GetParameters();

    DEC_RolePion_Decision* pPion      = params[0].ToUserObject( pPion );
    DIA_Variable_ABC*      pOrderList = &params[1];
    DIA_Variable_ABC*      pHint      = &params[2];

    assert( pPion );
    pPion->GetPion().GetOrderManager().SendOrderConduiteRequestsToDIA( *pOrderList, *pHint );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_SendOrderConduiteToPion
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_SendOrderConduiteToPion( DIA_Call_ABC& call, MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );
    pPion->GetPion().GetOrderManager().OnReceiveOrderConduite( call.GetParameters() );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_CreatePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_CreatePionMission( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    // Pion
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );

    // Instanciate and check the new mission
    const MIL_PionMissionType* pMissionType = MIL_PionMissionType::FindPionMissionType( call.GetParameter( 1 ).ToId() );
    assert( pMissionType );

    MIL_PionMission_ABC* pPionMission = callerAutomate.GetOrderManager().CDT_CreatePionMission( pPion->GetPion(), *pMissionType );
    call.GetResult().SetValue( *pPionMission );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GivePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GivePionMission( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypeMissionPion( call.GetParameter( 0 ) ) );

    MIL_PionMission_ABC* pPionMission = call.GetParameter( 0 ).ToUserObject( pPionMission );
    assert( pPionMission );

    callerAutomate.GetOrderManager().CDT_GivePionMission( *pPionMission );
}