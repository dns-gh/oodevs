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

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetStealthFactor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetStealthFactor( MIL_Agent_ABC& callerAgent, float factor )
{
    callerAgent.GetRole< PHY_RoleInterface_Posture >().SetStealthFactor( factor );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::IsStealth
// Created: DDA 2010-06-08
// -----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::IsStealth( MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Posture >().IsStealth();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableCoupDeSonde
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableCoupDeSonde( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableCoupDeSonde();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableCoupDeSonde
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableCoupDeSonde( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableCoupDeSonde();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::IsPointVisible
// Created: AGN 03-01-21
//-----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::IsPointVisible( const MIL_Agent_ABC& callerAgent, MT_Vector2D* pPt )
{
    if( !pPt )
        throw MASA_EXCEPTION( "invalid parameter." );
    const PHY_PerceptionLevel& level = callerAgent.GetRole< PHY_RoleInterface_Perceiver >().ComputePerception( *pPt );
    return( level != PHY_PerceptionLevel::notSeen_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModeDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModeDirection( MIL_Agent_ABC& callerAgent, boost::shared_ptr< MT_Vector2D > pDir )
{
    if( !pDir )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().SetVisionModeDirection( *pDir );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModePoint
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModePoint( MIL_Agent_ABC& callerAgent, const MT_Vector2D* pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().SetVisionModePoint( *pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModePointPtr
// Created: SLG 2010-01-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModePointPtr( MIL_Agent_ABC& callerAgent, boost::shared_ptr< MT_Vector2D > point )
{
    if( point )
        callerAgent.GetRole< PHY_RoleInterface_Perceiver >().SetVisionModePoint( *point );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModeNormal
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModeNormal( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().SetVisionModeNormal();
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
void DEC_PerceptionFunctions::DisableRecognitionPoint( MIL_Agent_ABC& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoPoint( id );
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
void DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation( MIL_Agent_ABC& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoObjects( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionLocalisation
// Created: MGD 2010-06-28
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionLocalisation( MIL_Agent_ABC& callerAgent, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecoLocalisation( *pLocalisation, callerAgent.GetRole< DEC_Decision_ABC >() );
}


// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionLocalisation
// Created: MGD 2010-06-28
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionLocalisation( MIL_Agent_ABC& callerAgent, const TER_Localisation* pLocalisation, float rGrowthSpeed )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecoLocalisation( *pLocalisation, rGrowthSpeed, callerAgent.GetRole< DEC_Decision_ABC >() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionLocalisation
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionUrbanBlock( MIL_Agent_ABC& callerAgent, MIL_UrbanObject_ABC* pUrbanBlock )
{
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecoUrbanBlock( pUrbanBlock );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionLocalisation( MIL_Agent_ABC& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionUrbanBlock( MIL_Agent_ABC& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoUrbanBlock( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadarOnLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRadarOnLocalisation( MIL_Agent_ABC& callerAgent, int nRadarClass, const TER_Localisation* pLocalisation )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    if( !pRadarClass || !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRadarOnLocalisation( *pRadarClass, *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRadarOnLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRadarOnLocalisation( MIL_Agent_ABC& callerAgent, int nRadarClass, int id )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    if( !pRadarClass )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRadarOnLocalisation( *pRadarClass, id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRadar( MIL_Agent_ABC& callerAgent, int nRadarClass )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    if( !pRadarClass )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRadar( *pRadarClass );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRadar( MIL_Agent_ABC& callerAgent, int nRadarClass )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    if( !pRadarClass )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRadar( *pRadarClass );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadarOnPointPtr
// Created: LMT 2010-02-04
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRadarOnPointPtr( MIL_Agent_ABC& callerAgent, int nRadarClass, boost::shared_ptr< MT_Vector2D > point )
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
void DEC_PerceptionFunctions::EnableRecordMode( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecordMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecordMode( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecordMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecoAlat
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecoAlat( MIL_Agent_ABC& callerAgent, const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "invalid parameter." );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecoAlat( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecoAlat
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecoAlat( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoAlat();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::HasNoDelayedPeceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::HasNoDelayedPeceptions( const MIL_Agent_ABC& callerAgent )
{
    return !callerAgent.GetRole< PHY_RoleInterface_Perceiver >().HasDelayedPerceptions();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecoSurveillance
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableSurveillanceLocalisation( MIL_Agent_ABC& callerAgent, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableSurveillanceLocalisation( *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableSurveillanceLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableSurveillanceLocalisation( MIL_Agent_ABC& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableSurveillanceLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableFlyingShellDetection
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableFlyingShellDetection( MIL_Agent_ABC& callerAgent, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableFlyingShellDetection( *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableFlyingShellDetection
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableFlyingShellDetection( MIL_Agent_ABC& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableFlyingShellDetection( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableSensors( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableSensors();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableSensors( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableSensors();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::GetPerception
// Created: LMT 2010-07-02
// -----------------------------------------------------------------------------
double DEC_PerceptionFunctions::GetPerception( const MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > pPoint, boost::shared_ptr< MT_Vector2D > pTarget )
{
    if( !pTarget || !pPoint )
        return 0.;
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().GetPerception( *pPoint, *pTarget );
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
void DEC_PerceptionFunctions::EnableFireObserver( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableFireObserver();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableFireObserver( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableFireObserver();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnablePerceptionUponRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnablePerceptionUponRequest( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnablePerceptionUponRequest();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisablePerceptionUponRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisablePerceptionUponRequest( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisablePerceptionUponRequest();
}
