//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#include "clients_kernel_pch.h"
#include "SensorType.h"
#include "Tools.h"
#include "Agent_ABC.h"
#include "Architecture_ABC.h"
#include "Object_ABC.h"
#include "meteo/Meteo.h"
#include "meteo/PHY_Precipitation.h"
#include "meteo/PHY_Lighting.h"

#include <boost/optional.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SensorType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SensorType::SensorType( const std::string& name, xml::xistream& xis )
    : strName_             ( name )
    , postureSourceFactors_( eNbrUnitPosture, 0. )
{
    InitializeAngle    ( xis );
    InitializeDistances( xis );

    xis >> xml::start( "distance-modifiers" );
    InitializeEnvironnementFactors( xis );
    InitializeUrbanBlockMaterialFactors( xis );
    InitializePostureSourceFactors( xis );
    InitializePrecipitationFactors( xis );
    InitializeLightingFactors( xis );
    xis >> xml::end;
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
    xis >> xml::start( "terrain-modifiers" )
            >> xml::list( "distance-modifier", *this, &SensorType::ReadEnvironnementFactor )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializePostureSourceFactors
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void SensorType::InitializePostureSourceFactors( xml::xistream& xis )
{
    xis >> xml::start( "source-posture-modifiers" )
            >> xml::list( "distance-modifier", *this, &SensorType::ReadPostureFactor )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializePrecipitationFactors
// Created: LDC 2013-02-22
// -----------------------------------------------------------------------------
void SensorType::InitializePrecipitationFactors( xml::xistream& xis )
{
    xis >> xml::start( "precipitation-modifiers" )
            >> xml::list( "distance-modifier", *this, &SensorType::ReadPrecipitationModifier )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeLightingFactors
// Created: LDC 2013-02-22
// -----------------------------------------------------------------------------
void SensorType::InitializeLightingFactors( xml::xistream& xis )
{
    xis >> xml::start( "visibility-modifiers" )
            >> xml::list( "distance-modifier", *this, &SensorType::ReadLightingModifier )
        >> xml::end;
}

namespace
{
    void ReadStringFloatModifier( xml::xistream& xis, std::map< std::string, float >& map )
    {
        std::string type;
        float value;
        xis >> xml::attribute( "type", type )
            >> xml::attribute( "value", value );
        map[ type ] = value;
    }
}

// -----------------------------------------------------------------------------
// Name: SensorType::ReadLightingModifier
// Created: LDC 2013-02-22
// -----------------------------------------------------------------------------
void SensorType::ReadLightingModifier( xml::xistream& xis )
{
    ReadStringFloatModifier( xis, lightingFactors_ );
}

// -----------------------------------------------------------------------------
// Name: SensorType::ReadPrecipitationModifier
// Created: LDC 2013-02-22
// -----------------------------------------------------------------------------
void SensorType::ReadPrecipitationModifier( xml::xistream& xis )
{
    ReadStringFloatModifier( xis, precipitationFactors_ );
}

// -----------------------------------------------------------------------------
// Name: SensorType::ReadEnvironnementFactor
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void SensorType::ReadEnvironnementFactor( xml::xistream& xis )
{
    std::string type; float value;
    xis >> xml::attribute( "type", type )
        >> xml::attribute( "value", value );
    if( type == "Foret" )
        factorInForest_ = value;
    else if( type == "Urbain" )
        factorInTown_ = value;
    else if( type == "Sol" )
        factorInGround_ = value;
}

namespace
{
    std::map< std::string, unsigned int > postureMap;
    void InitPostureMap()
    {
        if( postureMap.empty() )
        {
            postureMap[ "Mouvement" ] = 0;
            postureMap[ "MouvementDiscret" ] = 1;
            postureMap[ "Arret" ] = 2;
            postureMap[ "PosteReflexe"] = 3;
            postureMap[ "Poste" ] = 4;
            postureMap[ "PosteAmenage" ] = 5;
            postureMap[ "PostePrepareGenie" ] = 6;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SensorType::ReadPostureFactor
// Created: LDC 2013-02-22
// -----------------------------------------------------------------------------
void SensorType::ReadPostureFactor( xml::xistream& xis )
{
    std::string type; float value;
    xis >> xml::attribute( "type", type )
        >> xml::attribute( "value", value );
    InitPostureMap();
    postureSourceFactors_[ postureMap[ type ] ] = value;
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeUrbanBlockMaterialFactors
// Created: SLG 2010-03-10
// -----------------------------------------------------------------------------
void SensorType::InitializeUrbanBlockMaterialFactors( xml::xistream& xis )
{
    xis >> xml::start( "urbanBlock-material-modifiers" )
        >> xml::list( "distance-modifier", *this, &SensorType::ReadUrbanBlockMaterialFactor )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorType::ReadUrbanBlockMaterialFactor
// Created: SLG 2010-03-10
// -----------------------------------------------------------------------------
void SensorType::ReadUrbanBlockMaterialFactor( xml::xistream& xis )
{
    std::string type; float value;
    xis >> xml::attribute( "type", type )
        >> xml::attribute( "value", value );
    urbanBlockFactors_.insert( make_pair( type, value ) );
}

// -----------------------------------------------------------------------------
// Name: SensorType::InitializeAngle
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorType::InitializeAngle( xml::xistream& xis )
{
    xis >> xml::attribute( "angle", rAngle_ )
        >> xml::attribute( "scanning", bScanningAllowed_ );
    rAngle_ *= ( std::acos( -1.f ) / 180.f );
}

// -----------------------------------------------------------------------------
// Name: PHY2_SensorTypeAgent::InitializeDistances
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorType::InitializeDistances( xml::xistream& xis )
{
    xis >> xml::start( "base-distances" )
            >> xml::attribute( "close-range", rSquareProximityDist_ )
            >> xml::list( "base-distance", *this, &SensorType::ReadDistance )
        >> xml::end;
    rSquareProximityDist_ *= rSquareProximityDist_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::ReadDistance
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void SensorType::ReadDistance( xml::xistream& xis )
{
    std::string level;
    float distance;
    xis >> xml::attribute( "level",    level )
        >> xml::attribute( "distance", distance );
    if( level == "identification" )
        rIdentificationDist_ = distance;
    else if( level == "recognition" )
        rRecognitionDist_ = distance;
    else if( level == "detection" )
        rDetectionDist_ = distance;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetMaxDistance
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
float SensorType::GetMaxDistance( float distanceModificator ) const
{
    return rDetectionDist_ * distanceModificator;
}

//-----------------------------------------------------------------------------
// Name: SensorType::ComputeEnvironmentFactor
// Created: JVT 03-04-28
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
float SensorType::ComputeEnvironmentFactor( bool inForest, bool inTown, bool inGround ) const
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
float SensorType::ComputeExtinction( float rDistanceModificator, float rCurrentNRJ,
    bool inForest, bool inTown, bool inGround, float distance,
    const boost::optional< std::string >& material,
    const weather::Meteo& weather ) const
{
    bool bIsAroundBU = false;
    bIsAroundBU = ComputeUrbanExtinction( rCurrentNRJ, distance, material, weather );
    if( rCurrentNRJ > 0 && !bIsAroundBU )
    {
        rDistanceModificator *= ComputeEnvironmentFactor( inForest, inTown, inGround ) * GetWeatherModifier( weather );
        return rDistanceModificator <= 1e-8 ? -1.f : rCurrentNRJ - distance / rDistanceModificator;
    }
    else
        return rCurrentNRJ;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetWeatherModifier
// Created: LDC 2013-02-22
// -----------------------------------------------------------------------------
float SensorType::GetWeatherModifier( const weather::Meteo& weather ) const
{
    auto it = precipitationFactors_.find( weather.GetPrecipitation().GetName() );
    return it != precipitationFactors_.end() ? it->second : 1;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetLightingFactor
// Created: LDC 2014-03-20
// -----------------------------------------------------------------------------
float SensorType::GetLightingFactor( const weather::Meteo& weather ) const
{
    auto it = lightingFactors_.find( weather.GetLighting().GetName() );
    return it != lightingFactors_.end() ? it->second : 1;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetName
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string SensorType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetAngle
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
float SensorType::GetAngle() const
{
    return rAngle_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetPostureSourceFactors
// Created: LDC 2013-02-22
// -----------------------------------------------------------------------------
const std::vector< float >& SensorType::GetPostureSourceFactors() const
{
    return postureSourceFactors_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::ComputeExtinction
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
float SensorType::ComputeExtinction( float distanceModificator,
    bool inForest, bool inTown, bool inGround, float distance,
    const boost::optional< std::string >& material,
    const weather::Meteo& weather, const weather::Meteo& targetWeather ) const
{
    float result = ComputeExtinction( distanceModificator, rDetectionDist_, inForest, inTown, inGround, distance, material, weather );
    result *= GetLightingFactor( targetWeather );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SensorType::InterpreteNRJ
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
E_PerceptionResult SensorType::InterpreteNRJ( float rNRJ ) const
{
    if( rNRJ >= rDetectionDist_ - rIdentificationDist_ )
        return eIdentification;
    if( rNRJ >= rDetectionDist_ - rRecognitionDist_ )
        return  eRecognition;
    if( rNRJ >= 0 )
        return eDetection;
    return eNotSeen;
}

// -----------------------------------------------------------------------------
// Name: SensorType::ComputeUrbanExtinction
// Created: SLG 2010-03-10
// -----------------------------------------------------------------------------
bool SensorType::ComputeUrbanExtinction( float& rVisionNRJ, float distance, const boost::optional< std::string >& material, const weather::Meteo& weather ) const
{
    bool bIsAroundBU = false;
    if( material )
    {
        bIsAroundBU = true;
        float rDistanceModificator = urbanBlockFactors_.find( *material )->second * GetWeatherModifier( weather );
        rVisionNRJ = rDistanceModificator <= 1e-8 ? -1 : rVisionNRJ - distance / rDistanceModificator ;
    }
    return bIsAroundBU;
}
