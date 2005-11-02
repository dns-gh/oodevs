//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_SensorType.h"

#include "MOS_App.h"
#include "MOS_Net_Def.h"
#include "MOS_Tools.h"
#include "MOS_Attr_Def.h"
#include "MOS_World.h"
#include "MOS_Agent.h"
#include "MOS_Experience.h"
#include "MOS_Tiredness.h"

// -----------------------------------------------------------------------------
// Name: MOS_SensorType constructor
// Created: NLD 2004-09-10
// Modified: JVT 2004-09-27
// -----------------------------------------------------------------------------
MOS_SensorType::MOS_SensorType( const std::string& strName, MOS_InputArchive& archive )
    : strName_             ( strName )
    , postureSourceFactors_( eNbrUnitPosture, 0. )
    , weatherFactors_      ( eNbrWeatherType, 0. )
    , lightingFactors_     ( eNbrLightingType, 0. )
    , environementFactors_ ( MOS_RawVisionData::eNbrVisionObjects, 0. )
{
    InitializeAngle    ( archive );
    InitializeDistances( archive );

    archive.Section( "ModificateursDeDistance" );

    InitializePostureSourceFactors( archive );
    InitializeWeatherFactors      ( archive );
    InitializeLightingFactors     ( archive );
    InitializeEnvironnementFactors( archive );
    InitializePopulationFactors   ( archive );
    
    archive.EndSection(); // ModificateursDeDistance
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType destructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
MOS_SensorType::~MOS_SensorType()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::InitializeEnvironnementFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void MOS_SensorType::InitializeEnvironnementFactors( MOS_InputArchive& archive )
{
    archive.Section( "Environnements" );
    for ( uint idx = 0; idx < MOS_RawVisionData::eNbrVisionObjects; ++idx )
    {
        assert( environementFactors_.size() > idx );
        MT_Float& rFactor = environementFactors_[ idx ];

        if ( !archive.ReadField( MOS_Tools::ConvertEnvironementType( MOS_RawVisionData::ConvertObjectIdxToEnvironnement( idx ) ), rFactor ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        if ( rFactor < 0. || rFactor > 1. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    }

    archive.EndSection(); // Precipitations    

}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::InitializeWeatherFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void MOS_SensorType::InitializeWeatherFactors( MOS_InputArchive& archive )
{
    archive.Section( "Precipitations" );

    for( uint i = 0; i < eNbrWeatherType; ++i )
    {
        assert( weatherFactors_.size() > i );
        MT_Float& rFactor = weatherFactors_[ i ];

        archive.ReadField( MOS_Tools::ConvertWeatherType( (E_WeatherType)i ), rFactor );
        if ( rFactor < 0. || rFactor > 1. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    }

    archive.EndSection(); // Precipitations    
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::InitializeLightingFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void MOS_SensorType::InitializeLightingFactors( MOS_InputArchive& archive )
{
    archive.Section( "Eclairements" );
    for( uint i = 0; i < eNbrLightingType; ++i )
    {
        assert( lightingFactors_.size() > i );
        MT_Float& rFactor = lightingFactors_[ i ];

        archive.ReadField( MOS_Tools::ConvertLightingType( (E_LightingType)i ), rFactor );
        if ( rFactor < 0. || rFactor > 1. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    }

    archive.EndSection(); // Precipitations     
}


// -----------------------------------------------------------------------------
// Name: MOS_SensorType::InitializeAngle
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void MOS_SensorType::InitializeAngle( MOS_InputArchive& archive )
{
    archive.Section( "Angle" );
    archive.Read( rAngle_ );
    std::string strUnit;
    archive.ReadAttribute( "unite", strUnit );
    if ( sCaseInsensitiveEqual()( strUnit, "degre" ) )
        rAngle_ *= ( MT_PI / 180. );
    else if ( !sCaseInsensitiveEqual()( strUnit, "radian" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    archive.ReadAttribute( "balayage", bScanningAllowed_ );
    archive.EndSection(); // Angle
}

// -----------------------------------------------------------------------------
// Name: PHY2_SensorTypeAgent::InitializeDistances
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void MOS_SensorType::InitializeDistances( MOS_InputArchive& archive )
{
    archive.ReadField( "DistanceDeProximite", rSquareProximityDist_ );
    rSquareProximityDist_ *= rSquareProximityDist_;

    archive.Section( "DistancesDeBase" );
    archive.ReadField( "DD", rDetectionDist_ );
    archive.ReadField( "DR", rRecognitionDist_ );
    archive.ReadField( "DI", rIdentificationDist_ );

    if ( rDetectionDist_ < rRecognitionDist_ || rRecognitionDist_ < rIdentificationDist_ || rIdentificationDist_ < 0. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    archive.EndSection(); // DistancesDeBase
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::InitializePopulationFactors
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void MOS_SensorType::InitializePopulationFactors( MOS_InputArchive& archive )
{
    archive.Section( "PresencePopulation" );

    archive.ReadAttribute( "densitePopulation", rPopulationDensity_, CheckValueGreaterOrEqual( 0. ) );
    archive.ReadAttribute( "modificateur"     , rPopulationFactor_ , CheckValueBound( 0., 1. )      );

    archive.EndSection(); // PresencePopulation
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::InitializePostureSourceFactors
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void MOS_SensorType::InitializePostureSourceFactors( MOS_InputArchive& archive )
{
    archive.Section( "PosturesSource" );
    for( uint i = 0; i < eNbrUnitPosture; ++i )
    {
        assert( postureSourceFactors_.size() > i );
        MT_Float& rFactor = postureSourceFactors_[ i ];
        const std::string strPosture = MOS_Tools::ToString( (E_UnitPosture)i ).ascii();

        archive.ReadField( strPosture, rFactor );
        if ( rFactor < 0. || rFactor > 1. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    }

    archive.EndSection(); // PosturesSource
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::GetPostureSourceFactor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
MT_Float MOS_SensorType::GetPostureSourceFactor( const MOS_Agent& agent ) const
{
    E_UnitPosture nOldPosture     = agent.GetOldStance();
    E_UnitPosture nCurrentPosture = agent.GetStance();
    MT_Float      rPourcentage    = agent.GetStanceCompletion() / 100.;

    assert( postureSourceFactors_.size() > (uint)nOldPosture );
    assert( postureSourceFactors_.size() > (uint)nCurrentPosture );
    return postureSourceFactors_[ nOldPosture ] + rPourcentage * ( postureSourceFactors_[ nCurrentPosture ] - postureSourceFactors_[ nOldPosture ] );
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::GetDistanceModificator
// Created: NLD 2004-12-02
// -----------------------------------------------------------------------------
MT_Float MOS_SensorType::GetDistanceModificator( const MOS_Agent& agent ) const
{
    const MT_Float rPopulationCollisionDensity = agent.GetPopulationCollisionDensity();
          MT_Float rPopulationFactor = 1.;
    if( rPopulationCollisionDensity != 0. )
        rPopulationFactor = min( 1., rPopulationFactor_ * rPopulationDensity_ / rPopulationCollisionDensity );
       
    return    GetPostureSourceFactor( agent ) * agent.GetElongationFactor()
            * agent.GetTiredness ().GetCoefSensorDistanceModificator()
            * agent.GetExperience().GetCoefSensorDistanceModificator()
            * rPopulationFactor;
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::GetMaxDistance
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
MT_Float MOS_SensorType::GetMaxDistance( const MOS_Agent& agent ) const
{
    return rDetectionDist_ * GetDistanceModificator( agent );
}

//-----------------------------------------------------------------------------
// Name: MOS_SensorType::ComputeEnvironementFactor
// Created: JVT 03-04-28
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
inline
MT_Float MOS_SensorType::ComputeEnvironementFactor( MOS_RawVisionData::envBits nEnv ) const
{
    MT_Float res = nEnv & MOS_RawVisionData::eVisionEmpty ? environementFactors_[ 0 ] : 1.;

    for( uint mask = 1, idx = 1; idx < MOS_RawVisionData::eNbrVisionObjects; mask <<= 1, ++idx )
        if( mask & nEnv )
            res *= environementFactors_[ idx ];
    return res;
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::ComputeExtinction
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
MT_Float MOS_SensorType::ComputeExtinction( const MOS_RawVisionData::Iterator& env, const MOS_Agent& srcAgent, MT_Float rCurrentNRJ ) const
{
    assert( rCurrentNRJ <= rDetectionDist_ );
    assert( rCurrentNRJ > 0 );

    MT_Float rDistanceModificator = GetDistanceModificator( srcAgent );
    // Prise en compte de l'éclairement
    rDistanceModificator *= lightingFactors_[ env.GetMeteo().GetLighting() ];
    // Prise en compte des précipitations
    rDistanceModificator *= weatherFactors_ [ env.GetMeteo().GetWeather() ];
    // Prise en compte des objets
    rDistanceModificator *= ComputeEnvironementFactor( env.GetCurrentEnv() );

    return rDistanceModificator <= MT_Epsilon ? -1. : rCurrentNRJ - env.Length() / rDistanceModificator;
}
