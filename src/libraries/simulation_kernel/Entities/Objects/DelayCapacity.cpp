// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DelayCapacity.h"
#include "DelayAttribute.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DelayCapacity )

// -----------------------------------------------------------------------------
// Name: DelayCapacity constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayCapacity::DelayCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayCapacity::DelayCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity constructor
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
DelayCapacity::DelayCapacity( const DelayCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity destructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayCapacity::~DelayCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity::serialize
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
template< typename Archive >
void DelayCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity::Instanciate
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    DelayCapacity* capacity = new DelayCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
    object.GetAttribute< DelayAttribute >() = DelayAttribute();
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity::Register
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity::CanInteractWith
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void DelayCapacity::CanInteractWith( const MIL_Object_ABC& object, const MIL_Agent_ABC& agent, bool& canInteract )
{
    auto it = agentInsideMap_.find( &agent );
    if( it != agentInsideMap_.end() )
        canInteract &= object.GetAttribute< DelayAttribute >().IsOverDelay( it->second );
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity::PreprocessAgent
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void DelayCapacity::PreprocessAgent( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    auto it = agentInsideMap_.find( &agent );
    if( it == agentInsideMap_.end() )
        agentInsideMap_[ &agent ] = 0;
    else
        it->second++;
}

// -----------------------------------------------------------------------------
// Name: DelayCapacity::ProcessAgentExiting
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayCapacity::ProcessAgentExiting( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    agentInsideMap_.erase( &agent );
}
