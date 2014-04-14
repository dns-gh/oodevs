// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Sensors/PHY_SensorTypeObjectData.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 18:35 $
// $Revision: 5 $
// $Workfile: PHY_SensorTypeObjectData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SensorTypeObjectData.h"
#include "PerceptionDistanceComputer.h"
#include "DistanceModifiersHelpers.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::PHY_SensorTypeObjectData
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
PHY_SensorTypeObjectData::PHY_SensorTypeObjectData( xml::xistream& xis )
    : rDD_                 ( 0. )
    , postureSourceFactors_( PHY_Posture::GetPostureCount(), 0. )
    , rPopulationDensity_  ( 1. )
    , rPopulationFactor_   ( 1. )
{
    xis >> xml::attribute( "detection-distance", rDD_ );
    rDD_ = MIL_Tools::ConvertMeterToSim( rDD_ );
    distance_modifiers::ReadPostureFactors( xis, "source-posture-modifiers", postureSourceFactors_ );
    InitializePopulationFactors( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData destructor
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
PHY_SensorTypeObjectData::~PHY_SensorTypeObjectData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::InitializePopulationFactors
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
void PHY_SensorTypeObjectData::InitializePopulationFactors( xml::xistream& xis )
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
// Name: PHY_SensorTypeObjectData::GetPopulationFactor
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
double PHY_SensorTypeObjectData::GetPopulationFactor( double rDensity ) const
{
    if( rDensity == 0. || rPopulationDensity_ == 0. )
        return 1.;
    return std::min( 1., rPopulationFactor_ * rPopulationDensity_ / rDensity );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::GetSourceFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
double PHY_SensorTypeObjectData::GetSourceFactor( const MIL_Agent_ABC& source ) const
{
    // Posture
    const PHY_RoleInterface_Posture& sourcePosture = source.GetRole< PHY_RoleInterface_Posture >();

    const unsigned int nOldPostureIdx = sourcePosture.GetLastPosture   ().GetID();
    const unsigned int nCurPostureIdx = sourcePosture.GetCurrentPosture().GetID();

    assert( postureSourceFactors_.size() > nOldPostureIdx );
    assert( postureSourceFactors_.size() > nCurPostureIdx );
    double rModificator =     postureSourceFactors_[ nOldPostureIdx ] + sourcePosture.GetPostureCompletionPercentage()
                            * ( postureSourceFactors_[ nCurPostureIdx ] - postureSourceFactors_[ nOldPostureIdx ] );

    MIL_Agent_ABC& tempSource = const_cast< MIL_Agent_ABC& >( source );//@TODO MGD FIND A BETTER WAY
    detection::PerceptionDistanceComputer computer;
    rModificator *= tempSource.Execute( computer ).GetFactor();

    // Population
    const double rPopulationDensity = source.GetRole< PHY_RoleInterface_Population >().GetCollidingPopulationDensity();
    rModificator *= GetPopulationFactor( rPopulationDensity );

    return rModificator;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObjectData::ComputePerception( const MIL_Agent_ABC& source, const MIL_Object_ABC& target, double /*rSensorHeight*/ ) const
{
    const double     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.Intersect2DWithCircle( vSourcePos, rDD_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObjectData::ComputePerception( const MIL_Agent_ABC& source, const DEC_Knowledge_Object& target, double /*rSensorHeight*/ ) const
{
    const double     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.GetLocalisation().Intersect2DWithCircle( vSourcePos, rDD_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::GetMaxDistance
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
double PHY_SensorTypeObjectData::GetMaxDistance() const
{
    return rDD_;
}
