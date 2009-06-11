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
#include "Object.h"
#include "simulation_kernel/Entities/Agents/MIL_Agent_ABC.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( InteractWithEnemyCapacity, "InteractWithEnemyCapacity" )

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
// Name: InteractWithEnemyCapacity::CanInteractWith
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
void InteractWithEnemyCapacity::CanInteractWith( const Object& object, const MIL_Agent_ABC& agent, bool& canInteract )
{
    if( &agent.GetArmy() == &object.GetArmy() )
        canInteract = false;
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity::serialize
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
template< typename Archive > void InteractWithEnemyCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity::Register
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
void InteractWithEnemyCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity::Instanciate
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
void InteractWithEnemyCapacity::Instanciate( Object& object ) const
{
    InteractWithEnemyCapacity* capacity = new InteractWithEnemyCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: InteractWithEnemyCapacity constructor
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
InteractWithEnemyCapacity::InteractWithEnemyCapacity( const InteractWithEnemyCapacity& )
{
    // NOTHING
}
