// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AutomateFunctions.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 11:03 $
// $Revision: 20 $
// $Workfile: DEC_AutomateFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_AutomateFunctions.h"

#include "DEC_GeometryFunctions.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Decision/DEC_Tools.h"
    
// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsWithoutPC
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionsWithoutPC( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    T_ObjectVector result;

    const MIL_Automate::T_PionVector& pions = callerAutomate.GetPions();
    for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        if( callerAutomate.GetPionPC() != (**itPion) )
            result.push_back( &(**itPion).GetDecision() );
    }

    call.GetResult().SetValue( result );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 0 ) ) );

    DEC_AutomateDecision* pAutomate = call.GetParameter( 0 ).ToUserObject( pAutomate );
    assert( pAutomate );
    
    GetPionsWithoutPC( call, pAutomate->GetAutomate() );    
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsWithPC
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionsWithPC( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    T_ObjectVector result; 

    const MIL_Automate::T_PionVector& pions = callerAutomate.GetPions();
    result.reserve( pions.size() );
    for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        result.push_back( &(**itPion).GetDecision() );
    call.GetResult().SetValue( result );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsOfAutomateWithPC
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionsOfAutomateWithPC( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 0 ) ) );

    DEC_AutomateDecision* pAutomate = call.GetParameter( 0 ).ToUserObject( pAutomate );
    assert( pAutomate );
    
    GetPionsWithPC( call, pAutomate->GetAutomate() );    
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionPC
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionPC( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( callerAutomate.GetPionPC().GetDecision() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionPCOfAutomate
// Created: JVT 2005-01-18
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionPCOfAutomate( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 0 ) ) );
    
    DEC_AutomateDecision* pAutomate = call.GetParameter( 0 ).ToUserObject( pAutomate );
    assert( pAutomate );
    
    DEC_AutomateFunctions::GetPionPC( call, pAutomate->GetAutomate() );
}

//-----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::Trace
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void DEC_AutomateFunctions::Trace( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    std::string msg( call.GetParameter( 0 ).ToString() );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)callerAutomate.GetID();
    dinMsg << msg;

    msgMgr.SendMsgTrace( dinMsg );
}

//-----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::Debug
// Created: NLD 2002-12-17
// Last modified: JVT 03-03-03
//-----------------------------------------------------------------------------
void DEC_AutomateFunctions::Debug( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    if( !MIL_AgentServer::GetWorkspace().GetConfig().UseDecDebug() )
        return;

    std::string msg( call.GetParameter( 0 ).ToString() );
    MT_LOG_INFO_MSG( MT_FormatString( "Automate %d says : [%s]", callerAutomate.GetID(), msg.c_str() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::DebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::DebugDrawPoints( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );

    T_PointVector* pPoints = call.GetParameter( 0 ).ToUserPtr( pPoints );
    assert( pPoints );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)callerAutomate.GetID();
    dinMsg << (uint32)pPoints->size();
    for( CIT_PointVector itPoint = pPoints->begin(); itPoint != pPoints->end(); ++itPoint )
        dinMsg << *itPoint;
    msgMgr.SendMsgDebugDrawPoints( dinMsg );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::DebugDrawPoint
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::DebugDrawPoint( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    const MT_Vector2D* pPoint = call.GetParameter( 0 ).ToUserPtr( pPoint );
    assert( pPoint );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)callerAutomate.GetID();
    dinMsg << (uint32)1;
    dinMsg << *pPoint;
    msgMgr.SendMsgDebugDrawPoints( dinMsg );
}

// -----------------------------------------------------------------------------
// Name: static void DEC_AutomateFunctions::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::NotifyForceRatioStateChanged( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    callerAutomate.GetDecision().NotifyForceRatioStateChanged( (E_ForceRatioState)call.GetParameter( 0 ).ToId() );
}

// -----------------------------------------------------------------------------
// Name: static void DEC_AutomateFunctions::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::NotifyRulesOfEngagementStateChanged( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    callerAutomate.GetDecision().NotifyRulesOfEngagementStateChanged( (E_RulesOfEngagementState )call.GetParameter( 0 ).ToId() );
}  

// -----------------------------------------------------------------------------
// Name: static void DEC_AutomateFunctions::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::NotifyCloseCombatStateChanged( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    callerAutomate.GetDecision().NotifyCloseCombatStateChanged( (E_CloseCombatState)call.GetParameter( 0 ).ToId() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::NotifyOperationalStateChanged( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    callerAutomate.GetDecision().NotifyOperationalStateChanged( (E_OperationalState)call.GetParameter( 0 ).ToId() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::PionChangeAutomate
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::PionChangeAutomate( DIA_Call_ABC& call, MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion    ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 1 ) ) );

    DEC_RolePion_Decision* pPion     = call.GetParameter( 0 ).ToUserObject( pPion );
    DEC_AutomateDecision*  pAutomate = call.GetParameter( 1 ).ToUserObject( pAutomate );

    assert( pPion     );
    assert( pAutomate );

    if( pAutomate->GetAutomate().GetArmy() != pPion->GetPion().GetArmy() )
    {
        call.GetResult().SetValue( false );
        return;
    }

    pPion->GetPion().ChangeAutomate( pAutomate->GetAutomate() );
    call.GetResult().SetValue( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsEmbraye
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsEmbraye( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( callerAutomate.IsEmbraye() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsSurrendered
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsSurrendered( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( callerAutomate.IsSurrendered() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsPrisoner( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( callerAutomate.IsPrisoner() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionInAutomate
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsPionInAutomate( const MIL_Automate& automate, const MIL_AgentPion& pion )
{
    const MIL_Automate::T_PionVector& pions = automate.GetPions();
    
    return std::find( pions.begin(), pions.end(), &pion ) != pions.end();
}


// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionConstructObject
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::CanPionConstructObject( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
     
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 1 ).ToId() );
    assert( pObjectType );

    call.GetResult().SetValue( pObjectType && pPion->GetPion().GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( *pObjectType ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionBypassObject
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::CanPionBypassObject( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );

    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 1 ).ToId() );
    assert( pObjectType );

    call.GetResult().SetValue( pObjectType && pPion->GetPion().GetRole< PHY_RoleAction_Objects >().CanBypassWithReinforcement( *pObjectType ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionDestroyObject
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::CanPionDestroyObject( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );

    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 1 ).ToId() );
    assert( pObjectType );
     
    call.GetResult().SetValue( pObjectType && pPion->GetPion().GetRole< PHY_RoleAction_Objects >().CanDestroyWithReinforcement( *pObjectType ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionMineObject
// Created: NLD 2005-09-13
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::CanPionMineObject( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );

    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 1 ).ToId() );
    assert( pObjectType );
     
    call.GetResult().SetValue( pObjectType && pPion->GetPion().GetRole< PHY_RoleAction_Objects >().CanMineWithReinforcement( *pObjectType ) );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionActivateObject
// Created: NLD 2005-09-13
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::CanPionActivateObject( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );

    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 1 ).ToId() );
    assert( pObjectType );

    call.GetResult().SetValue( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::PionTimeLeftForMoving
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::PionTimeLeftForMoving( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );   
    
    const MT_Float rTime = pPion->GetPion().GetRole< PHY_RolePion_Dotations >().GetMaxTimeForConsumption( PHY_ConsumptionType::moving_ );
    call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMinutes( rTime ) );
}   

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::PionTimeToMoveDistance
// Created: NLD 2005-03-21
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::PionTimeToMoveDistance( DIA_Call_ABC& call, const MIL_Automate&  )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );   

   const MT_Float rDistance = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );
   const MT_Float rMaxSpeed = pPion->GetPion().GetRole< PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement();
   
   if( rMaxSpeed == 0. )
       call.GetResult().SetValue( std::numeric_limits< float >::max() );
   else
       call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMinutes( rDistance / rMaxSpeed ) );   
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionContaminated
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsPionContaminated( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( pPion->GetPion().GetRole< PHY_RolePion_NBC >().IsContaminated() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionFlying
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsPionFlying( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );  
    assert( pPion );
    
    call.GetResult().SetValue( pPion->GetPion().GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionTransported
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsPionTransported( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( pPion->GetPion().GetRole< PHY_RolePion_Transported >().IsTransported() );
}


// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionTransportPion
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::CanPionTransportPion( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    const DEC_RolePion_Decision* pTransporter           = call.GetParameter( 0 ).ToUserObject( pTransporter );
    const DEC_RolePion_Decision* pTransported           = call.GetParameter( 1 ).ToUserObject( pTransported );
    const bool                   bTransportOnlyLoadable = call.GetParameter( 2 ).ToBool();

    assert( pTransporter );
    assert( pTransported );
    call.GetResult().SetValue( pTransporter->GetPion().GetRole< PHY_RoleAction_Transport >().CanTransportPion( pTransported->GetPion(), bTransportOnlyLoadable ) );
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionNBCProtected
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsPionNBCProtected( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( pPion->GetPion().GetRole< PHY_RolePion_NBC >().IsWearingNbcProtectionSuit() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionMoving
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsPionMoving( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( pPion->GetPion().GetRole< PHY_RolePion_Location >().GetCurrentSpeed() != 0. );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionNeutralized
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsPionNeutralized( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( pPion->GetPion().IsNeutralized() );
}


// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPointInPionFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsPointInPionFuseau( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 1 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 1 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    DEC_GeometryFunctions::IsPointInFuseau< MIL_AgentPion >( call, pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionOperationalState
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionOperationalState( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
//    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( (float)pPion->GetPion().GetRole< PHY_RolePion_Composantes >().GetOperationalState() );    
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionMajorOperationalState
// Created: NLD 2005-11-25
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionMajorOperationalState( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
//    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( (float)pPion->GetPion().GetRole< PHY_RolePion_Composantes >().GetMajorOperationalState() );    
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionPosition
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionPosition( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
//    assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( (void*)&pPion->GetPion().GetRole< PHY_RolePion_Location >().GetPosition(), &DEC_Tools::GetTypePoint(), 1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionDirection
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionDirection( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
//  assert( IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    
    call.GetResult().SetValue( (void*)&pPion->GetPion().GetRole< PHY_RolePion_Location >().GetDirection(), &DEC_Tools::GetTypeDirection(), 1 );    
}


// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::MakePionRelievePion
// Created: NLD 2003-09-30
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::MakePionRelievePion( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 1 ) ) );

    DEC_RolePion_Decision* pAgentRelieving = call.GetParameter( 0 ).ToUserObject( pAgentRelieving );
    DEC_RolePion_Decision* pAgentToRelieve = call.GetParameter( 1 ).ToUserObject( pAgentToRelieve );
    
    assert( pAgentRelieving );
    assert( IsPionInAutomate( callerAutomate, pAgentRelieving->GetPion() ) );
    assert( pAgentToRelieve );
    assert( IsPionInAutomate( callerAutomate, pAgentToRelieve->GetPion() ) );

    if( !pAgentRelieving->GetPion().GetOrderManager().RelievePion( pAgentToRelieve->GetPion() ) )
    {
        call.GetResult().SetValue( false );
        return;
    }

    pAgentToRelieve->GetPion().GetOrderManager().OnReceiveRCMissionFinished(); //$$$ TMP - renommer la méthode
    call.GetResult().SetValue( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionRelievePion
// Created: NLD 2004-12-09
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::CanPionRelievePion( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 1 ) ) );

    DEC_RolePion_Decision* pAgentRelieving = call.GetParameter( 0 ).ToUserObject( pAgentRelieving );
    DEC_RolePion_Decision* pAgentToRelieve = call.GetParameter( 1 ).ToUserObject( pAgentToRelieve );
    
    assert( pAgentRelieving );
    assert( IsPionInAutomate( callerAutomate, pAgentRelieving->GetPion() ) );
    assert( pAgentToRelieve );
    assert( IsPionInAutomate( callerAutomate, pAgentToRelieve->GetPion() ) );

    call.GetResult().SetValue( pAgentRelieving->GetPion().GetOrderManager().CanRelievePion( pAgentToRelieve->GetPion() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::DebrayeAutomate
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::DebrayeAutomate( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( callerAutomate.IsEmbraye() );
    callerAutomate.Debraye();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::ComputePointBeforeLimaForPion
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::ComputePointBeforeLimaForPion( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 3 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 3 ).ToUserObject( pPion );
    
    DEC_GeometryFunctions::ComputePointBeforeLima< MIL_AgentPion >( call, pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 2 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 2 ).ToUserObject( pPion );
    
    DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau( call, pPion->GetPion() );
}


// =============================================================================
// ETAT REFUGEE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::NotifyRefugeeManagedStateChanged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::NotifyRefugeeManagedStateChanged( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    callerAutomate.NotifyRefugeeManagedStateChanged( call.GetParameter( 0 ).ToBool() );
}
