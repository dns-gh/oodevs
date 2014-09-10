// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PopulationFilterCapacity.h"
#include "PopulationAttribute.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PopulationFilterCapacity )

// -----------------------------------------------------------------------------
// Name: PopulationFilterCapacity::PopulationFilterCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
PopulationFilterCapacity::PopulationFilterCapacity( xml::xistream& xis )
    : density_( xis.attribute< double >( "density" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFilterCapacity::PopulationFilterCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
PopulationFilterCapacity::PopulationFilterCapacity()
    : density_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFilterCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
PopulationFilterCapacity::~PopulationFilterCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFilterCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PopulationFilterCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & density_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFilterCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void PopulationFilterCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFilterCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void PopulationFilterCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new PopulationFilterCapacity( *this ) );
    object.GetAttribute< PopulationAttribute >() = PopulationAttribute( density_ );
}
