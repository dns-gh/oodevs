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

#define VECTOR_TO_POINT( point ) geometry::Point2f( static_cast< float >( ( point ).rX_ ), static_cast< float >( ( point ).rY_ ) )

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer constructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
PHY_ZURBPerceptionComputer::PHY_ZURBPerceptionComputer( const MIL_Agent_ABC& perceiver, double roll, unsigned int tick )
    : PHY_PerceptionComputer_ABC( perceiver )
    , roll_( roll )
    , tick_( tick )
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
    ComputeParametersPerception( target, bestSensorParameters );
    TER_Polygon polygon;
    const PHY_RoleInterface_UrbanLocation& role = target.GetRole< PHY_RoleInterface_UrbanLocation >();
    ComputePerceptionPolygon( bestSensorParameters.detectionDist_, polygon );
    if( roll_ < role.ComputeRatioPionInside( polygon, roll_ ) )
    {
        ComputePerceptionPolygon( bestSensorParameters.recognitionDist_, polygon );
        if( roll_ < role.ComputeRatioPionInside( polygon, roll_ ) )
        {
            ComputePerceptionPolygon( bestSensorParameters.identificationDist_, polygon );
            if( roll_ < role.ComputeRatioPionInside( polygon, roll_ ) )
                return GetLevelWithDelay( bestSensorParameters.delay_, PHY_PerceptionLevel::identified_ );
            else
                return GetLevelWithDelay( bestSensorParameters.delay_, PHY_PerceptionLevel::recognized_ );
        }
        else
            return GetLevelWithDelay( bestSensorParameters.delay_, PHY_PerceptionLevel::detected_ );;
    }
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::ComputePerceptionPolygon
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
void PHY_ZURBPerceptionComputer::ComputePerceptionPolygon( double distance, TER_Polygon& polygon ) const
{
    const PHY_Posture& currentPerceiverPosture = perceiver_.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();
    const UrbanObjectWrapper* perceiverUrbanBlock = perceiver_.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( perceiverUrbanBlock && ( &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_ ) )
        MIL_Geometry::Scale( polygon, perceiverUrbanBlock->GetLocalisation().GetPoints(), distance );
    else
    {
        const MT_Vector2D& targetPosition = perceiver_.GetRole< PHY_RoleInterface_Location >().GetPosition();
        T_PointVector vector;
        vector.push_back( MT_Vector2D( targetPosition.rX_ - distance, targetPosition.rY_ - distance ) ); // bottom left
        vector.push_back( MT_Vector2D( targetPosition.rX_ - distance, targetPosition.rY_ + distance ) ); // top left
        vector.push_back( MT_Vector2D( targetPosition.rX_ + distance, targetPosition.rY_ + distance ) ); // top right
        vector.push_back( MT_Vector2D( targetPosition.rX_ + distance, targetPosition.rY_ - distance ) ); // bottom right
        polygon.Reset( vector );
    }
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
        CollateSensorFunctor( std::set< const PHY_SensorTypeAgent* >& sensors )
            : sensors_( &sensors )
        {
            // NOTHING
        }
        void operator()( const PHY_Sensor& sensor )
        {
            const PHY_SensorTypeAgent* sensorTypeAgent = sensor.GetType().GetTypeAgent();
            if( sensorTypeAgent )
                sensors_->insert( sensorTypeAgent );
        }
    private:
        std::set< const PHY_SensorTypeAgent* >* sensors_;
    };

    class CollateSensorComponentFunctor : public OnComponentFunctor_ABC
    {
    public:
        void operator()( PHY_ComposantePion& composante )
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

    std::set< const urban::TerrainObject_ABC* > list;
    MIL_AgentServer::GetWorkspace().GetUrbanModel().GetListWithinSegment( vSourcePoint, vTargetPoint, list );
    const UrbanObjectWrapper* perceiverUrbanBlock = perceiver_.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    const PHY_Posture& currentPerceiverPosture = perceiver_.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();

    CollateSensorComponentFunctor dataFunctor;
    std::auto_ptr< OnComponentComputer_ABC > dataComputer( perceiver_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    const_cast< MIL_Agent_ABC&>( perceiver_ ).Execute( *dataComputer );

    double perceiverUrbanBlockHeight = 2; //2 = SensorHeight
    if( perceiverUrbanBlock )
        perceiverUrbanBlockHeight += perceiverUrbanBlock->GetHeight();

    for( std::set< const PHY_SensorTypeAgent* >::const_iterator itSensor = dataFunctor.sensors_.begin(); itSensor != dataFunctor.sensors_.end(); ++itSensor )
    {
        double worstFactor = 1.;
        for( std::set< const urban::TerrainObject_ABC* >::const_iterator it = list.begin(); it != list.end() && worstFactor > 0.; ++it )
            if( perceiverUrbanBlock == 0 || !( perceiverUrbanBlock->Is( **it ) && ( &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_ ) ) )
            {
                const urban::TerrainObject_ABC& object = **it;
                const geometry::Polygon2f& footPrint = object.Get< urban::GeometryAttribute >().Geometry();
                std::vector< geometry::Point2f > intersectPoints = footPrint.Intersect( geometry::Segment2f( vSourcePoint, vTargetPoint ) );
                if( !intersectPoints.empty() || footPrint.IsInside( vSourcePoint ) || footPrint.IsInside( vTargetPoint ) )
                {
                    double objectHeight = 2; //2 = SensorHeight
                    double occupation = 1;
                    if( const urban::PhysicalAttribute* pPhysical = object.Retrieve< urban::PhysicalAttribute >() )
                        if( const urban::Architecture* architecture = pPhysical->GetArchitecture() )
                        {
                            objectHeight += architecture->GetHeight();
                            occupation = architecture->GetOccupation();
                        }
                    double urbanFactor = ( *itSensor )->GetUrbanBlockFactor( object ) * perceiverUrbanBlockHeight / objectHeight;
                    urbanFactor = 1. + occupation * ( urbanFactor - 1. ) ;
                    worstFactor = std::min( worstFactor, urbanFactor );
                }
            }
        sensorsParameters.identificationDist_ = std::max( sensorsParameters.identificationDist_,
            worstFactor * ( *itSensor )->ComputeIdentificationDist( perceiver_, target ) );
        sensorsParameters.recognitionDist_ = std::max( sensorsParameters.recognitionDist_,
            worstFactor * ( *itSensor )->ComputeRecognitionDist( perceiver_, target ) );
        sensorsParameters.detectionDist_ = std::max( sensorsParameters.detectionDist_,
            worstFactor * ( *itSensor )->ComputeDetectionDist( perceiver_, target ) );
        sensorsParameters.delay_ = std::min( sensorsParameters.delay_, ( *itSensor )->GetDelay() );
    }
}
