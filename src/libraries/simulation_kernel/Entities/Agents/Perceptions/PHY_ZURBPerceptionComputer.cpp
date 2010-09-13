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
#include "AlgorithmsFactories.h"
#include "OnComponentComputer_ABC.h"
#include "OnComponentFunctorComputerFactory_ABC.h"
#include "OnComponentFunctor_ABC.h"
#include "UrbanModel.h"
#include <urban/Architecture.h>
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#pragma warning( push )
#pragma warning( disable : 4127 4100 )
#include <boost/geometry/geometry.hpp>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

namespace bg = boost::geometry;
// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer constructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
PHY_ZURBPerceptionComputer::PHY_ZURBPerceptionComputer( const MIL_Agent_ABC& perceiver, float roll, int tick )
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
    const Polygons perceptionPolygons = ComputePerceptionPolygon( target, bestSensorParameters );
    float identificationPercentage = target.GetRole< PHY_RoleInterface_UrbanLocation >().ComputeRatioPionInside( perceptionPolygons.identificationPolygon, roll_ );
    float recognitionPercentage = target.GetRole< PHY_RoleInterface_UrbanLocation >().ComputeRatioPionInside( perceptionPolygons.recognitionPolygon, roll_ );
    float detectionPercentage  = target.GetRole< PHY_RoleInterface_UrbanLocation >().ComputeRatioPionInside( perceptionPolygons.detectionPolygon, roll_ );
    unsigned int delay = bestSensorParameters.delay;
    if( roll_ < identificationPercentage )
        return GetLevelWithDelay( delay, PHY_PerceptionLevel::identified_ );
    else if( roll_ < recognitionPercentage )
        return GetLevelWithDelay( delay, PHY_PerceptionLevel::recognized_ );
    else if( roll_ < detectionPercentage )
        return GetLevelWithDelay( delay, PHY_PerceptionLevel::detected_ );
    else
        return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::ComputePerceptionPolygon
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
const PHY_ZURBPerceptionComputer::Polygons PHY_ZURBPerceptionComputer::ComputePerceptionPolygon( const MIL_Agent_ABC& target, BestSensorsParameters& sensorPerception ) const
{
    sensorPerception = ComputeParametersPerception( target );
    Distances distancePerception = sensorPerception .distances;
    const PHY_Posture& currentPerceiverPosture = perceiver_.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();
    const UrbanObjectWrapper* perceiverUrbanBlock = perceiver_.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    Polygons returnValue;
    if( perceiverUrbanBlock && ( &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_ ) )
    {
        returnValue.identificationPolygon = MakePolygon( perceiverUrbanBlock->GetObject(), distancePerception.identificationDist );
        returnValue.recognitionPolygon = MakePolygon( perceiverUrbanBlock->GetObject(), distancePerception.recognitionDist );
        returnValue.detectionPolygon = MakePolygon( perceiverUrbanBlock->GetObject(), distancePerception.detectionDist );
    }
    else
    {
        const MT_Vector2D& perceiverPosition = perceiver_.GetRole< PHY_RoleInterface_Location >().GetPosition();
        returnValue.identificationPolygon = MakePolygon( geometry::Point2f( static_cast< float >( perceiverPosition.rX_ ), static_cast< float >( perceiverPosition.rY_ ) ), distancePerception.identificationDist );
        returnValue.recognitionPolygon = MakePolygon( geometry::Point2f( static_cast< float >( perceiverPosition.rX_ ), static_cast< float >( perceiverPosition.rY_ ) ), distancePerception.recognitionDist );
        returnValue.detectionPolygon = MakePolygon( geometry::Point2f( static_cast< float >( perceiverPosition.rX_ ), static_cast< float >( perceiverPosition.rY_ ) ), distancePerception.detectionDist );
    }
    return returnValue;
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::MakePolygon
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
geometry::Polygon2f PHY_ZURBPerceptionComputer::MakePolygon( const urban::TerrainObject_ABC& block, double distance ) const
{
    geometry::Polygon2f polygon = MIL_Geometry::Scale( *( block.GetFootprint() ), static_cast< float >( distance ) );
    return polygon;
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::MakePolygon
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
geometry::Polygon2f PHY_ZURBPerceptionComputer::MakePolygon( const geometry::Point2f targetPosition, double distance ) const
{
    geometry::Point2f bottomLeftPoint( static_cast< float >( targetPosition.X() - distance ), static_cast< float >( targetPosition.Y() - distance ) );
    geometry::Point2f topLeftPoint( static_cast< float >( targetPosition.X() - distance ), static_cast< float >( targetPosition.Y() + distance ) );
    geometry::Point2f topRightPoint( static_cast< float >( targetPosition.X() + distance ), static_cast< float >( targetPosition.Y() + distance ) );
    geometry::Point2f bottomeRightPoint( static_cast< float >( targetPosition.X() + distance ), static_cast< float >( targetPosition.Y() - distance ) );
    geometry::Polygon2f polygon;
    polygon.Add( bottomLeftPoint );
    polygon.Add( topLeftPoint );
    polygon.Add( topRightPoint );
    polygon.Add( bottomeRightPoint );
    return polygon;
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::MakePolygon
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
    class SensorFunctor : private boost::noncopyable
    {
    public:
        SensorFunctor(const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target )
            : perceiver_( perceiver ), target_( target ), distanceId_( 0 ), distanceRec_( 0 ), distanceDet_( 0 ), delay_( std::numeric_limits< int >::max() )
        {}
        ~SensorFunctor()
        {}
        double GetIdentificationDistance()      { return distanceId_;   }
        double GetRecognitionDistance()         { return distanceRec_;  }
        double GetDetectionDistance()           { return distanceDet_;  }
        unsigned int GetDelay()                 { return delay_;        }

        void operator() ( const PHY_Sensor& sensor )
        {
            const PHY_SensorTypeAgent* sensorTypeAgent = sensor.GetType().GetTypeAgent();
            if( sensorTypeAgent )
            {
                MT_Vector2D targetPosition = target_.GetRole< PHY_RoleInterface_Location >().GetPosition();
                MT_Vector2D perceiverPosition = perceiver_.GetRole< PHY_RoleInterface_Location >().GetPosition();

                geometry::Point2f vSourcePoint( static_cast< float >( perceiverPosition.rX_ ), static_cast< float >( perceiverPosition.rY_ ) );
                geometry::Point2f vTargetPoint( static_cast< float >( targetPosition.rX_ ), static_cast< float >( targetPosition.rY_ ) );

                geometry::Point2f center( ( vSourcePoint.X() + vTargetPoint.X() ) * 0.5f, ( vSourcePoint.Y() + vTargetPoint.Y() ) * 0.5f );
                float radius = vSourcePoint.Distance( vTargetPoint ) * 0.5f;
                std::vector< const urban::TerrainObject_ABC* > list;
                UrbanModel::GetSingleton().GetModel().GetListWithinSegment( vSourcePoint, vTargetPoint, list );
                double worstFactor = 1.f;
                const UrbanObjectWrapper* perceiverUrbanBlock = perceiver_.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
                const PHY_Posture& currentPerceiverPosture = perceiver_.GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture();
                if( !list.empty() )
                {
                    for( std::vector< const urban::TerrainObject_ABC* >::const_iterator it = list.begin(); it != list.end() && worstFactor > 0.; it++ )
                    {
                        if( perceiverUrbanBlock == 0 || !( &perceiverUrbanBlock->GetObject() == *it && ( &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_ ) ) )
                        {
                            const urban::TerrainObject_ABC& object = **it;
                            const geometry::Polygon2f* footPrint = object.GetFootprint();
                            std::vector< geometry::Point2f > intersectPoints = footPrint->Intersect( geometry::Segment2f( vSourcePoint, vTargetPoint ) );
                            if( !intersectPoints.empty() || footPrint->IsInside( vSourcePoint ) || footPrint->IsInside( vTargetPoint ) )
                            {
                                float perceiverUrbanBlockHeight = 2; //2 = SensorHeight
                                float objectHeight = 2; //2 = SensorHeight
                                double urbanFactor = sensorTypeAgent->GetUrbanBlockFactor( object );
                                const urban::Architecture* architecture = object.RetrievePhysicalFeature< urban::Architecture >();
                                if( perceiverUrbanBlock )
                                {
                                    const urban::Architecture* perceiverUrbanBlockArchitecture = perceiverUrbanBlock->GetObject().RetrievePhysicalFeature< urban::Architecture >();
                                    if( perceiverUrbanBlockArchitecture )
                                        perceiverUrbanBlockHeight += perceiverUrbanBlockArchitecture->GetHeight();
                                }
                                const urban::Architecture* objectArchitecture = object.RetrievePhysicalFeature< urban::Architecture >();
                                if( objectArchitecture )
                                    objectHeight += objectArchitecture->GetHeight();
                                urbanFactor *= static_cast< double >( perceiverUrbanBlockHeight / objectHeight );
                                if( architecture )
                                    urbanFactor = 1. + architecture->GetOccupation() * ( urbanFactor -1. ) ;

                                worstFactor = std::min( worstFactor, urbanFactor );
                            }
                        }
                    }
                }

                distanceId_     = std::max( distanceId_, worstFactor * sensorTypeAgent->ComputeIdentificationDist( perceiver_, target_ ) );
                distanceRec_    = std::max( distanceRec_, worstFactor * sensorTypeAgent->ComputeRecognitionDist( perceiver_, target_ ) );
                distanceDet_    = std::max( distanceDet_, worstFactor * sensorTypeAgent->ComputeDetectionDist( perceiver_, target_ ) );
                delay_          = std::min( delay_, sensorTypeAgent->GetDelay() );
            }
        }
    private:
        const MIL_Agent_ABC& perceiver_;
        const MIL_Agent_ABC& target_;
        double distanceId_;
        double distanceRec_;
        double distanceDet_;
        unsigned int delay_;

    };

    class Functor : public OnComponentFunctor_ABC
    {
        public:
            Functor( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target )
                : perceiver_( perceiver ), target_( target )
            {}
            ~Functor()
            {}

        void operator() ( PHY_ComposantePion& composante )
        {
            if( !composante.CanPerceive() )
                return;
            SensorFunctor dataFunctor( perceiver_, target_ );
            composante.ApplyOnSensors( dataFunctor );
            distances_.identificationDist = std::max( distances_.identificationDist, dataFunctor.GetIdentificationDistance() );
            distances_.recognitionDist = std::max( distances_.recognitionDist, dataFunctor.GetRecognitionDistance() );
            distances_.detectionDist = std::max( distances_.detectionDist, dataFunctor.GetDetectionDistance() );

            delay_ = std::min( delay_, dataFunctor.GetDelay() );
        }
        PHY_ZURBPerceptionComputer::Distances GetDistances(){ return distances_; }
        unsigned int GetDelay(){ return delay_; }

    private:
        const MIL_Agent_ABC& perceiver_;
        const MIL_Agent_ABC& target_;
        PHY_ZURBPerceptionComputer::Distances distances_;
        unsigned int delay_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_ZURBPerceptionComputer::ComputeDistancePerception
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
const PHY_ZURBPerceptionComputer::BestSensorsParameters PHY_ZURBPerceptionComputer::ComputeParametersPerception( const MIL_Agent_ABC& target ) const
{
    BestSensorsParameters sensorsParameters;
    Functor dataFunctor( perceiver_, target );
    std::auto_ptr< OnComponentComputer_ABC > dataComputer( perceiver_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    const_cast< MIL_Agent_ABC&>( perceiver_ ).Execute( *dataComputer );
    sensorsParameters.distances = dataFunctor.GetDistances();
    sensorsParameters.delay = dataFunctor.GetDelay();
    return sensorsParameters;
}
