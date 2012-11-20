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
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: DisasterAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
DisasterAttribute::DisasterAttribute( kernel::PropertiesDictionary& dictionary, const QString& source, const QDateTime& date )
    : source_( source )
    , date_  ( date )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
DisasterAttribute::DisasterAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary )
{
    std::string source;
    unsigned int time = 0u;
    xis >> xml::attribute( "source", source )
        >> xml::optional
        >> xml::attribute( "date", time );

    if( time != 0u )
        date_.setTime_t( time );

    source_ = source.c_str();
    CreateDictionary( dictionary );
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
void DisasterAttribute::CreateDictionary( kernel::PropertiesDictionary& dictionary )
{
    dictionary.Register( *this, tools::translate( "DisasterAttribute", "Info/Data source" ), source_, true );
    if( !date_.isNull() )
        dictionary.Register( *this, tools::translate( "DisasterAttribute", "Info/Start date" ), date_ );
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
        xos << xml::attribute( "date", date_.toTime_t() );
    xos << xml::end;
}
