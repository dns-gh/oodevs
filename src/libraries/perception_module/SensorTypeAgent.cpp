// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SensorTypeAgent.h"
#include "RadarClass.h" // LTO
#include "PerceptionLevel.h"
#include "SensorType.h"
#include "MT_Tools/MT_Tools_Types.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include <xeumeuleu/xml.hpp>
#include <boost/range/algorithm/find.hpp>

using namespace sword;
using namespace sword::perception;

class PHY_Volume;

DECLARE_HOOK( IsUsingSpecializedActiveRadar, bool, ( const SWORD_Model* entity, const char* radarType ) )
DECLARE_HOOK( GetEnvironmentAssociation, size_t, ( const char* environment ) )
DECLARE_HOOK( ComputeRayTrace, double, ( const MT_Vector2D& source, double sourceAltitude, const MT_Vector2D& target, double targetAltitude,
                                         double maxDectectionDistance, double distanceMaxModificator,
                                         const double* urbanBlockFactors, const double* lightingFactors, const double* precipitationFactors,
                                         double(*FindEnvironmentFactor)( unsigned int environment, const void* userData ), const void* userData ) )
DECLARE_HOOK( GetAltitude, double, ( double x, double y ) )
DECLARE_HOOK( GetLastPostureIdentifier, size_t, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetCurrentPostureIdentifier, size_t, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetPostureCompletionPercentage, double, ( const SWORD_Model* entity ) )
DECLARE_HOOK( ComputePerceptionDistanceFactor, double, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetCollidingPopulationDensity, double, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetSignificantVolume, const PHY_Volume*, ( const SWORD_Model* entity, const double* volumeFactors ) )
DECLARE_HOOK( GetVolumeIdentifierFromInstance, size_t, ( const PHY_Volume* volume ) ) // $$$$ _RC_ SLI 2012-09-17: remove dependency on PHY_Volume
DECLARE_HOOK( PopulationElementIntersectWithCircle, bool, ( const SWORD_Model* element, MT_Vector2D circleCenter, double radius, void(*AddShapePoint)( MT_Vector2D point, void* userData ), void* userData ) )
DECLARE_HOOK( GetUrbanBlockFactor, double, ( const SWORD_Model* block, const double* urbanBlockFactors ) )
DECLARE_HOOK( IsMaterialType, bool, ( const char* material ) )
DECLARE_HOOK( GetPostureSize, size_t, () )
DECLARE_HOOK( GetVolumeSize, size_t, () )
DECLARE_HOOK( GetPrecipitationSize, size_t, () )
DECLARE_HOOK( GetLightingSize, size_t, () )
DECLARE_HOOK( GetMaterialTypeSize, size_t, () )
DECLARE_HOOK( GetVolumeIdentifier, bool, ( const char* type, size_t* identifier ) )
DECLARE_HOOK( GetPostureIdentifier, bool, ( const char* type, size_t* identifier ) )
DECLARE_HOOK( GetPrecipitationIdentifier, bool, ( const char* type, size_t* identifier ) )
DECLARE_HOOK( GetLightingIdentifier, bool, ( const char* type, size_t* identifier ) )
DECLARE_HOOK( PostureCanModifyDetection, bool, ( const char* type ) )

namespace
{
   template< typename Hook >
   class Loader : private boost::noncopyable
   {
   public:
        explicit Loader( Hook hook )
            : hook_( hook )
        {}

        void ReadFactor( xml::xistream& xis, SensorTypeAgent::T_FactorVector& factors )
        {
            std::string containerType;
            xis >> xml::attribute( "type", containerType );
            size_t identifier = 0;
            if( hook_( containerType.c_str(), &identifier ) )
            {
                assert( static_cast< int >( factors.size() ) > identifier );
                double& rFactor = factors[ identifier ];
                xis >> xml::attribute( "value", rFactor );
                if( rFactor < 0 || rFactor > 1 )
                    xis.error( "distance-modifier: value not in [0..1]" );
            }
            else
                xis.error( "distance-modifier: unknown type" );
        }
   private:
       Hook hook_;
    };

   class PostureLoader : private boost::noncopyable
   {
   public:
       void ReadFactor( xml::xistream& xis, SensorTypeAgent::T_FactorVector& factors )
       {
           std::string postureType;
           xis >> xml::attribute( "type", postureType );
           size_t identifier = 0;
           if( GET_HOOK( GetPostureIdentifier )( postureType.c_str(), &identifier ) )
           {
               if( !GET_HOOK( PostureCanModifyDetection )( postureType.c_str() ) )
                   return;
               assert( factors.size() > identifier );
               double& rFactor = factors[ identifier ];

               xis >> xml::attribute( "value", rFactor );
               if( rFactor < 0 || rFactor > 1 )
                   xis.error( "distance-modifier: value not in [0..1]" );
           }
           else
               xis.error( "distance-modifier: unknown type" );
       }
   };

    template< typename Hook >
    void InitializeFactors( Hook hook, const std::string& strTagName, SensorTypeAgent::T_FactorVector& factors, xml::xistream& xis )
    {
        Loader< Hook > loader( hook );
        xis >> xml::start( strTagName )
                >> xml::list( "distance-modifier", loader, &Loader< Hook >::ReadFactor, factors )
            >> xml::end;
    }
    void InitializePostureFactors( const std::string& strTagName, SensorTypeAgent::T_FactorVector& factors, xml::xistream& xis )
    {
        PostureLoader loader;
        xis >> xml::start( strTagName )
                >> xml::list( "distance-modifier", loader, &PostureLoader::ReadFactor, factors )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
SensorTypeAgent::SensorTypeAgent( const SensorType& type, xml::xistream& xis )
    : type_                ( type )
    , volumeFactors_       ( GET_HOOK( GetVolumeSize )(), 0. )
    , precipitationFactors_( GET_HOOK( GetPrecipitationSize )(), 0. )
    , lightingFactors_     ( GET_HOOK( GetLightingSize )(), 0. )
    , postureSourceFactors_( GET_HOOK( GetPostureSize )(), 0. )
    , postureTargetFactors_( GET_HOOK( GetPostureSize )(), 0. )
    , urbanBlockFactors_   ( GET_HOOK( GetMaterialTypeSize )(), 1. )
    , rPopulationDensity_  ( 1. )
    , rPopulationFactor_   ( 1. )
    , isLimitedToSensors_  ( false ) // LTO
{
    InitializeAngle           ( xis );
    InitializeLimitedToSensors( xis ); // LTO
    InitializeDistances       ( xis );

    xis >> xml::start( "distance-modifiers" );

    InitializeFactors( GET_HOOK( GetVolumeIdentifier ), "size-modifiers", volumeFactors_, xis );
    InitializeFactors( GET_HOOK( GetPrecipitationIdentifier ), "precipitation-modifiers", precipitationFactors_, xis );
    InitializeFactors( GET_HOOK( GetLightingIdentifier ), "visibility-modifiers", lightingFactors_, xis );
    InitializePostureFactors( "source-posture-modifiers", postureSourceFactors_, xis );
    InitializePostureFactors( "target-posture-modifiers", postureTargetFactors_, xis );
    InitializeEnvironmentFactors( xis );
    InitializePopulationFactors ( xis );
    InitializeUrbanBlockFactors( xis );

    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
SensorTypeAgent::~SensorTypeAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::InitializeAngle
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorTypeAgent::InitializeAngle( xml::xistream& xis )
{
    xis >> xml::attribute( "angle", rAngle_ )
        >> xml::attribute( "scanning", bScanningAllowed_ )
        >> xml::attribute( "firer-detection-distance", rRecognitionFirerDist_ );

    rAngle_ *= ( MT_PI / 180. );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::InitializeLimitedToSensors
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void SensorTypeAgent::InitializeLimitedToSensors( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "limited-to-sensors" )
            >> xml::list( "sensor", *this, &SensorTypeAgent::ReadLimitedToSensorsList )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::InitializeDistances
// Created: NLD 2004-08-06
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
void SensorTypeAgent::InitializeDistances( xml::xistream& xis )
{
    xis >> xml::start( "base-distances" )
            >> xml::attribute( "close-range", rSquareProximityDist_ );
    rSquareProximityDist_ *= rSquareProximityDist_;

    xis     >> xml::list( "base-distance", *this, &SensorTypeAgent::ReadDistance )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ReadDistance
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void SensorTypeAgent::ReadDistance( xml::xistream& xis )
{
    std::string distanceType;
    xis >> xml::attribute( "level", distanceType );
    if( distanceType == "identification" )
    {
        xis >> xml::attribute( "distance", rIdentificationDist_ );
        if( rIdentificationDist_ < 0 )
            xis.error( "base-distance: identification distance < 0" );
    }
    else if( distanceType == "recognition" )
    {
        xis >> xml::attribute( "distance", rRecognitionDist_ );
        if( rRecognitionDist_ < rIdentificationDist_ )
            xis.error( "base-distance: recognition distance < identification distance" );
    }
    else if( distanceType == "detection" )
    {
        xis >> xml::attribute( "distance", rDetectionDist_ );
        if( rDetectionDist_ < rRecognitionDist_ )
            xis.error( "base-distance: detection distance < recognition distance" );
    }
    else
        xis.error( "base-distance: unknow distance level" );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ReadLimitedToSensorsList
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void SensorTypeAgent::ReadLimitedToSensorsList( xml::xistream& xis )
{
    isLimitedToSensors_ = true;
    std::string sensorTypeString;
    xis >> xml::attribute( "name", sensorTypeString );
    if( sensorTypeString == "" )
        xis.error( "No sensor defined in limited-to-sensor list" );
    limitedToSensorsList_.push_back( sensorTypeString );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::InitializeEnvironmentFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorTypeAgent::InitializeEnvironmentFactors( xml::xistream& xis )
{
    xis >> xml::start( "terrain-modifiers" )
            >> xml::list( "distance-modifier", *this, &SensorTypeAgent::ReadTerrainModifier )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ReadTerrainModifier
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void SensorTypeAgent::ReadTerrainModifier( xml::xistream& xis )
{
    std::string terrainType;
    xis >> xml::attribute( "type", terrainType );
    double rFactor;
    xis >> xml::attribute( "value", rFactor );
    if( rFactor < 0 || rFactor > 1 )
        xis.error( "terrain-modifier: value not in [0..1]" );
    environmentFactors_.insert( std::pair< std::size_t, double >( GET_HOOK( GetEnvironmentAssociation )( terrainType.c_str() ), rFactor ) );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::InitializePopulationFactors
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
void SensorTypeAgent::InitializePopulationFactors( xml::xistream& xis )
{
    xis >> xml::start( "population-modifier" )
            >> xml::attribute( "density", rPopulationDensity_ )
            >> xml::attribute( "modifier", rPopulationFactor_ )
        >> xml::end;

    if( rPopulationDensity_ < 0 )
        xis.error( "population-modifier: density < 0" );
    if( rPopulationFactor_ < 0 || rPopulationFactor_ > 1 )
        xis.error( "population-modifier: modifier not in [0..1]" );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::InitializeUrbanBlockFactors
// Created: SLG 2010-03-03
// -----------------------------------------------------------------------------
void SensorTypeAgent::InitializeUrbanBlockFactors( xml::xistream& xis )
{
    unsigned int visionUrbanBlockMaterial = 0;
    xis >> xml::start( "urbanBlock-material-modifiers" )
            >> xml::list( "distance-modifier", *this, &SensorTypeAgent::ReadUrbanBlockModifier, visionUrbanBlockMaterial )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ReadUrbanBlockModifier
// Created: SLG 2010-03-03
// -----------------------------------------------------------------------------
void SensorTypeAgent::ReadUrbanBlockModifier( xml::xistream& xis, unsigned int& visionUrbanBlockMaterial )
{
    assert( urbanBlockFactors_.size() > visionUrbanBlockMaterial );  // $$$$ _RC_ ABL 2007-07-27: use exception instead
    double& rFactor = urbanBlockFactors_[ visionUrbanBlockMaterial ];
    std::string materialType;
    xis >> xml::attribute( "type", materialType )
        >> xml::attribute( "value", rFactor );
    if( rFactor < 0 || rFactor > 1 )
        xis.error( "urbanBlock-modifier: value not in [0..1]" );
    if( ! GET_HOOK( IsMaterialType )( materialType.c_str() ) )
        xis.error( "material type doesn't exist" );
    ++visionUrbanBlockMaterial;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetPopulationFactor
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
inline
double SensorTypeAgent::GetPopulationFactor( double rDensity ) const
{
    if( rDensity == 0. || rPopulationDensity_ == 0. )
        return 1.;
    return std::min( 1., rPopulationFactor_ * rPopulationDensity_ / rDensity );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetSourceFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double SensorTypeAgent::GetSourceFactor( const wrapper::View& source ) const
{
    const std::size_t nOldPostureIdx = GET_HOOK( GetLastPostureIdentifier )( source );
    const std::size_t nCurPostureIdx = GET_HOOK( GetCurrentPostureIdentifier )( source );

    assert( postureSourceFactors_.size() > nOldPostureIdx );
    assert( postureSourceFactors_.size() > nCurPostureIdx );

    double rModifier = postureSourceFactors_[ nOldPostureIdx ] + GET_HOOK( GetPostureCompletionPercentage )( source )
                       * ( postureSourceFactors_[ nCurPostureIdx ] - postureSourceFactors_[ nOldPostureIdx ] );
    rModifier *= GET_HOOK( ComputePerceptionDistanceFactor )( source );
    return rModifier * GetPopulationFactor( GET_HOOK( GetCollidingPopulationDensity )( source ) );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetTargetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double SensorTypeAgent::GetTargetFactor( const wrapper::View& target ) const
{
    if( IsLimitedToSensors( target ) )
        return 0;

    const std::size_t nOldPostureIdx = GET_HOOK( GetLastPostureIdentifier )( target );
    const std::size_t nCurPostureIdx = GET_HOOK( GetCurrentPostureIdentifier )( target );

    assert( postureTargetFactors_.size() > nOldPostureIdx );
    assert( postureTargetFactors_.size() > nCurPostureIdx );

    double rModifier = 1.;
    if( !target[ "is-surrendered" ] )
        rModifier = postureTargetFactors_[ nOldPostureIdx ] + GET_HOOK( GetPostureCompletionPercentage )( target ) * ( postureTargetFactors_[ nCurPostureIdx ] - postureTargetFactors_[ nOldPostureIdx ] );
    return rModifier * GetPopulationFactor( GET_HOOK( GetCollidingPopulationDensity )( target ) );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::IsLimitedToSensors
// Created: LDC 2012-06-21
// -----------------------------------------------------------------------------
bool SensorTypeAgent::IsLimitedToSensors( const wrapper::View& target ) const
{
    if( !isLimitedToSensors_ )
        return false;
    const wrapper::View& surfaces = target[ "perceptions/surfaces-agent" ];
    for( std::size_t surface = 0; surface < surfaces.GetSize(); ++surface )
    {
        const std::string sensorName = surfaces.GetElement( surface )[ "sensor-type-name" ];
        if( boost::find( limitedToSensorsList_, sensorName ) != limitedToSensorsList_.end() )
            return false;
    }
    RadarClass::T_RadarClassMap radarClasses = RadarClass::GetRadarClasses();
    for( RadarClass::CIT_RadarClassMap it = radarClasses.begin(); it != radarClasses.end(); ++it )
    {
        if( !GET_HOOK( IsUsingSpecializedActiveRadar )( target, it->first.c_str() ) )
            continue;
        const wrapper::View& radars = target[ "perceptions/radars" ][ it->first.c_str() ][ "list" ];
        for( std::size_t radar = 0; radar < radars.GetSize(); ++radar )
        {
            const std::string radarName = radars.GetElement( radar )[ "name" ];
            if( boost::find( limitedToSensorsList_, radarName ) != limitedToSensorsList_.end() )
                return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// Name: SensorTypeAgent::InterpreteExtinction
// Created: JVT 02-07-12
// Last modified: JVT 03-01-27
//-----------------------------------------------------------------------------
inline
const PerceptionLevel& SensorTypeAgent::InterpretExtinction( double rExtinction ) const
{
    if( rExtinction >= rDetectionDist_ - rIdentificationDist_ )
        return PerceptionLevel::identified_;
    if( rExtinction >= rDetectionDist_ - rRecognitionDist_ )
        return  PerceptionLevel::recognized_;
    if( rExtinction >= 0 )
         return PerceptionLevel::detected_;
    return PerceptionLevel::notSeen_;
}

namespace
{
     double FindEnvironmentFactor( unsigned int environment, const void* userData )
     {
         return static_cast< const SensorTypeAgent::T_FactorMap* >( userData )->find( environment )->second;
     }
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::RayTrace
// Created: LMT 2010-07-02
// -----------------------------------------------------------------------------
const double SensorTypeAgent::RayTrace( const MT_Vector2D& vSource , const MT_Vector2D& vTarget, double sensorHeight ) const
{
    if( vSource.Distance( vTarget ) > GetMaxDistance() )
        return 0.;
    const double sourceAltitude = sensorHeight + GET_HOOK( GetAltitude )( vSource.rX_, vSource.rY_ );
    const double targetAltitude = GET_HOOK( GetAltitude )( vTarget.rX_, vTarget.rY_ );
    return GET_HOOK( ComputeRayTrace )( vSource, sourceAltitude, vTarget, targetAltitude, rDetectionDist_, 1,
                                        &urbanBlockFactors_[0], &lightingFactors_[0], &precipitationFactors_[0], &FindEnvironmentFactor, &environmentFactors_ );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::RayTrace
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeAgent::RayTrace( const MT_Vector2D& vSource, double rSourceAltitude, const MT_Vector2D& vTarget, double rTargetAltitude, double rDistanceMaxModificator ) const
{
    if( vSource.Distance( vTarget ) > GetMaxDistance() * rDistanceMaxModificator )
        return PerceptionLevel::notSeen_;
    const double rVisionNRJ = GET_HOOK( ComputeRayTrace )( vSource, rSourceAltitude, vTarget, rTargetAltitude, rDetectionDist_, rDistanceMaxModificator,
                                                           &urbanBlockFactors_[0], &lightingFactors_[0], &precipitationFactors_[0], &FindEnvironmentFactor, &environmentFactors_ );
    return InterpretExtinction( rVisionNRJ );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ComputePointPerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeAgent::ComputePointPerception( const wrapper::View& source, const MT_Vector2D& vTargetPos, double rSensorHeight ) const
{
    const double rDistanceMaxModificator = GetSourceFactor( source );

    const MT_Vector2D vSourcePos( source[ "movement/position/x" ], source[ "movement/position/y" ] );
    const double     rSourceAltitude = static_cast< double >( source[ "movement/height" ] ) + rSensorHeight + GET_HOOK( GetAltitude )( vSourcePos.rX_, vSourcePos.rY_ );
    const double     rTargetAltitude = GET_HOOK( GetAltitude )( vTargetPos.rX_, vTargetPos.rY_ ) + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ComputeAgentPerception
// Created: NLD 2004-08-30
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeAgent::ComputeAgentPerception( const wrapper::View& source, const wrapper::View& target, const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos, double rSensorHeight ) const
{
    if( vSourcePos.SquareDistance( vTargetPos ) > rDetectionDist_ * rDetectionDist_ )
        return PerceptionLevel::notSeen_;

    if( target[ "is-dead" ] )
        return ComputePointPerception( source, vTargetPos, rSensorHeight );

    const PHY_Volume* pSignificantVolume = GET_HOOK( GetSignificantVolume )( target, &volumeFactors_[0] );
    if( !pSignificantVolume )
        return PerceptionLevel::notSeen_;

    double rDistanceMaxModificator  = GetFactor      ( GET_HOOK( GetVolumeIdentifierFromInstance )( pSignificantVolume ) );
           rDistanceMaxModificator *= GetTargetFactor( target );
           rDistanceMaxModificator *= GetSourceFactor( source );

    const double rSourceAltitude = static_cast< double >( source[ "movement/height" ] ) + rSensorHeight + GET_HOOK( GetAltitude )( vSourcePos.rX_, vSourcePos.rY_ );
    const double rTargetAltitude = static_cast< double >( target[ "movement/height" ] ) + 2             + GET_HOOK( GetAltitude )( vTargetPos.rX_, vTargetPos.rY_ );

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ComputeConcentrationPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeAgent::ComputeConcentrationPerception( const wrapper::View& source, const wrapper::View& target, double /*rSensorHeight*/ ) const
{
    const double     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D vSourcePos( source[ "movement/position/x" ], source[ "movement/position/y" ] );

    if( rDistanceMaxModificator == 0. )
        return PerceptionLevel::notSeen_;

    if( !GET_HOOK( PopulationElementIntersectWithCircle )( target, vSourcePos, rDetectionDist_ * rDistanceMaxModificator, 0, 0 ) )
        return PerceptionLevel::notSeen_;
    return PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ComputePerceptionAccuracy
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
double SensorTypeAgent::ComputePerceptionAccuracy( const wrapper::View& source, const wrapper::View& /*target*/, double /*rSensorHeight*/ ) const
{
    const double rDistanceMaxModificator = GetSourceFactor( source );
    return rDetectionDist_ * rDistanceMaxModificator;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ComputeFlowPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeAgent::ComputeFlowPerception( const wrapper::View& source, const wrapper::View& target, double /*rSensorHeight*/, std::vector< MT_Vector2D >& shape ) const
{
    const double     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D vSourcePos( source[ "movement/position/x" ], source[ "movement/position/y" ] );

    if( rDistanceMaxModificator == 0. )
        return PerceptionLevel::notSeen_;

    struct ShapeIntersection : private boost::noncopyable
    {
        explicit ShapeIntersection( std::vector< MT_Vector2D >& shape ) : shape_( shape ) {}
        static void AddPoint( MT_Vector2D point, void* userData )
        {
            static_cast< ShapeIntersection* >( userData )->shape_.push_back( point );
        }
        std::vector< MT_Vector2D >& shape_;
    } intersector( shape );
    if( !GET_HOOK( PopulationElementIntersectWithCircle )( target, vSourcePos, rDetectionDist_ * rDistanceMaxModificator, &ShapeIntersection::AddPoint, &intersector ) )
        return PerceptionLevel::notSeen_;
    return PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ComputeDistances
// Created: LDC 2012-01-04
// -----------------------------------------------------------------------------
void SensorTypeAgent::ComputeDistances( const wrapper::View& perceiver, const wrapper::View& target, double& identification, double& recognition, double& detection ) const
{
    double distanceModifier = ComputeDistanceModificator( perceiver, target );
    identification = rIdentificationDist_ * distanceModifier;
    recognition = rRecognitionDist_ * distanceModifier;
    detection = rDetectionDist_ * distanceModifier;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::ComputeDistanceModificator
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
const double SensorTypeAgent::ComputeDistanceModificator( const wrapper::View& perceiver, const wrapper::View& target ) const
{
    double rDistanceMaxModificator = 1.;
    if( const PHY_Volume* pSignificantVolume = GET_HOOK( GetSignificantVolume )( target, &volumeFactors_[0] ) )
        rDistanceMaxModificator  = GetFactor( GET_HOOK( GetVolumeIdentifierFromInstance )( pSignificantVolume ) );
    rDistanceMaxModificator *= GetTargetFactor( target );
    rDistanceMaxModificator *= GetSourceFactor( perceiver );
    return rDistanceMaxModificator;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetSquareProximityDistance
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
double SensorTypeAgent::GetSquareProximityDistance() const
{
    return rSquareProximityDist_;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetMaxDistance
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
double SensorTypeAgent::GetMaxDistance() const
{
    return rDetectionDist_;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetAngle
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
double SensorTypeAgent::GetAngle() const
{
    return rAngle_;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const SensorType& SensorTypeAgent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double SensorTypeAgent::GetFactor( size_t identifier ) const
{
    assert( volumeFactors_.size() > identifier );
    return volumeFactors_[ identifier ];
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetUrbanFactor
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
double SensorTypeAgent::GetUrbanBlockFactor( const wrapper::View& block ) const
{
    return GET_HOOK( ::GetUrbanBlockFactor )( block, &urbanBlockFactors_[0] );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::CanScan
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool SensorTypeAgent::CanScan() const
{
    return bScanningAllowed_;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeAgent::GetDelay
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
unsigned int SensorTypeAgent::GetDelay() const
{
    return type_.GetDelay();
}
