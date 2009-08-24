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
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.h>

struct PHY_SensorTypeObjectData::LoadingWrapper
{
    void ReadPosture( xml::xistream& xis, const PHY_Posture::T_PostureMap& container, PHY_SensorTypeObjectData::T_FactorVector& factors )
    {
        PHY_SensorTypeObjectData::ReadPosture( xis, container, factors );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::InitializeFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
template<>
void PHY_SensorTypeObjectData::InitializeFactors( const PHY_Posture::T_PostureMap& container, const std::string& strTagName, T_FactorVector& factors, xml::xistream& xis )
{
    LoadingWrapper loader;

    xis >> xml::start( strTagName )
            >> xml::list( "distance-modifier", loader, &LoadingWrapper::ReadPosture, container, factors )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::ReadPosture
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorTypeObjectData::ReadPosture( xml::xistream& xis, const PHY_Posture::T_PostureMap& container, T_FactorVector& factors )
{
    std::string postureType;
    xis >> xml::attribute( "type", postureType );

    PHY_Posture::CIT_PostureMap it = container.find( postureType );
    if( it != container.end() )
    {
        if( !it->second->CanModifyDetection() )
            return;

        assert( factors.size() > it->second->GetID() );
        MT_Float rFactor = 0;
        xis >> xml::attribute( "value", rFactor );
        if( rFactor < 0 || rFactor > 1 )
            xis.error( "distance-modifier: value not in [0..1]" );
        factors[ it->second->GetID() ] = rFactor;
    }
    else
        xis.error( "distance-modifier: unknow type" );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::PHY_SensorTypeObjectData
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
PHY_SensorTypeObjectData::PHY_SensorTypeObjectData( xml::xistream& xis )
    : rDD_                 ( 0. )
    , postureSourceFactors_( PHY_Posture::GetPostures().size(), 0. )
    , rPopulationDensity_  ( 1. )
    , rPopulationFactor_   ( 1. )
{
    xis >> xml::attribute( "detection-distance", rDD_ );
    rDD_ = MIL_Tools::ConvertMeterToSim( rDD_ );

    InitializeFactors          ( PHY_Posture::GetPostures(), "source-posture-modifiers", postureSourceFactors_, xis );
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
        >> xml::end();

    if( rPopulationDensity_ < 0 )
        xis.error( "population-modifier: density < 0" );
    if( rPopulationFactor_ < 0 || rPopulationFactor_ > 1 )
        xis.error( "population-modifier: modifier not in [0..1]" );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::GetPopulationFactor
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeObjectData::GetPopulationFactor( MT_Float rDensity ) const
{
    if( rDensity == 0. || rPopulationDensity_ == 0. )
        return 1.;
    return std::min( 1., rPopulationFactor_ * rPopulationDensity_ / rDensity );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::GetSourceFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeObjectData::GetSourceFactor( const MIL_AgentPion& source ) const
{
    // Posture
    const PHY_RoleInterface_Posture& sourcePosture = source.GetRole< PHY_RoleInterface_Posture >();

    const uint nOldPostureIdx = sourcePosture.GetLastPosture   ().GetID();
    const uint nCurPostureIdx = sourcePosture.GetCurrentPosture().GetID();

    assert( postureSourceFactors_.size() > nOldPostureIdx );
    assert( postureSourceFactors_.size() > nCurPostureIdx );
    MT_Float rModificator =     postureSourceFactors_[ nOldPostureIdx ] + sourcePosture.GetPostureCompletionPercentage() 
                            * ( postureSourceFactors_[ nCurPostureIdx ] - postureSourceFactors_[ nOldPostureIdx ] );

    // Elongation
    rModificator *= sourcePosture.GetElongationFactor(); 

    // Human factors
    rModificator *= source.GetRole< PHY_RoleInterface_HumanFactors >().GetSensorDistanceModificator();

    // Population
    const MT_Float rPopulationDensity = source.GetRole< PHY_RoleInterface_Population >().GetCollidingPopulationDensity();
    rModificator *= GetPopulationFactor( rPopulationDensity );

    return rModificator;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObjectData::ComputePerception( const MIL_AgentPion& source, const MIL_Object_ABC& target, MT_Float /*rSensorHeight*/ ) const
{   
    const MT_Float     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.Intersect2DWithCircle( vSourcePos, rDD_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObjectData::ComputePerception( const MIL_AgentPion& source, const DEC_Knowledge_Object& target, MT_Float /*rSensorHeight*/ ) const
{
    const MT_Float     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.GetLocalisation().Intersect2DWithCircle( vSourcePos, rDD_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;    
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::GetMaxDistance
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeObjectData::GetMaxDistance() const
{
    return rDD_;
}
