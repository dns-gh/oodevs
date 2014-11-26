//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_PerceptionFunctions.cpp $
// $Author: Jvt $
// $Modtime: 24/05/05 10:18 $
// $Revision: 17 $
// $Workfile: DEC_PerceptionFunctions.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PerceptionFunctions.h"
#include "Decision/Brain.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "simulation_terrain/TER_Localisation.h"
#include "Tools/MIL_Tools.h"

void DEC_PerceptionFunctions::Register( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_Perception_EstFurtif", &DEC_PerceptionFunctions::IsStealth );
    brain.RegisterFunction( "_DEC_Perception_Furtivite", &DEC_PerceptionFunctions::SetStealthFactor );
    brain.RegisterFunction( "DEC_Perception_AjouterPointReconnuAutomateParPion", &DEC_PerceptionFunctions::AddReconnoiteredPointBy );
    brain.RegisterFunction( "DEC_Perception_EstPointReconnuParPionOuPersonne", &DEC_PerceptionFunctions::PointIsReconnoiteredByMeOrNoOne );
    brain.RegisterFunction( "_DEC_Perception_ActiverCoupsDeSonde", &DEC_PerceptionFunctions::EnableCoupDeSonde );
    brain.RegisterFunction( "_DEC_Perception_DesactiverCoupsDeSonde", &DEC_PerceptionFunctions::DisableCoupDeSonde );
    brain.RegisterFunction( "_DEC_Perception_ActiverSenseursSurDecision", &DEC_PerceptionFunctions::EnablePerceptionUponRequest );
    brain.RegisterFunction( "_DEC_Perception_DesactiverSenseursSurDecision", &DEC_PerceptionFunctions::DisablePerceptionUponRequest );
    brain.RegisterFunction( "_DEC_Perception_ActiverSenseurs", &DEC_PerceptionFunctions::EnableSensors );
    brain.RegisterFunction( "_DEC_Perception_ActiverRadar", &DEC_PerceptionFunctions::EnableRadar );
    brain.RegisterFunction( "_DEC_Perception_DesactiverRadar", &DEC_PerceptionFunctions::DisableRadar );
    brain.RegisterFunction( "_DEC_Perception_ActiverRadarSurLocalisation", &DEC_PerceptionFunctions::EnableRadarOnLocalisation );
    brain.RegisterFunction( "_DEC_Perception_ActiverRadarSurPointPtr", &DEC_PerceptionFunctions::EnableRadarOnPointPtr );
    brain.RegisterFunction( "_DEC_Perception_DesactiverRadarSurLocalisation", &DEC_PerceptionFunctions::DisableRadarOnLocalisation );
    brain.RegisterFunction( "_DEC_Perception_ActiverPerceptionTirsIndirect", &DEC_PerceptionFunctions::EnableFlyingShellDetection );
    brain.RegisterFunction( "_DEC_Perception_DesactiverPerceptionTirsIndirect", &DEC_PerceptionFunctions::DisableFlyingShellDetection );
    brain.RegisterFunction( "_DEC_Perception_ActiverReconnaissanceLocalisation", static_cast< int(*)( DEC_Decision_ABC*, const TER_Localisation* ) >( &DEC_PerceptionFunctions::EnableRecognitionLocalisation ) );
    brain.RegisterFunction( "_DEC_Perception_ActivateLocationProgressiveRecce", static_cast< int(*)( DEC_Decision_ABC*, const TER_Localisation*, float ) >( &DEC_PerceptionFunctions::EnableRecognitionLocalisation ) );
    brain.RegisterFunction( "_DEC_Perception_DesactiverReconnaissanceLocalisation", &DEC_PerceptionFunctions::DisableRecognitionLocalisation );
    brain.RegisterFunction( "_DEC_Perception_ActiverReconnaissanceDansBlocUrbain", &DEC_PerceptionFunctions::EnableRecognitionUrbanBlock );
    brain.RegisterFunction( "_DEC_Perception_DesactiverReconnaissanceDansBlocUrbain", &DEC_PerceptionFunctions::DisableRecognitionUrbanBlock );
    brain.RegisterFunction( "_DEC_Perception_ActiverDetectionObjetLocalisation", &DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation );
    brain.RegisterFunction( "_DEC_Perception_DesactiverDetectionObjetLocalisation", &DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation );
    brain.RegisterFunction( "_DEC_Perception_ActiverReconnaissancePoint", &DEC_PerceptionFunctions::EnableRecognitionPoint );
    brain.RegisterFunction( "_DEC_Perception_DesactiverReconnaissancePoint", &DEC_PerceptionFunctions::DisableRecognitionPoint );
    brain.RegisterFunction( "_DEC_Perception_VisionVerrouilleeSurDirection", &DEC_PerceptionFunctions::SetVisionModeDirection );
    brain.RegisterFunction( "_DEC_Perception_VisionVerrouilleeSurPoint", &DEC_PerceptionFunctions::SetVisionModePoint );
    brain.RegisterFunction( "_DEC_Perception_VisionVerrouilleeSurPointPtr", &DEC_PerceptionFunctions::SetVisionModePointPtr );
    brain.RegisterFunction( "_DEC_Perception_VisionNormale", &DEC_PerceptionFunctions::SetVisionModeNormal );
    brain.RegisterFunction( "_DEC_Perception_ActiverObserveurTir", &DEC_PerceptionFunctions::EnableFireObserver );
    brain.RegisterFunction( "_DEC_Perception_DesactiverObserveurTir", &DEC_PerceptionFunctions::DisableFireObserver );
    brain.RegisterFunction( "_DEC_Perception_PointEstVisible", &DEC_PerceptionFunctions::IsPointVisible );
    brain.RegisterFunction( "_DEC_GetPerception", &DEC_PerceptionFunctions::GetPerception );
    brain.RegisterFunction( "_DEC_Perception_ActiverModeEnregistrement", &DEC_PerceptionFunctions::EnableRecordMode );
    brain.RegisterFunction( "_DEC_Perception_DesactiverModeEnregistrement", &DEC_PerceptionFunctions::DisableRecordMode );
    // ALAT
    brain.RegisterFunction( "_DEC_ALAT_ActiverReconnaissance", &DEC_PerceptionFunctions::EnableRecoAlat );
    brain.RegisterFunction( "_DEC_ALAT_DesactiverReconnaissance", &DEC_PerceptionFunctions::DisableRecoAlat );
    brain.RegisterFunction( "_DEC_ALAT_ReconnaissanceNonVuTerminee", &DEC_PerceptionFunctions::HasNoDelayedPeceptions );
    brain.RegisterFunction( "_DEC_Perception_ActiverSurveillance", &DEC_PerceptionFunctions::EnableSurveillanceLocalisation );
    brain.RegisterFunction( "_DEC_Perception_DesactiverSurveillance", &DEC_PerceptionFunctions::DisableSurveillanceLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetStealthFactor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetStealthFactor( DEC_Decision_ABC* callerAgent, double factor )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Posture >().SetStealthFactor( factor );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::IsStealth
// Created: DDA 2010-06-08
// -----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::IsStealth( const DEC_Decision_ABC* callerAgent )
{
    return callerAgent->GetPion().GetRole< PHY_RoleInterface_Posture >().IsStealth();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableCoupDeSonde
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableCoupDeSonde( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableCoupDeSonde();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableCoupDeSonde
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableCoupDeSonde( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableCoupDeSonde();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::IsPointVisible
// Created: AGN 03-01-21
//-----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::IsPointVisible( const DEC_Decision_ABC* callerAgent, MT_Vector2D* pPt )
{
    if( !pPt )
        throw MASA_EXCEPTION( "invalid parameter." );
    const PHY_PerceptionLevel& level = callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().ComputePerception( *pPt );
    return( level != PHY_PerceptionLevel::notSeen_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModeDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModeDirection( DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > pDir )
{
    if( !pDir )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().SetVisionModeDirection( *pDir );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModePoint
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModePoint( DEC_Decision_ABC* callerAgent, const MT_Vector2D* pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().SetVisionModePoint( *pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModePointPtr
// Created: SLG 2010-01-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModePointPtr( DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > point )
{
    if( point )
        callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().SetVisionModePoint( *point );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModeNormal
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModeNormal( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().SetVisionModeNormal();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionPoint
// Created: JVT 2004-10-21
// Returns an identifier allowing to retrieve the recon point.
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionPoint( DEC_Decision_ABC& callerAgent, MT_Vector2D* pCenter, double rSize, double rGrowthSpeed )
{
    if( !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    const double rSimSize = MIL_Tools::ConvertMeterToSim( rSize );
    const double rSimGrowthSpeed = MIL_Tools::ConvertSpeedMosToSim( rGrowthSpeed );
    return callerAgent.GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRecoPoint( *pCenter, rSimSize, rSimGrowthSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionPoint( DEC_Decision_ABC* callerAgent, int id )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableRecoPoint( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation( DEC_Decision_ABC& callerAgent, const TER_Localisation* pLocalisation, const MT_Vector2D* pCenter, double rGrowthSpeed )
{
    if( !pLocalisation || !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent.GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRecoObjects( *pLocalisation, *pCenter, rGrowthSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation( DEC_Decision_ABC* callerAgent, int id )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableRecoObjects( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionLocalisation
// Created: MGD 2010-06-28
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionLocalisation( DEC_Decision_ABC* callerAgent, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRecoLocalisation( *pLocalisation, callerAgent->GetPion().GetRole< DEC_Decision_ABC >() );
}


// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionLocalisation
// Created: MGD 2010-06-28
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionLocalisation( DEC_Decision_ABC* callerAgent, const TER_Localisation* pLocalisation, float rGrowthSpeed )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_AgentPion& agent = callerAgent->GetPion();
    return agent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecoLocalisation( *pLocalisation, rGrowthSpeed, agent.GetRole< DEC_Decision_ABC >() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionLocalisation
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionUrbanBlock( DEC_Decision_ABC* callerAgent, MIL_UrbanObject_ABC* pUrbanBlock )
{
    return callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRecoUrbanBlock( pUrbanBlock );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionLocalisation( DEC_Decision_ABC* callerAgent, int id )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableRecoLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionUrbanBlock( DEC_Decision_ABC* callerAgent, int id )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableRecoUrbanBlock( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadarOnLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRadarOnLocalisation( DEC_Decision_ABC* callerAgent, int nRadarClass, const TER_Localisation* pLocalisation )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    if( !pRadarClass || !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRadarOnLocalisation( *pRadarClass, *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRadarOnLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRadarOnLocalisation( DEC_Decision_ABC* callerAgent, int nRadarClass, int id )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    if( !pRadarClass )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableRadarOnLocalisation( *pRadarClass, id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRadar( DEC_Decision_ABC* callerAgent, int nRadarClass )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    if( !pRadarClass )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRadar( *pRadarClass );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRadar( DEC_Decision_ABC* callerAgent, int nRadarClass )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    if( !pRadarClass )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableRadar( *pRadarClass );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadarOnPointPtr
// Created: LMT 2010-02-04
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRadarOnPointPtr( DEC_Decision_ABC* callerAgent, int nRadarClass, boost::shared_ptr< MT_Vector2D > point )
{
    if( !point )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_PointVector vector;
    vector.push_back( *point );
    TER_Localisation localisation ( TER_Localisation::ePoint, vector );
    return EnableRadarOnLocalisation( callerAgent, nRadarClass, &localisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecordMode( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRecordMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecordMode( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableRecordMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecoAlat
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecoAlat( DEC_Decision_ABC* callerAgent, const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRecoAlat( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecoAlat
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecoAlat( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableRecoAlat();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::HasNoDelayedPeceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::HasNoDelayedPeceptions( const DEC_Decision_ABC* callerAgent )
{
    return !callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().HasDelayedPerceptions();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecoSurveillance
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableSurveillanceLocalisation( DEC_Decision_ABC* callerAgent, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableSurveillanceLocalisation( *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableSurveillanceLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableSurveillanceLocalisation( DEC_Decision_ABC* callerAgent, int id )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableSurveillanceLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableFlyingShellDetection
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableFlyingShellDetection( DEC_Decision_ABC* callerAgent, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableFlyingShellDetection( *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableFlyingShellDetection
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableFlyingShellDetection( DEC_Decision_ABC* callerAgent, int id )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableFlyingShellDetection( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableSensors( DEC_Decision_ABC* callerAgent, bool enable )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableSensors( enable );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::GetPerception
// Created: LMT 2010-07-02
// -----------------------------------------------------------------------------
double DEC_PerceptionFunctions::GetPerception( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > pPoint, boost::shared_ptr< MT_Vector2D > pTarget )
{
    if( !pTarget || !pPoint )
        return 0.;
    return callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().GetPerception( *pPoint, *pTarget );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::AddReconnoiteredPointBy
// Created: LMT 2011-08-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::AddReconnoiteredPointBy( DEC_Decision_ABC* pPion, MT_Vector2D& point )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    DEC_AutomateDecision* pAutomat = pPion->GetDecAutomate();
    if( !pAutomat )
        throw MASA_EXCEPTION( "invalid parameter." );
    pAutomat->AddReconnoiteredPointBy( pPion, point );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::PointIsReconnoiteredByMeOrNoOne
// Created: LMT 2011-08-16
// -----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::PointIsReconnoiteredByMeOrNoOne( DEC_Decision_ABC* pPion, MT_Vector2D& point )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    DEC_AutomateDecision* pAutomat = pPion->GetDecAutomate();
    if( !pAutomat )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pAutomat->PointIsReconnoiteredByMeOrNoOne( pPion, point );
}
// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableFireObserver( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableFireObserver();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableFireObserver( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableFireObserver();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnablePerceptionUponRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnablePerceptionUponRequest( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnablePerceptionUponRequest();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisablePerceptionUponRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisablePerceptionUponRequest( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisablePerceptionUponRequest();
}
