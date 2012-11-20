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
DisasterAttribute::DisasterAttribute( kernel::PropertiesDictionary& dictionary, const QString& source, const QTime& time )
    : source_( source )
    , time_  ( time )
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
    unsigned int time;
    xis >> xml::attribute( "source", source )
        >> xml::optional
        >> xml::attribute( "time", time );

    time_ = time_.addSecs( time );

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
    if( !time_.isNull() )
        dictionary.Register( *this, tools::translate( "DisasterAttribute", "Info/Start time" ), time_ );
}

namespace
{
    unsigned int Convert( const QTime& time )
    {
        return  time.hour() * 3600 + time.minute() * 60 + time.second();;
    }
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::SerializeObjectAttributes
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
void DisasterAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "disaster" )
           << xml::attribute( "source", source_ );
    if( !time_.isNull() )
        xos << xml::attribute( "time", Convert( time_ ) );
    xos << xml::end;
}
