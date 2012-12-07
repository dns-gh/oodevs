// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SensorTypeDisaster.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Objects/DisasterCapacity.h"
#include "Entities/Objects/DisasterAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_DisasterType.h"
#include "Knowledge/DEC_Knowledge_Object.h"

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeDisaster constructor
// Created: LGY 2012-12-05
// -----------------------------------------------------------------------------
PHY_SensorTypeDisaster::PHY_SensorTypeDisaster( xml::xistream& xis )
{
    xis >> xml::list( "disaster", *this, &PHY_SensorTypeDisaster::ReadDisaster );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeDisaster destructor
// Created: LGY 2012-12-05
// -----------------------------------------------------------------------------
PHY_SensorTypeDisaster::~PHY_SensorTypeDisaster()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeDisaster::ReadDisaster
// Created: LGY 2012-12-05
// -----------------------------------------------------------------------------
void PHY_SensorTypeDisaster::ReadDisaster( xml::xistream& xis )
{
    std::string disasterType;
    float detectionThreshold;
    xis >> xml::attribute( "type", disasterType )
        >> xml::attribute( "detection-threshold", detectionThreshold );
    disasters_[ disasterType ] = detectionThreshold;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeDisaster::ComputePerception
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeDisaster::ComputePerception( const MT_Vector2D& position, const MIL_Object_ABC& object ) const
{
    if( disasters_.empty() )
        return PHY_PerceptionLevel::notSeen_;
    const DisasterCapacity* capacity = object.Retrieve< DisasterCapacity >();
    if( !capacity )
        return PHY_PerceptionLevel::notSeen_;
    const std::string& disasterName = capacity->GetDisasterType().GetName();
    auto it = disasters_.find( disasterName );
    if( it == disasters_.end() )
        return PHY_PerceptionLevel::notSeen_;
    float dose = capacity->GetDose( object, position );
    if( dose == 0.f )
        return PHY_PerceptionLevel::notSeen_;

    return dose >= it->second ? PHY_PerceptionLevel::identified_ : PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeDisaster::ComputePerception
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeDisaster::ComputePerception( const MT_Vector2D& position, const DEC_Knowledge_Object& knowledge ) const
{
    if( disasters_.empty() )
        return PHY_PerceptionLevel::notSeen_;
    const DisasterCapacity* capacity = knowledge.GetType().GetCapacity< DisasterCapacity >();
    if( !capacity )
        return PHY_PerceptionLevel::notSeen_;
    const std::string& disasterName = capacity->GetDisasterType().GetName();
    auto it = disasters_.find( disasterName );
    if( it == disasters_.end() )
        return PHY_PerceptionLevel::notSeen_;
    const DisasterAttribute* attribute = knowledge.RetrieveAttribute< DisasterAttribute >();
    if( !attribute )
        return PHY_PerceptionLevel::notSeen_;
    float dose = attribute->GetDose( position );
    if( dose == 0.f )
        return PHY_PerceptionLevel::notSeen_;

    return dose >= it->second ? PHY_PerceptionLevel::identified_ : PHY_PerceptionLevel::notSeen_;
}
