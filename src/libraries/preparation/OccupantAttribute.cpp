// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
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
OccupantAttribute::OccupantAttribute( kernel::PropertiesDictionary& dico )
: iMaxSize_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute constructor
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
OccupantAttribute::OccupantAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : iMaxSize_( xis.attribute<int>("max-size") )
{
    CreateDictionary( dico );
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
// Name: OccupantAttribute::SerializeAttributes
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
void OccupantAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "max-size" )
            << xml::attribute( "max-size", iMaxSize_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::CreateDictionary
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
void OccupantAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "Object", "Info/Occupant attributes/Max Entities" ), iMaxSize_ );
}