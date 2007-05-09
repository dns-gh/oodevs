//*****************************************************************************
//
// $Created: FBD 02-12-16 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Scipio_enum.h $
// $Author: Nld $
// $Modtime: 17/01/05 16:26 $
// $Revision: 29 $
// $Workfile: MT_Scipio_enum.h $
//
//*****************************************************************************

#ifndef __MT_Scipio_enum_h_
#define __MT_Scipio_enum_h_

//$$$$ DEGUEU - A DEPLACER

enum E_FireAvailability
{
    eFireAvailabilityNone,
    eFireAvailabilityReady,
    eFireAvailabilityNotReady
};
enum E_ForceRatioState
{
    eForceRatioStateNone,
    eForceRatioStateNeutral,
    eForceRatioStateFavorable,
    eForceRatioStateUnfavorable,
};
enum E_RulesOfEngagementState 
{
    eRoeStateNone,
    eRoeStateFreeFire,
    eRoeStateRestrictedFire,
    eRoeStateFireByOrder
};
enum E_CloseCombatState 
{
    eCloseCombatStateNone,
    eCloseCombatStateDodge,
    eCloseCombatStateFixed,
    eCloseCombatStateProceedMission
};

enum E_OperationalState
{
    eOpStateFullyDestroyed,
    eOpStateTacticallyDestroyed,
    eOpStateOperational,
};

#endif // __MT_Scipio_enum_h_