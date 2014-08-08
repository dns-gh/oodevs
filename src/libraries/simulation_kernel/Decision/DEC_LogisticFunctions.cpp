//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_LogisticFunctions.cpp $
// $Author: Jvt $
// $Modtime: 12/05/05 14:34 $
// $Revision: 12 $
// $Workfile: DEC_LogisticFunctions.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_LogisticFunctions.h"
#include "DEC_Decision_ABC.h"
#include "DEC_ResourceNetwork.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Logistic/SupplyRecipient_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

using namespace human;

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceEnableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >();//@TODO Maybe use Get and assert this DEC functions are just register on good PionLog
    if( role )
        role->EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceDisableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >();
    if( role )
        role->DisableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceChangePriorities( MIL_Agent_ABC& callerAgent, const std::vector< const PHY_ComposanteTypePion* >& priorities )
{
    PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >();
    if( role )
        role->ChangePriorities( priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities( MIL_Agent_ABC& callerAgent, const std::vector< const DEC_Decision_ABC* >& priorities )
{
    T_AutomateVector automates;
    automates.reserve( priorities.size() );
    for( std::vector< const DEC_Decision_ABC* >::const_iterator it = priorities.begin(); it != priorities.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
        automates.push_back( &(*it)->GetAutomate() );
    }
    PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >();
    if( role )
        role->ChangePriorities( automates );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceChangeWorkRate
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceChangeWorkRate( MIL_Agent_ABC& callerAgent, int workRate )
{
    if( const PHY_MaintenanceWorkRate* pWorkRate = PHY_MaintenanceWorkRate::Find( sword::EnumLogMaintenanceWorkRate( workRate ) ) )
        if( PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >() )
            role->ChangeWorkRate( *pWorkRate );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceEnableSystem( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMaintenanceEnableSystem( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceDisableSystem( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMaintenanceDisableSystem( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangePriorities( MIL_Automate& callerAutomate, const std::vector< const PHY_ComposanteTypePion* >& priorities )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMaintenanceChangePriorities( *pionPc, priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMaintenancePriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
std::vector< const PHY_ComposanteTypePion* > DEC_LogisticFunctions::GetAutomateMaintenancePriorities( MIL_Automate& callerAutomate )
{
    std::vector< const PHY_ComposanteTypePion* > result;
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        if( PHY_RoleInterface_Maintenance* role = pionPc->RetrieveRole< PHY_RoleInterface_Maintenance >() )
            result = role->GetMaintenancePriorities();
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities( MIL_Automate& callerAutomate, const std::vector< const DEC_Decision_ABC* >& priorities )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMaintenanceChangeTacticalPriorities( *pionPc, priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMaintenanceTacticalPriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
std::vector< const DEC_Decision_ABC* > DEC_LogisticFunctions::GetAutomateMaintenanceTacticalPriorities( MIL_Automate& callerAutomate )
{
    std::vector< const DEC_Decision_ABC* > result;
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        if( PHY_RoleInterface_Maintenance* role = pionPc->RetrieveRole< PHY_RoleInterface_Maintenance >() )
        {
            T_AutomateVector priorities = role->GetAutomatePriorities();
            for( T_AutomateVector::const_iterator it = priorities.begin(); it != priorities.end(); ++it )
                result.push_back( &(*it)->GetDecision() );
        }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate( MIL_Automate& callerAutomate, int workRate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMaintenanceChangeWorkRate( *pionPc, workRate );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMaintenanceWorkRate
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
int DEC_LogisticFunctions::GetAutomateMaintenanceWorkRate( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        if( PHY_RoleInterface_Maintenance* role = pionPc->RetrieveRole< PHY_RoleInterface_Maintenance >() )
            return role->GetWorkRate();
    return -1;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::EvacuateWoundedHumansToTC2
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::EvacuateWoundedHumansToTC2( DEC_Decision_ABC* pPionWounded, DEC_Decision_ABC* pDecTC2 )
{
    if( !pPionWounded || !pDecTC2 )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_AutomateLOG* tc2 = pDecTC2->GetAutomate().FindLogisticManager();
    if( !tc2 )
        throw MASA_EXCEPTION( "invalid parameter." );
    pPionWounded->GetPion().GetRole< PHY_RoleInterface_Humans >().EvacuateWoundedHumans( *tc2 );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool DEC_LogisticFunctions::HasWoundedHumansToEvacuate( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPion->GetPion().GetRole< PHY_RoleInterface_Humans >().HasWoundedHumansToEvacuate();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    pPion->GetPion().GetRole< PHY_RoleInterface_Humans >().ChangeEvacuationMode( PHY_RoleInterface_Humans::eEvacuationMode_Manual );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    pPion->GetPion().GetRole< PHY_RoleInterface_Humans >().ChangeEvacuationMode( PHY_RoleInterface_Humans::eEvacuationMode_Auto );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalEnableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalDisableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->DisableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalEnableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalEnableSortingFunction( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->EnableSortingFunction();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalDisableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalDisableSortingFunction( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->DisableSortingFunction();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalEnableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalEnableHealingFunction( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->EnableHealingFunction();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalDisableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalDisableHealingFunction( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->DisableHealingFunction();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalChangePriorities( MIL_Agent_ABC& callerAgent, const std::vector< const PHY_HumanWound* >& priorities )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->ChangePriorities( priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities( MIL_Agent_ABC& callerAgent, const std::vector< const DEC_Decision_ABC* >& priorities )
{
    T_AutomateVector automates;
    automates.reserve( priorities.size() );
    for( std::vector< const DEC_Decision_ABC* >::const_iterator it = priorities.begin(); it != priorities.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
        automates.push_back( &(*it)->GetAutomate() );
    }
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->ChangePriorities( automates );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalEnableSystem( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMedicalEnableSystem( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalDisableSystem( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMedicalDisableSystem( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalEnableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalEnableSortingFunction( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMedicalEnableSortingFunction( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalDisableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalDisableSortingFunction( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMedicalDisableSortingFunction( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalEnableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalEnableHealingFunction( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMedicalEnableHealingFunction( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalDisableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalDisableHealingFunction( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMedicalDisableHealingFunction( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalChangePriorities( MIL_Automate& callerAutomate, const std::vector< const PHY_HumanWound* >& priorities )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMedicalChangePriorities( *pionPc, priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMedicalPriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
std::vector< const PHY_HumanWound* > DEC_LogisticFunctions::GetAutomateMedicalPriorities( MIL_Automate& callerAutomate )
{
    std::vector< const PHY_HumanWound* > result;
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        if( PHY_RoleInterface_Medical* role = pionPc->RetrieveRole< PHY_RoleInterface_Medical >() )
            result = role->GetMedicalPriorities();
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities( MIL_Automate& callerAutomate, const std::vector< const DEC_Decision_ABC* >& priorities )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionMedicalChangeTacticalPriorities( *pionPc, priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMedicalTacticalPriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
std::vector< const DEC_Decision_ABC* > DEC_LogisticFunctions::GetAutomateMedicalTacticalPriorities( MIL_Automate& callerAutomate )
{
    std::vector< const DEC_Decision_ABC* > result;
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        if( PHY_RoleInterface_Medical* role = pionPc->RetrieveRole< PHY_RoleInterface_Medical >() )
        {
            T_AutomateVector priorities = role->GetAutomatePriorities();
            for( T_AutomateVector::const_iterator it = priorities.begin(); it != priorities.end(); ++it )
                result.push_back( &(*it)->GetDecision() );
        }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ChangeDotationsValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ChangeDotationsValueUsingTC2( MIL_Agent_ABC& callerAgent, int dotationType, const double rCapacityFactor, int ammoDotationClassId )
{
    const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( dotationType );
    assert( pDotationType );
    const PHY_AmmoDotationClass* pAmmoDotationClass = PHY_AmmoDotationClass::Find( ammoDotationClassId );
    callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >().ChangeDotationsValueUsingTC2( *pDotationType, pAmmoDotationClass, rCapacityFactor );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::EnforceAviationResources
// Created: JSR 2013-10-21
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::EnforceAviationResources( DEC_Decision_ABC* callerAgent, int aviationRange )
{
    if( !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent->GetPion().GetRole< dotation::PHY_RoleInterface_Dotations >().EnforceAviationResources( static_cast< E_AviationRange >( aviationRange ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConnectToResourceNode
// Created: JSR 2012-04-23
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConnectToResourceNode( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
{
    if( !callerAgent || !resourceNetwork )
        throw MASA_EXCEPTION( "invalid parameter." );
    PHY_RoleInterface_Supply* role = callerAgent->GetPion().RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ConnectToResourceNode( resourceNetwork->GetObjectId(), resourceNetwork->GetResource() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::DisconnectFromResourceNode
// Created: JSR 2012-04-23
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::DisconnectFromResourceNode( DEC_Decision_ABC* callerAgent )
{
    if( !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    PHY_RoleInterface_Supply* role = callerAgent->GetPion().RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->DisconnectFromResourceNode();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionSupplyEnableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionSupplyEnableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionSupplyDisableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionSupplyDisableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->DisableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateSupplyEnableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateSupplyEnableSystem( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionSupplyEnableSystem( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateSupplyDisableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateSupplyDisableSystem( MIL_Automate& callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate.GetPionPC() )
        PionSupplyDisableSystem( *pionPc );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateRequestSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateRequestSupply( MIL_Automate& callerAutomate )
{
    callerAutomate.RequestDotationSupply();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionRequestSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionRequestSupply( MIL_Agent_ABC& callerAgent )
{
    AutomateRequestSupply( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyNotifyMovedToSupplier
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyNotifyMovedToSupplier( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ConvoyNotifyMovedToSupplier();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyNotifyMovedToTransportersProvider
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyNotifyMovedToTransportersProvider( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ConvoyNotifyMovedToTransportersProvider();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyNotifyMovedToSupplyRecipient
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyNotifyMovedToSupplyRecipient( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ConvoyNotifyMovedToSupplyRecipient();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyEndMission
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyEndMission( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->ConvoyEndMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetCurrentAction
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
int DEC_LogisticFunctions::ConvoyGetCurrentAction( const MIL_Agent_ABC& callerAgent )
{
    const PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    return role ? role->ConvoyGetCurrentAction() : 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetCurrentSupplyRecipient
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_LogisticFunctions::ConvoyGetCurrentSupplyRecipient( const MIL_Agent_ABC& callerAgent )
{
    const PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
    {
        logistic::SupplyRecipient_ABC* recipient = role->ConvoyGetCurrentSupplyRecipient();
        if( recipient )
            if( const MIL_AgentPion* pionPc = recipient->GetPC() )
                return ( const_cast< DEC_Decision_ABC* >( &pionPc->GetDecision() ) );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetTransportersProvider
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_LogisticFunctions::ConvoyGetTransportersProvider( const MIL_Agent_ABC& callerAgent )
{
    const PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
    {
        logistic::SupplySupplier_ABC* recipient = role->ConvoyGetTransportersProvider();
        if( recipient )
        {
            const MIL_AgentPion* pc = recipient->GetPC();
            if( pc )
                return const_cast< DEC_Decision_ABC* >( &pc->GetDecision() );
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetSupplier
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_LogisticFunctions::ConvoyGetSupplier( const MIL_Agent_ABC& callerAgent )
{
    const PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
    {
        const MIL_Agent_ABC* provider = role->ConvoyGetSupplier();
        if( provider )
            return const_cast< DEC_Decision_ABC* >( &provider->GetDecision() );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetPathToNextDestination
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_LogisticFunctions::ConvoyGetPathToNextDestination( const MIL_Agent_ABC& callerAgent )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    const PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
    {
        const T_PointVector* wayPoints = role->ConvoyGetPathToNextDestination();
        if( wayPoints )
        {
            BOOST_FOREACH( const MT_Vector2D& wayPoint, *wayPoints )
                result.push_back( boost::make_shared< MT_Vector2D >( wayPoint ) );
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyIsPushedFlow
// Created: LDC 2013-02-04
// -----------------------------------------------------------------------------
bool DEC_LogisticFunctions::ConvoyIsPushedFlow( const MIL_Agent_ABC& callerAgent )
{
    const PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        return role->ConvoyIsPushedFlow();
    return false;
}

namespace
{
    template< typename Pred >
    void UndoLendComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack, Pred funcPredicate )
    {
        if( !pTarget )
            throw MASA_EXCEPTION( "invalid parameter." );
        const unsigned int nNbrGotBack   = callerAgent.GetRole< PHY_RolePion_Composantes >().RetrieveLentComposantes( pTarget->GetPion(), nNbrToGetBack, funcPredicate );
        if( nNbrGotBack == 0 )
            MIL_Report::PostEvent( callerAgent, report::eRC_RecuperationMaterielPreteImpossible );
        else
        {
            MIL_Report::PostEvent( pTarget->GetPion(), report::eRC_MaterielRendu );
            if( nNbrGotBack < nNbrToGetBack )
                MIL_Report::PostEvent( callerAgent, report::eRC_RecuperationMaterielPretePartiellementEffectuee );
            else
                MIL_Report::PostEvent( callerAgent, report::eRC_RecuperationMaterielPreteEffectuee );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::UndoLendCollectionComposantes
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::UndoLendCollectionComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack )
{
    UndoLendComposantes( callerAgent, pTarget, nNbrToGetBack, boost::bind( &PHY_ComposantePion::CanCollectCasualties, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::UndoLendHaulerComposantes
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::UndoLendHaulerComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack )
{
    UndoLendComposantes( callerAgent, pTarget, nNbrToGetBack, boost::bind( &PHY_ComposantePion::CanHaul, _1 ) );
}

namespace
{
    bool CheckComposante( const PHY_ComposantePion& composante, PHY_ComposanteTypePion* type )
    {
        return &composante.GetType() == type;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::UndoLendSpecificComposantes
// Created: JSR 2012-08-06
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::UndoLendSpecificComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, PHY_ComposanteTypePion* type, const unsigned int nNbrToGetBack )
{
    UndoLendComposantes( callerAgent, pTarget, nNbrToGetBack, boost::bind( &CheckComposante, _1, type ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionGetTC2
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_LogisticFunctions::PionGetPcTC2( const MIL_Agent_ABC& agent )
{
    if( MIL_AutomateLOG* pTC2 = agent.GetLogisticHierarchy().GetPrimarySuperior() )
    {
        const MIL_AgentPion* pc = pTC2->GetPC();
        if( pc )
            return const_cast< DEC_Decision_ABC* >( &pc->GetDecision() );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateGetTC2
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_LogisticFunctions::AutomateGetPcTC2( const MIL_Automate& agent )
{
    if( MIL_AutomateLOG* pTC2 = agent.GetLogisticHierarchy().GetPrimarySuperior() )
    {
        const MIL_AgentPion* pc = pTC2->GetPC();
        if( pc )
            return const_cast< DEC_Decision_ABC* >( &pc->GetDecision() );
    }
    return 0;
}
