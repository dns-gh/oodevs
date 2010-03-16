// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PerceptionCapacity.h"
#include "Object.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( PerceptionCapacity )

// -----------------------------------------------------------------------------
// Name: PerceptionCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
PerceptionCapacity::PerceptionCapacity( xml::xistream& xis )    
:  modifier_( xml::attribute< bool >( xis, "blinded" ) )
{
}

// -----------------------------------------------------------------------------
// Name: PerceptionCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
PerceptionCapacity::PerceptionCapacity()    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionCapacity constructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
PerceptionCapacity::PerceptionCapacity( const PerceptionCapacity& from )    
: modifier_ ( from.modifier_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionCapacity destructor
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
PerceptionCapacity::~PerceptionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionCapacity::serialize
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
template< typename Archive >
void PerceptionCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & modifier_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionCapacity::Register
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
void PerceptionCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionCapacity::Instanciate
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
void PerceptionCapacity::Instanciate( Object& object ) const
{    
    PerceptionCapacity* capacity = new PerceptionCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: AttitudeModifierCapacity::ProcessAgentInside
// Created: MGD 2010-03-15
// -----------------------------------------------------------------------------
void PerceptionCapacity::ProcessPopulationInside( Object& object, MIL_PopulationElement_ABC& population )
{
    population.GetPopulation().SetBlinded( modifier_ );
}
