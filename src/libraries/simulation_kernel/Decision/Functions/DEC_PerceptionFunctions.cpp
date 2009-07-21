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
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetStealthFactor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetStealthFactor( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Posture >().SetStealthFactor( call.GetParameter( 0 ).ToFloat() );
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::GetMaxDetectionDistance
// Created: JVT 04-05-18
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::GetMaxDetectionDistance( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( callerAgent.GetRole< PHY_RolePion_Perceiver >().GetMaxAgentPerceptionDistance() ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableCoupDeSonde
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableCoupDeSonde( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableCoupDeSonde();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableCoupDeSonde
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableCoupDeSonde( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableCoupDeSonde();
}

//-----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::IsPointVisible
// Created: AGN 03-01-21
//-----------------------------------------------------------------------------
void DEC_PerceptionFunctions::IsPointVisible( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    MT_Vector2D* pPt = call.GetParameter( 0 ).ToUserPtr( pPt );
    assert( pPt );
    const PHY_PerceptionLevel& level = callerAgent.GetRole< PHY_RolePion_Perceiver >().ComputePerception( *pPt );
    call.GetResult().SetValue( level != PHY_PerceptionLevel::notSeen_ ); 
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModeDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModeDirection( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );
    const MT_Vector2D* pDir = call.GetParameter( 0 ).ToUserPtr( pDir );
    assert( pDir );
    callerAgent.GetRole< PHY_RolePion_Perceiver >().SetVisionModeDirection( *pDir );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModePoint
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModePoint( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    const MT_Vector2D* pPoint = call.GetParameter( 0 ).ToUserPtr( pPoint );
    assert( pPoint );
    callerAgent.GetRole< PHY_RolePion_Perceiver >().SetVisionModePoint( *pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::SetVisionModeNormal
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::SetVisionModeNormal( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Perceiver >().SetVisionModeNormal();
}


// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecognitionPoint( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    const MT_Vector2D* pCenter = call.GetParameter( 0 ).ToUserPtr( pCenter );
    assert( pCenter );

    const MT_Float rSize        = MIL_Tools::ConvertMeterToSim   ( call.GetParameter( 1 ).ToFloat() );
    const MT_Float rGrowthSpeed = MIL_Tools::ConvertSpeedMosToSim( call.GetParameter( 2 ).ToFloat() );

    void* pReturnCode = callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableRecoPoint( *pCenter, rSize, rGrowthSpeed, call.GetParameter( 3 ) );
    call.GetResult().SetValue( pReturnCode, &DEC_Tools::GetTypePerceptionPoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionPoint( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePerceptionPoint( call.GetParameter( 0 ) ) );
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableRecoPoint( call.GetParameter( 0 ).ToPtr() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    const TER_Localisation* pLocalisation = call.GetParameter( 0 ).ToUserPtr( pLocalisation );
    assert( pLocalisation );
    
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );
    const MT_Vector2D* pCenter = call.GetParameter( 1 ).ToUserPtr( pCenter );
    assert( pCenter );
    
    const MT_Float rGrowthSpeed = MIL_Tools::ConvertSpeedMosToSim( call.GetParameters().GetParameters().size() >= 4 ? call.GetParameter( 3 ).ToFloat() : 5. );

    void* pReturnCode = callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableRecoObjects( *pLocalisation, *pCenter, rGrowthSpeed, call.GetParameter( 2 ) );
    call.GetResult().SetValue( pReturnCode, &DEC_Tools::GetTypePerceptionObjectsLocalisation() );
}
 
// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation( DIA_Call_ABC& call,MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePerceptionObjectsLocalisation( call.GetParameter( 0 ) ) );
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableRecoObjects( call.GetParameter( 0 ).ToPtr() );
}


// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecognitionLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecognitionLocalisation( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    const TER_Localisation* pLocalisation = call.GetParameter( 0 ).ToUserPtr( pLocalisation );
    assert( pLocalisation );

    void* pReturnCode = 0;
    if ( call.GetParameters().GetParameters().size() >= 2 )
    {
        const MT_Float rRadius = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );
        pReturnCode = callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableRecoLocalisation( *pLocalisation, rRadius );
    }
    else
        pReturnCode = callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableRecoLocalisation( *pLocalisation );

    call.GetResult().SetValue( pReturnCode, &DEC_Tools::GetTypePerceptionLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecognitionLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecognitionLocalisation( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePerceptionLocalisation( call.GetParameter( 0 ) ) );
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableRecoLocalisation( call.GetParameter( 0 ).ToPtr() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadarOnLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRadarOnLocalisation( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 1 ) ) );

    int nRadarClass = call.GetParameter( 0 ).ToId();
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    assert( pRadarClass );

    const TER_Localisation* pLocalisation = call.GetParameter( 1 ).ToUserPtr( pLocalisation );
    assert( pLocalisation );

    void* pReturnCode = callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableRadarOnLocalisation( *pRadarClass, *pLocalisation );
    call.GetResult().SetValue( pReturnCode, &DEC_Tools::GetTypePerceptionRadar() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRadarOnLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRadarOnLocalisation( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    int nRadarClass = call.GetParameter( 0 ).ToId();
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    assert( pRadarClass );

    assert( DEC_Tools::CheckTypePerceptionRadar( call.GetParameter( 1 ) ) );
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableRadarOnLocalisation( *pRadarClass, call.GetParameter( 1 ).ToPtr() );
}


// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRadar( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    int nRadarClass = call.GetParameter( 0 ).ToId();
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    assert( pRadarClass );

    callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableRadar( *pRadarClass );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRadar( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    int nRadarClass = call.GetParameter( 0 ).ToId();
    const PHY_RadarClass* pRadarClass = PHY_RadarClass::Find( nRadarClass );
    assert( pRadarClass );

    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableRadar( *pRadarClass );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecordMode( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableRecordMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecordMode( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableRecordMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecoAlat
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableRecoAlat( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    
    TER_Localisation* pLoc = call.GetParameter( 0 ).ToUserPtr( pLoc );
    assert( pLoc );
    
    callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableRecoAlat( *pLoc );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableRecoAlat
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableRecoAlat( DIA_Call_ABC& /*call*/, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableRecoAlat();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::HasNoDelayedPeceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::HasNoDelayedPeceptions( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    call.GetResult().SetValue( !callerAgent.GetRole< PHY_RolePion_Perceiver >().HasDelayedPerceptions() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableRecoSurveillance
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableSurveillanceLocalisation( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    const TER_Localisation* pLocalisation = call.GetParameter( 0 ).ToUserPtr( pLocalisation );
    assert( pLocalisation );
    void* pReturnCode = callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableSurveillanceLocalisation( *pLocalisation );
    call.GetResult().SetValue( pReturnCode, &DEC_Tools::GetTypePerceptionSurveillance() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableSurveillanceLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableSurveillanceLocalisation( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePerceptionSurveillance( call.GetParameter( 0 ) ) );
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableSurveillanceLocalisation( call.GetParameter( 0 ).ToPtr() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableFlyingShellDetection
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableFlyingShellDetection( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    const TER_Localisation* pLocalisation = call.GetParameter( 0 ).ToUserPtr( pLocalisation );
    assert( pLocalisation );
    void* pReturnCode = callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableFlyingShellDetection( *pLocalisation );
    call.GetResult().SetValue( pReturnCode, &DEC_Tools::GetTypePerceptionFlyingShell() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableFlyingShellDetection
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableFlyingShellDetection( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePerceptionFlyingShell( call.GetParameter( 0 ) ) );
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableFlyingShellDetection( call.GetParameter( 0 ).ToPtr() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::DisableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::DisableSensors( DIA_Call_ABC&, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Perceiver >().DisableSensors();
}

// -----------------------------------------------------------------------------
// Name: DEC_PerceptionFunctions::EnableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void DEC_PerceptionFunctions::EnableSensors( DIA_Call_ABC&, MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RolePion_Perceiver >().EnableSensors();
}

