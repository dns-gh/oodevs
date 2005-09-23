// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_OrdersFunctions.h $
// $Author: Nld $
// $Modtime: 2/12/04 10:15 $
// $Revision: 3 $
// $Workfile: DEC_OrdersFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_OrdersFunctions_h_
#define __DEC_OrdersFunctions_h_

#include "MIL.h"
#include "MT_Tools/MT_Random.h"

class MIL_AgentPion;
class MIL_Automate;

// =============================================================================
// Created: NLD 2004-07-08
// =============================================================================
class DEC_OrdersFunctions
{
public:
    // Conduite
    template< typename T > static void WaitForOrder       ( DIA_Call_ABC& call, T& caller );            
    template< typename T > static void StopWaitingForOrder( DIA_Call_ABC& call, T& caller );            

    // Mission
    template< typename T > static void FinishMission      ( DIA_Call_ABC& call, T& caller );
    template< typename T > static void IsNewMissionStarted( DIA_Call_ABC& call, T& caller );


    // Limas
    template< typename T > static void SetMissionLimaFlag        ( DIA_Call_ABC& call,       T& caller );
    template< typename T > static void GetMissionLimaFlag        ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetLima                   ( DIA_Call_ABC& call, const T& caller );
                           static void AutomateSetMissionLimaFlag( DIA_Call_ABC& call,       MIL_AgentPion& caller );


    // Automate
    static void MRT_CreatePionMission               ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void MRT_Validate                        ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void MRT_AffectFuseaux                   ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    
    static void CDT_SendOrderConduiteToPion         ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void CDT_CreatePionMission               ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void CDT_GivePionMission                 ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void CDT_IsPionWaitingForOrderConduite   ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void CDT_GetPionsWaitingForOrderConduite ( DIA_Call_ABC& call, MIL_Automate& callerAutomate );
    static void CDT_GetListOrderConduiteWaitedByPion( DIA_Call_ABC& call, MIL_Automate& callerAutomate );                               

private:
    static MT_Random randomGenerator_; //$$$ Pourri TMP
};

#include "DEC_OrdersFunctions.inl"

#endif // __DEC_OrdersFunctions_h_
