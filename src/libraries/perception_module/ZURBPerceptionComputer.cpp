// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ZURBPerceptionComputer.h"
#include "PerceptionLevel.h"
#include "SensorType.h"
#include "SensorTypeAgent.h"
#include "tools/Set.h"
#include "MT_Tools/MT_Vector2D.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include <tools/Exception.h>
#include <boost/foreach.hpp>
#include <limits>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( ComputeAgentRatioInsidePerceptionPolygon, double, ( const SWORD_Model* perceiver, const SWORD_Model* target, double distance, double roll ) )
DECLARE_HOOK( GetCurrentUrbanBlock, const SWORD_Model*, ( const SWORD_Model* root, const SWORD_Model* entity ) )
DECLARE_HOOK( GetUrbanBlocksListWithinSegment, void, ( const SWORD_Model* root, MT_Vector2D first, MT_Vector2D second, void (*callback)( const SWORD_Model* urbanObjectWrapper, void* userData ), void* userData ) )
DECLARE_HOOK( IsPostureStationed, bool, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetUrbanObjectStructuralHeight, double, ( const SWORD_Model* urbanObject ) )
DECLARE_HOOK( GetUrbanObjectOccupation, double, ( const SWORD_Model* urbanObject ) )
DECLARE_HOOK( GetUrbanObjectStructuralState, double, ( const SWORD_Model* urbanObject ) )
DECLARE_HOOK( HasUrbanObjectArchitecture, bool, ( const SWORD_Model* urbanObject ) )

// -----------------------------------------------------------------------------
// Name: ZURBPerceptionComputer constructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
ZURBPerceptionComputer::ZURBPerceptionComputer( double roll, unsigned int tick )
    : roll_( roll )
    , tick_( tick )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ZURBPerceptionComputer destructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
ZURBPerceptionComputer::~ZURBPerceptionComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ZURBPerceptionComputer::ComputePerception
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
const PerceptionLevel& ZURBPerceptionComputer::ComputePerception( const wrapper::View& model, const wrapper::View& perceiver, const wrapper::View& target ) const
{
    BestSensorsParameters bestSensorParameters;
    if ( !ComputeParametersPerception( model, perceiver, target, bestSensorParameters ) )
        return PerceptionLevel::notSeen_;
    if( roll_ < GET_HOOK( ComputeAgentRatioInsidePerceptionPolygon )( perceiver, target, bestSensorParameters.detectionDist_, roll_ )/*role.ComputeRatioPionInside( polygon, roll_ )*/ )
    {
        if( roll_ < GET_HOOK( ComputeAgentRatioInsidePerceptionPolygon )( perceiver, target, bestSensorParameters.recognitionDist_, roll_ )/*role.ComputeRatioPionInside( polygon, roll_ )*/ )
        {
            if( roll_ < GET_HOOK( ComputeAgentRatioInsidePerceptionPolygon )( perceiver, target, bestSensorParameters.identificationDist_, roll_ )/*role.ComputeRatioPionInside( polygon, roll_ )*/ )
                return GetLevelWithDelay( bestSensorParameters.delay_, PerceptionLevel::identified_ );
            else
                return GetLevelWithDelay( bestSensorParameters.delay_, PerceptionLevel::recognized_ );
        }
        else
            return GetLevelWithDelay( bestSensorParameters.delay_, PerceptionLevel::detected_ );;
    }
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: ZURBPerceptionComputer::GetLevelWithDelay
// Created: SLG 2010-05-06
// -----------------------------------------------------------------------------
const PerceptionLevel& ZURBPerceptionComputer::GetLevelWithDelay( unsigned int delay, const PerceptionLevel& level ) const
{
    if( tick_ < delay && level > PerceptionLevel::notSeen_ )
        return PerceptionLevel::notSeen_;
    return level;
}

namespace
{
    typedef tools::Set< const SensorTypeAgent* > T_Sensors;
    void FillSensorTypeAgents( const wrapper::View& perceiver, T_Sensors& sensors )
    {
        for( std::size_t i = 0; i < perceiver[ "components" ].GetSize(); ++i )
        {
            const wrapper::View& component = perceiver[ "components" ].GetElement( i );
            for( std::size_t j = 0; j < component[ "sensors" ].GetSize(); ++j )
            {
                const wrapper::View& sensor = component[ "sensors" ].GetElement( j );
                const SensorType* sensorType = SensorType::FindSensorType( static_cast< std::string >( sensor[ "type" ] ) );
                if( !sensorType )
                    throw MASA_EXCEPTION( "Invalid sensor type : " + sensor[ "type" ] );
                const SensorTypeAgent* sensorTypeAgent = sensorType->GetTypeAgent();
                if( sensorTypeAgent )
                    sensors.insert( sensorTypeAgent );
            }
        }
    }
    typedef std::vector< wrapper::View > T_UrbanObjects;
    struct FillUrbanBlocks : private boost::noncopyable
    {
        explicit FillUrbanBlocks( T_UrbanObjects& list )
            : list_( list )
        {}
        static void Notify( const SWORD_Model* urbanObject, void* userData )
        {
            static_cast< FillUrbanBlocks* >( userData )->list_.push_back( urbanObject );
        }
        T_UrbanObjects& list_;
    };
}

// -----------------------------------------------------------------------------
// Name: ZURBPerceptionComputer::ComputeDistancePerception
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
bool ZURBPerceptionComputer::ComputeParametersPerception( const wrapper::View& model, const wrapper::View& perceiver, const wrapper::View& target, BestSensorsParameters& sensorsParameters ) const
{
    static const double epsilon = 1e-8;
    static const double sensorHeight = 2.;

    sensorsParameters.delay_ = std::numeric_limits< int >::max();

    MT_Vector2D targetPosition( target[ "movement/position/x" ], target[ "movement/position/y" ] );
    MT_Vector2D perceiverPosition( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] );

    std::vector< wrapper::View > list;
    FillUrbanBlocks filler( list );
    GET_HOOK( GetUrbanBlocksListWithinSegment )( model, perceiverPosition, targetPosition, &FillUrbanBlocks::Notify, &filler );
    const SWORD_Model* perceiverUrbanBlock = GET_HOOK( GetCurrentUrbanBlock )( model, perceiver );

    T_Sensors sensors;
    FillSensorTypeAgents( perceiver, sensors );
    double perceiverUrbanBlockHeight = sensorHeight;
    if( perceiverUrbanBlock )
        perceiverUrbanBlockHeight += GET_HOOK( GetUrbanObjectStructuralHeight )( perceiverUrbanBlock );
    if( perceiverUrbanBlockHeight < sensorHeight )
        perceiverUrbanBlockHeight = sensorHeight;

    int numberOfBlocksInBetween = 0;
    BOOST_FOREACH( const SensorTypeAgent* sensorType, sensors )
    {
        double worstFactor = 1.;
        for( T_UrbanObjects::const_iterator it = list.begin(); it != list.end() && worstFactor > 0.; ++it )
            if( perceiverUrbanBlock == 0 || !( perceiverUrbanBlock == *it && GET_HOOK( IsPostureStationed )( target ) ) )
            {
                const wrapper::View& object = *it;
                double objectHeight = sensorHeight;
                double occupation = 1.;
                double structuralState = 1.;
                if( GET_HOOK( HasUrbanObjectArchitecture )( object ) )
                {
                    objectHeight += GET_HOOK( GetUrbanObjectStructuralHeight )( object );
                    structuralState = GET_HOOK( GetUrbanObjectStructuralState )( object );
                    occupation = GET_HOOK( GetUrbanObjectOccupation )( object );
                }
                if( occupation > 0 )
                {
                    ++numberOfBlocksInBetween;
                    if( numberOfBlocksInBetween > 2 )
                        return false;
                }
                double heightFactor         = perceiverUrbanBlockHeight / objectHeight;
                double materialVisibility   = std::min( 1., sensorType->GetUrbanBlockFactor( object ) );
                double opacityFactor        = std::min( 1., occupation * ( 1. - materialVisibility ) * structuralState );
                double visibilityFactor     = std::min( 1., ( 1. - opacityFactor ) * heightFactor );
                worstFactor = std::min( worstFactor, visibilityFactor );
            }
        double identification = 0.;
        double recognition = 0.;
        double detection = 0.;
        sensorType->ComputeDistances( perceiver, target, identification, recognition, detection );
        sensorsParameters.identificationDist_ = std::max( sensorsParameters.identificationDist_, worstFactor * identification );
        sensorsParameters.recognitionDist_ = std::max( sensorsParameters.recognitionDist_, worstFactor * recognition );
        sensorsParameters.detectionDist_ = std::max( sensorsParameters.detectionDist_, worstFactor * detection );
        sensorsParameters.delay_ = std::min( sensorsParameters.delay_, sensorType->GetDelay() );
    }
    return !( sensorsParameters.identificationDist_ < epsilon && sensorsParameters.recognitionDist_ < epsilon && sensorsParameters.detectionDist_ < epsilon );
}
