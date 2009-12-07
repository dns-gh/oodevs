//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_LogisticFunctions.h $
// $Author: Jvt $
// $Modtime: 12/05/05 14:34 $
// $Revision: 10 $
// $Workfile: DEC_LogisticFunctions.h $
//
//*****************************************************************************

#ifndef __DEC_LogisticFunctions_h_
#define __DEC_LogisticFunctions_h_

#include "MIL.h"

class DEC_Decision_ABC;
class DEC_RolePion_Decision;
class DEC_AutomateDecision;
class MIL_AgentPion;
class MIL_Automate;
class PHY_ComposanteTypePion;
class PHY_ComposantePion;
class PHY_HumanWound;

namespace
{
    typedef bool (PHY_ComposantePion::*T_ComposantePredicate)() const;
}


// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_LogisticFunctions
{
public:
    //! @name Maintenance
    //@{
    static void PionMaintenanceEnableSystem            ( MIL_AgentPion& callerAgent );
    static void PionMaintenanceDisableSystem           ( MIL_AgentPion& callerAgent );
    static void PionMaintenanceChangePriorities        ( MIL_AgentPion& callerAgent, const std::vector< const PHY_ComposanteTypePion* >& priorities );
    static void PionMaintenanceChangeTacticalPriorities( MIL_AgentPion& callerAgent, const std::vector< const DEC_Decision_ABC* >& priorities );
    static void PionMaintenanceChangeWorkRate          ( MIL_AgentPion& callerAgent, int workRate );

    static void AutomateMaintenanceEnableSystem            ( MIL_Automate& callerAutomate );
    static void AutomateMaintenanceDisableSystem           ( MIL_Automate& callerAutomate );
    static void AutomateMaintenanceChangePriorities        ( MIL_Automate& callerAutomate, const std::vector< const PHY_ComposanteTypePion* >& priorities );
    static void AutomateMaintenanceChangeTacticalPriorities( MIL_Automate& callerAutomate, const std::vector< const DEC_Decision_ABC* >& priorities );
    static void AutomateMaintenanceChangeWorkRate          ( MIL_Automate& callerAutomate, int workRate );
    //@}

    //! @name Medical
    //@{
    static void PionMedicalEnableSystem            ( MIL_AgentPion& callerAgent );
    static void PionMedicalDisableSystem           ( MIL_AgentPion& callerAgent );
    static void PionMedicalEnableSortingFunction   ( MIL_AgentPion& callerAgent );
    static void PionMedicalDisableSortingFunction  ( MIL_AgentPion& callerAgent );
    static void PionMedicalEnableHealingFunction   ( MIL_AgentPion& callerAgent );
    static void PionMedicalDisableHealingFunction  ( MIL_AgentPion& callerAgent );
    static void PionMedicalChangePriorities        ( MIL_AgentPion& callerAgent, const std::vector< const PHY_HumanWound* >& priorities );
    static void PionMedicalChangeTacticalPriorities( MIL_AgentPion& callerAgent, const std::vector< const DEC_Decision_ABC* >& priorities );

    static void AutomateMedicalEnableSystem            ( MIL_Automate& callerAutomate );
    static void AutomateMedicalDisableSystem           ( MIL_Automate& callerAutomate );
    static void AutomateMedicalEnableSortingFunction   ( MIL_Automate& callerAutomate );
    static void AutomateMedicalDisableSortingFunction  ( MIL_Automate& callerAutomate );
    static void AutomateMedicalEnableHealingFunction   ( MIL_Automate& callerAutomate );
    static void AutomateMedicalDisableHealingFunction  ( MIL_Automate& callerAutomate );
    static void AutomateMedicalChangePriorities        ( MIL_Automate& callerAutomate, const std::vector< const PHY_HumanWound* >& priorities );
    static void AutomateMedicalChangeTacticalPriorities( MIL_Automate& callerAutomate, const std::vector< const DEC_Decision_ABC* >& priorities );

    static void EvacuateWoundedHumansToTC2       ( DEC_RolePion_Decision* pPionWounded, DEC_AutomateDecision* pDecTC2 );
    static bool HasWoundedHumansToEvacuate       ( DEC_RolePion_Decision* pPion );
    static void ForbidWoundedHumansAutoEvacuation( DEC_RolePion_Decision* pPion );
    static void AllowWoundedHumansAutoEvacuation ( DEC_RolePion_Decision* pPion ); 
    //@}

    //! @name Supply
    //@{
    static void PionSupplyEnableSystem     ( MIL_AgentPion& callerAgent );
    static void PionSupplyDisableSystem    ( MIL_AgentPion& callerAgent );
    static void AutomateSupplyEnableSystem ( MIL_Automate& callerAutomate );
    static void AutomateSupplyDisableSystem( MIL_Automate& callerAutomate );
    
    static bool ConvoyIsLoadingDone        ( const MIL_AgentPion& callerAgent );
    static bool ConvoyIsUnloadingDone      ( const MIL_AgentPion& callerAgent );
    static boost::shared_ptr< DEC_Decision_ABC > ConvoyGetSupplyingAutomate( const MIL_AgentPion& callerAgent );
    static boost::shared_ptr< DEC_Decision_ABC > ConvoyGetConvoyingAutomate( const MIL_AgentPion& callerAgent );  
    static boost::shared_ptr< DEC_Decision_ABC > ConvoyGetSuppliedAutomate ( const MIL_AgentPion& callerAgent );
    static void ConvoyEndMission           ( MIL_AgentPion& callerAgent );

    static void AutomateRequestSupply      ( MIL_Automate&  callerAutomate );
    static void PionRequestSupply          ( MIL_AgentPion& callerAgent );

    static void ChangeDotationsValueUsingTC2( MIL_AgentPion& callerAgent, int dotationType, const MT_Float rCapacityFactor, int ammoDotationClassId );
    //@}
    
    //! @name Misc
    //@{
    static DEC_Decision_ABC*  PionGetTC2    ( const MIL_AgentPion& );
    static DEC_Decision_ABC* AutomateGetTC2( const MIL_Automate& );

    static void UndoLendCollectionComposantes( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pTarget, const uint nNbrToGetBack ); 
    static void UndoLendHaulerComposantes    ( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pTarget, const uint nNbrToGetBack ); 
    //@}

    //! @name Helpers
    //@{
private:
    static void DEC_LogisticFunctions::UndoLendComposantes( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pTarget, const uint nNbrToGetBack, T_ComposantePredicate funcPredicate );
    //@}
};

#include "DEC_LogisticFunctions.inl"

#endif // __DEC_LogisticFunctions_h_