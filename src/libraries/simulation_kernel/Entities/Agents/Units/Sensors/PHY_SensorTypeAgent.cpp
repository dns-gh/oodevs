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

#include "simulation_kernel_pch.h"
#include "PHY_SensorTypeAgent.h"
#include "AlgorithmsFactories.h"
#include "DetectionComputerFactory_ABC.h"
#include "PerceptionDistanceComputer_ABC.h"
#include "PHY_MaterialCompositionType.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h" // LTO
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h" // LTO
#include "Entities/Agents/Units/Radars/PHY_RadarType.h" // LTO
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "meteo/PHY_Lighting.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/PHY_Precipitation.h"
#include "Meteo/RawVisionData/PHY_RawVisionDataIterator.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_Tools.h"
#include "tools/Resolver.h"
#include <geometry/Types.h>
#include <urban/PhysicalAttribute.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/Model.h>
#include <urban/TerrainObjectVisitor_ABC.h>
#include <xeumeuleu/xml.hpp>

namespace
{
   template< typename C >
   class Loader : private boost::noncopyable
   {
   public:
        explicit Loader( const C& container )
            : container_( container )
        {}

        void ReadFactor( xml::xistream& xis, PHY_SensorTypeAgent::T_FactorVector& factors )
        {
            std::string containerType;
            xis >> xml::attribute( "type", containerType );

            C::const_iterator it = container_.find( containerType );
            if( it != container_.end() )
            {
                assert( static_cast< int >( factors.size() ) > it->second->GetID() );
                double& rFactor = factors[ it->second->GetID() ];

                xis >> xml::attribute( "value", rFactor );
                if( rFactor < 0 || rFactor > 1 )
                    xis.error( "distance-modifier: value not in [0..1]" );
            }
            else
                xis.error( "distance-modifier: unknow type" );
        }
   private:
       const C& container_;
    };

   template<>
   class Loader< PHY_Posture::T_PostureMap > : private boost::noncopyable
   {
   public:
        explicit Loader( const PHY_Posture::T_PostureMap& container )
            : container_( container )
        {}

        void ReadFactor( xml::xistream& xis, PHY_SensorTypeAgent::T_FactorVector& factors )
        {
            std::string containerType;
            xis >> xml::attribute( "type", containerType );

            PHY_Posture::CIT_PostureMap it = container_.find( containerType );
            if( it != container_.end() )
            {
                if( !it->second->CanModifyDetection() )
                    return;
                assert( factors.size() > it->second->GetID() );
                double& rFactor = factors[ it->second->GetID() ];

                xis >> xml::attribute( "value", rFactor );
                if( rFactor < 0 || rFactor > 1 )
                    xis.error( "distance-modifier: value not in [0..1]" );
            }
            else
                xis.error( "distance-modifier: unknow type" );
        }

   private:
       const PHY_Posture::T_PostureMap& container_;
    };

    template< typename C >
    void InitializeFactors( const C& container, const std::string& strTagName, PHY_SensorTypeAgent::T_FactorVector& factors, xml::xistream& xis )
    {
        typedef typename Loader< C > T_Loader;
        T_Loader loader( container );
        xis >> xml::start( strTagName )
                >> xml::list( "distance-modifier", loader, &T_Loader::ReadFactor, factors )
            >> xml::end;
    }

    std::map< std::string, PHY_RawVisionData::E_VisionObject > environmentAssociation;
    void InitializeEnvironmentAssociation()
    {
        environmentAssociation[ "Sol"    ] = PHY_RawVisionData::eVisionGround;
        environmentAssociation[ "Vide"   ] = PHY_RawVisionData::eVisionEmpty;
        environmentAssociation[ "Foret"  ] = PHY_RawVisionData::eVisionForest;
        environmentAssociation[ "Urbain" ] = PHY_RawVisionData::eVisionUrban;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeAgent::PHY_SensorTypeAgent( const PHY_SensorType& type, xml::xistream& xis )
    : type_                ( type )
    , volumeFactors_       ( PHY_Volume       ::GetVolumes       ().size(), 0. )
    , precipitationFactors_( weather::PHY_Precipitation::GetPrecipitations().size(), 0. )
    , lightingFactors_     ( weather::PHY_Lighting     ::GetLightings     ().size(), 0. )
    , postureSourceFactors_( PHY_Posture      ::GetPostures      ().size(), 0. )
    , postureTargetFactors_( PHY_Posture      ::GetPostures      ().size(), 0. )
    , urbanBlockFactors_   ( PHY_MaterialCompositionType::Count(), 1. )
    , rPopulationDensity_  ( 1. )
    , rPopulationFactor_   ( 1. )
    , isLimitedToSensors_  ( false ) // LTO
{
    InitializeAngle           ( xis );
    InitializeLimitedToSensors( xis ); // LTO
    InitializeDistances       ( xis );

    xis >> xml::start( "distance-modifiers" );

    InitializeFactors( PHY_Volume       ::GetVolumes        (), "size-modifiers"  , volumeFactors_       , xis );
    InitializeFactors( weather::PHY_Precipitation::GetPrecipitations (), "precipitation-modifiers", precipitationFactors_, xis );
    InitializeFactors( weather::PHY_Lighting     ::GetLightings      (), "visibility-modifiers"  , lightingFactors_     , xis );
    InitializeFactors( PHY_Posture      ::GetPostures       (), "source-posture-modifiers", postureSourceFactors_, xis );
    InitializeFactors( PHY_Posture      ::GetPostures       (), "target-posture-modifiers" , postureTargetFactors_, xis );

    InitializeEnvironmentAssociation();
    InitializeEnvironmentFactors( xis );
    InitializePopulationFactors ( xis );
    InitializeUrbanBlockFactors( xis );

    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeAgent::~PHY_SensorTypeAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeAngle
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeAngle( xml::xistream& xis )
{
    xis >> xml::attribute( "angle", rAngle_ )
        >> xml::attribute( "scanning", bScanningAllowed_ )
        >> xml::attribute( "firer-detection-distance", rRecognitionFirerDist_ );

    rAngle_ *= ( MT_PI / 180. );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeLimitedToSensors
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeLimitedToSensors( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "limited-to-sensors" )
            >> xml::list( "sensor", *this, &PHY_SensorTypeAgent::ReadLimitedToSensorsList )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeDistances
// Created: NLD 2004-08-06
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeDistances( xml::xistream& xis )
{
    xis >> xml::start( "base-distances" )
            >> xml::attribute( "close-range", rSquareProximityDist_ );
    rSquareProximityDist_ = MIL_Tools::ConvertMeterToSim( rSquareProximityDist_ );
    rSquareProximityDist_ *= rSquareProximityDist_;

    xis     >> xml::list( "base-distance", *this, &PHY_SensorTypeAgent::ReadDistance )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ReadDistance
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::ReadDistance( xml::xistream& xis )
{
    std::string distanceType;
    xis >> xml::attribute( "level", distanceType );
    if( distanceType == "identification" )
    {
        xis >> xml::attribute( "distance", rIdentificationDist_ );
        if( rIdentificationDist_ < 0 )
            xis.error( "base-distance: identification distance < 0" );
        rIdentificationDist_ = MIL_Tools::ConvertMeterToSim( rIdentificationDist_ );
    }
    else if( distanceType == "recognition" )
    {
        xis >> xml::attribute( "distance", rRecognitionDist_ );
        if( rRecognitionDist_ < rIdentificationDist_ )
            xis.error( "base-distance: recognition distance < identification distance" );
        rRecognitionDist_ = MIL_Tools::ConvertMeterToSim( rRecognitionDist_ );
    }
    else if( distanceType == "detection" )
    {
        xis >> xml::attribute( "distance", rDetectionDist_ );
        if( rDetectionDist_ < rRecognitionDist_ )
            xis.error( "base-distance: detection distance < recognition distance" );
        rDetectionDist_ = MIL_Tools::ConvertMeterToSim( rDetectionDist_ );
    }
    else
        xis.error( "base-distance: unknow distance level" );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ReadLimitedToSensorsList
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::ReadLimitedToSensorsList( xml::xistream& xis )
{
    isLimitedToSensors_ = true;
    std::string sensorTypeString;
    xis >> xml::attribute( "name", sensorTypeString );
    if( sensorTypeString == "" )
        xis.error( "No sensor defined in limited-to-sensor list" );
    limitedToSensorsList_.push_back( sensorTypeString );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeEnvironmentFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeEnvironmentFactors( xml::xistream& xis )
{
    xis >> xml::start( "terrain-modifiers" )
            >> xml::list( "distance-modifier", *this, &PHY_SensorTypeAgent::ReadTerrainModifier )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ReadTerrainModifier
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::ReadTerrainModifier( xml::xistream& xis )
{
    std::string terrainType;
    xis >> xml::attribute( "type", terrainType );
    double rFactor;
    xis >> xml::attribute( "value", rFactor );
    if( rFactor < 0 || rFactor > 1 )
        xis.error( "terrain-modifier: value not in [0..1]" );
    environmentFactors_.insert( std::pair< unsigned, double >( environmentAssociation[ terrainType ], rFactor ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializePopulationFactors
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializePopulationFactors( xml::xistream& xis )
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
// Name: PHY_SensorTypeAgent::InitializeUrbanBlockFactors
// Created: SLG 2010-03-03
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeUrbanBlockFactors( xml::xistream& xis )
{
    unsigned int visionUrbanBlockMaterial = 0;
    xis >> xml::start( "urbanBlock-material-modifiers" )
        >> xml::list( "distance-modifier", *this, &PHY_SensorTypeAgent::ReadUrbanBlockModifier, visionUrbanBlockMaterial )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ReadUrbanBlockModifier
// Created: SLG 2010-03-03
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::ReadUrbanBlockModifier( xml::xistream& xis, unsigned int& visionUrbanBlockMaterial )
{
    assert( urbanBlockFactors_.size() > visionUrbanBlockMaterial );  // $$$$ _RC_ ABL 2007-07-27: use exception instead
    double& rFactor = urbanBlockFactors_[ visionUrbanBlockMaterial ];
    std::string materialType;
    xis >> xml::attribute( "type", materialType )
        >> xml::attribute( "value", rFactor );
    if( rFactor < 0 || rFactor > 1 )
        xis.error( "urbanBlock-modifier: value not in [0..1]" );
    if( !PHY_MaterialCompositionType::Find( materialType ) )
        xis.error( "material type doesn't exist" );
    ++visionUrbanBlockMaterial;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetPopulationFactor
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
inline
double PHY_SensorTypeAgent::GetPopulationFactor( double rDensity ) const
{
    if( rDensity == 0. || rPopulationDensity_ == 0. )
        return 1.;
    return std::min( 1., rPopulationFactor_ * rPopulationDensity_ / rDensity );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetSourceFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::GetSourceFactor( const MIL_Agent_ABC& source ) const
{
    // Posture
    const PHY_RoleInterface_Posture& sourcePosture = source.GetRole< PHY_RoleInterface_Posture >();

    const unsigned int nOldPostureIdx = sourcePosture.GetLastPosture   ().GetID();
    const unsigned int nCurPostureIdx = sourcePosture.GetCurrentPosture().GetID();

    assert( postureSourceFactors_.size() > nOldPostureIdx );
    assert( postureSourceFactors_.size() > nCurPostureIdx );

    double rModifier =   postureSourceFactors_[ nOldPostureIdx ] + sourcePosture.GetPostureCompletionPercentage()
                          * ( postureSourceFactors_[ nCurPostureIdx ] - postureSourceFactors_[ nOldPostureIdx ] );


    MIL_Agent_ABC& tempSource = const_cast< MIL_Agent_ABC& >( source );//@TODO MGD FIND A BETTER WAY
    std::auto_ptr< detection::PerceptionDistanceComputer_ABC > computer( tempSource.GetAlgorithms().detectionComputerFactory_->CreateDistanceComputer() );
    rModifier *= tempSource.Execute( *computer ).GetFactor();

    // Population
    const double rPopulationDensity = source.GetRole< PHY_RoleInterface_Population >().GetCollidingPopulationDensity();
    rModifier *= GetPopulationFactor( rPopulationDensity );

    return rModifier;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetTargetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::GetTargetFactor( const MIL_Agent_ABC& target ) const
{
    // LTO begin
    if( isLimitedToSensors_ && ContainsSensorFromLimitedList( target ) == false )
        return 0;
    // LTO end

    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

    const unsigned int nOldPostureIdx = targetPosture.GetLastPosture   ().GetID();
    const unsigned int nCurPostureIdx = targetPosture.GetCurrentPosture().GetID();

    assert( postureTargetFactors_.size() > nOldPostureIdx );
    assert( postureTargetFactors_.size() > nCurPostureIdx );
    double rModifier = postureTargetFactors_[ nOldPostureIdx ] + targetPosture.GetPostureCompletionPercentage() * ( postureTargetFactors_[ nCurPostureIdx ] - postureTargetFactors_[ nOldPostureIdx ] );

    // Population
    const double rPopulationDensity = target.GetRole< PHY_RoleInterface_Population >().GetCollidingPopulationDensity();
    return rModifier * GetPopulationFactor( rPopulationDensity );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetTargetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::GetTargetFactor( const DEC_Knowledge_Agent& target ) const
{
    // LTO begin
    if( isLimitedToSensors_ && ContainsSensorFromLimitedList( target.GetAgentKnown() ) == false )
        return 0;
    // LTO end

    const unsigned int nOldPostureIdx = target.GetLastPosture   ().GetID();
    const unsigned int nCurPostureIdx = target.GetCurrentPosture().GetID();

    assert( postureTargetFactors_.size() > nOldPostureIdx );
    assert( postureTargetFactors_.size() > nCurPostureIdx );
    double rModifier = postureTargetFactors_[ nOldPostureIdx ] + target.GetPostureCompletionPercentage() * ( postureTargetFactors_[ nCurPostureIdx ] - postureTargetFactors_[ nOldPostureIdx ] );

    return rModifier * GetPopulationFactor( target.GetPopulationDensity() );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ContainsSensorFromLimitedList
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
bool PHY_SensorTypeAgent::ContainsSensorFromLimitedList( const MIL_Agent_ABC& target ) const
{
    const PHY_RoleInterface_Perceiver& targetPerceiver = target.GetRole< PHY_RoleInterface_Perceiver >();

    const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = targetPerceiver.GetSurfacesAgent();
    for( PHY_RoleInterface_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const PHY_PerceptionSurfaceAgent& surface = itSurface->second;
        const std::string& sensorName = surface.GetSensorTypeName();

        for( std::vector< std::string >::const_iterator it = limitedToSensorsList_.begin(); it != limitedToSensorsList_.end(); ++it )
            if( *it == sensorName )
                return true;
    }

    PHY_RadarClass::T_RadarClassMap radarClasses = PHY_RadarClass::GetRadarClasses();
    for( PHY_RadarClass::CIT_RadarClassMap itRadarClass = radarClasses.begin(); itRadarClass != radarClasses.end(); ++itRadarClass )
    {
        if( targetPerceiver.IsUsingActiveRadar( *itRadarClass->second ) == false )
            continue;

        const PHY_RoleInterface_Perceiver::T_RadarSet& radars = const_cast< PHY_RoleInterface_Perceiver& >( targetPerceiver ).GetRadars( *itRadarClass->second );
        for( PHY_RoleInterface_Perceiver::CIT_RadarSet itRadar = radars.begin(); itRadar != radars.end(); ++itRadar )
        {
            const std::string& radarName = (*itRadar)->GetName();

            for( std::vector< std::string >::const_iterator it = limitedToSensorsList_.begin(); it != limitedToSensorsList_.end(); ++it )
            if( *it == radarName )
                return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeEnvironementFactor
// Created: JVT 03-04-28
//-----------------------------------------------------------------------------
double PHY_SensorTypeAgent::ComputeEnvironementFactor( PHY_RawVisionData::envBits nEnv ) const
{
    double res = nEnv & PHY_RawVisionData::eVisionEmpty ? environmentFactors_.find( 0 )->second : 1.;
    for( unsigned int mask = 1, idx = 1; idx < PHY_RawVisionData::eNbrVisionObjects; mask <<= 1, ++idx )
        if( mask & nEnv )
            res *= environmentFactors_.find( mask )->second;
    return res;
}

namespace
{
    const double epsilon = 1e-8;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeExtinction
// Created: JVT 02-11-20
// Last modified: JVT 04-02-12
//-----------------------------------------------------------------------------
double PHY_SensorTypeAgent::ComputeExtinction( const PHY_RawVisionDataIterator& env, double rDistanceModificator, double rVisionNRJ, bool bIsAroundBU ) const
{
    assert( rVisionNRJ <= rDetectionDist_ );
    assert( rVisionNRJ > 0 );
    rDistanceModificator *= lightingFactors_[ env.GetLighting().GetID() ];
    rDistanceModificator *= precipitationFactors_ [ env.GetPrecipitation().GetID() ];
    if( !bIsAroundBU )
        rDistanceModificator *= ComputeEnvironementFactor( env.GetCurrentEnv() );
    return rDistanceModificator <= epsilon ? -1. : rVisionNRJ - env.Length() / rDistanceModificator ;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeUrbanExtinction
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
bool PHY_SensorTypeAgent::ComputeUrbanExtinction( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double& rVisionNRJ ) const
{
    bool bIsAroundBU = false;

    geometry::Point2f vSourcePoint( static_cast< float >( vSource.rX_ ), static_cast< float >( vSource.rY_ ) );
    geometry::Point2f vTargetPoint( static_cast< float >( vTarget.rX_ ), static_cast< float >( vTarget.rY_ ) );

    std::set< const urban::TerrainObject_ABC* > list;
    MIL_AgentServer::GetWorkspace().GetUrbanModel().GetListWithinSegment( vSourcePoint, vTargetPoint, list );

    if( !list.empty() )
    {
        for( std::set< const urban::TerrainObject_ABC* >::const_iterator it = list.begin(); it != list.end() && rVisionNRJ > 0; it++ )
        {
            const urban::TerrainObject_ABC& object = **it;

            const urban::PhysicalAttribute* pPhysical = object.Retrieve< urban::PhysicalAttribute >();

            if( pPhysical == 0 || !pPhysical->GetArchitecture() )
                continue;

            const geometry::Polygon2f* footPrint = object.GetFootprint();
            std::vector< geometry::Point2f > intersectPoints = footPrint->Intersect( geometry::Segment2f( vSourcePoint, vTargetPoint ) );
            if( !intersectPoints.empty() || footPrint->IsInside( vSourcePoint ) || footPrint->IsInside( vTargetPoint ) )
            {
                bIsAroundBU = true;
                if( pPhysical && pPhysical->GetArchitecture() )
                {
                    float intersectionDistance = 0;
                    std::sort( intersectPoints.begin(), intersectPoints.end() );
                    if( intersectPoints.size() == 1 )
                    {
                        if( footPrint->IsInside( vSourcePoint ) )
                            intersectionDistance = vSourcePoint.Distance( *intersectPoints.begin() );
                        else if( footPrint->IsInside( vTargetPoint ) )
                            intersectionDistance = vTargetPoint.Distance( *intersectPoints.begin() );
                    }
                    else if( intersectPoints.empty() )
                        intersectionDistance = vSourcePoint.Distance( vTargetPoint );
                    else
                        intersectionDistance = ( *intersectPoints.begin() ).Distance( *intersectPoints.rbegin() );

                    double rDistanceModificator = urbanBlockFactors_[ PHY_MaterialCompositionType::Find( pPhysical->GetArchitecture()->GetMaterial() )->GetId() ];
                    double occupationFactor = std::sqrt( pPhysical->GetArchitecture()->GetOccupation() );
                    if( occupationFactor == 1. && rDistanceModificator <= epsilon )
                        rVisionNRJ = -1 ;
                    else
                    {
                        double referenceDistance = 200; // $$$$ LDC Hard coded 200m. reference distance
                        double distanceFactor = std::min( ( intersectionDistance / referenceDistance ) * occupationFactor * ( 1 - rDistanceModificator ), 1. );
                        rVisionNRJ -= rVisionNRJ * distanceFactor + intersectionDistance;
                    }
                }
            }
        }
    }

    return bIsAroundBU;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InterpreteExtinction
// Created: JVT 02-07-12
// Last modified: JVT 03-01-27
//-----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& PHY_SensorTypeAgent::InterpretExtinction( double rExtinction ) const
{
    if( rExtinction >= rDetectionDist_ - rIdentificationDist_ )
        return PHY_PerceptionLevel::identified_;
    if( rExtinction >= rDetectionDist_ - rRecognitionDist_ )
        return  PHY_PerceptionLevel::recognized_;
    if( rExtinction >= 0 )
         return PHY_PerceptionLevel::detected_;
    return PHY_PerceptionLevel::notSeen_;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::IdentificationDistance
// Created: DDA 10-03-24
//-----------------------------------------------------------------------------
const double PHY_SensorTypeAgent::IdentificationDistance() const
{
    return rIdentificationDist_;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ReconnoissanceDistance
// Created: GGE & PSN 10-04-20
//-----------------------------------------------------------------------------
const double PHY_SensorTypeAgent::ReconnoissanceDistance() const
{
    return rRecognitionDist_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::RayTrace
// Created: LMT 2010-07-02
// -----------------------------------------------------------------------------
const double PHY_SensorTypeAgent::RayTrace( const MT_Vector2D& vSource , const MT_Vector2D& vTarget, double sensorHeight ) const
{
    if( vSource.Distance( vTarget ) > GetMaxDistance() )
        return 0.;

    const MT_Vector3D vSource3D( vSource.rX_, vSource.rY_, sensorHeight + MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( vSource.rX_, vSource.rY_ ) );
    const MT_Vector3D vTarget3D( vTarget.rX_, vTarget.rY_, MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( vTarget.rX_, vTarget.rY_ ) );

    double rVisionNRJ = rDetectionDist_;
    bool bIsAroundBU = ComputeUrbanExtinction( vSource, vTarget, rVisionNRJ );

    PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
    if( rVisionNRJ > 0 )
        rVisionNRJ = it.End() ? std::numeric_limits< double >::max() : ComputeExtinction( it, 1, rVisionNRJ, bIsAroundBU );

    while ( rVisionNRJ > 0 && !(++it).End() )
        rVisionNRJ = ComputeExtinction( it, 1, rVisionNRJ, bIsAroundBU );

    return rVisionNRJ;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::RayTrace
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::RayTrace( const MT_Vector2D& vSource, double rSourceAltitude, const MT_Vector2D& vTarget, double rTargetAltitude, double rDistanceMaxModificator ) const
{
    if( vSource.Distance( vTarget ) > GetMaxDistance() * rDistanceMaxModificator )
        return PHY_PerceptionLevel::notSeen_;

    const MT_Vector3D vSource3D( vSource.rX_, vSource.rY_, rSourceAltitude );
    const MT_Vector3D vTarget3D( vTarget.rX_, vTarget.rY_, rTargetAltitude );

    double rVisionNRJ = rDetectionDist_;
    bool bIsAroundBU = ComputeUrbanExtinction( vSource, vTarget, rVisionNRJ );

    PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
    if( rVisionNRJ > 0 )
        rVisionNRJ = it.End() ? std::numeric_limits< double >::max() : ComputeExtinction( it, rDistanceMaxModificator, rVisionNRJ, bIsAroundBU );

    while ( rVisionNRJ > 0 && !(++it).End() )
        rVisionNRJ = ComputeExtinction( it, rDistanceMaxModificator, rVisionNRJ, bIsAroundBU );

    return InterpretExtinction( rVisionNRJ );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& source, const MT_Vector2D& vTargetPos, double rSensorHeight ) const
{
    const double rDistanceMaxModificator = GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const double     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const double     rTargetAltitude = MIL_Tools::GetAltitude( vTargetPos ) + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-08-30
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& source, const MIL_Agent_ABC& target, double rSensorHeight ) const
{
    const MT_Vector2D& vTargetPos = target.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( target.IsDead() )
        return ComputePerception( source, vTargetPos, rSensorHeight );

    const PHY_Volume* pSignificantVolume = target.GetRole< PHY_RoleInterface_Composantes >().GetSignificantVolume( *this );
    if( !pSignificantVolume )
        return PHY_PerceptionLevel::notSeen_;

    double rDistanceMaxModificator  = GetFactor      ( *pSignificantVolume );
             rDistanceMaxModificator *= GetTargetFactor( target );
             rDistanceMaxModificator *= GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const double     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const double     rTargetAltitude = target.GetRole< PHY_RoleInterface_Location >().GetAltitude() + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-09-07
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& source, const DEC_Knowledge_Agent& target, double rSensorHeight ) const
{
    const MT_Vector2D& vTargetPos = target.GetPosition();

    if( target.IsDead() )
        return ComputePerception( source, vTargetPos, rSensorHeight );

    const PHY_Volume* pSignificantVolume = target.GetSignificantVolume( *this );
    if( !pSignificantVolume )
        return PHY_PerceptionLevel::notSeen_;

    double rDistanceMaxModificator  = GetFactor      ( *pSignificantVolume );
             rDistanceMaxModificator *= GetTargetFactor( target );
             rDistanceMaxModificator *= GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const double     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const double     rTargetAltitude = target.GetAltitude() + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& source, const MIL_PopulationConcentration& target, double /*rSensorHeight*/ ) const
{
    const double     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.Intersect2DWithCircle( vSourcePos, rDetectionDist_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerceptionAccuracy
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::ComputePerceptionAccuracy( const MIL_Agent_ABC& source, const MIL_PopulationFlow& /*target*/, double /*rSensorHeight*/ ) const
{
    const double rDistanceMaxModificator = GetSourceFactor( source );
    return rDetectionDist_ * rDistanceMaxModificator;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& source, const MIL_PopulationFlow& target, double /*rSensorHeight*/, T_PointVector& shape ) const
{
    const double     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. )
        return PHY_PerceptionLevel::notSeen_;

    if( !target.Intersect2DWithCircle( vSourcePos, rDetectionDist_ * rDistanceMaxModificator, shape ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: MGD 2009-11-25
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& perceiver, const urban::TerrainObject_ABC& target, double rSensorHeight ) const
{
    geometry::Point2f barycenter = target.GetFootprint()->Barycenter();
    const MT_Vector2D vTargetPos( barycenter.X(), barycenter.Y() );

    double rDistanceMaxModificator  = GetFactor( *PHY_Volume::FindVolume( 0 ) );//@TODO MGD find a rule
    rDistanceMaxModificator *= GetSourceFactor( perceiver );

    const MT_Vector2D& vSourcePos      = perceiver.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const double     rSourceAltitude = perceiver.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const double     rTargetAltitude = MIL_Tools::GetAltitude( vTargetPos );//@TODO MGD Add height notion

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeIdentificationDist
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
const double PHY_SensorTypeAgent::ComputeIdentificationDist( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const
{
    return rIdentificationDist_ * ComputeDistanceModificator( perceiver, target );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeRecognitionDist
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
const double PHY_SensorTypeAgent::ComputeRecognitionDist( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const
{
    return rRecognitionDist_ * ComputeDistanceModificator( perceiver, target );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeDetectionDist
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
const double PHY_SensorTypeAgent::ComputeDetectionDist( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const
{
    return rDetectionDist_ * ComputeDistanceModificator( perceiver, target );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeDistanceModificator
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
const double PHY_SensorTypeAgent::ComputeDistanceModificator( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const
{
    double rDistanceMaxModificator = 1.;
    if( const PHY_Volume* pSignificantVolume = target.GetRole< PHY_RoleInterface_Composantes >().GetSignificantVolume( *this ) )
        rDistanceMaxModificator  = GetFactor( *pSignificantVolume );
    rDistanceMaxModificator *= GetTargetFactor( target );
    rDistanceMaxModificator *= GetSourceFactor( perceiver );
    return rDistanceMaxModificator;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetSquareProximityDistance
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::GetSquareProximityDistance() const
{
    return rSquareProximityDist_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetMaxDistance
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::GetMaxDistance() const
{
    return rDetectionDist_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetAngle
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::GetAngle() const
{
    return rAngle_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const PHY_SensorType& PHY_SensorTypeAgent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::GetFactor( const PHY_Volume& volume ) const
{
    assert( volumeFactors_.size() > volume.GetID() );
    return volumeFactors_[ volume.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetUrbanFactor
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
double PHY_SensorTypeAgent::GetUrbanBlockFactor( const urban::TerrainObject_ABC& block ) const
{
    const urban::PhysicalAttribute* pPhysical = block.Retrieve< urban::PhysicalAttribute >();
    if( pPhysical && pPhysical->GetArchitecture() )
        return urbanBlockFactors_[ PHY_MaterialCompositionType::Find( pPhysical->GetArchitecture()->GetMaterial() )->GetId() ];
    return 1.f;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::CanScan
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool PHY_SensorTypeAgent::CanScan() const
{
    return bScanningAllowed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::CanDetectFirer
// Created: SLG 2010-05-20
// -----------------------------------------------------------------------------
bool PHY_SensorTypeAgent::CanDetectFirer( double distance ) const
{
    return distance < rRecognitionFirerDist_;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ConvertEnvironementToObjectIdx
// Created: JVT 03-04-28
//-----------------------------------------------------------------------------
unsigned int PHY_SensorTypeAgent::ConvertEnvironementToObjectIdx( PHY_RawVisionData::E_VisionObject obj )
{
    if( obj == PHY_RawVisionData::eVisionEmpty )
        return 0;

    unsigned int res = 1;
    for ( unsigned int idx = 1; !( idx & obj ); idx <<= 1 )
        ++res;
    return res;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ConvertObjectIdxToEnvironnement
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
PHY_RawVisionData::E_VisionObject PHY_SensorTypeAgent::ConvertObjectIdxToEnvironnement( unsigned int val )
{
    return (PHY_RawVisionData::E_VisionObject)( val ? 1 << ( val - 1 ) : 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetDelay
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
unsigned int PHY_SensorTypeAgent::GetDelay() const
{
    return type_.GetDelay();
}
