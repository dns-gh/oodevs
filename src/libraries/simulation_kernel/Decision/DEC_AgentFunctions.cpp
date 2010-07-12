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

#include "simulation_kernel_pch.h"
#include "DEC_AgentFunctions.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Experience.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_Suicide.h"
#include "Entities/Objects/ActivableCapacity.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Tools/MIL_Tools.h"
#include "DEC_AutomateFunctions.h"
#include "DEC_GeometryFunctions.h"
#include "protocol/ClientSenders.h"

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsNeutralized
// Created: JVT 03-10-01
//-----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsNeutralized( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.IsNeutralized();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::WearNbcProtectionSuit
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::WearNbcProtectionSuit( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().WearNbcProtectionSuit();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RemoveNbcProtectionSuit
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::RemoveNbcProtectionSuit( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().RemoveNbcProtectionSuit();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SelfDecontaminate
// Created: NLD 2005-03-25
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SelfDecontaminate( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().Decontaminate();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ActivateBlackout
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ActivateBlackout( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Communications >().ActivateBlackout();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ActivatePartialBlackout
// Created: HBD 2010-06-16
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ActivatePartialBlackout( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Communications >().ActivatePartialBlackout();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DeactivateBlackout
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DeactivateBlackout( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Communications >().DeactivateBlackout();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsContaminated
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsContaminated( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().IsContaminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsTransported
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsTransported( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleInterface_Transported >().IsTransported();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SetFlyingHeight
// Created: JVT 2004-11-02
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SetFlyingHeight( MIL_Agent_ABC& callerAgent, MT_Float height )
{
    assert( height >= 0. && "T'as deja essaye de voler � cette hauteur ?");
    callerAgent.GetRole< PHY_RoleAction_InterfaceFlying >().SetFlyingHeight( height );
}


// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsFlying
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsFlying( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SetElongation
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SetElongationFactor( MIL_Agent_ABC& callerAgent, MT_Float factor )
{
    assert( factor > 0. );
    callerAgent.GetRole< PHY_RoleInterface_Posture >().SetElongationFactor( factor );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsDead
// Created: NLD 2006-02-01
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsDead( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.IsDead();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsPC
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsPC( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.IsPC();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsAutomateEngaged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsAutomateEngaged( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetAutomate().IsEngaged();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetOperationalState
// Created: NLD 2004-04-16
// -----------------------------------------------------------------------------
MT_Float DEC_AgentFunctions::GetOperationalState( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetOperationalState();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetMajorOperationalState
// Created: NLD 2005-11-25
// -----------------------------------------------------------------------------
MT_Float DEC_AgentFunctions::GetMajorOperationalState( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorOperationalState();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPosition
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AgentFunctions::GetPosition( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Location >().GetSharedPosition();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetAgentPositionPtr
// Created: DDA 2010-03-25
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AgentFunctions::GetAgentPositionPtr( DEC_Decision_ABC* brain )
{
    return brain->GetPion().GetRole< PHY_RoleInterface_Location >().GetSharedPosition();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetDirection
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
const MT_Vector2D* DEC_AgentFunctions::GetDirection( const MIL_Agent_ABC& callerAgent )
{
    return &callerAgent.GetRole< PHY_RoleInterface_Location >().GetDirection();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetIdentificationDistance
// Created: DDA 2010-03-24
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::GetIdentificationDistance( MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorComposante()->GetIdentificationMaxRange();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetReconnoissanceDistance
// Created: GGE & PSN 2010-04-20
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::GetReconnoissanceDistance( MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorComposante()->GetReconnoissanceMaxRange();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetDetectionDistance
// Created: DDA 2010-06-09
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::GetDetectionDistance( MIL_Agent_ABC& callerAgent )
{
    return (float)callerAgent.GetRole< PHY_RoleInterface_Perceiver >().GetMaxAgentPerceptionDistance();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanConstructObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanConstructObject( const MIL_Agent_ABC& callerAgent, const std::string& type )
{
    return callerAgent.GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( type );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasDotationForBuilding
// Created: LMT 2010-07-07
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasDotationForBuilding( MIL_Agent_ABC& callerAgent, const std::string& type )
{
    return callerAgent.GetRole< PHY_RoleAction_Objects >().EnoughtDotationForBuilding( type, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanBypassObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanBypassObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && callerAgent.GetRole< PHY_RoleAction_Objects >().CanBypassWithReinforcement( objectKnowledge->GetType() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanDestroyObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanDestroyObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && callerAgent.GetRole< PHY_RoleAction_Objects >().CanDestroyWithReinforcement( objectKnowledge->GetType() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanMineObject
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanMineObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && callerAgent.GetRole< PHY_RoleAction_Objects >().CanMineWithReinforcement( objectKnowledge->GetType() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanActivateObject
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanActivateObject( boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && objectKnowledge->GetType().GetCapacity< ActivableCapacity >();
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EnableDiscreteMode
// Created: JVT 04-05-17
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::EnableDiscreteMode( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Posture >().EnableDiscreteMode();
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisableDiscreteMode
// Created: JVT 04-05-17
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::DisableDiscreteMode( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Posture >().DisableDiscreteMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DecisionalState( const MIL_Agent_ABC& callerAgent, const std::string& key, const std::string& value )
{
    client::DecisionalState msg;
    msg().set_oid( callerAgent.GetID() );
    msg().set_key( key.c_str() );
    msg().set_value( value.c_str() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsLoaded
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsLoaded( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleAction_Loading >().IsLoaded();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetLoadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
MT_Float DEC_AgentFunctions::GetLoadingTime( const MIL_Agent_ABC& callerAgent )
{
    return MIL_Tools::ConvertSimToMinutes( callerAgent.GetRole< transport::PHY_RoleAction_Loading >().GetLoadingTime() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetUnloadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
MT_Float DEC_AgentFunctions::GetUnloadingTime( const MIL_Agent_ABC& callerAgent )
{
    return MIL_Tools::ConvertSimToMinutes( callerAgent.GetRole< transport::PHY_RoleAction_Loading >().GetUnloadingTime() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisableHumanTransportersNow
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DisableHumanTransportersNow( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< transport::PHY_RoleInterface_Transported >().DisableHumanTransporters( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RecoverHumanTransportersNow
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::RecoverHumanTransportersNow( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< transport::PHY_RoleInterface_Transported >().RecoverHumanTransporters();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AreHumanTransportersReady
// Created: JVT 2005-01-31
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AreHumanTransportersReady( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleInterface_Transported >().HasHumanTransportersReady();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyForceRatioStateChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyForceRatioStateChanged( (E_ForceRatioState)state );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged
// Created: NLD 2005-10-19
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyIndirectFireAvailabilityChanged( (E_FireAvailability)state );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyRulesOfEngagementStateChanged( (E_RulesOfEngagementState )state );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetRulesOfEngagementState
// Created: DDA 2010-02-03
// -----------------------------------------------------------------------------
int DEC_AgentFunctions::GetRulesOfEngagementState( const MIL_Agent_ABC& callerAgent )
{
   return static_cast< int >( callerAgent.GetRole< DEC_RolePion_Decision >().GetRulesOfEngagementState() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged
// Created: SBO 2005-11-22
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged( MIL_Agent_ABC& callerAgent, int population )
{
    const PHY_RoePopulation* pRoe = PHY_RoePopulation::Find( population );
    assert( pRoe );
    callerAgent.GetRole< DEC_RolePion_Decision >().NotifyRoePopulationChanged( *pRoe );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyOperationalStateChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyOperationalStateChanged( (E_OperationalState)state );
}

// -----------------------------------------------------------------------------
// Name: static void DEC_AgentFunctions::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyCloseCombatStateChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyCloseCombatStateChanged( (E_CloseCombatState)state );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanFactorTiredness
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
unsigned int DEC_AgentFunctions::GetHumanFactorTiredness( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_HumanFactors >().GetTiredness().GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanFactorMorale
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
unsigned int DEC_AgentFunctions::GetHumanFactorMorale( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_HumanFactors >().GetMorale().GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RelievePion
// Created: NLD 2003-09-30
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::RelievePion( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* agentToRelieve )
{
    if( !callerAgent.GetOrderManager().RelievePion( agentToRelieve->GetPion() ) )
        return false;
    agentToRelieve->GetPion().GetOrderManager().CancelMission();
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanRelievePion
// Created: NLD 2004-12-09
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanRelievePion( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pAgentToRelieve )
{
    assert( pAgentToRelieve );
    return callerAgent.GetOrderManager().CanRelievePion( pAgentToRelieve->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::GetPionsWithPC( const MIL_Agent_ABC& callerAgent )
{
    return DEC_AutomateFunctions::GetPionsWithPC( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithoutPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::GetPionsWithoutPC( const MIL_Agent_ABC& callerAgent )
{
    return DEC_AutomateFunctions::GetPionsWithoutPC( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_AgentFunctions::GetPionPC( const MIL_Agent_ABC& callerAgent )
{
    return DEC_AutomateFunctions::GetPionPC( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionPCOfAutomate
// Created: JVT 2005-01-18
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_AgentFunctions::GetPionPCOfAutomate( const DEC_Decision_ABC* automat )
{
    assert( automat );
    return DEC_AutomateFunctions::GetPionPC( automat->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithoutPCOfAutomate
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::GetPionsWithoutPCOfAutomate( const DEC_Decision_ABC* automat )
{
    assert( automat );
    return DEC_AutomateFunctions::GetPionsWithoutPC( automat->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ChangeAutomate
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::ChangeAutomate( MIL_Agent_ABC& callerAgent, DEC_Decision_ABC* automat )
{
    assert( automat );
    if( automat->GetAutomate().GetArmy() == callerAgent.GetArmy() )
    {
        callerAgent.ChangeSuperior( automat->GetAutomate() );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPosture
// Created: JVT 2005-01-31
// -----------------------------------------------------------------------------
int DEC_AgentFunctions::GetPosture( const MIL_Agent_ABC& callerAgent )
{
    const PHY_RoleInterface_Posture& rolePosture = callerAgent.GetRole< PHY_RoleInterface_Posture >();
    return (int)( rolePosture.GetPostureCompletionPercentage() >= 1. ? rolePosture.GetCurrentPosture().GetID() : rolePosture.GetLastPosture().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInCity
// Created: MGD 2010-05-11
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInCity( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::TimeLeftForMoving
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::TimeLeftForMoving( const MIL_Agent_ABC& callerAgent )
{
    const MT_Float rTime = callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >().GetMaxTimeForConsumption( PHY_ConsumptionType::moving_ );
    return static_cast< float >( MIL_Tools::ConvertSimToMinutes( rTime ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::TimeToMoveDistance
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::TimeToMoveDistance( const MIL_Agent_ABC& callerAgent, float distance )
{
   const MT_Float rMaxSpeed = callerAgent.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement();
   if( rMaxSpeed == 0 )
       return std::numeric_limits< float >::max();
    return static_cast< float >( MIL_Tools::ConvertSimToMinutes( MIL_Tools::ConvertMeterToSim( distance ) / rMaxSpeed ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetInterceptionPoint
// Created: JVT 2005-02-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AgentFunctions::GetInterceptionPoint( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    boost::shared_ptr< MT_Vector2D > result;
    if( pKnowledge && pKnowledge->IsValid())
    {
        result.reset( new MT_Vector2D() );
        DEC_GeometryFunctions::GetInterceptionPoint( pKnowledge->GetPosition(), pKnowledge->GetDirection() * pKnowledge->GetSpeed(),
                callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(),
                callerAgent.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement(), *result );
    }
    return result;
}

// =============================================================================
// POPULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetRoePopulation
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
int DEC_AgentFunctions::GetRoePopulation( const MIL_Agent_ABC& callerAgent )
{
    return static_cast< int >( callerAgent.GetRole< DEC_RolePion_Decision >().GetRoePopulation().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ChannelPopulation
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ChannelPopulations( const TER_Localisation* location )
{
    assert( location );
    MIL_AgentServer::GetWorkspace().GetEntityManager().ChannelPopulations( *location );
}

// =============================================================================
// INSTALLATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInstalled
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInstalled( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Posture >().IsInstalled();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsUninstalled
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsUninstalled( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Posture >().IsUninstalled();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::Install
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::Install( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Posture >().Install();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetDirectionDanger
// Created: LDC 2009-04-21
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AgentFunctions::GetDirectionDanger( const boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    if( !pMission.get() )
        return boost::shared_ptr< MT_Vector2D >();
    boost::shared_ptr< MT_Vector2D > result( new MT_Vector2D( pMission->GetDirDanger() ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetMission
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_AgentFunctions::GetMission( DEC_Decision_ABC* pAgent )
{
    return pAgent->GetMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasMission
// Created: LDC 2010-06-23
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasMission( DEC_Decision_ABC* pAgent )
{
    return ( 0 != pAgent->GetMission().get() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SetMission
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SetMission( DEC_Decision_ABC* pAgent, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    pAgent->SetMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasDotation
// Created: LDC 2010-03-26
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasDotation( const MIL_Agent_ABC& callerAgent, const PHY_DotationCategory* category )
{
    return callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >().GetDotationNumber( *category ) > 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::Suicide
// Created: LDC 2010-03-29
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::Suicide( MIL_Agent_ABC& callerAgent )
{
    MIL_Effect_Suicide* pEffect = new MIL_Effect_Suicide( callerAgent );
    MIL_EffectManager::GetEffectManager().Register( *pEffect );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanBeIlluminate
// Created: GGE 2010-06-17
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanIlluminate( DEC_Decision_ABC* pAgent )
{
    const dotation::PHY_RoleInterface_Dotations& roleDotations = pAgent->GetPion().GetRole< dotation::PHY_RoleInterface_Dotations >();
    if( roleDotations.GetIlluminationDotations( 0.0, true ) || roleDotations.GetIlluminationDotations( 0.0, false ) )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetilluminatingRange
// Created: GGE 2010-06-23
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::GetIlluminatingRange( const MIL_Agent_ABC& callerAgent )
{
    const dotation::PHY_RoleInterface_Dotations& roleDotations = callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >();
    return roleDotations.GetIlluminatingRange( );
}
