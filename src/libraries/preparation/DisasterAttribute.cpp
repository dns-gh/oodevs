// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "DisasterAttribute.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: DisasterAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
DisasterAttribute::DisasterAttribute( gui::PropertiesDictionary& dictionary, const tools::Path& source, const QDateTime& date, const kernel::Entity_ABC& entity )
    : source_( source )
    , date_  ( date )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
DisasterAttribute::DisasterAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    std::string date;
    xis >> xml::attribute( "source", source_ )
        >> xml::optional
        >> xml::attribute( "date", date );

    if( date != "" )
        date_ = QDateTime::fromString( date.c_str(), "yyyyMMddThhmmss" );

    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute destructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
DisasterAttribute::~DisasterAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::CreateDictionary
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
void DisasterAttribute::CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "DisasterAttribute", "Info/Data source" ), source_ );
    if( !date_.isNull() )
        dictionary.RegisterExtension( entity, this, tools::translate( "DisasterAttribute", "Info/Start date" ), date_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::SerializeObjectAttributes
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
void DisasterAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "disaster" )
           << xml::attribute( "source", source_ );
    if( !date_.isNull() )
        xos << xml::attribute( "date", date_.toString( "yyyyMMddThhmmss" ) );
    xos << xml::end;
}
