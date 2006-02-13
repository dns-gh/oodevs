//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#include "astec_pch.h"

#include "SensorType.h"

#include "App.h"
#include "Net_Def.h"
#include "Tools.h"
#include "Attr_Def.h"
#include "World.h"
#include "Agent.h"
#include "Experience.h"
#include "Tiredness.h"

// -----------------------------------------------------------------------------
// Name: SensorType constructor
// Created: NLD 2004-09-10
// Modified: JVT 2004-09-27
// -----------------------------------------------------------------------------
SensorType::SensorType( const std::string& strName, InputArchive& archive )
    : strName_             ( strName )
    , postureSourceFactors_( eNbrUnitPosture, 0. )
    , weatherFactors_      ( eNbrWeatherType, 0. )
    , lightingFactors_     ( eNbrLightingType, 0. )
    , environementFactors_ ( RawVisionData::eNbrVisionObjects, 0. )
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
// Name: SensorType destructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
SensorType::~SensorType()
{
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeEnvironnementFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void SensorType::InitializeEnvironnementFactors( InputArchive& archive )
{
    archive.Section( "Environnements" );
    for ( uint idx = 0; idx < RawVisionData::eNbrVisionObjects; ++idx )
    {
        assert( environementFactors_.size() > idx );
        MT_Float& rFactor = environementFactors_[ idx ];

        if ( !archive.ReadField( Tools::ConvertEnvironementType( RawVisionData::ConvertObjectIdxToEnvironnement( idx ) ), rFactor ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        if ( rFactor < 0. || rFactor > 1. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    }

    archive.EndSection(); // Precipitations    

}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeWeatherFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void SensorType::InitializeWeatherFactors( InputArchive& archive )
{
    archive.Section( "Precipitations" );

    for( uint i = 0; i < eNbrWeatherType; ++i )
    {
        assert( weatherFactors_.size() > i );
        MT_Float& rFactor = weatherFactors_[ i ];

        archive.ReadField( Tools::ConvertWeatherType( (E_WeatherType)i ), rFactor );
        if ( rFactor < 0. || rFactor > 1. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    }

    archive.EndSection(); // Precipitations    
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeLightingFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void SensorType::InitializeLightingFactors( InputArchive& archive )
{
    archive.Section( "Eclairements" );
    for( uint i = 0; i < eNbrLightingType; ++i )
    {
        assert( lightingFactors_.size() > i );
        MT_Float& rFactor = lightingFactors_[ i ];

        archive.ReadField( Tools::ConvertLightingType( (E_LightingType)i ), rFactor );
        if ( rFactor < 0. || rFactor > 1. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    }

    archive.EndSection(); // Precipitations     
}


// -----------------------------------------------------------------------------
// Name: SensorType::InitializeAngle
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorType::InitializeAngle( InputArchive& archive )
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
void SensorType::InitializeDistances( InputArchive& archive )
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
// Name: SensorType::InitializePopulationFactors
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void SensorType::InitializePopulationFactors( InputArchive& archive )
{
    archive.Section( "PresencePopulation" );

    archive.ReadAttribute( "densitePopulation", rPopulationDensity_, CheckValueGreaterOrEqual( 0. ) );
    archive.ReadAttribute( "modificateur"     , rPopulationFactor_ , CheckValueBound( 0., 1. )      );

    archive.EndSection(); // PresencePopulation
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializePostureSourceFactors
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void SensorType::InitializePostureSourceFactors( InputArchive& archive )
{
    archive.Section( "PosturesSource" );
    for( uint i = 0; i < eNbrUnitPosture; ++i )
    {
        assert( postureSourceFactors_.size() > i );
        MT_Float& rFactor = postureSourceFactors_[ i ];
        const std::string strPosture = Tools::ToString( (E_UnitPosture)i ).ascii();

        archive.ReadField( strPosture, rFactor );
        if ( rFactor < 0. || rFactor > 1. )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    }

    archive.EndSection(); // PosturesSource
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetPostureSourceFactor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
MT_Float SensorType::GetPostureSourceFactor( const Agent& agent ) const
{
    E_UnitPosture nOldPosture     = agent.GetOldStance();
    E_UnitPosture nCurrentPosture = agent.GetStance();
    MT_Float      rPourcentage    = agent.GetStanceCompletion() / 100.;

    assert( postureSourceFactors_.size() > (uint)nOldPosture );
    assert( postureSourceFactors_.size() > (uint)nCurrentPosture );
    return postureSourceFactors_[ nOldPosture ] + rPourcentage * ( postureSourceFactors_[ nCurrentPosture ] - postureSourceFactors_[ nOldPosture ] );
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetDistanceModificator
// Created: NLD 2004-12-02
// -----------------------------------------------------------------------------
MT_Float SensorType::GetDistanceModificator( const Agent& agent ) const
{
    const MT_Float rPopulationCollisionDensity = agent.GetPopulationCollisionDensity();
          MT_Float rPopulationFactor = 1.;
    if( rPopulationCollisionDensity != 0. )
        rPopulationFactor = std::min( 1., rPopulationFactor_ * rPopulationDensity_ / rPopulationCollisionDensity );
       
    return    GetPostureSourceFactor( agent ) * agent.GetElongationFactor()
            * agent.GetTiredness ().GetCoefSensorDistanceModificator()
            * agent.GetExperience().GetCoefSensorDistanceModificator()
            * rPopulationFactor;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetMaxDistance
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
MT_Float SensorType::GetMaxDistance( const Agent& agent ) const
{
    return rDetectionDist_ * GetDistanceModificator( agent );
}

//-----------------------------------------------------------------------------
// Name: SensorType::ComputeEnvironementFactor
// Created: JVT 03-04-28
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
inline
MT_Float SensorType::ComputeEnvironementFactor( RawVisionData::envBits nEnv ) const
{
    MT_Float res = nEnv & RawVisionData::eVisionEmpty ? environementFactors_[ 0 ] : 1.;

    for( uint mask = 1, idx = 1; idx < RawVisionData::eNbrVisionObjects; mask <<= 1, ++idx )
        if( mask & nEnv )
            res *= environementFactors_[ idx ];
    return res;
}

// -----------------------------------------------------------------------------
// Name: SensorType::ComputeExtinction
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
MT_Float SensorType::ComputeExtinction( const RawVisionData::Iterator& env, const Agent& srcAgent, MT_Float rCurrentNRJ ) const
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
