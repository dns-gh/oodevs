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
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( kernel::PropertiesDictionary& dictionary, double value, const kernel::Entity_ABC& entity )
    : max_( value, Units::tons )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : max_( xis.attribute< double >( "max" ), Units::tons )
{
    CreateDictionary( dictionary, entity );
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
// Name: TrafficabilityAttribute::SerializeObjectAttributes
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "trafficability" )
            << xml::attribute( "max", double( max_.value_ ) )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::CreateDictionary
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "TrafficabilityAttribute", "Info/Trafficability/Max:" ), double( max_.value_ ) );
}
