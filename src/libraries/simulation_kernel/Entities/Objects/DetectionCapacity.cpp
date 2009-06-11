// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DetectionCapacity.h"
#include "AnimatorAttribute.h"
#include "Object.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( DetectionCapacity, "DetectionCapacity" )

// -----------------------------------------------------------------------------
// Name: DetectionCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity( xml::xistream& xis )
{
    UNREFERENCED_PARAMETER( xis );
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity( const DetectionCapacity& from )
{
    UNREFERENCED_PARAMETER( from );
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity Destructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::~DetectionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DetectionCapacity::serialize
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
template< typename Archive > 
void DetectionCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::Register
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void DetectionCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::Instanciate
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void DetectionCapacity::Instanciate( Object& object ) const
{    
    DetectionCapacity* capacity = new DetectionCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ProcessAgentInside
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void DetectionCapacity::ProcessAgentInside( Object& object, MIL_Agent_ABC& agent )
{
    const AnimatorAttribute::T_AgentSet& animators = object.GetAttribute< AnimatorAttribute >().GetAnimators();

    for( AnimatorAttribute::CIT_AgentSet itAnimator = animators.begin(); itAnimator != animators.end(); ++itAnimator )
        const_cast< MIL_Agent_ABC& >(**itAnimator).GetRole< PHY_RolePion_Perceiver >().NotifyExternalPerception( agent, PHY_PerceptionLevel::identified_ );
}
