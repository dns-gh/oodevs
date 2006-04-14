//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#include "astec_pch.h"

#include "SensorType.h"

#include "Net_Def.h"
#include "Tools.h"
#include "Attr_Def.h"
#include "Agent.h"
#include "Experience.h"
#include "Tiredness.h"
#include "Attributes.h"

#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: SensorType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SensorType::SensorType( const std::string& name, xml::xistream& xis )
    : strName_             ( name )
    , postureSourceFactors_( eNbrUnitPosture, 0. )
    , weatherFactors_      ( eNbrWeatherType, 0. )
    , lightingFactors_     ( eNbrLightingType, 0. )
{
    InitializeAngle    ( xis );
    InitializeDistances( xis );

    xis >> start( "ModificateursDeDistance" );

    InitializePostureSourceFactors( xis );
    InitializeWeatherFactors      ( xis );
    InitializeLightingFactors     ( xis );
    InitializeEnvironnementFactors( xis );
    InitializePopulationFactors   ( xis );
    
    xis >> end();
}

// -----------------------------------------------------------------------------
// Name: SensorType destructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
SensorType::~SensorType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeEnvironnementFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void SensorType::InitializeEnvironnementFactors( xml::xistream& xis )
{
    xis >> start( "Environnements" )
            >> content( "Foret", factorInForest_ )
            >> content( "Urbain", factorInTown_ )
            >> content( "Sol", factorInGround_ )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeWeatherFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void SensorType::InitializeWeatherFactors( xml::xistream& xis )
{
    xis >> start( "Precipitations" );
    for( unsigned i = 0; i < eNbrWeatherType; ++i )
    {
        float& rFactor = weatherFactors_[ i ];
        xis >> content( Tools::ConvertWeatherType( (E_WeatherType)i ), rFactor );
    }
    xis >> end();
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeLightingFactors
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void SensorType::InitializeLightingFactors( xml::xistream& xis )
{
    xis >> start( "Eclairements" );
    for( unsigned i = 0; i < eNbrLightingType; ++i )
    {
        float& rFactor = lightingFactors_[ i ];
        xis >> content( Tools::ConvertLightingType( (E_LightingType)i ), rFactor );
    }
    xis >> end();
}


// -----------------------------------------------------------------------------
// Name: SensorType::InitializeAngle
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorType::InitializeAngle( xml::xistream& xis )
{
    std::string unit;
    xis >> start( "Angle" ) 
            >> rAngle_
            >> attribute( "unite", unit )
            >> attribute( "balayage", bScanningAllowed_ )
        >> end();
    if ( sCaseInsensitiveEqual()( unit, "degre" ) )
        rAngle_ *= ( std::acos( -1.f ) / 180. );
}

// -----------------------------------------------------------------------------
// Name: PHY2_SensorTypeAgent::InitializeDistances
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorType::InitializeDistances( xml::xistream& xis )
{
    xis >> content( "DistanceDeProximite", rSquareProximityDist_ );
    rSquareProximityDist_ *= rSquareProximityDist_;

    xis >> start( "DistancesDeBase" )
            >> content( "DD", rDetectionDist_ )
            >> content( "DR", rRecognitionDist_ )
            >> content( "DI", rIdentificationDist_ )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializePopulationFactors
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void SensorType::InitializePopulationFactors( xml::xistream& xis )
{
    xis >> start( "PresencePopulation" )
            >> attribute( "densitePopulation", rPopulationDensity_ )
            >> attribute( "modificateur", rPopulationFactor_ )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializePostureSourceFactors
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void SensorType::InitializePostureSourceFactors( xml::xistream& xis )
{
    xis >> start( "PosturesSource" );
    for( unsigned i = 0; i < eNbrUnitPosture; ++i )
    {
        float& rFactor = postureSourceFactors_[ i ];
        xis >> content( Tools::ToString( (E_UnitPosture)i ).ascii(), rFactor );
    }
    xis >> end();
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetPostureSourceFactor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
float SensorType::GetPostureSourceFactor( const Agent& agent ) const
{
    return 1.f;
//    E_UnitPosture nOldPosture     = agent.GetOldStance();
//    E_UnitPosture nCurrentPosture = agent.GetStance();
//    float         rPourcentage    = agent.GetStanceCompletion() / 100.;
//
//    assert( postureSourceFactors_.size() > (unsigned)nOldPosture );
//    assert( postureSourceFactors_.size() > (unsigned)nCurrentPosture );
//    return postureSourceFactors_[ nOldPosture ] + rPourcentage * ( postureSourceFactors_[ nCurrentPosture ] - postureSourceFactors_[ nOldPosture ] );
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetDistanceModificator
// Created: NLD 2004-12-02
// -----------------------------------------------------------------------------
float SensorType::GetDistanceModificator( const Agent& agent ) const
{
//    const MT_Float rPopulationCollisionDensity = agent.GetPopulationCollisionDensity();
//          MT_Float rPopulationFactor = 1.;
//    if( rPopulationCollisionDensity != 0. )
//        rPopulationFactor = std::min( 1., rPopulationFactor_ * rPopulationDensity_ / rPopulationCollisionDensity );
//       
//    return    GetPostureSourceFactor( agent ) * agent.GetElongationFactor()
//            * agent.GetTiredness ().GetCoefSensorDistanceModificator()
//            * agent.GetExperience().GetCoefSensorDistanceModificator()
//            * rPopulationFactor;
    return 1.f;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetMaxDistance
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
float SensorType::GetMaxDistance( const Agent& agent ) const
{
    return rDetectionDist_ * GetDistanceModificator( agent );
}

//-----------------------------------------------------------------------------
// Name: SensorType::ComputeEnvironementFactor
// Created: JVT 03-04-28
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
inline
float SensorType::ComputeEnvironementFactor( bool inForest, bool inTown, bool inGround ) const
{
    float res = 1.f;
    if( inForest )
        res *= factorInForest_;
    if( inTown )
        res *= factorInTown_;
    if( inGround )
        res *= factorInGround_;
    return res;
}

// -----------------------------------------------------------------------------
// Name: SensorType::ComputeExtinction
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
float SensorType::ComputeExtinction( const Agent& srcAgent, float rCurrentNRJ, bool inForest, bool inTown, bool inGround, float distance  ) const
{
    assert( rCurrentNRJ <= rDetectionDist_ );
    assert( rCurrentNRJ > 0 );

    float rDistanceModificator = GetDistanceModificator( srcAgent );
//    rDistanceModificator *= lightingFactors_[ env.GetMeteo().GetLighting() ];
//    rDistanceModificator *= weatherFactors_ [ env.GetMeteo().GetWeather() ];
    rDistanceModificator *= ComputeEnvironementFactor( inForest, inTown, inGround );
    return rDistanceModificator <= MT_Epsilon ? -1. : rCurrentNRJ - distance / rDistanceModificator;
}

