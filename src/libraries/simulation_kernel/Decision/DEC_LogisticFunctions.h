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

#include "MT_Tools/MT_Vector2DTypes.h"

class DEC_Decision_ABC;
class MIL_Agent_ABC;
class MIL_Automate;
class PHY_ComposanteTypePion;
class PHY_ComposantePion;
class PHY_HumanWound;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_LogisticFunctions
{
    typedef bool (PHY_ComposantePion::*T_ComposantePredicate)() const;

public:
    //! @name Maintenance
    //@{
    static void PionMaintenanceEnableSystem( MIL_Agent_ABC& callerAgent );
    static void PionMaintenanceDisableSystem( MIL_Agent_ABC& callerAgent );
    static void PionMaintenanceChangePriorities( MIL_Agent_ABC& callerAgent, const std::vector< const PHY_ComposanteTypePion* >& priorities );
    static void PionMaintenanceChangeTacticalPriorities( MIL_Agent_ABC& callerAgent, const std::vector< const DEC_Decision_ABC* >& priorities );
    static void PionMaintenanceChangeWorkRate( MIL_Agent_ABC& callerAgent, int workRate );

    static void AutomateMaintenanceEnableSystem( MIL_Automate& callerAutomate );
    static void AutomateMaintenanceDisableSystem( MIL_Automate& callerAutomate );
    static void AutomateMaintenanceChangePriorities( MIL_Automate& callerAutomate, const std::vector< const PHY_ComposanteTypePion* >& priorities );
    static void AutomateMaintenanceChangeTacticalPriorities( MIL_Automate& callerAutomate, const std::vector< const DEC_Decision_ABC* >& priorities );
    static void AutomateMaintenanceChangeWorkRate( MIL_Automate& callerAutomate, int workRate );
    //@}

    //! @name Medical
    //@{
    static void PionMedicalEnableSystem( MIL_Agent_ABC& callerAgent );
    static void PionMedicalDisableSystem( MIL_Agent_ABC& callerAgent );
    static void PionMedicalEnableSortingFunction( MIL_Agent_ABC& callerAgent );
    static void PionMedicalDisableSortingFunction( MIL_Agent_ABC& callerAgent );
    static void PionMedicalEnableHealingFunction( MIL_Agent_ABC& callerAgent );
    static void PionMedicalDisableHealingFunction( MIL_Agent_ABC& callerAgent );
    static void PionMedicalChangePriorities( MIL_Agent_ABC& callerAgent, const std::vector< const PHY_HumanWound* >& priorities );
    static void PionMedicalChangeTacticalPriorities( MIL_Agent_ABC& callerAgent, const std::vector< const DEC_Decision_ABC* >& priorities );

    static void AutomateMedicalEnableSystem( MIL_Automate& callerAutomate );
    static void AutomateMedicalDisableSystem( MIL_Automate& callerAutomate );
    static void AutomateMedicalEnableSortingFunction ( MIL_Automate& callerAutomate );
    static void AutomateMedicalDisableSortingFunction( MIL_Automate& callerAutomate );
    static void AutomateMedicalEnableHealingFunction( MIL_Automate& callerAutomate );
    static void AutomateMedicalDisableHealingFunction( MIL_Automate& callerAutomate );
    static void AutomateMedicalChangePriorities( MIL_Automate& callerAutomate, const std::vector< const PHY_HumanWound* >& priorities );
    static void AutomateMedicalChangeTacticalPriorities( MIL_Automate& callerAutomate, const std::vector< const DEC_Decision_ABC* >& priorities );

    static void EvacuateWoundedHumansToTC2( DEC_Decision_ABC* pPionWounded, DEC_Decision_ABC* pDecTC2 );
    static bool HasWoundedHumansToEvacuate( DEC_Decision_ABC* pPion );
    static void ForbidWoundedHumansAutoEvacuation( DEC_Decision_ABC* pPion );
    static void AllowWoundedHumansAutoEvacuation( DEC_Decision_ABC* pPion );
    //@}

    //! @name Supply
    //@{
    static void PionSupplyEnableSystem( MIL_Agent_ABC& callerAgent );
    static void PionSupplyDisableSystem( MIL_Agent_ABC& callerAgent );
    static void AutomateSupplyEnableSystem( MIL_Automate& callerAutomate );
    static void AutomateSupplyDisableSystem( MIL_Automate& callerAutomate );

    static void ConvoyNotifyMovedToSupplier( MIL_Agent_ABC& callerAgent );
    static void ConvoyNotifyMovedToTransportersProvider( MIL_Agent_ABC& callerAgent );
    static void ConvoyNotifyMovedToSupplyRecipient( MIL_Agent_ABC& callerAgent );
    static void ConvoyEndMission( MIL_Agent_ABC& callerAgent );
    static int  ConvoyGetCurrentAction( const MIL_Agent_ABC& callerAgent );
    static DEC_Decision_ABC* ConvoyGetCurrentSupplyRecipient( const MIL_Agent_ABC& callerAgent );
    static DEC_Decision_ABC* ConvoyGetTransportersProvider  ( const MIL_Agent_ABC& callerAgent );
    static DEC_Decision_ABC* ConvoyGetSupplier              ( const MIL_Agent_ABC& callerAgent );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ConvoyGetPathToNextDestination( const MIL_Agent_ABC& callerAgent );

    static void AutomateRequestSupply( MIL_Automate&  callerAutomate );
    static void PionRequestSupply( MIL_Agent_ABC& callerAgent );

    static void ChangeDotationsValueUsingTC2( MIL_Agent_ABC& callerAgent, int dotationType, const double rCapacityFactor, int ammoDotationClassId );
    //@}

    //! @name Misc
    //@{
    static DEC_Decision_ABC* PionGetPcTC2( const MIL_Agent_ABC& );
    static DEC_Decision_ABC* AutomateGetPcTC2( const MIL_Automate& );

    static void UndoLendCollectionComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack );
    static void UndoLendHaulerComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack );
    //@}

private:
    //! @name Helpers
    //@{
    static void DEC_LogisticFunctions::UndoLendComposantes( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pTarget, const unsigned int nNbrToGetBack, T_ComposantePredicate funcPredicate );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const MIL_Automate* > T_AutomateVector;
    //@}
};

#endif // __DEC_LogisticFunctions_h_