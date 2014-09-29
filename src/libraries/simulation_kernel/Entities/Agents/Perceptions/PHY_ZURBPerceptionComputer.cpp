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
#include "MIL_AgentServer.h"
#include "OnComponentComputer_ABC.h"
#include "OnComponentFunctor_ABC.h"
#include "DefaultComponentFunctorComputer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
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
#include "Entities/Objects/StructuralCapacity.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include "simulation_terrain/TER_Geometry.h"
#include <tools/Set.h>

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer constructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
PHY_ZURBPerceptionComputer::PHY_ZURBPerceptionComputer( const MIL_Agent_ABC& perceiver, double roll, unsigned int tick )
    : perceiver_( perceiver )
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
    if ( !ComputeParametersPerception( target, bestSensorParameters ) )
        return PHY_PerceptionLevel::notSeen_;

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
            return GetLevelWithDelay( bestSensorParameters.delay_, PHY_PerceptionLevel::recognized_ );
        }
        return GetLevelWithDelay( bestSensorParameters.delay_, PHY_PerceptionLevel::detected_ );
    }
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::ComputePerceptionPolygon
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
void PHY_ZURBPerceptionComputer::ComputePerceptionPolygon( double distance, TER_Polygon& polygon ) const
{
    const MIL_UrbanObject_ABC* perceiverUrbanBlock = perceiver_.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( perceiverUrbanBlock && IsPosted( perceiver_ ) )
        polygon = perceiverUrbanBlock->GetScaledLocation( distance );
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
    typedef tools::Set< const PHY_SensorTypeAgent* > T_SensorTypeAgents;

    class CollateSensorFunctor
    {
    public:
        CollateSensorFunctor( T_SensorTypeAgents& sensors )
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
        T_SensorTypeAgents* sensors_;
    };

    class CollateSensorComponentFunctor : public OnComponentFunctor_ABC
    {
    public:
        CollateSensorComponentFunctor( const MIL_Agent_ABC& perceiver )
            : perceiver_( perceiver.RetrieveRole< transport::PHY_RoleAction_Loading >() ) {}
        void operator()( PHY_ComposantePion& composante )
        {
            if( !composante.CanPerceive( perceiver_ ) )
                return;
            CollateSensorFunctor dataFunctor( sensors_ );
            composante.ApplyOnSensors( dataFunctor );
        }
        T_SensorTypeAgents sensors_;
        const transport::PHY_RoleAction_Loading* perceiver_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::ComputeDistancePerception
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
bool PHY_ZURBPerceptionComputer::ComputeParametersPerception( const MIL_Agent_ABC& target, BestSensorsParameters& sensorsParameters ) const
{
    static const double epsilon = 1e-8;
    static const double sensorHeight = 2.;

    sensorsParameters.delay_ = std::numeric_limits< int >::max();

    MT_Vector2D targetPosition = target.GetRole< PHY_RoleInterface_Location >().GetPosition();
    MT_Vector2D perceiverPosition = perceiver_.GetRole< PHY_RoleInterface_Location >().GetPosition();

    std::vector< const MIL_UrbanObject_ABC* > list;
    MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocksWithinSegment( perceiverPosition, targetPosition, list );
    const MIL_UrbanObject_ABC* perceiverUrbanBlock = perceiver_.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    const PHY_Posture& currentPerceiverPosture = perceiver_.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();

    CollateSensorComponentFunctor dataFunctor( perceiver_ );
    DefaultComponentFunctorComputer dataComputer( dataFunctor );
    const_cast< MIL_Agent_ABC&>( perceiver_ ).Execute< OnComponentComputer_ABC >( dataComputer );

    double perceiverUrbanBlockHeight = sensorHeight;
    if( perceiverUrbanBlock )
    {
        if( const UrbanPhysicalCapacity* physical = perceiverUrbanBlock->Retrieve< UrbanPhysicalCapacity >() )
            if( const StructuralCapacity* structuralCapacity = perceiverUrbanBlock->Retrieve< StructuralCapacity >() )
                perceiverUrbanBlockHeight += structuralCapacity->GetStructuralState() * physical->GetHeight();
    }
    if( perceiverUrbanBlockHeight < sensorHeight )
        perceiverUrbanBlockHeight = sensorHeight;
    assert( perceiverUrbanBlockHeight );

    for( auto itSensor = dataFunctor.sensors_.begin(); itSensor != dataFunctor.sensors_.end(); ++itSensor )
    {
        int numberOfBlocksInBetween = 0;
        double worstFactor = 1.;
        for( auto it = list.begin(); it != list.end() && worstFactor > 0.; ++it )
            if( perceiverUrbanBlock == 0 || !( perceiverUrbanBlock == *it && ( &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_ ) ) )
            {
                const MIL_UrbanObject_ABC& object = **it;
                double objectHeight = sensorHeight;
                double occupation = 1.;
                double structuralState = 1.;
                if( const UrbanPhysicalCapacity* physical = object.Retrieve< UrbanPhysicalCapacity >() )
                {
                    structuralState = object.GetStructuralState();
                    objectHeight += structuralState * physical->GetHeight();
                    occupation = physical->GetOccupation();
                }
                if( occupation > 0 )
                {
                    ++numberOfBlocksInBetween;
                    if( numberOfBlocksInBetween > 2 )
                        return false;
                }
                assert( objectHeight );
                double heightFactor         = perceiverUrbanBlockHeight / objectHeight;
                double materialVisibility   = std::min( 1., ( *itSensor )->GetUrbanBlockFactor( object ) );
                double opacityFactor        = std::min( 1., occupation * ( 1. - materialVisibility ) * structuralState );
                double visibilityFactor     = std::min( 1., ( 1. - opacityFactor ) * heightFactor );
                worstFactor = std::min( worstFactor, visibilityFactor );
            }
        double identification = 0.;
        double recognition = 0.;
        double detection = 0.;
        ( *itSensor )->ComputeDistances( perceiver_, target, identification, recognition, detection );
        sensorsParameters.identificationDist_ = std::max( sensorsParameters.identificationDist_, worstFactor * identification );
        sensorsParameters.recognitionDist_ = std::max( sensorsParameters.recognitionDist_, worstFactor * recognition );
        sensorsParameters.detectionDist_ = std::max( sensorsParameters.detectionDist_, worstFactor * detection );
        sensorsParameters.delay_ = std::min( sensorsParameters.delay_, ( *itSensor )->GetDelay() );
    }
    return !( sensorsParameters.identificationDist_ < epsilon && sensorsParameters.recognitionDist_ < epsilon && sensorsParameters.detectionDist_ < epsilon );
}
