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

#include "MIL_pch.h"

#include "PHY_SensorTypeObjectData.h"

#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Object.h"

#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::InitializeFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
template<>
void PHY_SensorTypeObjectData::InitializeFactors( const PHY_Posture::T_PostureMap& container, const std::string& strTagName, T_FactorVector& factors, MIL_InputArchive& archive )
{
    archive.Section( strTagName );

    for ( PHY_Posture::CIT_PostureMap it = container.begin(); it != container.end(); ++it )
    {
        if( !it->second->CanModifyDetection() )
            continue;

        assert( factors.size() > it->second->GetID() );
        MT_Float& rFactor = factors[ it->second->GetID() ];
        archive.ReadField( it->second->GetName(), rFactor, CheckValueBound( 0., 1. ) );
    }

    archive.EndSection(); // XXX
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::PHY_SensorTypeObjectData
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
PHY_SensorTypeObjectData::PHY_SensorTypeObjectData( MIL_InputArchive& archive )
    : rDD_                 ( 0. )
    , postureSourceFactors_( PHY_Posture::GetPostures().size(), 0. )
{
    archive.ReadField( "DD", rDD_ );
    rDD_ = MIL_Tools::ConvertMeterToSim( rDD_ );

    archive.Section( "ModificateursDeDistance" );
    InitializeFactors( PHY_Posture::GetPostures(), "PosturesSource", postureSourceFactors_, archive );
    archive.EndSection(); // ModificateursDeDistance
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData destructor
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
PHY_SensorTypeObjectData::~PHY_SensorTypeObjectData()
{

}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObjectData::GetSourceFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeObjectData::GetSourceFactor( const MIL_AgentPion& source ) const
{
    const PHY_RolePion_Posture& sourcePosture = source.GetRole< PHY_RolePion_Posture >();

    const uint nOldPostureIdx = sourcePosture.GetLastPosture   ().GetID();
    const uint nCurPostureIdx = sourcePosture.GetCurrentPosture().GetID();

    assert( postureSourceFactors_.size() > nOldPostureIdx );
    assert( postureSourceFactors_.size() > nCurPostureIdx );
    const MT_Float rModificator =     postureSourceFactors_[ nOldPostureIdx ] + sourcePosture.GetPostureCompletionPercentage() 
                                  * ( postureSourceFactors_[ nCurPostureIdx ] - postureSourceFactors_[ nOldPostureIdx ] );
    return rModificator * sourcePosture.GetElongationFactor() * source.GetRole< PHY_RolePion_HumanFactors >().GetSensorDistanceModificator();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObjectData::ComputePerception( const MIL_AgentPion& source, const MIL_RealObject_ABC& target, MT_Float /*rSensorHeight*/ ) const
{   
          MT_Float     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RolePion_Location >().GetPosition();

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
          MT_Float     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RolePion_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.GetLocalisation().Intersect2DWithCircle( vSourcePos, rDD_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;    
}
