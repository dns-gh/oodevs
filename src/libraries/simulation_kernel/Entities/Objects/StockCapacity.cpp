// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "StockCapacity.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( StockCapacity )

// -----------------------------------------------------------------------------
// Name: StockCapacity constructor
// Created: LDC 2013-11-07
// -----------------------------------------------------------------------------
StockCapacity::StockCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockCapacity constructor
// Created: LDC 2013-11-07
// -----------------------------------------------------------------------------
StockCapacity::StockCapacity( xml::xistream& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockCapacity destructor
// Created: LDC 2013-11-07
// -----------------------------------------------------------------------------
StockCapacity::~StockCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void StockCapacity::serialize
// Created: LDC 2013-11-07
// -----------------------------------------------------------------------------
template< typename Archive > void StockCapacity::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}
    
// -----------------------------------------------------------------------------
// Name: StockCapacity::Register
// Created: LDC 2013-11-07
// -----------------------------------------------------------------------------
void StockCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: StockCapacity::Instanciate
// Created: LDC 2013-11-07
// -----------------------------------------------------------------------------
void StockCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    StockCapacity* capacity = new StockCapacity();
    object.AddCapacity( capacity );
}
