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

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class MIL_UrbanObject_ABC;
class TER_Localisation;
class MT_Vector2D;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_PerceptionFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Functions
    //@{
    static void SetVisionModeDirection( DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > );
    static void SetVisionModePoint( DEC_Decision_ABC* callerAgent, const MT_Vector2D* pPoint );
    static void SetVisionModePointPtr( DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > point );
    static void SetVisionModeNormal( DEC_Decision_ABC* callerAgent );
    static void SetStealthFactor( DEC_Decision_ABC* callerAgent, double factor );
    static bool IsStealth( const DEC_Decision_ABC* callerAgent );
    static bool IsPointVisible( const DEC_Decision_ABC* callerAgent, MT_Vector2D* pPt );
    static void EnableRecoAlat( DEC_Decision_ABC* callerAgent, const TER_Localisation* location );
    static void DisableRecoAlat( DEC_Decision_ABC* callerAgent );
    static bool HasNoDelayedPeceptions( const DEC_Decision_ABC* callerAgent );
    static void EnableCoupDeSonde( DEC_Decision_ABC* callerAgent );
    static void DisableCoupDeSonde( DEC_Decision_ABC* callerAgent );
    static void EnablePerceptionUponRequest( DEC_Decision_ABC* callerAgent );
    static void DisablePerceptionUponRequest( DEC_Decision_ABC* callerAgent );
    static int EnableObjectRecognitionLocalisation( DEC_Decision_ABC& callerAgent, const TER_Localisation* pLocalisation, const MT_Vector2D* pCenter, double rGrowthSpeed );
    static void DisableObjectRecognitionLocalisation( DEC_Decision_ABC* callerAgent, int id );
    static int EnableRecognitionLocalisation( DEC_Decision_ABC* callerAgent, const TER_Localisation* pLocalisation );
    static int EnableRecognitionLocalisation( DEC_Decision_ABC* callerAgent, const TER_Localisation* pLocalisation, float rGrowthSpeed );
    static int EnableRecognitionUrbanBlock( DEC_Decision_ABC* callerAgent, MIL_UrbanObject_ABC* pUrbanBlock );
    static void DisableRecognitionLocalisation( DEC_Decision_ABC* callerAgent, int id );
    static void DisableRecognitionUrbanBlock( DEC_Decision_ABC* callerAgent, int id );
    static int EnableRecognitionPoint( DEC_Decision_ABC& callerAgent, MT_Vector2D* pCenter, double rSize, double rGrowthSpeed );
    static void DisableRecognitionPoint( DEC_Decision_ABC* callerAgent, int id );
    static void EnableRadar( DEC_Decision_ABC* callerAgent, int nRadarClass );
    static void DisableRadar( DEC_Decision_ABC* callerAgent, int nRadarClass );
    static int EnableRadarOnPointPtr( DEC_Decision_ABC* callerAgent, int nRadarClass, boost::shared_ptr< MT_Vector2D > point );
    static int EnableRadarOnLocalisation( DEC_Decision_ABC* callerAgent, int nRadarClass, const TER_Localisation* pLocalisation );
    static void DisableRadarOnLocalisation( DEC_Decision_ABC* callerAgent, int nRadarClass, int id );
    static void EnableRecordMode( DEC_Decision_ABC* callerAgent );
    static void DisableRecordMode( DEC_Decision_ABC* callerAgent );
    static int EnableSurveillanceLocalisation( DEC_Decision_ABC* callerAgent, const TER_Localisation* pLocalisation );
    static void DisableSurveillanceLocalisation( DEC_Decision_ABC* callerAgent, int id );
    static int EnableFlyingShellDetection( DEC_Decision_ABC* callerAgent, const TER_Localisation* pLocalisation );
    static void DisableFlyingShellDetection( DEC_Decision_ABC* callerAgent, int id );
    static void EnableSensors( DEC_Decision_ABC* callerAgent, bool enable );
    static double GetPerception( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > pPoint, boost::shared_ptr< MT_Vector2D > pTarget );
    static void AddReconnoiteredPointBy( DEC_Decision_ABC* pPion, MT_Vector2D& point );
    static bool PointIsReconnoiteredByMeOrNoOne( DEC_Decision_ABC* pPion, MT_Vector2D& point );
    static void EnableFireObserver( DEC_Decision_ABC* callerAgent );
    static void DisableFireObserver( DEC_Decision_ABC* callerAgent );
    //@}
};

#endif // __DEC_PerceptionFunctions_h_
