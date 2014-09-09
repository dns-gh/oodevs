// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AttitudeModifierCapacity.h"
#include "Object.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( AttitudeModifierCapacity )

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
AttitudeModifierCapacity::AttitudeModifierCapacity( xml::xistream& xis )
{
    std::string sAttitude;
    xis >> xml::attribute( "attitude", sAttitude );
    attitude_ = MIL_PopulationAttitude::Find( sAttitude );
    if( !attitude_ )
        throw MASA_EXCEPTION( "Attitude modifier '" + sAttitude + "' unknown" );
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
AttitudeModifierCapacity::AttitudeModifierCapacity()
    : attitude_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
AttitudeModifierCapacity::AttitudeModifierCapacity( const AttitudeModifierCapacity& from )
    : attitude_( from.attitude_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity destructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
AttitudeModifierCapacity::~AttitudeModifierCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity::serialize
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
template< typename Archive >
void AttitudeModifierCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
//    file & attitude_; // FIXME
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity::Register
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
void AttitudeModifierCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity::Instanciate
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
void AttitudeModifierCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    AttitudeModifierCapacity* capacity = new AttitudeModifierCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity::ProcessAgentInside
// Created: MGD 2010-03-15
// -----------------------------------------------------------------------------
void AttitudeModifierCapacity::ProcessPopulationInside( MIL_Object_ABC& /*object*/, MIL_PopulationElement_ABC& population )
{
    population.SetAttitude( *attitude_ );
}
