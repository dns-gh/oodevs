// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ZURBPerceptionComputer.h"
#include "AlgorithmsFactories.h"
#include "MIL_AgentServer.h"
#include "OnComponentComputer_ABC.h"
#include "OnComponentFunctorComputerFactory_ABC.h"
#include "OnComponentFunctor_ABC.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Tools/MIL_Geometry.h"
#include <urban/GeometryAttribute.h>
#include <urban/PhysicalAttribute.h>
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#pragma warning( push, 0 )
#include <boost/geometry/geometry.hpp>
#pragma warning( pop )

namespace bg = boost::geometry;

#define VECTOR_TO_POINT( point ) geometry::Point2f( static_cast< float >( ( point ).rX_ ), static_cast< float >( ( point ).rY_ ) )

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer constructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
PHY_ZURBPerceptionComputer::PHY_ZURBPerceptionComputer( const MIL_Agent_ABC& perceiver, float roll, unsigned int tick )
    : PHY_PerceptionComputer_ABC( perceiver )
    , roll_                     ( roll )
    , tick_                     ( tick )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer destructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
PHY_ZURBPerceptionComputer::~PHY_ZURBPerceptionComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::ComputePerception
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_ZURBPerceptionComputer::ComputePerception( const MIL_Agent_ABC& target ) const
{
    BestSensorsParameters bestSensorParameters;
    Polygons perceptionPolygons;
    ComputePerceptionPolygon( target, bestSensorParameters, perceptionPolygons );
    float identificationPercentage = target.GetRole< PHY_RoleInterface_UrbanLocation >().ComputeRatioPionInside( perceptionPolygons.identificationPolygon_, roll_ );
    float recognitionPercentage = target.GetRole< PHY_RoleInterface_UrbanLocation >().ComputeRatioPionInside( perceptionPolygons.recognitionPolygon_, roll_ );
    float detectionPercentage  = target.GetRole< PHY_RoleInterface_UrbanLocation >().ComputeRatioPionInside( perceptionPolygons.detectionPolygon_, roll_ );
    if( roll_ < identificationPercentage )
        return GetLevelWithDelay( bestSensorParameters.delay_, PHY_PerceptionLevel::identified_ );
    else if( roll_ < recognitionPercentage )
        return GetLevelWithDelay( bestSensorParameters.delay_, PHY_PerceptionLevel::recognized_ );
    else if( roll_ < detectionPercentage )
        return GetLevelWithDelay( bestSensorParameters.delay_, PHY_PerceptionLevel::detected_ );
    else
        return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::ComputePerceptionPolygon
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
void PHY_ZURBPerceptionComputer::ComputePerceptionPolygon( const MIL_Agent_ABC& target, BestSensorsParameters& sensorPerception, Polygons& polygons ) const
{
    ComputeParametersPerception( target, sensorPerception );
    const Distances& distancePerception = sensorPerception.distances_;
    const PHY_Posture& currentPerceiverPosture = perceiver_.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();
    const UrbanObjectWrapper* perceiverUrbanBlock = perceiver_.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( perceiverUrbanBlock && ( &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_ ) )
    {
        MakePolygon( polygons.identificationPolygon_, perceiverUrbanBlock->GetObject(), distancePerception.identificationDist_ );
        MakePolygon( polygons.recognitionPolygon_, perceiverUrbanBlock->GetObject(), distancePerception.recognitionDist_ );
        MakePolygon( polygons.detectionPolygon_, perceiverUrbanBlock->GetObject(), distancePerception.detectionDist_ );
    }
    else
    {
        const MT_Vector2D& perceiverPosition = perceiver_.GetRole< PHY_RoleInterface_Location >().GetPosition();
        geometry::Point2f pos = VECTOR_TO_POINT( perceiverPosition );
        MakePolygon( polygons.identificationPolygon_, pos, distancePerception.identificationDist_ );
        MakePolygon( polygons.recognitionPolygon_, pos, distancePerception.recognitionDist_ );
        MakePolygon( polygons.detectionPolygon_, pos, distancePerception.detectionDist_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::MakePolygon
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
void PHY_ZURBPerceptionComputer::MakePolygon( geometry::Polygon2f& polygon, const urban::TerrainObject_ABC& block, double distance ) const
{
    if( const urban::GeometryAttribute* geom = block.Retrieve< urban::GeometryAttribute >() )
        MIL_Geometry::Scale( polygon, geom->Geometry(), static_cast< float >( distance ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::MakePolygon
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
void PHY_ZURBPerceptionComputer::MakePolygon( geometry::Polygon2f& polygon, const geometry::Point2f& targetPosition, double distance ) const
{
    float fDistance = static_cast< float >( distance );
    polygon.Add( geometry::Point2f( targetPosition.X() - fDistance, targetPosition.Y() - fDistance ) ); // bottom left
    polygon.Add( geometry::Point2f( targetPosition.X() - fDistance , targetPosition.Y() + fDistance ) ); // top left
    polygon.Add( geometry::Point2f( targetPosition.X() + fDistance , targetPosition.Y() + fDistance ) ); // top right
    polygon.Add( geometry::Point2f( targetPosition.X() + fDistance, targetPosition.Y() - fDistance ) ); // bottom right
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::GetLevelWithDelay
// Created: SLG 2010-05-06
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_ZURBPerceptionComputer::GetLevelWithDelay( unsigned int delay, const PHY_PerceptionLevel& level ) const
{
    if( tick_ < delay && level > PHY_PerceptionLevel::notSeen_ )
        return PHY_PerceptionLevel::notSeen_;
    return level;
}

namespace
{
    class CollateSensorFunctor
    {
    public:
        CollateSensorFunctor( std::set< const PHY_SensorTypeAgent* >& sensors ) : sensors_( sensors ) {}
        void operator() ( const PHY_Sensor& sensor )
        {
            const PHY_SensorTypeAgent* sensorTypeAgent = sensor.GetType().GetTypeAgent();
            if( sensorTypeAgent )
                sensors_.insert( sensorTypeAgent );
        }
    private:
        std::set< const PHY_SensorTypeAgent* >& sensors_;
    };

    class CollateSensorComponentFunctor : public OnComponentFunctor_ABC
    {
    public:
        void operator() ( PHY_ComposantePion& composante )
        {
            if( !composante.CanPerceive() )
                return;
            CollateSensorFunctor dataFunctor( sensors_ );
            composante.ApplyOnSensors( dataFunctor );
        }
        std::set< const PHY_SensorTypeAgent* > sensors_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::ComputeDistancePerception
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
void PHY_ZURBPerceptionComputer::ComputeParametersPerception( const MIL_Agent_ABC& target, BestSensorsParameters& sensorsParameters ) const
{
    sensorsParameters.delay_ = std::numeric_limits< int >::max();

    MT_Vector2D targetPosition = target.GetRole< PHY_RoleInterface_Location >().GetPosition();
    MT_Vector2D perceiverPosition = perceiver_.GetRole< PHY_RoleInterface_Location >().GetPosition();
    geometry::Point2f vSourcePoint( VECTOR_TO_POINT( perceiverPosition ) );
    geometry::Point2f vTargetPoint( VECTOR_TO_POINT( targetPosition ) );
    geometry::Segment2f segment( vSourcePoint, vTargetPoint );

    std::set< const urban::TerrainObject_ABC* > list;
    MIL_AgentServer::GetWorkspace().GetUrbanModel().GetListWithinSegment( vSourcePoint, vTargetPoint, list );
    const UrbanObjectWrapper* perceiverUrbanBlock = perceiver_.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    const PHY_Posture& currentPerceiverPosture = perceiver_.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();

    CollateSensorComponentFunctor dataFunctor;
    std::auto_ptr< OnComponentComputer_ABC > dataComputer( perceiver_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    const_cast< MIL_Agent_ABC&>( perceiver_ ).Execute( *dataComputer );

    for( std::set<const PHY_SensorTypeAgent*>::const_iterator itSensor = dataFunctor.sensors_.begin(); itSensor != dataFunctor.sensors_.end(); ++itSensor )
    {
        double worstFactor = 1.;
        for( std::set< const urban::TerrainObject_ABC* >::const_iterator it = list.begin(); it != list.end() && worstFactor > 0.; ++it )
            if( perceiverUrbanBlock == 0 || !( &perceiverUrbanBlock->GetObject() == *it && ( &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_ ) ) )
            {
                const urban::TerrainObject_ABC& object = **it;
                const urban::GeometryAttribute* pGeom = object.Retrieve< urban::GeometryAttribute >();
                if( pGeom )
                {
                    const geometry::Polygon2f& footPrint = pGeom->Geometry();
                    std::vector< geometry::Point2f > intersectPoints = footPrint.Intersect( segment );
                    if( !intersectPoints.empty() || footPrint.IsInside( vSourcePoint ) || footPrint.IsInside( vTargetPoint ) )
                    {
                        float perceiverUrbanBlockHeight = 2; //2 = SensorHeight
                        float objectHeight = 2; //2 = SensorHeight

                        const urban::PhysicalAttribute* pPhysical = object.Retrieve< urban::PhysicalAttribute >();
                        if( perceiverUrbanBlock )
                        {
                            const urban::PhysicalAttribute* perceiverUrbanBlockPhysical = perceiverUrbanBlock->GetObject().Retrieve< urban::PhysicalAttribute >();
                            if( perceiverUrbanBlockPhysical && perceiverUrbanBlockPhysical->GetArchitecture() )
                                perceiverUrbanBlockHeight += perceiverUrbanBlockPhysical->GetArchitecture()->GetHeight();
                        }
                        if( pPhysical && pPhysical->GetArchitecture() )
                            objectHeight += pPhysical->GetArchitecture()->GetHeight();
                        double urbanFactor = ( *itSensor )->GetUrbanBlockFactor( object ) * perceiverUrbanBlockHeight / objectHeight;
                        if( pPhysical && pPhysical->GetArchitecture() )
                            urbanFactor = 1. + pPhysical->GetArchitecture()->GetOccupation() * ( urbanFactor - 1. ) ;
                        worstFactor = std::min( worstFactor, urbanFactor );
                    }
                }
            }
        sensorsParameters.distances_.identificationDist_ = std::max( sensorsParameters.distances_.identificationDist_,
            worstFactor * ( *itSensor )->ComputeIdentificationDist( perceiver_, target ) );
        sensorsParameters.distances_.recognitionDist_ = std::max( sensorsParameters.distances_.recognitionDist_,
            worstFactor * ( *itSensor )->ComputeRecognitionDist( perceiver_, target ) );
        sensorsParameters.distances_.detectionDist_ = std::max( sensorsParameters.distances_.detectionDist_,
            worstFactor * ( *itSensor )->ComputeDetectionDist( perceiver_, target ) );
        sensorsParameters.delay_ = std::min( sensorsParameters.delay_, ( *itSensor )->GetDelay() );
    }
}
