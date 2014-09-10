// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InteractIfEquippedCapacity.h"
#include "SupplyRouteAttribute.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( InteractIfEquippedCapacity )

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity constructor
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
InteractIfEquippedCapacity::InteractIfEquippedCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity constructor
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
InteractIfEquippedCapacity::InteractIfEquippedCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity destructor
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
InteractIfEquippedCapacity::~InteractIfEquippedCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity::CanInteractWith
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
void InteractIfEquippedCapacity::CanInteractWith( const MIL_Object_ABC& object, const MIL_Agent_ABC& , bool& canInteract )
{
    canInteract &= object.GetAttribute< SupplyRouteAttribute >().IsEquipped();
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void InteractIfEquippedCapacity::serialize
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
template< typename Archive > void InteractIfEquippedCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity::Register
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
void InteractIfEquippedCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity::Instanciate
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
void InteractIfEquippedCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    InteractIfEquippedCapacity* capacity = new InteractIfEquippedCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );

    const TER_Localisation& currentLocalisation = object.GetLocalisation();

    if( currentLocalisation.GetType() == TER_Localisation::eLine && currentLocalisation.GetPoints().size() >= 2 )
    {
        TER_Localisation newLocalisation( currentLocalisation );
        newLocalisation.Scale( 1000 );
        object.UpdateLocalisation( newLocalisation );
    }
}

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity constructor
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
InteractIfEquippedCapacity::InteractIfEquippedCapacity( const InteractIfEquippedCapacity& )
{
    // NOTHING
}
