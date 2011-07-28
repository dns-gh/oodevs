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
    xis >> xml::optional
        >> xml::attribute( "symbol", symbol_ );
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
// Name: Symbol::GetValue
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
const std::string& Symbol::GetValue() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: Symbol::SetValue
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
void Symbol::SetValue( const std::string& value )
{
    symbol_ = value;
}

// -----------------------------------------------------------------------------
// Name: Symbol::SerializeAttributes
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
void Symbol::SerializeAttributes( xml::xostream& /*xos*/ ) const
{
    // NOTHING
}
