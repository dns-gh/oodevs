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
#include "Object.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( InteractIfEquippedCapacity, "InteractIfEquippedCapacity" )

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
void InteractIfEquippedCapacity::CanInteractWith( const Object& object, const MIL_Agent_ABC& , bool& canInteract )
{
    canInteract &= object.GetAttribute< SupplyRouteAttribute >().IsEquipped();
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void InteractIfEquippedCapacity::serialize
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
template< typename Archive > void InteractIfEquippedCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity::Register
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
void InteractIfEquippedCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: InteractIfEquippedCapacity::Instanciate
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
void InteractIfEquippedCapacity::Instanciate( Object& object ) const
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
