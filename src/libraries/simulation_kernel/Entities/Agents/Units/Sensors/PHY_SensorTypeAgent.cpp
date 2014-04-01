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
#include "PHY_SensorTypeObjectData.h"
#include "MIL_AgentServer.h"
#include "PerceptionDistanceComputer.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h" // LTO
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h" // LTO
#include "Entities/Agents/Units/Radars/PHY_RadarType.h" // LTO
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionDataIterator.h"
#include "Tools/MIL_Tools.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include <tools/Resolver.h>
#include <boost/range/algorithm.hpp>

namespace
{
    template< typename C >
    void InitializeFactors( const C& container, const std::string& strTagName, PHY_SensorTypeAgent::T_FactorVector& factors, xml::xistream& xis )
    {
        auto values = ReadDistanceModifiers( xis, strTagName );
        for( auto it = values.cbegin(); it != values.cend(); ++it )
        {
            auto ic = container.find( it->first );
            if( ic == container.end() )
                throw MASA_EXCEPTION( xis.context()
                        + "distance-modifier: unknown type: " + it->first );
            factors[ ic->second->GetID() ] = it->second;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeAgent::PHY_SensorTypeAgent( const PHY_SensorType& type, xml::xistream& xis )
    : type_                 ( type )
    , rAngle_               ( 0 )
    , bScanningAllowed_     ( false )
    , rRecognitionFirerDist_( 0 )
    , rSquareProximityDist_ ( 0 )
    , rIdentificationDist_  ( 0 )
    , rRecognitionDist_     ( 0 )
    , rDetectionDist_       ( 0 )
    , volumeFactors_        ( PHY_Volume::GetVolumes().size(), 0. )
    , precipitationFactors_ ( weather::PHY_Precipitation::GetPrecipitations().size(), 0. )
    , lightingFactors_      ( weather::PHY_Lighting::GetLightings().size(), 0. )
    , postureSourceFactors_ ( PHY_Posture::GetPostureCount(), 0. )
    , postureTargetFactors_ ( PHY_Posture::GetPostureCount(), 0. )
    , urbanBlockFactors_    ( PHY_MaterialCompositionType::Count(), 1. )
    , rPopulationDensity_   ( 1. )
    , rPopulationFactor_    ( 1. )
    , isLimitedToSensors_   ( false ) // LTO
{
    InitializeAngle           ( xis );
    InitializeLimitedToSensors( xis ); // LTO
    InitializeDistances       ( xis );

    xis >> xml::start( "distance-modifiers" );

    InitializeFactors( PHY_Volume::GetVolumes(), "size-modifiers", volumeFactors_, xis );
    InitializeFactors( weather::PHY_Precipitation::GetPrecipitations (), "precipitation-modifiers", precipitationFactors_, xis );
    InitializeFactors( weather::PHY_Lighting::GetLightings(), "visibility-modifiers", lightingFactors_, xis );
    ReadPostureFactors( xis, "source-posture-modifiers", postureSourceFactors_ );
    ReadPostureFactors( xis, "target-posture-modifiers", postureTargetFactors_ );
    InitializeEnvironmentFactors( xis );
    InitializePopulationFactors( xis );
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
            throw MASA_EXCEPTION( xis.context() + "base-distance: identification distance < 0" );
        rIdentificationDist_ = MIL_Tools::ConvertMeterToSim( rIdentificationDist_ );
    }
    else if( distanceType == "recognition" )
    {
        xis >> xml::attribute( "distance", rRecognitionDist_ );
        if( rRecognitionDist_ < rIdentificationDist_ )
            throw MASA_EXCEPTION( xis.context() + "base-distance: recognition distance < identification distance" );
        rRecognitionDist_ = MIL_Tools::ConvertMeterToSim( rRecognitionDist_ );
    }
    else if( distanceType == "detection" )
    {
        xis >> xml::attribute( "distance", rDetectionDist_ );
        if( rDetectionDist_ < rRecognitionDist_ )
            throw MASA_EXCEPTION( xis.context() + "base-distance: detection distance < recognition distance" );
        rDetectionDist_ = MIL_Tools::ConvertMeterToSim( rDetectionDist_ );
    }
    else
        throw MASA_EXCEPTION( xis.context() + "base-distance: unknow distance level" );
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
        throw MASA_EXCEPTION( xis.context() + "No sensor defined in limited-to-sensor list" );
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
        throw MASA_EXCEPTION( xis.context() + "terrain-modifier: value not in [0..1]" );
    environmentFactors_.insert( std::pair< unsigned, double >( PHY_RawVisionData::environmentAssociation_[ terrainType ], rFactor ) );
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
        throw MASA_EXCEPTION( xis.context() + "population-modifier: density < 0" );
    if( rPopulationFactor_ < 0 || rPopulationFactor_ > 1 )
        throw MASA_EXCEPTION( xis.context() + "population-modifier: modifier not in [0..1]" );
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
        throw MASA_EXCEPTION( xis.context() + "urbanBlock-modifier: value not in [0..1]" );
    if( !PHY_MaterialCompositionType::Find( materialType ) )
        throw MASA_EXCEPTION( xis.context() + "material type doesn't exist" );
    ++visionUrbanBlockMaterial;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetPopulationFactor
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
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
    detection::PerceptionDistanceComputer computer;
    rModifier *= tempSource.Execute( computer ).GetFactor();

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
    if( IsLimitedToSensors( target ) )
        return 0;

    double rModifier = 1.;
    if( !target.GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
    {
        const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

        const unsigned int nOldPostureIdx = targetPosture.GetLastPosture   ().GetID();
        const unsigned int nCurPostureIdx = targetPosture.GetCurrentPosture().GetID();

        assert( postureTargetFactors_.size() > nOldPostureIdx );
        assert( postureTargetFactors_.size() > nCurPostureIdx );
        rModifier = postureTargetFactors_[ nOldPostureIdx ] + targetPosture.GetPostureCompletionPercentage() * ( postureTargetFactors_[ nCurPostureIdx ] - postureTargetFactors_[ nOldPostureIdx ] );
    }

    // Population
    const double rPopulationDensity = target.GetRole< PHY_RoleInterface_Population >().GetCollidingPopulationDensity();
    return rModifier * GetPopulationFactor( rPopulationDensity );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::IsLimitedToSensors
// Created: LDC 2012-06-21
// -----------------------------------------------------------------------------
bool PHY_SensorTypeAgent::IsLimitedToSensors( const MIL_Agent_ABC& target ) const
{
    if( !isLimitedToSensors_ )
        return false;
    const PHY_RoleInterface_Perceiver& targetPerceiver = target.GetRole< PHY_RoleInterface_Perceiver >();
    const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = targetPerceiver.GetSurfacesAgent();
    for( auto it = surfaces.begin(); it != surfaces.end(); ++it )
        if( boost::find( limitedToSensorsList_, it->second.GetSensorTypeName() ) != limitedToSensorsList_.end() )
            return false;

    PHY_RadarClass::T_RadarClassMap radarClasses = PHY_RadarClass::GetRadarClasses();
    for( auto itRadarClass = radarClasses.begin(); itRadarClass != radarClasses.end(); ++itRadarClass )
    {
        if( targetPerceiver.IsUsingActiveRadar( *itRadarClass->second ) == false )
            continue;
        const PHY_RoleInterface_Perceiver::T_RadarMap& radars = const_cast< PHY_RoleInterface_Perceiver& >( targetPerceiver ).GetRadars( *itRadarClass->second );
        for( auto it = radars.begin(); it != radars.end(); ++it )
            if( boost::find( limitedToSensorsList_, it->first->GetName() ) != limitedToSensorsList_.end() )
                return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeEnvironmentFactor
// Created: JVT 03-04-28
//-----------------------------------------------------------------------------
double PHY_SensorTypeAgent::ComputeEnvironmentFactor( envBits nEnv ) const
{
    double res = nEnv & PHY_RawVisionData::eVisionEmpty ? environmentFactors_.find( 0 )->second : 1.;
    for( unsigned int mask = 1, idx = 1; idx < PHY_RawVisionData::eNbrVisionObject; mask <<= 1, ++idx )
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
    assert( rVisionNRJ >= 0 );
    rDistanceModificator *= precipitationFactors_ [ env.GetPrecipitation().GetID() ];
    if( !bIsAroundBU )
        rDistanceModificator *= ComputeEnvironmentFactor( env.GetCurrentEnv() );
    return rDistanceModificator <= epsilon ? -1. : rVisionNRJ - env.Length() / rDistanceModificator ;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeUrbanExtinction
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
bool PHY_SensorTypeAgent::ComputeUrbanExtinction( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double& rVisionNRJ, bool posted ) const
{
    bool bIsAroundBU = false;

    std::vector< const MIL_UrbanObject_ABC* > list;
    MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocksWithinSegment( vSource, vTarget, list );

    if( !list.empty() )
    {
        for( std::vector< const MIL_UrbanObject_ABC* >::const_iterator it = list.begin(); it != list.end() && rVisionNRJ >= 0; ++it )
        {
            const MIL_UrbanObject_ABC& object = **it;
            const UrbanPhysicalCapacity* pPhysical = object.Retrieve< UrbanPhysicalCapacity >();
            if( pPhysical == 0 )
                continue;
            const PHY_MaterialCompositionType* materialCompositionType = PHY_MaterialCompositionType::Find( pPhysical->GetMaterial() );
            if( !materialCompositionType )
                continue;

            const TER_Localisation& footPrint = object.GetLocalisation();

            TER_DistanceLess cmp ( vSource );
            T_PointSet intersectPoints( cmp );
            if( footPrint.IsInside( vSource ) || footPrint.IsInside( vTarget ) || footPrint.Intersect2D( MT_Line( vSource, vTarget ), intersectPoints ) )
            {
                bIsAroundBU = true;
                double intersectionDistance = 0;
                if( intersectPoints.size() == 1 )
                {
                    if( footPrint.IsInside( vSource ) )
                        intersectionDistance = posted ? 0 : vSource.Distance( *intersectPoints.begin() );
                    else if( footPrint.IsInside( vTarget ) )
                        intersectionDistance = vTarget.Distance( *intersectPoints.begin() );
                }
                else if( intersectPoints.empty() )
                    intersectionDistance = posted ? 0 : vSource.Distance( vTarget );
                else
                    intersectionDistance = ( *intersectPoints.begin() ).Distance( *intersectPoints.rbegin() );
                if( intersectionDistance == 0 )
                    continue;
                double rDistanceModificator = urbanBlockFactors_[ materialCompositionType->GetId() ];
                double occupationFactor = std::sqrt( pPhysical->GetOccupation() );
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
    return bIsAroundBU;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InterpreteExtinction
// Created: JVT 02-07-12
// Last modified: JVT 03-01-27
//-----------------------------------------------------------------------------
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
const double PHY_SensorTypeAgent::RayTrace( const MT_Vector2D& vSource , const MT_Vector2D& vTarget, double sensorHeight, bool posted ) const
{
    if( vSource.Distance( vTarget ) > GetMaxDistance() )
        return 0.;

    const MT_Vector3D vSource3D( vSource.rX_, vSource.rY_, sensorHeight + MIL_Tools::GetAltitude( vSource ) );
    const MT_Vector3D vTarget3D( vTarget.rX_, vTarget.rY_, MIL_Tools::GetAltitude( vTarget ) );

    double rVisionNRJ = rDetectionDist_;
    const auto& data = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData();
    rVisionNRJ *= lightingFactors_[ data.GetLighting( data( vTarget ) ).GetID() ];
    bool bIsAroundBU = ComputeUrbanExtinction( vSource, vTarget, rVisionNRJ, posted );

    PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
    if( rVisionNRJ >= 0 )
        rVisionNRJ = it.End() ? std::numeric_limits< double >::max() : ComputeExtinction( it, 1, rVisionNRJ, bIsAroundBU );

    while( rVisionNRJ >= 0 && !(++it).End() )
        rVisionNRJ = ComputeExtinction( it, 1, rVisionNRJ, bIsAroundBU );

    return rVisionNRJ;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::RayTrace
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::RayTrace( const MT_Vector2D& vSource, double rSourceAltitude, const MT_Vector2D& vTarget, double rTargetAltitude, double rDistanceMaxModificator, bool posted ) const
{
    if( vSource.Distance( vTarget ) > GetMaxDistance() * rDistanceMaxModificator )
        return PHY_PerceptionLevel::notSeen_;

    const MT_Vector3D vSource3D( vSource.rX_, vSource.rY_, rSourceAltitude );
    const MT_Vector3D vTarget3D( vTarget.rX_, vTarget.rY_, rTargetAltitude );

    double rVisionNRJ = rDetectionDist_;
    const auto& data = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData();
    rVisionNRJ *= lightingFactors_[ data.GetLighting( data( vTarget ) ).GetID() ];
    bool bIsAroundBU = ComputeUrbanExtinction( vSource, vTarget, rVisionNRJ, posted );

    PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
    if( rVisionNRJ >= 0 )
        rVisionNRJ = it.End() ? std::numeric_limits< double >::max() : ComputeExtinction( it, rDistanceMaxModificator, rVisionNRJ, bIsAroundBU );

    while( rVisionNRJ >= 0 && !(++it).End() )
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

    const MT_Vector2D& vSourcePos    = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const double     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const double     rTargetAltitude = MIL_Tools::GetAltitude( vTargetPos ) + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator, IsPosted( source ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-08-30
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& source, const MIL_Agent_ABC& target, double rSensorHeight ) const
{
    const PHY_RoleInterface_Location& targetRole = target.GetRole< PHY_RoleInterface_Location >();
    const MT_Vector2D& vTargetPos = targetRole.GetPosition();

    if( target.IsDead() )
        return ComputePerception( source, vTargetPos, rSensorHeight );

    const PHY_Volume* pSignificantVolume = target.GetRole< PHY_RoleInterface_Composantes >().GetSignificantVolume( *this );
    if( !pSignificantVolume )
        return PHY_PerceptionLevel::notSeen_;

    double rDistanceMaxModificator  = GetFactor      ( *pSignificantVolume );
           rDistanceMaxModificator *= GetTargetFactor( target );
           rDistanceMaxModificator *= GetSourceFactor( source );

    const MT_Vector2D vSourcePos      = source.GetRole< PHY_RoleInterface_UrbanLocation >().GetFirerPosition( vTargetPos );
    const double      rSourceAltitude = MIL_Tools::GetAltitude( vSourcePos ) + source.GetRole< PHY_RoleInterface_Location >().GetHeight() + rSensorHeight;
    const double      rTargetAltitude = targetRole.GetAltitude() + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator, IsPosted( source ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& source, const MIL_PopulationConcentration& target, double rSensorHeight ) const
{
    const double     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos            = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.TER_PopulationConcentration_ABC::Intersect2DWithCircle( vSourcePos, rDetectionDist_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;

    const double rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const double rTargetAltitude = MIL_Tools::GetAltitude( target.GetPosition() ) + 2;
    return RayTrace( vSourcePos, rSourceAltitude, target.GetPosition(), rTargetAltitude, rDistanceMaxModificator, IsPosted( source ) ) == PHY_PerceptionLevel::notSeen_ ?
        PHY_PerceptionLevel::notSeen_ : PHY_PerceptionLevel::identified_;
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
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_Agent_ABC& source, const MIL_PopulationFlow& target, double rSensorHeight, T_PointVector& shape ) const
{
    const double     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos            = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.Intersect2DWithCircle( vSourcePos, rDetectionDist_ * rDistanceMaxModificator, shape ) )
        return PHY_PerceptionLevel::notSeen_;

    MT_Vector2D targetPosition;
    target.GetLocation().ComputeNearestPoint( vSourcePos, targetPosition );
    const double rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const double rTargetAltitude = MIL_Tools::GetAltitude( targetPosition ) + 2;
    return RayTrace( vSourcePos, rSourceAltitude, targetPosition, rTargetAltitude, rDistanceMaxModificator, IsPosted( source ) ) == PHY_PerceptionLevel::notSeen_ ?
        PHY_PerceptionLevel::notSeen_ : PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeDistances
// Created: LDC 2012-01-04
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::ComputeDistances( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target, double& identification, double& recognition, double& detection ) const
{
    double distanceModifier = ComputeDistanceModificator( perceiver, target );
    identification = rIdentificationDist_ * distanceModifier;
    recognition = rRecognitionDist_ * distanceModifier;
    detection = rDetectionDist_ * distanceModifier;
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
double PHY_SensorTypeAgent::GetUrbanBlockFactor( const MIL_UrbanObject_ABC& block ) const
{
    if( const UrbanPhysicalCapacity* pPhysical = block.Retrieve< UrbanPhysicalCapacity >() )
        if( const PHY_MaterialCompositionType* materialCompositionType = PHY_MaterialCompositionType::Find( pPhysical->GetMaterial() ) )
            return urbanBlockFactors_[ materialCompositionType->GetId() ];
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
// Name: PHY_SensorTypeAgent::ConvertEnvironmentToObjectIdx
// Created: JVT 03-04-28
//-----------------------------------------------------------------------------
unsigned int PHY_SensorTypeAgent::ConvertEnvironmentToObjectIdx( PHY_RawVisionData::E_VisionObject obj )
{
    if( obj == PHY_RawVisionData::eVisionEmpty )
        return 0;

    unsigned int res = 1;
    for( unsigned int idx = 1; !( idx & obj ); idx <<= 1 )
        ++res;
    return res;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ConvertObjectIdxToEnvironnement
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
PHY_RawVisionData::E_VisionObject PHY_SensorTypeAgent::ConvertObjectIdxToEnvironnement( unsigned int val )
{
    return static_cast< PHY_RawVisionData::E_VisionObject >( val ? 1 << ( val - 1 ) : 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetDelay
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
unsigned int PHY_SensorTypeAgent::GetDelay() const
{
    return type_.GetDelay();
}
