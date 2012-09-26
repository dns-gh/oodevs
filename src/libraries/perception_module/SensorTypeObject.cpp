// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SensorTypeObject.h"
#include "SensorTypeObjectData.h"
#include "PerceptionLevel.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( FindObjectType, unsigned int, ( const char* type ) )
DECLARE_HOOK( GetObjectType, size_t, ( const SWORD_Model* object ) )
DECLARE_HOOK( GetKnowledgeObjectType, size_t, ( const SWORD_Model* object ) )

// -----------------------------------------------------------------------------
// Name: SensorTypeObject constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
SensorTypeObject::SensorTypeObject( const SensorType& type, xml::xistream& xis )
    : type_        ( type )
    , rMaxDistance_( 0 )
{
    xis >> xml::list( "object", *this, &SensorTypeObject::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObject::ReadObject
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void SensorTypeObject::ReadObject( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );
    try
    {
        const size_t objectTypeIdentifier = GET_HOOK( FindObjectType )( strType.c_str() );
        if( objectData_.size() <= objectTypeIdentifier )
            objectData_.resize( objectTypeIdentifier + 1, 0 );
        const SensorTypeObjectData* pObjectData = new SensorTypeObjectData( xis );
        objectData_[ objectTypeIdentifier ] = pObjectData;
        rMaxDistance_ = std::max( rMaxDistance_, pObjectData->GetMaxDistance() );
    }
    catch( std::exception& e )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObject destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
SensorTypeObject::~SensorTypeObject()
{
    for( CIT_ObjectDataVector it = objectData_.begin(); it != objectData_.end(); ++it )
        if( *it )
            delete *it;
    objectData_.clear();
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObject::ComputeObjectPerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeObject::ComputeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target, double rSensorHeight ) const
{
    const size_t type = GET_HOOK( GetObjectType )( target );
    if( objectData_.size() <= type )
        return PerceptionLevel::notSeen_;
    const SensorTypeObjectData* pObjectData = objectData_[ type ];
    if( !pObjectData )
        return PerceptionLevel::notSeen_;
    return pObjectData->ComputeObjectPerception( perceiver, target, rSensorHeight );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObject::ComputeKnowledgeObjectPerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& SensorTypeObject::ComputeKnowledgeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target, double rSensorHeight ) const
{
    const size_t type = GET_HOOK( GetKnowledgeObjectType )( target );
    if( objectData_.size() <= type )
        return PerceptionLevel::notSeen_;
    const SensorTypeObjectData* pObjectData = objectData_[ type ];
    if( !pObjectData )
        return PerceptionLevel::notSeen_;
    return pObjectData->ComputeKnowledgeObjectPerception( perceiver, target, rSensorHeight );
}

// -----------------------------------------------------------------------------
// Name: SensorTypeObject::GetType
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
const SensorType& SensorTypeObject::GetType() const
{
    return type_;
}
