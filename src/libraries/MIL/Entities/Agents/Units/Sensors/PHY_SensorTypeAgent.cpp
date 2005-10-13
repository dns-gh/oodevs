// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 16:19 $
// $Revision: 6 $
// $Workfile: PHY_SensorTypeAgent.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_SensorTypeAgent.h"

#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Meteo/PHY_Precipitation.h"
#include "Meteo/PHY_Lighting.h"
#include "Meteo/RawVisionData/PHY_RawVisionDataIterator.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
template< typename C >
void PHY_SensorTypeAgent::InitializeFactors( const C& container, const std::string& strTagName, T_FactorVector& factors, MIL_InputArchive& archive )
{
    archive.Section( strTagName );
    for ( C::const_iterator it = container.begin(); it != container.end(); ++it )
    {
        assert( factors.size() > it->second->GetID() );
        MT_Float& rFactor = factors[ it->second->GetID() ];

        archive.ReadField( it->second->GetName(), rFactor, CheckValueBound( 0., 1. ) );
    }
    archive.EndSection(); // XXX
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
template<>
void PHY_SensorTypeAgent::InitializeFactors( const PHY_Posture::T_PostureMap& container, const std::string& strTagName, T_FactorVector& factors, MIL_InputArchive& archive )
{
    archive.Section( strTagName );

    for( PHY_Posture::CIT_PostureMap it = container.begin(); it != container.end(); ++it )
    {
        if( !it->second->CanModifyDetection() )
            continue;

        assert( factors.size() > it->second->GetID() );
        MT_Float& rFactor = factors[ it->second->GetID() ];

        archive.ReadField( it->second->GetName(), rFactor, CheckValueBound( 0., 1. ) );
    }

    archive.EndSection(); // XXX
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeAgent::PHY_SensorTypeAgent( const PHY_SensorType& type, MIL_InputArchive& archive )
    : type_                ( type )
    , volumeFactors_       ( PHY_Volume       ::GetVolumes       ().size(), 0. )
    , precipitationFactors_( PHY_Precipitation::GetPrecipitations().size(), 0. )
    , lightingFactors_     ( PHY_Lighting     ::GetLightings     ().size(), 0. )
    , postureSourceFactors_( PHY_Posture      ::GetPostures      ().size(), 0. )
    , postureTargetFactors_( PHY_Posture      ::GetPostures      ().size(), 0. )
    , environmentFactors_  ( PHY_RawVisionData ::eNbrVisionObjects         , 0. )
{
    InitializeAngle        ( archive );
    InitializeDistances    ( archive );

    archive.Section( "ModificateursDeDistance" );

    InitializeFactors( PHY_Volume       ::GetVolumes        (), "VolumesCible"  , volumeFactors_       , archive );
    InitializeFactors( PHY_Precipitation::GetPrecipitations (), "Precipitations", precipitationFactors_, archive );
    InitializeFactors( PHY_Lighting     ::GetLightings      (), "Eclairements"  , lightingFactors_     , archive );
    InitializeFactors( PHY_Posture      ::GetPostures       (), "PosturesSource", postureSourceFactors_, archive );
    InitializeFactors( PHY_Posture      ::GetPostures       (), "PosturesCible" , postureTargetFactors_, archive );

    InitializeEnvironmentFactors( archive );

    archive.EndSection(); // ModificateursDeDistance
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeAgent::~PHY_SensorTypeAgent()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeAngle
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeAngle( MIL_InputArchive& archive )
{
    archive.Section( "Angle" );
    archive.Read( rAngle_ );

    std::string strUnit;
    archive.ReadAttribute( "unite", strUnit );
    if ( sCaseInsensitiveEqual()( strUnit, "degre" ) )
        rAngle_ *= ( MT_PI / 180. );
    else if ( !sCaseInsensitiveEqual()( strUnit, "radian" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown angle unit", archive.GetContext() );

    archive.ReadAttribute( "balayage", bScanningAllowed_ );
    archive.EndSection(); // Angle
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeDistances
// Created: NLD 2004-08-06
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeDistances( MIL_InputArchive& archive )
{
    archive.ReadField( "DistanceDeProximite", rSquareProximityDist_ );
    rSquareProximityDist_ = MIL_Tools::ConvertMeterToSim( rSquareProximityDist_ );
    rSquareProximityDist_ *= rSquareProximityDist_;

    archive.Section( "DistancesDeBase" );
    archive.ReadField( "DI", rIdentificationDist_, CheckValueGreaterOrEqual( 0. ) );
    archive.ReadField( "DR", rRecognitionDist_   , CheckValueGreaterOrEqual( rIdentificationDist_ ) );
    archive.ReadField( "DD", rDetectionDist_     , CheckValueGreaterOrEqual( rRecognitionDist_ ) );
    archive.EndSection(); // DistancesDeBase

    rDetectionDist_      = MIL_Tools::ConvertMeterToSim( rDetectionDist_      );
    rRecognitionDist_    = MIL_Tools::ConvertMeterToSim( rRecognitionDist_    );
    rIdentificationDist_ = MIL_Tools::ConvertMeterToSim( rIdentificationDist_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeEnvironmentFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeEnvironmentFactors( MIL_InputArchive& archive )
{
    archive.Section( "Environnements" );

    for ( uint i = 0; i < PHY_RawVisionData::eNbrVisionObjects; ++i )
    {
        PHY_RawVisionData::E_VisionObject nEnv = ConvertObjectIdxToEnvironnement( i );
        assert( environmentFactors_.size() > i );
        MT_Float& rFactor = environmentFactors_[ i ];

        archive.ReadField( MIL_Tools::GetEnvironnementTypeName( nEnv ), rFactor, CheckValueBound( 0., 1. ) );
    }

    archive.EndSection(); // Environnements
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetSourceFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetSourceFactor( const MIL_AgentPion& source ) const
{
    const PHY_RolePion_Posture& sourcePosture = source.GetRole< PHY_RolePion_Posture >();

    const uint nOldPostureIdx = sourcePosture.GetLastPosture   ().GetID();
    const uint nCurPostureIdx = sourcePosture.GetCurrentPosture().GetID();

    assert( postureSourceFactors_.size() > nOldPostureIdx );
    assert( postureSourceFactors_.size() > nCurPostureIdx );


    const MT_Float rModificator =    postureSourceFactors_[ nOldPostureIdx ] + sourcePosture.GetPostureCompletionPercentage() 
                                 * ( postureSourceFactors_[ nCurPostureIdx ] - postureSourceFactors_[ nOldPostureIdx ] );
    return rModificator * sourcePosture.GetElongationFactor() * source.GetRole< PHY_RolePion_HumanFactors >().GetSensorDistanceModificator();
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetTargetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetTargetFactor( const MIL_Agent_ABC& target ) const
{
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

    const uint nOldPostureIdx = targetPosture.GetLastPosture   ().GetID();
    const uint nCurPostureIdx = targetPosture.GetCurrentPosture().GetID();

    assert( postureTargetFactors_.size() > nOldPostureIdx );
    assert( postureTargetFactors_.size() > nCurPostureIdx );
    return postureTargetFactors_[ nOldPostureIdx ] + targetPosture.GetPostureCompletionPercentage() * ( postureTargetFactors_[ nCurPostureIdx ] - postureTargetFactors_[ nOldPostureIdx ] );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetTargetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetTargetFactor( const DEC_Knowledge_Agent& target ) const
{
    const uint nOldPostureIdx = target.GetLastPosture   ().GetID();
    const uint nCurPostureIdx = target.GetCurrentPosture().GetID();

    assert( postureTargetFactors_.size() > nOldPostureIdx );
    assert( postureTargetFactors_.size() > nCurPostureIdx );
    return postureTargetFactors_[ nOldPostureIdx ] + target.GetPostureCompletionPercentage() * ( postureTargetFactors_[ nCurPostureIdx ] - postureTargetFactors_[ nOldPostureIdx ] );
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeEnvironementFactor
// Created: JVT 03-04-28
//-----------------------------------------------------------------------------
inline
MT_Float PHY_SensorTypeAgent::ComputeEnvironementFactor( PHY_RawVisionData::envBits nEnv ) const
{
    MT_Float res = nEnv & PHY_RawVisionData::eVisionEmpty ? environmentFactors_[ 0 ] : 1.;

    for( uint mask = 1, idx = 1; idx < PHY_RawVisionData::eNbrVisionObjects; mask <<= 1, ++idx )
        if( mask & nEnv )
            res *= environmentFactors_[ idx ];
    return res;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeExtinction
// Created: JVT 02-11-20
// Last modified: JVT 04-02-12
//-----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::ComputeExtinction( const PHY_RawVisionDataIterator& env, MT_Float rDistanceModificator, MT_Float rVisionNRJ ) const
{ 
    assert( rVisionNRJ <= rDetectionDist_ );
    assert( rVisionNRJ > 0 );

    // Prise en compte de l'éclairement
    rDistanceModificator *= lightingFactors_[ env.GetLighting().GetID() ];
    // Prise en compte des précipitations
    rDistanceModificator *= precipitationFactors_ [ env.GetPrecipitation().GetID() ];
    // Prise en compte des objets
    rDistanceModificator *= ComputeEnvironementFactor( env.GetCurrentEnv() );
    return rDistanceModificator <= MT_Epsilon ? -1. : rVisionNRJ - env.Length() / rDistanceModificator ;
}


//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InterpreteExtinction
// Created: JVT 02-07-12
// Last modified: JVT 03-01-27
//-----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& PHY_SensorTypeAgent::InterpreteExtinction( MT_Float rExtinction ) const
{
    if ( rExtinction >= rDetectionDist_ - rIdentificationDist_ )
        return PHY_PerceptionLevel::identified_;
    if ( rExtinction >= rDetectionDist_ - rRecognitionDist_ )
        return  PHY_PerceptionLevel::recognized_;
    if ( rExtinction >= 0 )
         return PHY_PerceptionLevel::detected_;
    return PHY_PerceptionLevel::notSeen_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::RayTrace
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::RayTrace( const MT_Vector2D& vSource, MT_Float rSourceAltitude, const MT_Vector2D& vTarget, MT_Float rTargetAltitude, MT_Float rDistanceMaxModificator ) const
{
    if( vSource.Distance( vTarget ) > GetMaxDistance() * rDistanceMaxModificator )
        return PHY_PerceptionLevel::notSeen_;

    const MT_Vector3D vSource3D( vSource.rX_, vSource.rY_, rSourceAltitude );
    const MT_Vector3D vTarget3D( vTarget.rX_, vTarget.rY_, rTargetAltitude );

    PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
    MT_Float rVisionNRJ = it.End() ? std::numeric_limits< MT_Float >::max() : ComputeExtinction( it, rDistanceMaxModificator, rDetectionDist_ );

    while ( rVisionNRJ > 0 && !(++it).End() )
        rVisionNRJ = ComputeExtinction( it, rDistanceMaxModificator, rVisionNRJ );

    return InterpreteExtinction( rVisionNRJ );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const MT_Vector2D& vTargetPos, MT_Float rSensorHeight ) const
{
    const MT_Float rDistanceMaxModificator = GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Float     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const MT_Float     rTargetAltitude = MIL_Tools::GetAltitude( vTargetPos ) + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-08-30
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const MIL_Agent_ABC& target, MT_Float rSensorHeight ) const
{
    const MT_Vector2D& vTargetPos = target.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( target.IsDead() )
        return ComputePerception( source, vTargetPos, rSensorHeight );

    const PHY_Volume* pSignificantVolume = target.GetRole< PHY_RoleInterface_Composantes >().GetSignificantVolume( *this );
    if ( !pSignificantVolume )
        return PHY_PerceptionLevel::notSeen_;

    MT_Float rDistanceMaxModificator = GetFactor     ( *pSignificantVolume );
    rDistanceMaxModificator *= GetTargetFactor( target );
    rDistanceMaxModificator *= GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Float     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const MT_Float     rTargetAltitude = target.GetRole< PHY_RoleInterface_Location >().GetAltitude() + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-09-07
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const DEC_Knowledge_Agent& target, MT_Float rSensorHeight ) const
{
    const MT_Vector2D& vTargetPos = target.GetPosition();

    if( target.IsDead() )
        return ComputePerception( source, vTargetPos, rSensorHeight );

    const PHY_Volume* pSignificantVolume = target.GetSignificantVolume( *this );
    if ( !pSignificantVolume )
        return PHY_PerceptionLevel::notSeen_;

    MT_Float rDistanceMaxModificator  = GetFactor( *pSignificantVolume );
             rDistanceMaxModificator *= GetTargetFactor( target );
             rDistanceMaxModificator *= GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Float     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const MT_Float     rTargetAltitude = target.GetAltitude() + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const MIL_PopulationConcentration& target, MT_Float /*rSensorHeight*/ ) const
{
    const MT_Float     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.Intersect2DWithCircle( vSourcePos, rDetectionDist_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerceptionAccuracy
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::ComputePerceptionAccuracy( const MIL_AgentPion& source, const MIL_PopulationFlow& /*target*/, MT_Float /*rSensorHeight*/ ) const
{
    const MT_Float rDistanceMaxModificator = GetSourceFactor( source );
    return rDetectionDist_ * rDistanceMaxModificator;
}
          
// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const MIL_PopulationFlow& target, MT_Float rSensorHeight, T_PointVector& shape ) const
{
    const MT_Float     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. )
        return PHY_PerceptionLevel::notSeen_;

    if( !target.Intersect2DWithCircle( vSourcePos, rDetectionDist_ * rDistanceMaxModificator, shape ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;    
}
