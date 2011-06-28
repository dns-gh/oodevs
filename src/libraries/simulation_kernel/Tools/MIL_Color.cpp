// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Color.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Color )

// -----------------------------------------------------------------------------
// Name: MIL_Color constructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
MIL_Color::MIL_Color()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Color constructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
MIL_Color::MIL_Color( const MIL_Color& color )
    : color_( color.color_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Color constructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
MIL_Color::MIL_Color( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::attribute< std::string >( "color", color_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Color destructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
MIL_Color::~MIL_Color()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Color::load
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void MIL_Color::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> color_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Color::save
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void MIL_Color::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << color_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Color::WriteODB
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void MIL_Color::WriteODB( xml::xostream& xos ) const
{
    if( !color_.empty() )
        xos << xml::attribute( "color", color_ );
}
