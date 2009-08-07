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

class DEC_Decision_ABC;
class MIL_AgentPion;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_PerceptionFunctions
{
public:
    //! @name Functions
    //@{
    static void SetVisionModeDirection              ( MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > );
    static void SetVisionModePoint                  ( MIL_AgentPion& callerAgent, const MT_Vector2D* pPoint );
    static void SetVisionModeNormal                 ( MIL_AgentPion& callerAgent );
    static void SetStealthFactor                    ( MIL_AgentPion& callerAgent, float factor );
    static bool IsPointVisible                      ( const MIL_AgentPion& callerAgent, MT_Vector2D* pPt );
    static void EnableRecoAlat                      ( MIL_AgentPion& callerAgent, const TER_Localisation* location );
    static void DisableRecoAlat                     ( MIL_AgentPion& callerAgent );
    static bool HasNoDelayedPeceptions              ( const MIL_AgentPion& callerAgent );
    static void EnableCoupDeSonde                   ( MIL_AgentPion& callerAgent );
    static void DisableCoupDeSonde                  ( MIL_AgentPion& callerAgent );
    static int  EnableObjectRecognitionLocalisation ( DEC_Decision_ABC& callerAgent, const TER_Localisation* pLocalisation, const MT_Vector2D* pCenter, MT_Float rGrowthSpeed );
    static void DisableObjectRecognitionLocalisation( MIL_AgentPion& callerAgent, int id );
    static int  EnableRecognitionLocalisation       ( MIL_AgentPion& callerAgent, const TER_Localisation* pLocalisation );
    static void DisableRecognitionLocalisation      ( MIL_AgentPion& callerAgent, int id );
    static int  EnableRecognitionPoint              ( DEC_Decision_ABC& callerAgent, MT_Vector2D* pCenter, MT_Float rSize, MT_Float rGrowthSpeed );
    static void DisableRecognitionPoint             ( MIL_AgentPion& callerAgent, int id );
    
    static void EnableRadar                         ( MIL_AgentPion& callerAgent, int nRadarClass );
    static void DisableRadar                        ( MIL_AgentPion& callerAgent, int nRadarClass );
    static int  EnableRadarOnLocalisation           ( MIL_AgentPion& callerAgent, int nRadarClass, const TER_Localisation* pLocalisation );
    static void DisableRadarOnLocalisation          ( MIL_AgentPion& callerAgent, int nRadarClass, int id );

    static void EnableRecordMode                    ( MIL_AgentPion& callerAgent );
    static void DisableRecordMode                   ( MIL_AgentPion& callerAgent );
    static int  EnableSurveillanceLocalisation      ( MIL_AgentPion& callerAgent, const TER_Localisation* pLocalisation );
    static void DisableSurveillanceLocalisation     ( MIL_AgentPion& callerAgent, int id );
    static int EnableFlyingShellDetection           ( MIL_AgentPion& callerAgent, const TER_Localisation* pLocalisation );
    static void DisableFlyingShellDetection         ( MIL_AgentPion& callerAgent, int id );
    static void DisableSensors                      ( MIL_AgentPion& callerAgent );
    static void EnableSensors                       ( MIL_AgentPion& callerAgent );
    //@}
};

#endif // __DEC_PerceptionFunctions_h_