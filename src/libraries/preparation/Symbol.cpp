// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Symbol.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Symbol constructor
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
Symbol::Symbol()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Symbol constructor
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
Symbol::Symbol( xml::xistream& xis )
{
    if( xis.has_attribute( "nature" ) )
    {
        std::string nature = xis.attribute< std::string >( "nature" );
        if ( !nature.empty() && nature.find( "symbols/" ) == std::string::npos )
            nature = "symbols/" + nature;
        OverrideValue( nature );
    }
}

// -----------------------------------------------------------------------------
// Name: Symbol destructor
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
Symbol::~Symbol()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Symbol::SerializeAttributes
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
void Symbol::SerializeAttributes( xml::xostream& xos ) const
{
    if( IsOverriden() )
    {
        std::string nature = GetValue();
        if ( nature.find( "symbols/" ) == 0 )
            nature = nature.substr( 8, nature.length() - 8 );
        xos << xml::attribute( "nature", nature );
    }
}
