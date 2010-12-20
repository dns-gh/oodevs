// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FloodCapacity.h"
#include "MIL_Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: FloodCapacity constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodCapacity::FloodCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodCapacity::FloodCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity constructor
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
FloodCapacity::FloodCapacity( const FloodCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity destructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodCapacity::~FloodCapacity()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: FloodCapacity::serialize
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
template< typename Archive >
void FloodCapacity::serialize( Archive&, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}
    
// -----------------------------------------------------------------------------
// Name: FloodCapacity::Instanciate
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    FloodCapacity* capacity = new FloodCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}
    
// -----------------------------------------------------------------------------
// Name: FloodCapacity::Register
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::CanInteractWith
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodCapacity::CanInteractWith( const MIL_Object_ABC& /*object*/, const MIL_Agent_ABC& /*agent*/, bool& /*canInteract*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: FloodCapacity::PreprocessAgent
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodCapacity::PreprocessAgent( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ )
{
    // TODO
}
    
// -----------------------------------------------------------------------------
// Name: FloodCapacity::ProcessAgentExiting
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodCapacity::ProcessAgentExiting( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ )
{
    // TODO
}
