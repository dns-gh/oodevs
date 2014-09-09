// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PopulationAttribute.h"
#include "Object.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PopulationAttribute )

// -----------------------------------------------------------------------------
// Name: PopulationAttribute::PopulationAttribute
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
PopulationAttribute::PopulationAttribute()
    : density_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute constructor
// Created: LDC 2009-04-01
// -----------------------------------------------------------------------------
PopulationAttribute::PopulationAttribute( xml::xistream& xis )
    : density_( xis.attribute< double >( "density" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute constructor
// Created: NLD 2010-11-24
// -----------------------------------------------------------------------------
PopulationAttribute::PopulationAttribute( double density )
    : density_( density )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
PopulationAttribute::~PopulationAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: void PopulationAttribute::serialize
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
template< typename Archive >
void PopulationAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & density_;
}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void PopulationAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< PopulationAttribute, PopulationAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: DotationAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
PopulationAttribute& PopulationAttribute::operator=( const PopulationAttribute& rhs )
{
    density_ = rhs.density_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute::GetDensity
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
double PopulationAttribute::GetDensity() const
{
    return density_;
}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute::Reset
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void PopulationAttribute::Reset()
{
    // $$$ TODO
}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute::SetDensity
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void PopulationAttribute::SetDensity( double rDensity )
{
    density_ = rDensity;
}
