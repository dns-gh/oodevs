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
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include <boost/foreach.hpp>

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
        automates.push_back( &(*it)->GetAutomate() );
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
    PionMaintenanceEnableSystem( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceDisableSystem( MIL_Automate& callerAutomate )
{
    PionMaintenanceDisableSystem( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangePriorities( MIL_Automate& callerAutomate, const std::vector< const PHY_ComposanteTypePion* >& priorities )
{
    PionMaintenanceChangePriorities( callerAutomate.GetPionPC(), priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities( MIL_Automate& callerAutomate, const std::vector< const DEC_Decision_ABC* >& priorities )
{
    PionMaintenanceChangeTacticalPriorities( callerAutomate.GetPionPC(), priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate( MIL_Automate& callerAutomate, int workRate )
{
    PionMaintenanceChangeWorkRate( callerAutomate.GetPionPC(), workRate );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::EvacuateWoundedHumansToTC2
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::EvacuateWoundedHumansToTC2( DEC_Decision_ABC* pPionWounded, DEC_Decision_ABC* pDecTC2 )
{
    assert( pPionWounded );
    assert( pDecTC2 );
    if( !pDecTC2->GetAutomate().GetType().IsLogistic() )
        return;
    MIL_AutomateLOG& tc2 = *( pDecTC2->GetAutomate().GetBrainLogistic() );
    pPionWounded->GetPion().GetRole< PHY_RoleInterface_Humans >().EvacuateWoundedHumans( tc2 );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool DEC_LogisticFunctions::HasWoundedHumansToEvacuate( DEC_Decision_ABC* pPion )
{
    assert( pPion );
    return pPion->GetPion().GetRole< PHY_RoleInterface_Humans >().HasWoundedHumansToEvacuate();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation( DEC_Decision_ABC* pPion )
{
    assert( pPion );
    pPion->GetPion().GetRole< PHY_RoleInterface_Humans >().ChangeEvacuationMode( PHY_RoleInterface_Humans::eEvacuationMode_Manual );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation( DEC_Decision_ABC* pPion )
{
    assert( pPion );
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
        automates.push_back( &(*it)->GetAutomate() );
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
    PionMedicalEnableSystem( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalDisableSystem( MIL_Automate& callerAutomate )
{
    PionMedicalDisableSystem( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalEnableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalEnableSortingFunction( MIL_Automate& callerAutomate )
{
    PionMedicalEnableSortingFunction( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalDisableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalDisableSortingFunction( MIL_Automate& callerAutomate )
{
    PionMedicalDisableSortingFunction( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalEnableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalEnableHealingFunction( MIL_Automate& callerAutomate )
{
    PionMedicalEnableHealingFunction( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalDisableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalDisableHealingFunction( MIL_Automate& callerAutomate )
{
    PionMedicalDisableHealingFunction( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalChangePriorities( MIL_Automate& callerAutomate, const std::vector< const PHY_HumanWound* >& priorities )
{
    PionMedicalChangePriorities( callerAutomate.GetPionPC(), priorities );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities( MIL_Automate& callerAutomate, const std::vector< const DEC_Decision_ABC* >& priorities )
{
    PionMedicalChangeTacticalPriorities( callerAutomate.GetPionPC(), priorities );
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
    PionSupplyEnableSystem( callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateSupplyDisableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateSupplyDisableSystem( MIL_Automate& callerAutomate )
{
    PionSupplyDisableSystem( callerAutomate.GetPionPC() );
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
            return ( const_cast< DEC_Decision_ABC* >( &recipient->GetPC().GetDecision() ) );
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
        logistic::SupplySupplier_ABC* recipient = role->ConvoyGetSupplier();
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
                result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( wayPoint ) ) );
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: UndoLendComposantes
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::UndoLendComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack, T_ComposantePredicate funcPredicate )
{
    assert( pTarget );
    const unsigned int nNbrGotBack   = callerAgent.GetRole< PHY_RolePion_Composantes >().RetrieveLentComposantes( pTarget->GetPion(), nNbrToGetBack, std::mem_fun_ref( funcPredicate ) );
    if( nNbrGotBack == 0 )
        MIL_Report::PostEvent( callerAgent, MIL_Report::eReport_EquipmentLoanRetrievingImpossible );
    else
    {
        MIL_Report::PostEvent( pTarget->GetPion(), MIL_Report::eReport_EquipmentLoanGivenBack );
        if( nNbrGotBack < nNbrToGetBack )
            MIL_Report::PostEvent( callerAgent, MIL_Report::eReport_EquipmentLoanRetrievingPartiallyDone );
        else
            MIL_Report::PostEvent( callerAgent, MIL_Report::eReport_EquipmentLoanRetrievingDone );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::UndoLendCollectionComposantes
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::UndoLendCollectionComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack )
{
    UndoLendComposantes( callerAgent, pTarget, nNbrToGetBack, &PHY_ComposantePion::CanCollectCasualties );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::UndoLendHaulerComposantes
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::UndoLendHaulerComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack )
{
    UndoLendComposantes( callerAgent, pTarget, nNbrToGetBack, &PHY_ComposantePion::CanHaul );
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
