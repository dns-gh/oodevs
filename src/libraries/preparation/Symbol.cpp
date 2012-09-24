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
Symbol::Symbol( const std::string& symbol )
{
    MergeSymbol( symbol );
}

// -----------------------------------------------------------------------------
// Name: Symbol constructor
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
Symbol::Symbol( xml::xistream& xis, const std::string& defaultSymbol )
{
    if( xis.has_attribute( "symbol" ) )
    {
        std::string nature = xis.attribute< std::string >( "symbol" );
        if( nature.empty() )
            nature = defaultSymbol;
        else if( nature.find( "symbols/" ) == std::string::npos )
            nature = "symbols/" + nature;
        OverrideValue( nature );
        if( xis.has_attribute( "overridden-symbol" ) )
        {
            bool overridden = xis.attribute< bool >( "overridden-symbol" );
            SetOverriden( overridden );
        }
        else
        {
            ResetSymbol( nature );
            SetOverriden( false );
    }
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
    xos << xml::attribute( "symbol", GetValue() );;
    if( IsOverriden() )
        xos << xml::attribute( "overridden-symbol", true );
}
