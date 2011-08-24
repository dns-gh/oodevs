// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TrafficabilityAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( kernel::PropertiesDictionary& dictionary, double value )
    : max_( value, Units::tons )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary )
    : max_( xis.attribute< double >( "max" ), Units::tons )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute destructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::~TrafficabilityAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::Display
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Trafficability" ) )
             .Display( tools::translate( "Object", "Max:" ), max_ );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::SerializeAttributes
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "trafficability" )
            << xml::attribute( "max", double( max_.value_ ) )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::CreateDictionary
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::CreateDictionary( kernel::PropertiesDictionary& dictionary )
{
    dictionary.Register( *this, tools::translate( "TrafficabilityAttribute", "Info/Trafficability/Max:" ), double( max_.value_ ) );
}
