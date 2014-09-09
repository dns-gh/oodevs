// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ScatteringCapacity.h"
#include "MIL_Object_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ScatteringCapacity )

// -----------------------------------------------------------------------------
// Name: ScatteringCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
ScatteringCapacity::ScatteringCapacity( xml::xistream& xis )
    : humanByTimeStep_( xis.attribute< unsigned int >( "human-by-time-step" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScatteringCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
ScatteringCapacity::ScatteringCapacity()
    : humanByTimeStep_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScatteringCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
ScatteringCapacity::ScatteringCapacity( const ScatteringCapacity& from )
    : humanByTimeStep_( from.humanByTimeStep_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScatteringCapacity destructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
ScatteringCapacity::~ScatteringCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScatteringCapacity::serialize
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
template< typename Archive >
void ScatteringCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & humanByTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: ScatteringCapacity::Register
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
void ScatteringCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: ScatteringCapacity::Instanciate
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
void ScatteringCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    ScatteringCapacity* capacity = new ScatteringCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC* >( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity::ProcessAgentInside
// Created: MGD 2010-03-15
// -----------------------------------------------------------------------------
void ScatteringCapacity::ProcessPopulationInside( MIL_Object_ABC& /*object*/, MIL_PopulationElement_ABC& population )
{
    population.PullHumans( humanByTimeStep_ );
}
