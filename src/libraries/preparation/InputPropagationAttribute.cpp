// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InputPropagationAttribute.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::InputPropagationAttribute( kernel::PropertiesDictionary& dictionary, const QString& source )
    : source_( source )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::InputPropagationAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary )
{
    std::string source;
    xis >> xml::attribute( "source", source );

    source_ = source.c_str();
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute destructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::~InputPropagationAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute::CreateDictionary
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
void InputPropagationAttribute::CreateDictionary( kernel::PropertiesDictionary& dictionary )
{
    dictionary.Register( *this, tools::translate( "InputPropagationAttribute", "Info/Data source" ), source_, true );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute::SerializeObjectAttributes
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
void InputPropagationAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "input-propagation" )
           << xml::attribute( "source", source_ )
        << xml::end;
}
