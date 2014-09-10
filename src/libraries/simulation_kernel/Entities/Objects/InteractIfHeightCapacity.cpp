// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InteractIfHeightCapacity.h"
#include "InteractionHeightAttribute.h"
#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

BOOST_CLASS_EXPORT_IMPLEMENT( InteractIfHeightCapacity )

// -----------------------------------------------------------------------------
// Name: InteractIfHeightCapacity constructor
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
InteractIfHeightCapacity::InteractIfHeightCapacity( xml::xistream& xis )
    : height_( xis.attribute< double >( "height", 0. ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractIfHeightCapacity constructor
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
InteractIfHeightCapacity::InteractIfHeightCapacity()
    : height_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractIfHeightCapacity destructor
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
InteractIfHeightCapacity::~InteractIfHeightCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractIfHeightCapacity::CanInteractWith
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
void InteractIfHeightCapacity::CanInteractWith( const MIL_Object_ABC& object, const MIL_Agent_ABC& agent, bool& canInteract )
{
    canInteract &= ( agent.GetRole< PHY_RoleInterface_Location >().GetHeight() <= object.GetAttribute< InteractionHeightAttribute >().Get() );
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void InteractIfHeightCapacity::serialize
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
template< typename Archive > void InteractIfHeightCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
    file & height_;
}

// -----------------------------------------------------------------------------
// Name: InteractIfHeightCapacity::Register
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
void InteractIfHeightCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: InteractIfHeightCapacity::Instanciate
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
void InteractIfHeightCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    InteractIfHeightCapacity* capacity = new InteractIfHeightCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
    object.GetAttribute< InteractionHeightAttribute >() = InteractionHeightAttribute( height_ );
}

// -----------------------------------------------------------------------------
// Name: InteractIfHeightCapacity constructor
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
InteractIfHeightCapacity::InteractIfHeightCapacity( const InteractIfHeightCapacity& )
{
    // NOTHING
}
