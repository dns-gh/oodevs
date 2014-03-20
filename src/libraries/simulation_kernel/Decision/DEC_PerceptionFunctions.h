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

class DEC_Decision_ABC;
class MIL_UrbanObject_ABC;
class MIL_Agent_ABC;
class TER_Localisation;
class MT_Vector2D;
class MIL_AgentPion;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_PerceptionFunctions
{
public:
    //! @name Functions
    //@{
    static void SetVisionModeDirection( MIL_Agent_ABC& callerAgent, boost::shared_ptr< MT_Vector2D > );
    static void SetVisionModePoint( MIL_Agent_ABC& callerAgent, const MT_Vector2D* pPoint );
    static void SetVisionModePointPtr( MIL_Agent_ABC& callerAgent, boost::shared_ptr< MT_Vector2D > point );
    static void SetVisionModeNormal( MIL_Agent_ABC& callerAgent );
    static void SetStealthFactor( MIL_Agent_ABC& callerAgent, float factor );
    static bool IsStealth( MIL_Agent_ABC& callerAgent );
    static bool IsPointVisible( const MIL_Agent_ABC& callerAgent, MT_Vector2D* pPt );
    static void EnableRecoAlat( MIL_Agent_ABC& callerAgent, const TER_Localisation* location );
    static void DisableRecoAlat( MIL_Agent_ABC& callerAgent );
    static bool HasNoDelayedPeceptions( const MIL_Agent_ABC& callerAgent );
    static void EnableCoupDeSonde( MIL_Agent_ABC& callerAgent );
    static void DisableCoupDeSonde( MIL_Agent_ABC& callerAgent );
    static void EnablePerceptionUponRequest( MIL_Agent_ABC& callerAgent );
    static void DisablePerceptionUponRequest( MIL_Agent_ABC& callerAgent );
    static int EnableObjectRecognitionLocalisation( DEC_Decision_ABC& callerAgent, const TER_Localisation* pLocalisation, const MT_Vector2D* pCenter, double rGrowthSpeed );
    static void DisableObjectRecognitionLocalisation( MIL_Agent_ABC& callerAgent, int id );
    static int EnableRecognitionLocalisation( MIL_Agent_ABC& callerAgent, const TER_Localisation* pLocalisation );
    static int EnableRecognitionLocalisation( MIL_Agent_ABC& callerAgent, const TER_Localisation* pLocalisation, float rGrowthSpeed );
    static int EnableRecognitionUrbanBlock( MIL_Agent_ABC& callerAgent, MIL_UrbanObject_ABC* pUrbanBlock );
    static void DisableRecognitionLocalisation( MIL_Agent_ABC& callerAgent, int id );
    static void DisableRecognitionUrbanBlock( MIL_Agent_ABC& callerAgent, int id );
    static int EnableRecognitionPoint( DEC_Decision_ABC& callerAgent, MT_Vector2D* pCenter, double rSize, double rGrowthSpeed );
    static void DisableRecognitionPoint( MIL_Agent_ABC& callerAgent, int id );
    static void EnableRadar( MIL_Agent_ABC& callerAgent, int nRadarClass );
    static void DisableRadar( MIL_Agent_ABC& callerAgent, int nRadarClass );
    static int EnableRadarOnPointPtr( MIL_Agent_ABC& callerAgent, int nRadarClass, boost::shared_ptr< MT_Vector2D > point );
    static int EnableRadarOnLocalisation( MIL_Agent_ABC& callerAgent, int nRadarClass, const TER_Localisation* pLocalisation );
    static void DisableRadarOnLocalisation( MIL_Agent_ABC& callerAgent, int nRadarClass, int id );
    static void EnableRecordMode( MIL_Agent_ABC& callerAgent );
    static void DisableRecordMode( MIL_Agent_ABC& callerAgent );
    static int EnableSurveillanceLocalisation( MIL_Agent_ABC& callerAgent, const TER_Localisation* pLocalisation );
    static void DisableSurveillanceLocalisation( MIL_Agent_ABC& callerAgent, int id );
    static int EnableFlyingShellDetection( MIL_Agent_ABC& callerAgent, const TER_Localisation* pLocalisation );
    static void DisableFlyingShellDetection( MIL_Agent_ABC& callerAgent, int id );
    static void EnableSensors( MIL_Agent_ABC& callerAgent, bool enable );
    static double GetPerception( const MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > pPoint, boost::shared_ptr< MT_Vector2D > pTarget );
    static void AddReconnoiteredPointBy( DEC_Decision_ABC* pPion, MT_Vector2D& point );
    static bool PointIsReconnoiteredByMeOrNoOne( DEC_Decision_ABC* pPion, MT_Vector2D& point );
    static void EnableFireObserver( MIL_Agent_ABC& callerAgent );
    static void DisableFireObserver( MIL_Agent_ABC& callerAgent );
    //@}
};

#endif // __DEC_PerceptionFunctions_h_
