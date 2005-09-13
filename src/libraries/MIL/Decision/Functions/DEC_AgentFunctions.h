// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AgentFunctions.h $
// $Author: Jvt $
// $Modtime: 4/05/05 15:25 $
// $Revision: 28 $
// $Workfile: DEC_AgentFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_AgentFunctions_h_
#define __DEC_AgentFunctions_h_

#include "MIL.h"

class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_AgentFunctions
{
public:
    //! @name Functions
    //@{
    // Accessors
    static void IsPC                      ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsRefugee                 ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsMilita                  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsSurrendered             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsPrisoner                ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsAutomateEmbraye         ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsMoving                  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsTransported             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsNeutralized             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );            
    static void GetEtatOps                ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPosition               ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetDirection              ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void ActivateBlackout          ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DeactivateBlackout        ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void GetPosture                ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
        
    static void CanConstructObject        ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void CanBypassObject           ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void CanDestroyObject          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void CanMineObject             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void CanActivateObject         ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    
    static void EnableDiscreteMode        ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void DisableDiscreteMode       ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );

    // Embarquement / débarquement
    static void IsLoaded                          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );          
    static void GetLoadingTime                    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetUnloadingTime                  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetHumanTransportersRecoveringTime( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void AreHumanTransportersReady         ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void DisableHumanTransportersNow       ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void EnableHumanTransportersNow        ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );

    // Etat décisionnel
    static void NotifyForceRatioStateChanged       ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void NotifyRulesOfEngagementStateChanged( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void NotifyCloseCombatStateChanged      ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void NotifyOperationalStateChanged      ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );

    // Etat des facteurs humains
    static void GetHumanFactorTiredness ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetHumanFactorExperience( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetHumanFactorMorale    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetHumanFactorWeight    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

    // Etats du pion
    static void IsFlying           ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void SetFlyingHeight    ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void SetElongationFactor( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void TimeLeftForMoving  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void TimeToMoveDistance ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

    // hiérarchie
    static void GetPionsWithPC             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPionsWithoutPC          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPionPC                  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPionPCOfAutomate        ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPionsWithoutPCOfAutomate( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void ChangeAutomate             ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
       
    // Interception
    static void GetInterceptionPoint( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

    // NBC
    static void IsContaminated            ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsWearingNbcProtectionSuit( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void WearNbcProtectionSuit     ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void RemoveNbcProtectionSuit   ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void SelfDecontaminate         ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
   
    // Test / debug
    static void SetPosture     ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void Debug          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void Trace          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void DebugDrawPoints( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void DebugDrawPoint ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    
    static void RelievePion   ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void CanRelievePion( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    //@}
};

#endif // __DEC_AgentFunctions_h_
