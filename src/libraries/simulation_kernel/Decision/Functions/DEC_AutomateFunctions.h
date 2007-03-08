// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AutomateFunctions.h $
// $Author: Jvt $
// $Modtime: 11/05/05 10:42 $
// $Revision: 17 $
// $Workfile: DEC_AutomateFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_AutomateFunctions_h_
#define __DEC_AutomateFunctions_h_

#include "MIL.h"

class MIL_Automate;
class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_AutomateFunctions
{
public:
    //! @name Functions
    //@{
    // Accessors    
    static void IsSurrendered      ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void IsEngaged          ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void DebrayeAutomate    ( DIA_Call_ABC& call,       MIL_Automate& callerAutomate );

    static void GetPionsWithoutPC  ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void GetPionsWithPC     ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void GetPionPC          ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void GetPionPCOfAutomate( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
        
    static void GetPionsOfAutomateWithoutPC( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void GetPionsOfAutomateWithPC   ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );

    // Etat décisionnel
    static void NotifyForceRatioStateChanged       ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void NotifyRulesOfEngagementStateChanged( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void NotifyCloseCombatStateChanged      ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void NotifyOperationalStateChanged      ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
  
    // Test / debug
    static void Debug          ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void Trace          ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void DebugDrawPoints( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void DebugDrawPoint ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );

    // Pion management
    static void PionChangeAutomate( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    
    // Accesseurs sur les fonctions Pion
    static void IsPionFlying                                ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void IsPionTransported                           ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void IsPionContaminated                          ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate ); 
    static void IsPionNBCProtected                          ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate ); 
    static void IsPionMoving                                ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate ); 
    static void IsPionNeutralized                           ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate ); 
    static void IsPointInPionFuseau                         ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void MakePionRelievePion                         ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void GetPionOperationalState                     ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate ); 
    static void GetPionMajorOperationalState                ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate ); 
    static void GetPionPosition                             ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate ); 
    static void GetPionDirection                            ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void CanPionRelievePion                          ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void CanPionConstructObject                      ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void CanPionBypassObject                         ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void CanPionDestroyObject                        ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void CanPionMineObject                           ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void CanPionActivateObject                       ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void ComputePointBeforeLimaForPion               ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void ComputePionNearestLocalisationPointInFuseau ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void PionTimeLeftForMoving                       ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    static void PionTimeToMoveDistance                      ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );    
    static void CanPionTransportPion                        ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    //@}
    
private:
    static bool IsPionInAutomate( const MIL_Automate& automate, const MIL_AgentPion& pion );
};

#endif // __DEC_AutomateFunctions_h_
