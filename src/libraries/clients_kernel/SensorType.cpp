//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#include "clients_kernel_pch.h"
#include "SensorType.h"
#include "Tools.h"
#include "Agent_ABC.h"
#include "Architecture_ABC.h"
#include "Object_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SensorType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SensorType::SensorType( const std::string& name, xml::xistream& xis )
    : strName_             ( name )
    , postureSourceFactors_( eNbrUnitPosture, 0. )
    , weatherFactors_      ( eNbrWeatherType, 0. )
{
    InitializeAngle    ( xis );
    InitializeDistances( xis );

    xis >> xml::start( "distance-modifiers" );
    InitializeEnvironnementFactors( xis );
    InitializeUrbanBlockMaterialFactors( xis );
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
    std::string unit;
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
// Name: SensorType::GetPostureSourceFactor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
float SensorType::GetPostureSourceFactor( const Agent_ABC& /*agent*/ ) const
{
    return 1.f;
//    return agent.Get< Attributes_ABC >().ComputePostureFactor( postureSourceFactors_ );
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetDistanceModificator
// Created: NLD 2004-12-02
// -----------------------------------------------------------------------------
float SensorType::GetDistanceModificator( const Agent_ABC& agent ) const
{
    return GetPostureSourceFactor( agent );
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
float SensorType::ComputeExtinction( float rDistanceModificator, float rCurrentNRJ, bool inForest, bool inTown, bool inGround, float distance, const boost::optional< std::string >& material ) const
{
    bool bIsAroundBU = false;
    bIsAroundBU = ComputeUrbanExtinction( rCurrentNRJ, distance, material );
    if( rCurrentNRJ > 0 && !material )
    {
        rDistanceModificator *= ComputeEnvironementFactor( inForest, inTown, inGround );
        return rDistanceModificator <= 1e-8 ? -1.f : rCurrentNRJ - distance / rDistanceModificator;
    }
    else
        return rCurrentNRJ;
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
// Name: SensorType::ComputeExtinction
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
float SensorType::ComputeExtinction( float distanceModificator, bool inForest, bool inTown, bool inGround, float distance, const boost::optional< std::string >& material ) const
{
    return ComputeExtinction( distanceModificator, rDetectionDist_, inForest, inTown, inGround, distance, material );
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
bool SensorType::ComputeUrbanExtinction( float& rVisionNRJ, float distance, const boost::optional< std::string >& material ) const
{
    bool bIsAroundBU = false;
    if( material )
    {
        bIsAroundBU = true;
        float rDistanceModificator = urbanBlockFactors_.find( *material )->second;
        rDistanceModificator <= 1e-8 ? rVisionNRJ = -1 : rVisionNRJ = rVisionNRJ - distance / rDistanceModificator ;
    }
    return bIsAroundBU;
}