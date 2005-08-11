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

class MIL_AgentPion;
class MIL_Automate;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_LogisticFunctions
{
public:
    //! @name Maintenance
    //@{
    static void PionMaintenanceEnableSystem            ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionMaintenanceDisableSystem           ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionMaintenanceChangePriorities        ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionMaintenanceChangeTacticalPriorities( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionMaintenanceChangeWorkTime          ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );

    static void AutomateMaintenanceEnableSystem            ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateMaintenanceDisableSystem           ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateMaintenanceChangePriorities        ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateMaintenanceChangeTacticalPriorities( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateMaintenanceChangeWorkTime          ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    //@}

    //! @name Medical
    //@{
    static void PionMedicalEnableSystem            ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionMedicalDisableSystem           ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionMedicalChangePriorities        ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionMedicalChangeTacticalPriorities( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionMedicalChangeWorkTime          ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );

    static void AutomateMedicalEnableSystem            ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateMedicalDisableSystem           ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateMedicalChangePriorities        ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateMedicalChangeTacticalPriorities( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateMedicalChangeWorkTime          ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    
    static void UndoLendCollectionComposantes( DIA_Call_ABC& call, MIL_AgentPion& callerAgent ); 

    static void EvacuateWoundedHumansToTC2( DIA_Call_ABC& call );
    static void HasWoundedHumansToEvacuate( DIA_Call_ABC& call );
    //@}

    //! @name Supply
    //@{
    static void PionSupplyEnableSystem     ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void PionSupplyDisableSystem    ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void AutomateSupplyEnableSystem ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void AutomateSupplyDisableSystem( DIA_Call_ABC& call, MIL_Automate& callerAutomate );

    static void ConvoyIsSupplyDone         ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void ConvoyGetSuppliedAutomate  ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void ConvoyEndMission           ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );

    static void AutomateRequestSupply      ( DIA_Call_ABC& call, MIL_Automate&  callerAutomate );
    static void PionRequestSupply          ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );

    static void ChangeDotationValueUsingTC2( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    //@}
    
    //! @name Accessors
    //@{
    static void PionGetTC2    ( DIA_Call_ABC& call, const MIL_AgentPion& );
    static void AutomateGetTC2( DIA_Call_ABC& call, const MIL_Automate& );
    //@}
};

#include "DEC_LogisticFunctions.inl"

#endif // __DEC_LogisticFunctions_h_