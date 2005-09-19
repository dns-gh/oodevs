// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AgentFunctions.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 15:50 $
// $Revision: 37 $
// $Workfile: DEC_AgentFunctions.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_AgentFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Decision/DEC_Tools.h"
#include "Tools/MIL_Tools.h"
#include "DEC_AutomateFunctions.h"
#include "DEC_GeometryFunctions.h"
#include "DEC_FunctionsTools.h"

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsNeutralized
// Created: JVT 03-10-01
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::IsNeutralized( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.IsNeutralized() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsWearingNbcProtectionSuit
// Created: NLD 2004-05-11
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsWearingNbcProtectionSuit( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_NBC >().IsWearingNbcProtectionSuit() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::WearNbcProtectionSuit
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::WearNbcProtectionSuit( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_NBC >().WearNbcProtectionSuit();   
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RemoveNbcProtectionSuit
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::RemoveNbcProtectionSuit( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_NBC >().RemoveNbcProtectionSuit();    
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SelfDecontaminate
// Created: NLD 2005-03-25
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SelfDecontaminate( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_NBC >().Decontaminate();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ActivateBlackout
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ActivateBlackout( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Communications >().ActivateBlackout();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DeactivateBlackout
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DeactivateBlackout( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Communications >().DeactivateBlackout();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsContaminated
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsContaminated( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_NBC >().IsContaminated() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsTransported
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsTransported( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_Transported >().IsTransported() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsMoving
// Created: NLD 2004-04-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsMoving( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{   
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_Location >().GetCurrentSpeed() != 0. );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SetFlyingHeight
// Created: JVT 2004-11-02
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SetFlyingHeight( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    const MT_Float rHeight = call.GetParameter( 0 ).ToFloat();
    
    assert( rHeight >= 0. && "T'as deja essaye de voler à cette hauteur ?");

    callerAgent.GetRole< PHY_RoleAction_InterfaceFlying >().SetFlyingHeight( rHeight );
}


// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsFlying
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsFlying( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SetElongation
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SetElongationFactor( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    const MT_Float rElongationFactor = call.GetParameter( 0 ).ToFloat();
    
    assert( rElongationFactor > 0. );
    callerAgent.GetRole< PHY_RolePion_Posture >().SetElongationFactor( rElongationFactor );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsSurrendered
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsSurrendered( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_Surrender >().IsSurrendered() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsPrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsPrisoner( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_Surrender >().IsPrisoner() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsRefugee( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetType().IsRefugee() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsMilita
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsMilita( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetType().IsMilitia() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsPC
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsPC( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.IsPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsAutomateEmbraye
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsAutomateEmbraye( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetAutomate().IsEmbraye() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetEtatOps
// Created: NLD 2004-04-16
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetEtatOps( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (float)callerAgent.GetEtatOps() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPosition
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetPosition( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (void*)&callerAgent.GetRole< PHY_RolePion_Location >().GetPosition(), &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetDirection
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetDirection( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (void*)&callerAgent.GetRole< PHY_RolePion_Location >().GetDirection(), &DEC_Tools::GetTypeDirection() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanConstructObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::CanConstructObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );
    if( pObjectType )
        call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( *pObjectType ) );
    else
        call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanBypassObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::CanBypassObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );
    if( pObjectType )
        call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Objects >().CanBypassWithReinforcement( *pObjectType ) );
    else
        call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanDestroyObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::CanDestroyObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );
    if( pObjectType )
        call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Objects >().CanDestroyWithReinforcement( *pObjectType ) );
    else
        call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanMineObject
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::CanMineObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );
        if( pObjectType )
        call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Objects >().CanMineWithReinforcement( *pObjectType ) );
    else
        call.GetResult().SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanActivateObject
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::CanActivateObject( DIA_Call_ABC& call, const MIL_AgentPion& /*callerAgent*/ )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );
    call.GetResult().SetValue( true );
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EnableDiscreteMode
// Created: JVT 04-05-17
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::EnableDiscreteMode( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Posture >().EnableDiscreteMode();
}


//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisableDiscreteMode
// Created: JVT 04-05-17
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::DisableDiscreteMode( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Posture >().DisableDiscreteMode();
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::Trace
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::Trace( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    std::string msg( call.GetParameter( 0 ).ToString() );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)callerAgent.GetID();
    dinMsg << msg;

    msgMgr.SendMsgUnitTrace( dinMsg );
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::Debug
// Created: NLD 2002-12-17
// Last modified: JVT 03-03-03
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::Debug( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    if( !MIL_AgentServer::GetWorkspace().GetConfig().UseDecDebug() )
        return;

    std::string msg( call.GetParameter( 0 ).ToString() );
    MT_LOG_INFO_MSG( MT_FormatString( "Agent %d says : [%s]", callerAgent.GetID(), msg.c_str() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DebugDrawPoints( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );

    T_PointVector* pPoints = call.GetParameter( 0 ).ToUserPtr( pPoints );
    assert( pPoints );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)callerAgent.GetID();
    dinMsg << (uint32)pPoints->size();
    for( CIT_PointVector itPoint = pPoints->begin(); itPoint != pPoints->end(); ++itPoint )
        dinMsg << *itPoint;
    msgMgr.SendMsgDebugDrawPoints( dinMsg );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DebugDrawPoint
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DebugDrawPoint( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    const MT_Vector2D* pPoint = call.GetParameter( 0 ).ToUserPtr( pPoint );
    assert( pPoint );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)callerAgent.GetID();
    dinMsg << (uint32)1;
    dinMsg << *pPoint;
    msgMgr.SendMsgDebugDrawPoints( dinMsg );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SetPosture
// Created: NLD 2003-10-27
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SetPosture( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    // Tests
    PHY_RolePion_Posture& role = callerAgent.GetRole< PHY_RolePion_Posture >();

    int nPostureID = call.GetParameter( 0 ).ToId();
    switch( nPostureID )
    {
        case 0: role.SetPostureMovement         (); break;
        case 1: role.SetPosturePostePrepareGenie(); break;
        default:
        {
            role.UnsetPostureMovement();
            role.UnsetPosturePostePrepareGenie();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsLoaded
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IsLoaded( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RoleAction_Loading >().IsLoaded() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetLoadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetLoadingTime( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMinutes( callerAgent.GetRole< PHY_RoleAction_Loading >().GetLoadingTime() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetUnloadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetUnloadingTime( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMinutes( callerAgent.GetRole< PHY_RoleAction_Loading >().GetUnloadingTime() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanTransportersRecoveringTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetHumanTransportersRecoveringTime( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    if ( call.GetParameters().GetParameters().size() < 1 )
        call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMinutes( callerAgent.GetRole< PHY_RolePion_Transported >().ComputeHumanTransportersRecoveringTime() ) );
    else
    {
        assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
        MT_Vector2D* pPos;
        pPos = call.GetParameter( 0 ).ToUserPtr( pPos );
        assert( pPos );
        call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMinutes( callerAgent.GetRole< PHY_RolePion_Transported >().ComputeHumanTransportersRecoveringTime( *pPos ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisableHumanTransportersNow
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DisableHumanTransportersNow( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Transported >().DisableHumanTransporters( callerAgent.GetRole< PHY_RolePion_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EnableHumanTransportersNow
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::EnableHumanTransportersNow( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Transported >().RecoverHumanTransporters();
}


// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AreHumanTransportersReady
// Created: JVT 2005-01-31
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::AreHumanTransportersReady( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_Transported >().HasHumanTransportersReady() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyForceRatioStateChanged( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    callerAgent.GetDecision().NotifyForceRatioStateChanged( (E_ForceRatioState)call.GetParameter( 0 ).ToId() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    callerAgent.GetDecision().NotifyRulesOfEngagementStateChanged( (E_RulesOfEngagementState )call.GetParameter( 0 ).ToId() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyOperationalStateChanged( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    callerAgent.GetDecision().NotifyOperationalStateChanged( (E_OperationalState)call.GetParameter( 0 ).ToId() );
}

// -----------------------------------------------------------------------------
// Name: static void DEC_AgentFunctions::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyCloseCombatStateChanged( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    callerAgent.GetDecision().NotifyCloseCombatStateChanged( (E_CloseCombatState)call.GetParameter( 0 ).ToId() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanFactorTiredness
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetHumanFactorTiredness( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (int)callerAgent.GetRole< PHY_RolePion_HumanFactors >().GetTiredness().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanFactorExperience
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetHumanFactorExperience( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (int)callerAgent.GetRole< PHY_RolePion_HumanFactors >().GetExperience().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanFactorMorale
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetHumanFactorMorale( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (int)callerAgent.GetRole< PHY_RolePion_HumanFactors >().GetMorale().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanFactorWeight
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetHumanFactorWeight( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const PHY_RolePion_HumanFactors& humanFactors = callerAgent.GetRole< PHY_RolePion_HumanFactors >();
    const MT_Float rWeight = humanFactors.GetMorale().GetWeight()
                           * humanFactors.GetTiredness().GetWeight()
                           * humanFactors.GetExperience().GetWeight();
    call.GetResult().SetValue( (float)rWeight );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RelievePion
// Created: NLD 2003-09-30
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::RelievePion( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pAgentToRelieve = call.GetParameter( 0 ).ToUserObject( pAgentToRelieve );
    assert( pAgentToRelieve );

    if( !callerAgent.GetOrderManager().RelievePion( pAgentToRelieve->GetPion() ) )
    {
        call.GetResult().SetValue( false );
        return;
    }

    pAgentToRelieve->GetPion().GetOrderManager().OnReceiveRCMissionFinished(); //$$$ TMP - renommer la méthode
    call.GetResult().SetValue( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanRelievePion
// Created: NLD 2004-12-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::CanRelievePion( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pAgentToRelieve = call.GetParameter( 0 ).ToUserObject( pAgentToRelieve );
    assert( pAgentToRelieve );

    call.GetResult().SetValue( callerAgent.GetOrderManager().CanRelievePion( pAgentToRelieve->GetPion() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetPionsWithPC( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_AutomateFunctions::GetPionsWithPC( call, callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithoutPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetPionsWithoutPC( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_AutomateFunctions::GetPionsWithoutPC( call, callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetPionPC( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_AutomateFunctions::GetPionPC( call, callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionPCOfAutomate
// Created: JVT 2005-01-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetPionPCOfAutomate( DIA_Call_ABC& call, const MIL_AgentPion& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 0 ) ) );
    
    DEC_AutomateDecision* pAutomate = call.GetParameter( 0 ).ToUserObject( pAutomate );
    assert( pAutomate );
    
    DEC_AutomateFunctions::GetPionPC( call, pAutomate->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithoutPCOfAutomate
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetPionsWithoutPCOfAutomate( DIA_Call_ABC& call, const MIL_AgentPion& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 0 ) ) );
    
    DEC_AutomateDecision* pAutomate = call.GetParameter( 0 ).ToUserObject( pAutomate );
    assert( pAutomate );
    
    DEC_AutomateFunctions::GetPionsWithoutPC( call, pAutomate->GetAutomate() );
}


// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ChangeAutomate
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ChangeAutomate( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 0 ) ) );
    
    DEC_AutomateDecision* pAutomate = call.GetParameter( 0 ).ToUserObject( pAutomate );
    assert( pAutomate );
    
    if ( pAutomate->GetAutomate().GetArmy() != callerAgent.GetArmy() )
    {
        call.GetResult().SetValue( false );
        return;
    }
    
    callerAgent.ChangeAutomate( pAutomate->GetAutomate() );
    call.GetResult().SetValue( true );
}


// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPosture
// Created: JVT 2005-01-31
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetPosture( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const PHY_RolePion_Posture& rolePosture = callerAgent.GetRole< PHY_RolePion_Posture >();

    call.GetResult().SetValue( (int)( rolePosture.GetPostureCompletionPercentage() >= 1. ? rolePosture.GetCurrentPosture().GetID() : rolePosture.GetLastPosture().GetID() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::TimeLeftForMoving
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::TimeLeftForMoving( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const MT_Float rTime = callerAgent.GetRole< PHY_RolePion_Dotations >().GetMaxTimeForConsumption( PHY_ConsumptionType::moving_ );
    
    call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMinutes( rTime ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::TimeToMoveDistance
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::TimeToMoveDistance( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
   const MT_Float rDistance = MIL_Tools::ConvertMeterToSim( call.GetParameter( 0 ).ToFloat() );
   const MT_Float rMaxSpeed = callerAgent.GetRole< PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement();
   
   if( rMaxSpeed == 0. )
       call.GetResult().SetValue( std::numeric_limits< float >::max() );
   else
       call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMinutes( rDistance / rMaxSpeed ) );   
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetInterceptionPoint
// Created: JVT 2005-02-16
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::GetInterceptionPoint( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );

    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if ( !pKnowledge )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }
    
          MT_Vector2D                       vInterceptionPosition;
    const PHY_RolePion_Location& location = callerAgent.GetRole< PHY_RolePion_Location >();
    
    if ( !DEC_GeometryFunctions::GetInterceptionPoint( pKnowledge->GetPosition(), pKnowledge->GetDirection() * pKnowledge->GetSpeed(), location.GetPosition(), callerAgent.GetRole< PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement(), vInterceptionPosition ) )
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
    else
        call.GetResult().SetValue( new MT_Vector2D( vInterceptionPosition ), &DEC_Tools::GetTypePoint() );
}
