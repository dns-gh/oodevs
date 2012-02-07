// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InteractWithEnemyCapacity.h"
#include "MIL_Object_ABC.h"
#include "simulation_kernel/Entities/Agents/MIL_Agent_ABC.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( InteractWithEnemyCapacity )

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity constructor
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
InteractWithEnemyCapacity::InteractWithEnemyCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity constructor
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
InteractWithEnemyCapacity::InteractWithEnemyCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity destructor
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
InteractWithEnemyCapacity::~InteractWithEnemyCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity::serialize
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
template< typename Archive > void InteractWithEnemyCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity::Register
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
void InteractWithEnemyCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity::Instanciate
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
void InteractWithEnemyCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    InteractWithEnemyCapacity* capacity = new InteractWithEnemyCapacity( *this );
    object.AddCapacity( capacity );
}
