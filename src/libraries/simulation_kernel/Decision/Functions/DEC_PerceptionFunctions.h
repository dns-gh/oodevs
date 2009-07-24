//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_PerceptionFunctions.h $
// $Author: Nld $
// $Modtime: 2/05/05 18:11 $
// $Revision: 15 $
// $Workfile: DEC_PerceptionFunctions.h $
//
//*****************************************************************************

#ifndef __DEC_PerceptionFunctions_h_
#define __DEC_PerceptionFunctions_h_

#include "MIL.h"

class MIL_AgentPion;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_PerceptionFunctions
{
public:
    //! @name Functions
    //@{
    static void SetVisionModeDirection              ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void SetVisionModePoint                  ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void SetVisionModeNormal                 ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void SetStealthFactor                    ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void IsPointVisible                      ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void EnableRecoAlat                      ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableRecoAlat                     ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void HasNoDelayedPeceptions              ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void EnableCoupDeSonde                   ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableCoupDeSonde                  ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void EnableObjectRecognitionLocalisation ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableObjectRecognitionLocalisation( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void EnableRecognitionLocalisation       ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableRecognitionLocalisation      ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void EnableRecognitionPoint              ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableRecognitionPoint             ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    
    static void EnableRadar                         ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableRadar                        ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void EnableRadarOnLocalisation           ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableRadarOnLocalisation          ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );

    static void EnableRecordMode                    ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableRecordMode                   ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void EnableSurveillanceLocalisation      ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableSurveillanceLocalisation     ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void EnableFlyingShellDetection          ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableFlyingShellDetection         ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DisableSensors                      ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void EnableSensors                       ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    //@}
};

#endif // __DEC_PerceptionFunctions_h_