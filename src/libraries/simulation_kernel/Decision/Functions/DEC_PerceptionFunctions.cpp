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
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetStealthFactor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetStealthFactor( MIL_AgentPion& callerAgent, float factor )
{
    callerAgent.GetRole< PHY_RoleInterface_Posture >().SetStealthFactor( factor );
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableCoupDeSonde
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableCoupDeSonde( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableCoupDeSonde();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableCoupDeSonde
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableCoupDeSonde( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableCoupDeSonde();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::IsPointVisible
// Created: AGN 03-01-21
//-----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::IsPointVisible( const MIL_AgentPion& callerAgent, MT_Vector2D* pPt )
{
    assert( pPt );
    const PHY_PerceptionLevel& level = callerAgent.GetRole< PHY_RoleInterface_Perceiver >().ComputePerception( *pPt );
    return( level != PHY_PerceptionLevel::notSeen_ ); 
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModeDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModeDirection( MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > pDir )
{
    assert( pDir.get() );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().SetVisionModeDirection( *pDir );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModePoint
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModePoint( MIL_AgentPion& callerAgent, const MT_Vector2D* pPoint )
{
    assert( pPoint );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().SetVisionModePoint( *pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModeNormal
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModeNormal( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().SetVisionModeNormal();
}


// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionPoint
// Created: JVT 2004-10-21
// Returns an identifier allowing to retrieve the recon point.
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionPoint( DEC_Decision_ABC& callerAgent, MT_Vector2D* pCenter, MT_Float rSize, MT_Float rGrowthSpeed )
{
    assert( pCenter );

    const MT_Float rSimSize        = MIL_Tools::ConvertMeterToSim   ( rSize );
    const MT_Float rSimGrowthSpeed = MIL_Tools::ConvertSpeedMosToSim( rGrowthSpeed );

    return callerAgent.GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRecoPoint( *pCenter, rSimSize, rSimGrowthSpeed, callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionPoint( MIL_AgentPion& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoPoint( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation( DEC_Decision_ABC& callerAgent, const TER_Localisation* pLocalisation, const MT_Vector2D* pCenter, MT_Float rGrowthSpeed )
{
    assert( pLocalisation );
    assert( pCenter );

    return callerAgent.GetPion().GetRole< PHY_RoleInterface_Perceiver >().EnableRecoObjects( *pLocalisation, *pCenter, rGrowthSpeed, callerAgent );
}
 
// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation( MIL_AgentPion& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoObjects( id );
}


// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRecognitionLocalisation( MIL_AgentPion& callerAgent, const TER_Localisation* pLocalisation )
{
    assert( pLocalisation );

    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecoLocalisation( *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionLocalisation( MIL_AgentPion& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadarOnLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableRadarOnLocalisation( MIL_AgentPion& callerAgent, int nRadarClass, const TER_Localisation* pLocalisation )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    assert( pRadarClass );

    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRadarOnLocalisation( *pRadarClass, *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRadarOnLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRadarOnLocalisation( MIL_AgentPion& callerAgent, int nRadarClass, int id )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    assert( pRadarClass );

    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRadarOnLocalisation( *pRadarClass, id );
}


// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRadar( MIL_AgentPion& callerAgent, int nRadarClass )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    assert( pRadarClass );

    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRadar( *pRadarClass );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRadar( MIL_AgentPion& callerAgent, int nRadarClass )
{
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    assert( pRadarClass );

    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRadar( *pRadarClass );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecordMode( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecordMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecordMode( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecordMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecoAlat
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecoAlat( MIL_AgentPion& callerAgent, const TER_Localisation* location )
{
    assert( location );
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableRecoAlat( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecoAlat
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecoAlat( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableRecoAlat();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::HasNoDelayedPeceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool DEC_PerceptionFunctions::HasNoDelayedPeceptions( const MIL_AgentPion& callerAgent )
{
    return !callerAgent.GetRole< PHY_RoleInterface_Perceiver >().HasDelayedPerceptions();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecoSurveillance
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableSurveillanceLocalisation( MIL_AgentPion& callerAgent, const TER_Localisation* pLocalisation )
{
    assert( pLocalisation );
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableSurveillanceLocalisation( *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableSurveillanceLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableSurveillanceLocalisation( MIL_AgentPion& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableSurveillanceLocalisation( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableFlyingShellDetection
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
int DEC_PerceptionFunctions::EnableFlyingShellDetection( MIL_AgentPion& callerAgent, const TER_Localisation* pLocalisation )
{
    assert( pLocalisation );
    return callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableFlyingShellDetection( *pLocalisation );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableFlyingShellDetection
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableFlyingShellDetection( MIL_AgentPion& callerAgent, int id )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableFlyingShellDetection( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableSensors( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().DisableSensors();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableSensors( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Perceiver >().EnableSensors();
}

