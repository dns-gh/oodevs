// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_AutomateFunctions.h"
#include "DEC_GeometryFunctions.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "DEC_PerceptionFunctions.h"

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetBarycenter
// Created: LMT 2011-01-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AutomateFunctions::GetBarycenter( const DEC_Decision_ABC * automat )
{
    if( !automat )
        throw std::runtime_error( "Invalid automat in GetBarycenter" );
    boost::shared_ptr< MT_Vector2D > barycenter( new MT_Vector2D() );
    automat->GetAutomate().GetAlivePionsBarycenter( *barycenter );
    return barycenter;
}

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
    {
        if( !(*it ) )
            throw std::runtime_error( "Invalid automat in GetAutomates" );
        result.push_back( &(*it)->GetDecision() );
    }
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
    if( pions.size() > 0 )
    {
        result.reserve( pions.size() - 1 );
        for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
        {
            if( !( *it ) )
                throw std::runtime_error( "Invalid pion in GetPionsWithoutPC" );
            if( callerAutomate.GetPionPC() != (**it) )
            {
                const PHY_RolePion_Communications& role = (*it)->GetRole< PHY_RolePion_Communications >();
                if( role.CanReceive() )
                    result.push_back( &(*it)->GetDecision() );
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC( const DEC_Decision_ABC* automat )
{
    if( !automat )
        throw std::runtime_error( "Invalid automat in GetPionsOfAutomateWithoutPC" );
    return GetPionsWithoutPC( automat->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetAutomatPionsWithPC
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateFunctions::GetAutomatPionsWithPC( DEC_Decision_ABC* pAutomate )
{
    if( !pAutomate )
        throw std::runtime_error( "Invalid automat in GetAutomatPionsWithPC" );
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
    std::vector< DEC_Decision_ABC* > result;
    for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
    {
        if( !( *it ) )
            throw std::runtime_error( "Invalid pion in GetPionsWithPC" );
        const PHY_RolePion_Communications& role = (*it)->GetRole< PHY_RolePion_Communications >();
        if( role.CanReceive() )
            result.push_back( &(**it).GetDecision() );
    }
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
    if( !pAutomate )
        throw std::runtime_error( "Invalid automat in GetPionPCOfAutomate" );
    return DEC_AutomateFunctions::GetPionPC( pAutomate->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionsMelee
// Created: LMT 2010-09-29
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateFunctions::GetPionsMelee( const MIL_Automate& callerAutomate )
{
    const MIL_Automate::T_PionVector& pions = callerAutomate.GetPions();
    std::vector< DEC_Decision_ABC* > result;
    for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
    {
        if( ! (*it) )
            throw std::runtime_error( "Invalid pion in GetPionsMelee" );
        const std::string& type = (*it)->GetType().GetMilPionType();
        if( type == "Pion ABC" || type == "Pion INF" )
            result.push_back( &(**it).GetDecision() );
    }
    return result;
}


// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::DecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::DecisionalState( const MIL_Automate& callerAutomate, const std::string& key, const std::string& value )
{
    client::DecisionalState msg;
    msg().mutable_source()->mutable_automat()->set_id( callerAutomate.GetID() );
    msg().set_key  ( key.c_str() );
    msg().set_value( value.c_str() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::PionChangeAutomate
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::PionChangeAutomate( DEC_Decision_ABC* pion, const DEC_Decision_ABC* superior )
{
    if( !pion || !superior )
        throw std::runtime_error( "Invalid parameter in PionChangeAutomate" );
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
    if( !pAutomate )
        throw std::runtime_error( "Invalid automat in IsEngaged" );
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
    if( !pion || !IsPionInAutomate( callerAutomate, pion->GetPion() ) )
        throw std::runtime_error( "Invalid pion in CanPionConstructObject" );
    return pion->GetPion().GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( type, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionBypassObject
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::CanPionBypassObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pion || !IsPionInAutomate( callerAutomate, pion->GetPion() ) )
        throw std::runtime_error( "Invalid pion in CanPionBypassObject" );
    if( pKnowledge && pKnowledge->IsValid() )
        return pion->GetPion().GetRole< PHY_RoleAction_Objects >().CanBypassWithReinforcement( pKnowledge->GetType() );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::CanPionDestroyObject
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::CanPionDestroyObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pion || !IsPionInAutomate( callerAutomate, pion->GetPion() ) )
        throw std::runtime_error( "Invalid pion in CanPionDestroyObject" );
    if( pKnowledge && pKnowledge->IsValid() )
        return pion->GetPion().GetRole< PHY_RoleAction_Objects >().CanDestroyWithReinforcement( pKnowledge->GetType() );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::PionTimeToMoveDistance
// Created: NLD 2005-03-21
// -----------------------------------------------------------------------------
float DEC_AutomateFunctions::PionTimeToMoveDistance( const DEC_Decision_ABC* pion, float distance )
{
    if( !pion )
        throw std::runtime_error( "Invalid pion in PionTimeToMoveDistance" );
    const double rDistance = MIL_Tools::ConvertMeterToSim( distance );
    const double rMaxSpeed = pion->GetPion().GetRole< moving::PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement();
    return rMaxSpeed != 0. ? float( MIL_Tools::ConvertSimToMinutes( rDistance / rMaxSpeed ) ) : std::numeric_limits< float >::max();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionContaminated
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsPionContaminated( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion )
{
    if( !pPion || !pCallerAutomate || !IsPionInAutomate( pCallerAutomate->GetAutomate(), pPion->GetPion() ) )
        throw std::runtime_error( "Invalid parameter in IsPionContaminated" );
    return pPion->GetPion().GetRole< nbc::PHY_RoleInterface_NBC >().IsContaminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionNeutralized
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsPionNeutralized( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion )
{
    if( !pPion || !pCallerAutomate || !IsPionInAutomate( pCallerAutomate->GetAutomate(), pPion->GetPion() ) )
        throw std::runtime_error( "Invalid parameter in IsPionNeutralized" );
    return pPion->GetPion().IsNeutralized();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPionTransported
// Created: LDC 2011-08-08
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsPionTransported( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion )
{
    if( !pPion || !pCallerAutomate || !IsPionInAutomate( pCallerAutomate->GetAutomate(), pPion->GetPion() ) )
        throw std::runtime_error( "Invalid parameter in IsPionTransported" );
    return pPion->IsTransported();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::IsPointInPionFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::IsPointInPionFuseau( const MIL_Automate& callerAutomate, MT_Vector2D* pPoint, DEC_Decision_ABC* pPion )
{
    if( !pPion || !IsPionInAutomate( callerAutomate, pPion->GetPion() ) )
        throw std::runtime_error( "Invalid parameter in IsPointInPionFuseau" );
    return DEC_GeometryFunctions::IsPointInFuseau< MIL_AgentPion >( pPion->GetPion(), pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionPosition
// Created: JVT 2004-11-26
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AutomateFunctions::GetPionPosition( const DEC_Decision_ABC* pion )
{
    if( !pion )
        throw std::runtime_error( "Invalid pion in GetPionPosition" );
    return pion->GetPion().GetRole< PHY_RoleInterface_Location >().GetSharedPosition();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPerceptionForPion
// Created: DDA 2010-10-13
// -----------------------------------------------------------------------------
double DEC_AutomateFunctions::GetPerceptionForPion( const DEC_Decision_ABC* pPion, boost::shared_ptr< MT_Vector2D > pPoint, boost::shared_ptr< MT_Vector2D > pTarget)
{
    if( !pPion )
        throw std::runtime_error( "Invalid pion in GetPerceptionForPion" );
    return DEC_PerceptionFunctions::GetPerception( pPion->GetPion(), pPoint, pTarget );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::MakePionRelievePion
// Created: NLD 2003-09-30
// -----------------------------------------------------------------------------
bool DEC_AutomateFunctions::MakePionRelievePion( const MIL_Automate& callerAutomate, DEC_Decision_ABC* relieving, DEC_Decision_ABC* relieved )
{
    if( !relieving || !relieved || !IsPionInAutomate( callerAutomate, relieving->GetPion() ) || !IsPionInAutomate( callerAutomate, relieved->GetPion() ) )
        throw std::runtime_error( "Invalid parameter in MakePionRelievePion" );
    if( relieving->GetPion().GetOrderManager().RelievePion( relieved->GetPion() ) )
    {
        relieved->GetPion().GetOrderManager().CancelMission();
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
    if( !relieving || !relieved || !IsPionInAutomate( callerAutomate, relieving->GetPion() ) || !IsPionInAutomate( callerAutomate, relieved->GetPion() ) )
        throw std::runtime_error( "Invalid parameter in CanPionRelievePion" );
    return relieving->GetPion().GetOrderManager().CanRelievePion( relieved->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::ComputePointBeforeLimaForPion
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AutomateFunctions::ComputePointBeforeLimaForPion( int phaseLine, float distanceBefore, const DEC_Decision_ABC* pion )
{
    if( !pion )
        throw std::runtime_error( "Invalid pion in ComputePointBeforeLimaForPion" );
    return DEC_GeometryFunctions::ComputePointBeforeLima< MIL_AgentPion >( pion->GetPion(), phaseLine, distanceBefore );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau( const TER_Localisation* location, const DEC_Decision_ABC* pion )
{
    if( !pion )
        throw std::runtime_error( "Invalid pion in ComputePionNearestLocalisationPointInFuseau" );
    return DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau( pion->GetPion(), location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetPionEfficiency
// Created: JSR 2010-11-29
// -----------------------------------------------------------------------------
unsigned int DEC_AutomateFunctions::GetPionEfficiency( const DEC_Decision_ABC* pion, int pionEfficiency )
{
    if( !pion )
        throw std::runtime_error( "Invalid pion in GetPionEfficiency" );
    return pion->GetPion().GetType().GetUnitType().GetPionEfficiency( static_cast< E_PionEfficiency >( pionEfficiency ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::GetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_AutomateFunctions::GetMission( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in GetMission" );
    return pAgent->GetMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::SetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::SetMission( DEC_Decision_ABC* object, boost::shared_ptr< MIL_Mission_ABC > mission )
{
    if( !object )
        throw std::runtime_error( "Invalid parameter in SetMission" );
    object->SetMission( mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::NotifyRulesOfEngagementStateChanged
// Created: LDC 2011-08-05
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::NotifyRulesOfEngagementStateChanged( MIL_Automate& callerAutomate, int state )
{
    dynamic_cast< DEC_AutomateDecision& >( callerAutomate.GetDecision() ).NotifyRulesOfEngagementStateChanged( static_cast< E_RulesOfEngagementState >( state ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateFunctions::NotifyRulesOfEngagementPopulationStateChanged
// Created: LDC 2011-08-05
// -----------------------------------------------------------------------------
void DEC_AutomateFunctions::NotifyRulesOfEngagementPopulationStateChanged( MIL_Automate& /*callerAutomate*/, int /*state*/ )
{
    // $$$$ LDC This function could be implemented when/if the .proto includes a message for pop roe on automats
    //          It's actually only useful for gui, the sim doesn't care about it.
}
