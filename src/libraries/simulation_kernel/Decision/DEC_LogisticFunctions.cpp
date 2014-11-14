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

namespace
{

typedef std::vector< const MIL_Automate* > T_AutomateVector;

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PionMaintenanceEnableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >();//@TODO Maybe use Get and assert this DEC functions are just register on good PionLog
    if( role )
        role->EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PionMaintenanceDisableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >();
    if( role )
        role->DisableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PionMaintenanceChangePriorities( MIL_Agent_ABC& callerAgent, const std::vector< const PHY_ComposanteTypePion* >& priorities )
{
    PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >();
    if( role )
        role->ChangePriorities( priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PionMaintenanceChangeTacticalPriorities( MIL_Agent_ABC& callerAgent, const std::vector< const DEC_Decision_ABC* >& priorities )
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
void PionMaintenanceChangeWorkRate( MIL_Agent_ABC& callerAgent, int workRate )
{
    if( const PHY_MaintenanceWorkRate* pWorkRate = PHY_MaintenanceWorkRate::Find( sword::EnumLogMaintenanceWorkRate( workRate ) ) )
        if( PHY_RoleInterface_Maintenance* role = callerAgent.RetrieveRole< PHY_RoleInterface_Maintenance >() )
            role->ChangeWorkRate( *pWorkRate );
}

void ApplyOnCommandPost( DEC_Decision_ABC* caller, std::function< void( MIL_Agent_ABC& ) > f )
{
    switch( caller->GetKind() )
    {
        case DEC_Decision_ABC::ePion:
            f( caller->GetPion() );
            break;
        case DEC_Decision_ABC::eAutomate:
            if( MIL_AgentPion* pionPc = caller->GetAutomate().GetPionPC() )
                f( *pionPc );
            break;
        default:
            throw MASA_EXCEPTION( "ApplyOnCommandPost: cannot be called for this agent type" );
    }
}

}

void DEC_LogisticFunctions::MaintenanceEnableSystem( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionMaintenanceEnableSystem( pionPC ); } );
}

void DEC_LogisticFunctions::MaintenanceDisableSystem( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionMaintenanceDisableSystem( pionPC ); } );
}

void DEC_LogisticFunctions::MaintenanceChangePriorities( DEC_Decision_ABC* caller, const std::vector< const PHY_ComposanteTypePion* >& priorities )
{
    ApplyOnCommandPost( caller, [&]( MIL_Agent_ABC& pionPC ) { PionMaintenanceChangePriorities( pionPC, priorities ); } );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMaintenancePriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
std::vector< const PHY_ComposanteTypePion* > DEC_LogisticFunctions::GetAutomateMaintenancePriorities( const DEC_Decision_ABC* callerAutomate )
{
    std::vector< const PHY_ComposanteTypePion* > result;
    if( MIL_AgentPion* pionPc = callerAutomate->GetAutomate().GetPionPC() )
        if( PHY_RoleInterface_Maintenance* role = pionPc->RetrieveRole< PHY_RoleInterface_Maintenance >() )
            result = role->GetMaintenancePriorities();
    return result;
}

void DEC_LogisticFunctions::MaintenanceChangeTacticalPriorities( DEC_Decision_ABC* caller, const std::vector< const DEC_Decision_ABC* >& priorities )
{
    ApplyOnCommandPost( caller, [&]( MIL_Agent_ABC& pionPC ) { PionMaintenanceChangeTacticalPriorities( pionPC, priorities ); } );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMaintenanceTacticalPriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
std::vector< const DEC_Decision_ABC* > DEC_LogisticFunctions::GetAutomateMaintenanceTacticalPriorities( const DEC_Decision_ABC* callerAutomate )
{
    std::vector< const DEC_Decision_ABC* > result;
    if( MIL_AgentPion* pionPc = callerAutomate->GetAutomate().GetPionPC() )
        if( PHY_RoleInterface_Maintenance* role = pionPc->RetrieveRole< PHY_RoleInterface_Maintenance >() )
        {
            T_AutomateVector priorities = role->GetAutomatePriorities();
            for( T_AutomateVector::const_iterator it = priorities.begin(); it != priorities.end(); ++it )
                result.push_back( &(*it)->GetDecision() );
        }
    return result;
}

void DEC_LogisticFunctions::MaintenanceChangeWorkRate( DEC_Decision_ABC* caller, int workRate )
{
    ApplyOnCommandPost( caller, [=]( MIL_Agent_ABC& pionPC ) { PionMaintenanceChangeWorkRate( pionPC, workRate ); } );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMaintenanceWorkRate
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
int DEC_LogisticFunctions::GetAutomateMaintenanceWorkRate( const DEC_Decision_ABC* callerAutomate )
{
    if( MIL_AgentPion* pionPc = callerAutomate->GetAutomate().GetPionPC() )
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

namespace
{

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PionMedicalEnableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PionMedicalDisableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->DisableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalEnableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PionMedicalEnableSortingFunction( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->EnableSortingFunction();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalDisableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PionMedicalDisableSortingFunction( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->DisableSortingFunction();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalEnableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PionMedicalEnableHealingFunction( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->EnableHealingFunction();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalDisableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PionMedicalDisableHealingFunction( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->DisableHealingFunction();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PionMedicalChangePriorities( MIL_Agent_ABC& callerAgent, const std::vector< const PHY_HumanWound* >& priorities )
{
    PHY_RoleInterface_Medical* role = callerAgent.RetrieveRole< PHY_RoleInterface_Medical >();
    if( role )
        role->ChangePriorities( priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PionMedicalChangeTacticalPriorities( MIL_Agent_ABC& callerAgent, const std::vector< const DEC_Decision_ABC* >& priorities )
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

} 

void DEC_LogisticFunctions::MedicalEnableSystem( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionMedicalEnableSystem( pionPC ); } );
}

void DEC_LogisticFunctions::MedicalDisableSystem( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionMedicalDisableSystem( pionPC ); } );
}

void DEC_LogisticFunctions::MedicalEnableSortingFunction( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionMedicalEnableSortingFunction( pionPC ); } );
}

void DEC_LogisticFunctions::MedicalDisableSortingFunction( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionMedicalDisableSortingFunction( pionPC ); } );
}

void DEC_LogisticFunctions::MedicalEnableHealingFunction( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionMedicalEnableHealingFunction( pionPC ); } );
}

void DEC_LogisticFunctions::MedicalDisableHealingFunction( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionMedicalDisableHealingFunction( pionPC ); } );
}

void DEC_LogisticFunctions::MedicalChangePriorities( DEC_Decision_ABC* caller, const std::vector< const PHY_HumanWound* >& priorities )
{
    ApplyOnCommandPost( caller, [&]( MIL_Agent_ABC& pionPC ) { PionMedicalChangePriorities( pionPC, priorities ); } );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMedicalPriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
std::vector< const PHY_HumanWound* > DEC_LogisticFunctions::GetAutomateMedicalPriorities( const DEC_Decision_ABC* callerAutomate )
{
    std::vector< const PHY_HumanWound* > result;
    if( MIL_AgentPion* pionPc = callerAutomate->GetAutomate().GetPionPC() )
        if( PHY_RoleInterface_Medical* role = pionPc->RetrieveRole< PHY_RoleInterface_Medical >() )
            result = role->GetMedicalPriorities();
    return result;
}

void DEC_LogisticFunctions::MedicalChangeTacticalPriorities( DEC_Decision_ABC* caller, const std::vector< const DEC_Decision_ABC* >& priorities )
{
    ApplyOnCommandPost( caller, [&]( MIL_Agent_ABC& pionPC ) { PionMedicalChangeTacticalPriorities( pionPC, priorities ); } );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::GetAutomateMedicalTacticalPriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
std::vector< const DEC_Decision_ABC* > DEC_LogisticFunctions::GetAutomateMedicalTacticalPriorities( const DEC_Decision_ABC* callerAutomate )
{
    std::vector< const DEC_Decision_ABC* > result;
    if( MIL_AgentPion* pionPc = callerAutomate->GetAutomate().GetPionPC() )
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

namespace
{

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionSupplyEnableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void PionSupplyEnableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionSupplyDisableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void PionSupplyDisableSystem( MIL_Agent_ABC& callerAgent )
{
    PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        role->DisableSystem();
}

}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateSupplyEnableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::SupplyEnableSystem( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionSupplyEnableSystem( pionPC ); } );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateSupplyDisableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::SupplyDisableSystem( DEC_Decision_ABC* caller )
{
    ApplyOnCommandPost( caller, []( MIL_Agent_ABC& pionPC ) { PionSupplyDisableSystem( pionPC ); } );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateRequestSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateRequestSupply( DEC_Decision_ABC* callerAutomate )
{
    callerAutomate->GetAutomate().RequestDotationSupply();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionRequestSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionRequestSupply( MIL_Agent_ABC& callerAgent )
{
    AutomateRequestSupply( &callerAgent.GetAutomate().GetDecision() );
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
    if( auto role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >() )
        if( auto recipient = role->ConvoyGetCurrentSupplyRecipient() )
            if( auto pionPc = recipient->GetPC() )
                return ( const_cast< DEC_Decision_ABC* >( &pionPc->GetDecision() ) );
    return nullptr;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetTransportersProvider
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_LogisticFunctions::ConvoyGetTransportersProvider( const MIL_Agent_ABC& callerAgent )
{
    if( auto pc = callerAgent.GetAutomate().GetPionPC() )
        return const_cast< DEC_Decision_ABC* >( &pc->GetDecision() );
    return nullptr;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetSupplier
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_LogisticFunctions::ConvoyGetSupplier( const MIL_Agent_ABC& callerAgent )
{
    if( auto role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >() )
        if( const MIL_Agent_ABC* provider = role->ConvoyGetSupplier() )
            return const_cast< DEC_Decision_ABC* >( &provider->GetDecision() );
    return nullptr;
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetPathToNextDestination
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_LogisticFunctions::ConvoyGetPathToNextDestination( const MIL_Agent_ABC& callerAgent )
{
    const PHY_RoleInterface_Supply* role = callerAgent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        return role->ConvoyGetPathToNextDestination();
    return std::vector< boost::shared_ptr< MT_Vector2D > >();
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
DEC_Decision_ABC* DEC_LogisticFunctions::AutomateGetPcTC2( const DEC_Decision_ABC* agent )
{
    if( MIL_AutomateLOG* pTC2 = agent->GetAutomate().GetLogisticHierarchy().GetPrimarySuperior() )
    {
        const MIL_AgentPion* pc = pTC2->GetPC();
        if( pc )
            return const_cast< DEC_Decision_ABC* >( &pc->GetDecision() );
    }
    return 0;
}
