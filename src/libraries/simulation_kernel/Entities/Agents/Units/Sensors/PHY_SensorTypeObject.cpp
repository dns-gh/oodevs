// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_SensorTypeObject.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 10:08 $
// $Revision: 6 $
// $Workfile: PHY_SensorTypeObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_SensorTypeObject.h"

#include "PHY_SensorTypeObjectData.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "xeumeuleu/xml.h"



// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeObject::PHY_SensorTypeObject( const PHY_SensorType& type, xml::xistream& xis )
    : type_        ( type )
    , objectData_  ( MIL_RealObjectType::GetObjectTypes().size() )
    , rMaxDistance_( 0. )
{
    xis >> xml::list( "object", *this, &PHY_SensorTypeObject::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ReadObject
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorTypeObject::ReadObject( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );

    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::Find( strType );
    if( !pObjectType )
        xis.error( "Unknown object type" );

    assert( objectData_.size() > pObjectType->GetID() );
    const PHY_SensorTypeObjectData* pObjectData = new PHY_SensorTypeObjectData( xis );
    objectData_[ pObjectType->GetID() ] = pObjectData;
    rMaxDistance_ = std::max( rMaxDistance_, pObjectData->GetMaxDistance() );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeObject::~PHY_SensorTypeObject()
{
    for( CIT_ObjectDataVector it = objectData_.begin(); it != objectData_.end(); ++it )
        if( *it )
            delete *it;
    objectData_.clear();
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObject::ComputePerception( const MIL_AgentPion& perceiver, const MIL_RealObject_ABC& target, MT_Float rSensorHeight ) const
{
    assert( target.CanBePerceived() );

    assert( objectData_.size()  > target.GetType().GetID() );
    const PHY_SensorTypeObjectData* pObjectData = objectData_[ target.GetType().GetID() ];
    if( !pObjectData )
        return PHY_PerceptionLevel::notSeen_;

    return pObjectData->ComputePerception( perceiver, target, rSensorHeight );    
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObject::ComputePerception( const MIL_AgentPion& perceiver, const DEC_Knowledge_Object& target, MT_Float rSensorHeight ) const
{
    assert( objectData_.size()  > target.GetType().GetID() );
    const PHY_SensorTypeObjectData* pObjectData = objectData_[ target.GetType().GetID() ];
    if( !pObjectData )
        return PHY_PerceptionLevel::notSeen_;

    return pObjectData->ComputePerception( perceiver, target, rSensorHeight );    
}
