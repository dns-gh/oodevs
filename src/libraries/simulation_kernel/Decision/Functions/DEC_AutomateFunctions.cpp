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
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "DEC_FunctionsTools.h"
    

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsParentAutomateEngaged
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsParentAutomateEngaged( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( callerAutomate.GetParentAutomate() ? callerAutomate.GetParentAutomate()->IsEngaged() : false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetAutomates
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetAutomates( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    T_ObjectVector result;
    const MIL_Automate::T_AutomateVector& automates = callerAutomate.GetAutomates();
    for( MIL_Automate::CIT_AutomateVector it = automates.begin(); it != automates.end(); ++it )
            result.push_back( &(**it).GetDecision() );
    call.GetResult().SetValue( result );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsWithoutPC
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::GetPionsWithoutPC( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    T_ObjectVector result;
    const MIL_Automate::T_PionVector& pions = callerAutomate.GetPions();
    for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
    {
        if( callerAutomate.GetPionPC() != (**it) )
            result.push_back( &(**it).GetDecision() );
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
    for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
        result.push_back( &(**it).GetDecision() );
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
    std::string message( call.GetParameter( 0 ).ToString() );
    NET_ASN_MsgTrace msg;
    msg().oid = callerAutomate.GetID();
    msg().message = message.c_str();
    msg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::DecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::DecisionalState( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    std::string key  ( call.GetParameter( 0 ).ToString() );
    std::string value( call.GetParameter( 1 ).ToString() );

    NET_ASN_MsgDecisionalState msg;
    msg().oid     = callerAutomate.GetID();
    msg().key     = key.c_str();
    msg().value   = value.c_str();
    msg.Send();
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

    NET_ASN_MsgDebugPoints asn;
    asn().oid = callerAutomate.GetID();
    NET_ASN_Tools::WriteCoordinates( *pPoints, asn().coordinates );
    asn.Send();
    NET_ASN_Tools::Delete( asn().coordinates );
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

    NET_ASN_MsgDebugPoints asn;
    asn().oid = callerAutomate.GetID();
    asn().coordinates.n = 1;
    ASN1T_CoordLatLong coord;
    NET_ASN_Tools::WritePoint( *pPoint, coord );
    asn().coordinates.elem = &coord;
    asn.Send();
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

    pPion->GetPion().ChangeSuperior( pAutomate->GetAutomate() );
    call.GetResult().SetValue( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsEngaged
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::IsEngaged( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    call.GetResult().SetValue( callerAutomate.IsEngaged() );
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
     
    const std::string& type = call.GetParameter( 1 ).ToString();
    call.GetResult().SetValue( pPion->GetPion().GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( type ) );
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

    const DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 1 ), callerAutomate.GetArmy() );
    call.GetResult().SetValue( pKnowledge && pPion->GetPion().GetRole< PHY_RoleAction_Objects >().CanBypassWithReinforcement( pKnowledge->GetType() ) );
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

    const DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 1 ), callerAutomate.GetArmy() );
    call.GetResult().SetValue( pKnowledge && pPion->GetPion().GetRole< PHY_RoleAction_Objects >().CanDestroyWithReinforcement( pKnowledge->GetType() ) );
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

    pAgentToRelieve->GetPion().GetOrderManager().ReplaceMission();
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


