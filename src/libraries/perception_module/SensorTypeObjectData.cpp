// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SensorTypeObjectData.h"
#include "PerceptionLevel.h"
#include "MT_Tools/MT_Vector2DTypes.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include <xeumeuleu/xml.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( GetPostureSize, size_t, () )
DECLARE_HOOK( GetPostureIdentifier, bool, ( const char* type, size_t* identifier ) )
DECLARE_HOOK( PostureCanModifyDetection, bool, ( const char* type ) )
DECLARE_HOOK( GetLastPostureIdentifier, size_t, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetCurrentPostureIdentifier, size_t, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetPostureCompletionPercentage, double, ( const SWORD_Model* entity ) )
DECLARE_HOOK( ComputePerceptionDistanceFactor, double, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetCollidingPopulationDensity, double, ( const SWORD_Model* entity ) )
DECLARE_HOOK( ObjectIntersectWithCircle, bool, ( const SWORD_Model* object, const MT_Vector2D& center, double radius ) )
DECLARE_HOOK( KnowledgeObjectIntersectWithCircle, bool, ( const SWORD_Model* knowledgeObject, const MT_Vector2D& center, double radius ) )

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData::InitializeFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorTypeObjectData::InitializeFactors( const std::string& strTagName, T_FactorVector& factors, xml::xistream& xis )
{
    xis >> xml::start( strTagName )
            >> xml::list( "distance-modifier", *this, &SensorTypeObjectData::ReadPosture, factors )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData::ReadPosture
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void SensorTypeObjectData::ReadPosture( xml::xistream& xis, T_FactorVector& factors ) const
{
    std::string postureType;
    double rFactor = 0;
    xis >> xml::attribute( "type", postureType )
        >> xml::attribute( "value", rFactor );
    if( rFactor < 0 || rFactor > 1 )
        throw xml::exception( xis.context() + "distance-modifier: value not in [0..1]" );
    size_t postureId = 0;
    if( !GET_HOOK( GetPostureIdentifier )( postureType.c_str(), &postureId ) )
        throw xml::exception( xis.context() + "distance-modifier: unknown type" );
    if( !GET_HOOK( PostureCanModifyDetection )( postureType.c_str() ) )
        return;
    assert( factors.size() > postureId );
    factors[ postureId ] = rFactor;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData::SensorTypeObjectData
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
SensorTypeObjectData::SensorTypeObjectData( xml::xistream& xis )
    : rDD_                 ( 0. )
    , postureSourceFactors_( GET_HOOK( GetPostureSize )(), 0. )
    , rPopulationDensity_  ( 1. )
    , rPopulationFactor_   ( 1. )
{
    xis >> xml::attribute( "detection-distance", rDD_ );

    InitializeFactors          ( "source-posture-modifiers", postureSourceFactors_, xis );
    InitializePopulationFactors( xis );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData destructor
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
SensorTypeObjectData::~SensorTypeObjectData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData::InitializePopulationFactors
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
void SensorTypeObjectData::InitializePopulationFactors( xml::xistream& xis )
{
    xis >> xml::start( "population-modifier" )
            >> xml::attribute( "density", rPopulationDensity_ )
            >> xml::attribute( "modifier", rPopulationFactor_ )
        >> xml::end;

    if( rPopulationDensity_ < 0 )
        throw xml::exception( xis.context() + "population-modifier: density < 0" );
    if( rPopulationFactor_ < 0 || rPopulationFactor_ > 1 )
        throw xml::exception( xis.context() + "population-modifier: modifier not in [0..1]" );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData::GetPopulationFactor
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
double SensorTypeObjectData::GetPopulationFactor( double rDensity ) const
{
    if( rDensity == 0. || rPopulationDensity_ == 0. )
        return 1.;
    return std::min( 1., rPopulationFactor_ * rPopulationDensity_ / rDensity );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData::GetSourceFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double SensorTypeObjectData::GetSourceFactor( const wrapper::View& source ) const
{
    const std::size_t nOldPostureIdx = GET_HOOK( GetLastPostureIdentifier )( source );
    const std::size_t nCurPostureIdx = GET_HOOK( GetCurrentPostureIdentifier )( source );
    assert( postureSourceFactors_.size() > nOldPostureIdx );
    assert( postureSourceFactors_.size() > nCurPostureIdx );
    double rModificator =     postureSourceFactors_[ nOldPostureIdx ] + GET_HOOK( GetPostureCompletionPercentage )( source )
                          * ( postureSourceFactors_[ nCurPostureIdx ] - postureSourceFactors_[ nOldPostureIdx ] );
    rModificator *= GET_HOOK( ComputePerceptionDistanceFactor )( source );
    rModificator *= GetPopulationFactor( GET_HOOK( GetCollidingPopulationDensity )( source ) );
    return rModificator;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData::ComputePerception
// Created: SLI 2012-07-10
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeObjectData::ComputePerception( const wrapper::View& perceiver, const wrapper::View& target, bool(*intersectWithCircle) ( const SWORD_Model* object, const MT_Vector2D& center, double radius ) ) const
{
    const double rDistanceMaxModificator = GetSourceFactor( perceiver );
    const MT_Vector2D vSourcePos( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] );
    if( rDistanceMaxModificator == 0. || !intersectWithCircle( target, vSourcePos, rDD_ * rDistanceMaxModificator ) )
        return PerceptionLevel::notSeen_;
    return PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObject::ComputeObjectPerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeObjectData::ComputeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target, double /*rSensorHeight*/ ) const
{
    return ComputePerception( perceiver, target, GET_HOOK( ObjectIntersectWithCircle ) );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObject::ComputeKnowledgeObjectPerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeObjectData::ComputeKnowledgeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target, double /*rSensorHeight*/ ) const
{
    return ComputePerception( perceiver, target, GET_HOOK( KnowledgeObjectIntersectWithCircle ) );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObjectData::GetMaxDistance
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
double SensorTypeObjectData::GetMaxDistance() const
{
    return rDD_;
}
