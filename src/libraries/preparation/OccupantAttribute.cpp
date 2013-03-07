// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "OccupantAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OccupantAttribute constructor
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
OccupantAttribute::OccupantAttribute( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : iMaxSize_( 0 )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute constructor
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
OccupantAttribute::OccupantAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : iMaxSize_( xis.attribute< int >("max-size") )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute destructor
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
OccupantAttribute::~OccupantAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::Display
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
void OccupantAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Occupant:" ), iMaxSize_ );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::DisplayInTooltip
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
void OccupantAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Occupant:" ), iMaxSize_ );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::SerializeObjectAttributes
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
void OccupantAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "max-size" )
            << xml::attribute( "max-size", iMaxSize_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::CreateDictionary
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
void OccupantAttribute::CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "Object", "Info/Occupant attributes/Max Entities" ), iMaxSize_ );
}
