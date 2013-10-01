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
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/ObjectTypeResolver_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MT_Tools/MT_Logger.h"

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeObject::PHY_SensorTypeObject( const PHY_SensorType& type, xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
    : type_        ( type )
    , rMaxDistance_( 0 )
{
    xis >> xml::list( "object", *this, &PHY_SensorTypeObject::ReadObject, resolver );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ReadObject
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorTypeObject::ReadObject( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );
    try
    {
        const MIL_ObjectType_ABC& objectType = resolver.FindType( strType );
        if( objectData_.size() <= objectType.GetID() )
            objectData_.resize( objectType.GetID() + 1, 0 );
        const PHY_SensorTypeObjectData* pObjectData = new PHY_SensorTypeObjectData( xis );
        objectData_[ objectType.GetID() ] = pObjectData;
        rMaxDistance_ = std::max( rMaxDistance_, pObjectData->GetMaxDistance() );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeObject::~PHY_SensorTypeObject()
{
    for( auto it = objectData_.begin(); it != objectData_.end(); ++it )
        if( *it )
            delete *it;
    objectData_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::GetSensorTypeObjectData
// Created: MMC 2013-01-11
// -----------------------------------------------------------------------------
const PHY_SensorTypeObjectData* PHY_SensorTypeObject::GetSensorTypeObjectData( const MIL_ObjectType_ABC& objectType ) const
{
    if( objectType.GetID() < objectData_.size() )
        return objectData_[ objectType.GetID() ];
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObject::ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_Object_ABC& target, double rSensorHeight ) const
{
    assert( target().CanBePerceived() );
    if( const PHY_SensorTypeObjectData* pObjectData = GetSensorTypeObjectData( target.GetType() ) )
        return pObjectData->ComputePerception( perceiver, target, rSensorHeight );
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeObject::ComputePerception( const MIL_Agent_ABC& perceiver, const DEC_Knowledge_Object& target, double rSensorHeight ) const
{
    if( const PHY_SensorTypeObjectData* pObjectData = GetSensorTypeObjectData( target.GetType() ) )
        return pObjectData->ComputePerception( perceiver, target, rSensorHeight );
    return PHY_PerceptionLevel::notSeen_; 
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::GetMaxDistance
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double PHY_SensorTypeObject::GetMaxDistance() const
{
    return rMaxDistance_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::GetType
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
const PHY_SensorType& PHY_SensorTypeObject::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::CanPerceive
// Created: MMC 2013-01-11
// -----------------------------------------------------------------------------
bool PHY_SensorTypeObject::CanPerceive( const MIL_ObjectType_ABC& objectType ) const
{
    return GetSensorTypeObjectData( objectType ) != 0;
}
