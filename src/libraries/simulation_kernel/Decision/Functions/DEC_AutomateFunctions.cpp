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
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "DEC_FunctionsTools.h"
    

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsParentAutomateEngaged
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsParentAutomateEngaged( const MIL_Automate& callerAutomate )
{
    return callerAutomate.GetParentAutomate() ? callerAutomate.GetParentAutomate()->IsEngaged() : false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetAutomates
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateFunctions::GetAutomates( const MIL_Automate& callerAutomate )
{
    std::vector< DEC_Decision_ABC* > result;
    const MIL_Automate::T_AutomateVector& automates = callerAutomate.GetAutomates();
    result.reserve( automates.size() );
    for( MIL_Automate::CIT_AutomateVector it = automates.begin(); it != automates.end(); ++it )
        result.push_back( &(*it)->GetDecision() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsWithoutPC
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateFunctions::GetPionsWithoutPC( const MIL_Automate& callerAutomate )
{
    std::vector< DEC_Decision_ABC* > result;
    const MIL_Automate::T_PionVector& pions = callerAutomate.GetPions();
    result.reserve( pions.size() - 1 );
    for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
        if( callerAutomate.GetPionPC() != (**it) )
            result.push_back( &(*it)->GetDecision() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC( const DEC_Decision_ABC* automat )
{
    assert( automat );
    return GetPionsWithoutPC( automat->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetAutomatPionsWithPC
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateFunctions::GetAutomatPionsWithPC( DEC_Decision_ABC* pAutomate )
{
    const MIL_Automate& callerAutomate = pAutomate->GetAutomate();
    return GetPionsWithPC( callerAutomate );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsWithPC
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateFunctions::GetPionsWithPC( const MIL_Automate& callerAutomate )
{
    const MIL_Automate::T_PionVector& pions = callerAutomate.GetPions();
    std::vector< DEC_Decision_ABC* > result( pions.size() );
    for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
        result.push_back( &(**it).GetDecision() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionPC
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_AutomateFunctions::GetPionPC( const MIL_Automate& callerAutomate )
{
    return &callerAutomate.GetPionPC().GetDecision();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionPCOfAutomate
// Created: JVT 2005-01-18
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_AutomateFunctions::GetPionPCOfAutomate( DEC_Decision_ABC* pAutomate )
{
    assert( pAutomate );
    
    return DEC_AutomateFunctions::GetPionPC( pAutomate->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::DecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::DecisionalState( const MIL_Automate& callerAutomate, const std::string& key, const std::string& value )
{
    NET_ASN_MsgDecisionalState msg;
    msg().oid     = callerAutomate.GetID();
    msg().key     = key.c_str();
    msg().value   = value.c_str();
    msg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::PionChangeAutomate
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::PionChangeAutomate( DEC_Decision_ABC* pion, const DEC_Decision_ABC* superior )
{
    assert( pion && superior );
    if( superior->GetAutomate().GetArmy() == pion->GetPion().GetArmy() )
    {
        pion->GetPion().ChangeSuperior( superior->GetAutomate() );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsEngaged
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsEngaged( DEC_Decision_ABC* pAutomate )
{
    return pAutomate->GetAutomate().IsEngaged();
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
bool DEC_AutomateFunctions::CanPionConstructObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, const std::string& type )
{
    assert( pion && IsPionInAutomate( callerAutomate, pion->GetPion() ) );
    return pion->GetPion().GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( type );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionBypassObject
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::CanPionBypassObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, int knowledgeId )
{
    assert( pion && IsPionInAutomate( callerAutomate, pion->GetPion() ) );
    if( const DEC_Knowledge_Object* knowledge = callerAutomate.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        return pion->GetPion().GetRole< PHY_RoleAction_Objects >().CanBypassWithReinforcement( knowledge->GetType() );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionDestroyObject
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::CanPionDestroyObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, int knowledgeId )
{
    assert( pion && IsPionInAutomate( callerAutomate, pion->GetPion() ) );
    if( const DEC_Knowledge_Object* knowledge = callerAutomate.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( knowledgeId ) )
        return pion->GetPion().GetRole< PHY_RoleAction_Objects >().CanDestroyWithReinforcement( knowledge->GetType() );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::PionTimeToMoveDistance
// Created: NLD 2005-03-21
// -----------------------------------------------------------------------------
float DEC_AutomateFunctions::PionTimeToMoveDistance( const DEC_Decision_ABC* pion, float distance )
{
    assert( pion );
    const MT_Float rDistance = MIL_Tools::ConvertMeterToSim( distance );
    const MT_Float rMaxSpeed = pion->GetPion().GetRole< moving::PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement();
    return rMaxSpeed != 0. ? float( MIL_Tools::ConvertSimToMinutes( rDistance / rMaxSpeed ) ) : std::numeric_limits< float >::max();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionContaminated
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsPionContaminated( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion )
{
    assert( pPion && IsPionInAutomate( pCallerAutomate->GetAutomate(), pPion->GetPion() ) );
    return pPion->GetPion().GetRole< nbc::PHY_RoleInterface_NBC >().IsContaminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionNeutralized
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsPionNeutralized( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion )
{
    assert( pPion && IsPionInAutomate( pCallerAutomate->GetAutomate(), pPion->GetPion() ) );
    return pPion->GetPion().IsNeutralized();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPointInPionFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsPointInPionFuseau( const MIL_Automate& callerAutomate, MT_Vector2D* pPoint, DEC_Decision_ABC* pPion )
{
    assert( pPion && IsPionInAutomate( callerAutomate, pPion->GetPion() ) );
    return DEC_GeometryFunctions::IsPointInFuseau< MIL_AgentPion >( pPion->GetPion(), pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionPosition
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AutomateFunctions::GetPionPosition( const DEC_Decision_ABC* pion )
{
    assert( pion );
    return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( pion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::MakePionRelievePion
// Created: NLD 2003-09-30
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::MakePionRelievePion( const MIL_Automate& callerAutomate, DEC_Decision_ABC* relieving, DEC_Decision_ABC* relieved )
{
    assert( relieving && relieved );
    assert( IsPionInAutomate( callerAutomate, relieving->GetPion() ) );
    assert( IsPionInAutomate( callerAutomate, relieved->GetPion() ) );

    if( relieving->GetPion().GetOrderManager().RelievePion( relieved->GetPion() ) )
    {
        relieved->GetPion().GetOrderManager().ReplaceMission();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionRelievePion
// Created: NLD 2004-12-09
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::CanPionRelievePion( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* relieving, const DEC_Decision_ABC* relieved )
{
    assert( relieving && relieved );
    assert( IsPionInAutomate( callerAutomate, relieving->GetPion() ) );
    assert( IsPionInAutomate( callerAutomate, relieved->GetPion() ) );
    return relieving->GetPion().GetOrderManager().CanRelievePion( relieved->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::ComputePointBeforeLimaForPion
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AutomateFunctions::ComputePointBeforeLimaForPion( int phaseLine, float distanceBefore, const DEC_Decision_ABC* pion )
{
    assert( pion );
    return DEC_GeometryFunctions::ComputePointBeforeLima< MIL_AgentPion >( pion->GetPion(), phaseLine, distanceBefore );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau( const TER_Localisation* location, const DEC_Decision_ABC* pion )
{
    assert( pion );
    return DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau( pion->GetPion(), location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
MIL_Mission_ABC* DEC_AutomateFunctions::GetMission( DEC_Decision_ABC* pAgent )
{
    return pAgent->GetMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::SetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::SetMission( DEC_Decision_ABC* object, MIL_Mission_ABC* mission )
{
    object->SetMission( mission );
}
