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
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: PopulationAttribute::PopulationAttribute
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
PopulationAttribute::PopulationAttribute()
    : density_ ( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute constructor
// Created: LDC 2009-04-01
// -----------------------------------------------------------------------------
PopulationAttribute::PopulationAttribute( xml::xistream& xis )
    : density_( xml::attribute< float >( xis, "density" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationAttribute destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
PopulationAttribute::~PopulationAttribute()
{

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
MT_Float PopulationAttribute::GetDensity() const
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
void PopulationAttribute::SetDensity( MT_Float rDensity )
{
    density_ = rDensity;
}
