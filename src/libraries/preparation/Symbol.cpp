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
    if( xis.has_attribute( "symbol" ) )
        OverrideValue( xis.attribute< std::string >( "symbol" ) );
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
        xos << xml::attribute( "symbol", GetValue() );
}
