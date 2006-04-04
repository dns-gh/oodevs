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

#include "MIL_pch.h"

#include "DEC_LogisticFunctions.h"

#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePion_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePion_Medical.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Humans/PHY_RolePion_Humans.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/RC/MIL_RC.h"
#include "Decision/DEC_Tools.h"

// =============================================================================
// MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceEnableSystem( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Maintenance >().EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceDisableSystem( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Maintenance >().DisableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceChangePriorities( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeMaintenancePriorites( call.GetParameter( 0 ) ) );

    T_MaintenancePriorityVector* pData = call.GetParameter( 0 ).ToUserPtr( pData );
    assert( pData );
    callerAgent.GetRole< PHY_RolePion_Maintenance >().ChangePriorities( *pData );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeListeAutomates( call.GetParameter( 0 ) ) );

    T_ObjectVector data = call.GetParameter( 0 ).ToSelection();

    T_AutomateVector automates;
    automates.reserve( data.size() );
    for( CIT_ObjectVector it = data.begin(); it != data.end(); ++it )
        automates.push_back( &static_cast< DEC_AutomateDecision& >( **it ).GetAutomate() );

    callerAgent.GetRole< PHY_RolePion_Maintenance >().ChangePriorities( automates );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMaintenanceChangeWorkRate
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMaintenanceChangeWorkRate( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    const PHY_MaintenanceWorkRate* pWorkRate = PHY_MaintenanceWorkRate::Find( (ASN1T_EnumLogMaintenanceRegimeTravail)call.GetParameter( 0 ).ToId() );
    if( !pWorkRate )
        assert( false );
    else
        callerAgent.GetRole< PHY_RolePion_Maintenance >().ChangeWorkRate( *pWorkRate );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceEnableSystem( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    PionMaintenanceEnableSystem( call, callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceDisableSystem( DIA_Call_ABC& call, MIL_Automate&  callerAutomate )
{
    PionMaintenanceDisableSystem( call, callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangePriorities( DIA_Call_ABC& call, MIL_Automate&  callerAutomate )
{
    PionMaintenanceChangePriorities( call, callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities( DIA_Call_ABC& call, MIL_Automate&  callerAutomate )
{
    PionMaintenanceChangeTacticalPriorities( call, callerAutomate.GetPionPC() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate( DIA_Call_ABC& call, MIL_Automate&  callerAutomate )
{
    PionMaintenanceChangeWorkRate( call, callerAutomate.GetPionPC() );
}

// =============================================================================
// MEDICAL
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::EvacuateWoundedHumansToTC2
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::EvacuateWoundedHumansToTC2( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePion    ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeAutomate( call.GetParameter( 1 ) ) );

    DEC_RolePion_Decision* pPionWounded = call.GetParameter( 0 ).ToUserObject( pPionWounded );
    assert( pPionWounded );   

    DEC_AutomateDecision* pDecTC2 = call.GetParameter( 1 ).ToUserObject( pDecTC2 );
    assert( pDecTC2 );
    if( !pDecTC2->GetAutomate().GetType().IsLogistic() )
        return;

    MIL_AutomateLOG& tc2 = static_cast< MIL_AutomateLOG& >( pDecTC2->GetAutomate() );
    pPionWounded->GetPion().GetRole< PHY_RolePion_Humans >().EvacuateWoundedHumans( tc2 );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::HasWoundedHumansToEvacuate( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );
    call.GetResult().SetValue( pPion->GetPion().GetRole< PHY_RolePion_Humans >().HasWoundedHumansToEvacuate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );
    pPion->GetPion().GetRole< PHY_RolePion_Humans >().ChangeEvacuationMode( PHY_RolePion_Humans::eEvacuationMode_Manual );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );
    
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );
    pPion->GetPion().GetRole< PHY_RolePion_Humans >().ChangeEvacuationMode( PHY_RolePion_Humans::eEvacuationMode_Auto );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalEnableSystem( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Medical >().EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalDisableSystem( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Medical >().DisableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalChangePriorities( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeSantePriorites( call.GetParameter( 0 ) ) );

    T_MedicalPriorityVector* pData = call.GetParameter( 0 ).ToUserPtr( pData );
    assert( pData );
    callerAgent.GetRole< PHY_RolePion_Medical >().ChangePriorities( *pData );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeListeAutomates( call.GetParameter( 0 ) ) );

    T_ObjectVector data = call.GetParameter( 0 ).ToSelection();

    T_AutomateVector automates;
    automates.reserve( data.size() );
    for( CIT_ObjectVector it = data.begin(); it != data.end(); ++it )
        automates.push_back( &static_cast< DEC_AutomateDecision& >( **it ).GetAutomate() );

    callerAgent.GetRole< PHY_RolePion_Medical >().ChangePriorities( automates );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalEnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalEnableSystem( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    PionMedicalEnableSystem( call, callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalDisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalDisableSystem( DIA_Call_ABC& call, MIL_Automate&  callerAutomate )
{
    PionMedicalDisableSystem( call, callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalChangePriorities( DIA_Call_ABC& call, MIL_Automate&  callerAutomate )
{
    PionMedicalChangePriorities( call, callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities( DIA_Call_ABC& call, MIL_Automate&  callerAutomate )
{
    PionMedicalChangeTacticalPriorities( call, callerAutomate.GetPionPC() );
}
    
// =============================================================================
// SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ChangeDotationValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ChangeDotationValueUsingTC2( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    const PHY_DotationType*      pDotationType      = PHY_DotationType::FindDotationType( call.GetParameter( 0 ).ToId() );
    const MT_Float               rCapacityFactor    = call.GetParameter( 1 ).ToFloat();
    const PHY_AmmoDotationClass* pAmmoDotationClass = 0;
    
    assert( pDotationType );
    if( call.GetParameters().GetParameters().size() > 2 )
        pAmmoDotationClass = PHY_AmmoDotationClass::Find( call.GetParameter( 2 ).ToId() );
    
    callerAgent.GetRole< PHY_RolePion_Dotations >().ChangeDotationsValueUsingTC2( *pDotationType, pAmmoDotationClass, rCapacityFactor );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionSupplyEnableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionSupplyEnableSystem( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Supply >().EnableSystem();
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionSupplyDisableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionSupplyDisableSystem( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Supply >().DisableSystem();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateSupplyEnableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateSupplyEnableSystem( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    PionSupplyEnableSystem( call, callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateSupplyDisableSystem
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateSupplyDisableSystem( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    PionSupplyDisableSystem( call, callerAutomate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateRequestSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateRequestSupply( DIA_Call_ABC& /*call*/, MIL_Automate& callerAutomate )
{
    callerAutomate.RequestDotationSupply();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionRequestSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionRequestSupply( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    AutomateRequestSupply( call, callerAgent.GetAutomate() );
}

// =============================================================================
// SUPPLY - CONVOY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyIsLoadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyIsLoadingDone( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_Supply >().ConvoyIsLoadingDone() );   
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyIsUnloadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyIsUnloadingDone( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( callerAgent.GetRole< PHY_RolePion_Supply >().ConvoyIsUnloadingDone() );   
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetFormingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyGetFormingPoint( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (void*)callerAgent.GetRole< PHY_RolePion_Supply >().ConvoyGetFormingPoint(), &DEC_Tools::GetTypePoint(), 1 );   
}
    
// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetLoadingPoint

// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyGetLoadingPoint( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (void*)callerAgent.GetRole< PHY_RolePion_Supply >().ConvoyGetLoadingPoint(), &DEC_Tools::GetTypePoint(), 1 );   
}
    
// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyGetUnloadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyGetUnloadingPoint( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( (void*)callerAgent.GetRole< PHY_RolePion_Supply >().ConvoyGetUnloadingPoint(), &DEC_Tools::GetTypePoint(), 1 );   
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::ConvoyEndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::ConvoyEndMission( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Supply >().ConvoyEndMission();
}

// =============================================================================
// MISC
// =============================================================================

namespace
{
    typedef bool (PHY_ComposantePion::*T_ComposantePredicate)() const;
}

// -----------------------------------------------------------------------------
// Name: UndoLendComposantes
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
static void UndoLendComposantes( DIA_Call_ABC& call, MIL_AgentPion& callerAgent, T_ComposantePredicate funcPredicate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pTarget = call.GetParameter( 0 ).ToUserObject( pTarget );
    assert( pTarget );
    
    const uint nNbrToGetBack = (uint)call.GetParameter( 1 ).ToFloat();
    const uint nNbrGotBack   = callerAgent.GetRole< PHY_RolePion_Composantes >().UndoLendComposantes( pTarget->GetPion().GetRole< PHY_RolePion_Composantes>(), nNbrToGetBack, std::mem_fun_ref( funcPredicate ) );

    if( nNbrGotBack == 0 )
        MIL_RC::pRcRecuperationMaterielPreteImpossible_->Send( callerAgent, MIL_RC::eRcTypeOperational );
    else
    {
        MIL_RC::pRcMaterielRendu_->Send( pTarget->GetPion(), MIL_RC::eRcTypeOperational );
        if( nNbrGotBack < nNbrToGetBack )
            MIL_RC::pRcRecuperationMaterielPretePartiellementEffectuee_->Send( callerAgent, MIL_RC::eRcTypeOperational );
        else
            MIL_RC::pRcRecuperationMaterielPreteEffectuee_->Send( callerAgent, MIL_RC::eRcTypeOperational );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::UndoLendCollectionComposantes
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::UndoLendCollectionComposantes( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    UndoLendComposantes( call, callerAgent, &PHY_ComposantePion::CanCollectCasualties );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::UndoLendHaulerComposantes
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::UndoLendHaulerComposantes( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    UndoLendComposantes( call, callerAgent, &PHY_ComposantePion::CanHaul );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::PionGetTC2
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::PionGetTC2( DIA_Call_ABC& call, const MIL_AgentPion& agent )
{
    MIL_AutomateLOG* pTC2 = agent.GetAutomate().GetTC2();    
    call.GetResult().SetValue( pTC2 ? pTC2->GetDecision() : *(DEC_AutomateDecision*)( 0 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_LogisticFunctions::AutomateGetTC2
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void DEC_LogisticFunctions::AutomateGetTC2( DIA_Call_ABC& call, const MIL_Automate& agent )
{
    MIL_AutomateLOG* pTC2 = agent.GetTC2();    
    call.GetResult().SetValue( pTC2 ? pTC2->GetDecision() : *(DEC_AutomateDecision*)( 0 ) );
}

