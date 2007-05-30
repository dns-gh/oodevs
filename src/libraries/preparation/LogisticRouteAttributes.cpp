// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticRouteAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::LogisticRouteAttributes( kernel::PropertiesDictionary& dico )
    : flow_( 0, Units::vehiclesPerHour )
    , width_( 0, Units::meters )
    , length_( 0, Units::meters )
    , maxWeight_( 0, Units::tons )
    , equipped_( false )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
LogisticRouteAttributes::LogisticRouteAttributes( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : flow_( 0, Units::vehiclesPerHour )
    , width_( 0, Units::meters )
    , length_( 0, Units::meters )
    , maxWeight_( 0, Units::tons )
    , equipped_( false )
{
    xis >> start( "specific-attributes" )
            >> content( "flow", (int&)(flow_.value_) )
            >> content( "width", (int&)(width_.value_) )
            >> content( "length", (int&)(length_.value_) )
            >> content( "max-weight", (int&)(maxWeight_.value_) )
            >> content( "equipped", equipped_ )
        >> end();
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::~LogisticRouteAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Logistic route" ) )
                .Display( tools::translate( "Object", "Equipped:" ), equipped_ )
                .Display( tools::translate( "Object", "Flow:" ), flow_ )
                .Display( tools::translate( "Object", "Width:" ), width_ )
                .Display( tools::translate( "Object", "Length:" ), length_ )
                .Display( tools::translate( "Object", "Maximum weight:" ), maxWeight_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetFlow
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetFlow( unsigned int value )
{
    flow_.value_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetWidth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetWidth( unsigned int value )
{
    width_.value_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetLength
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetLength( unsigned int value )
{
    length_.value_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetMaxWeight
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetMaxWeight( unsigned int value )
{
    maxWeight_.value_ = value;
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetEquipped
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetEquipped( bool value )
{
    equipped_ = value;
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "specific-attributes" )
            << content( "flow", int( flow_.value_ ) )
            << content( "width", int( width_.value_ ) )
            << content( "length", int( length_.value_ ) )
            << content( "max-weight", int( maxWeight_.value_ ) )
            << content( "equipped", equipped_ )
        << end();
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "LogisticRouteAttributes", "Info/Logistic route attributes/Flow" ), flow_ );
    dico.Register( *this, tools::translate( "LogisticRouteAttributes", "Info/Logistic route attributes/Width" ), width_ );
    dico.Register( *this, tools::translate( "LogisticRouteAttributes", "Info/Logistic route attributes/Length" ), length_ );
    dico.Register( *this, tools::translate( "LogisticRouteAttributes", "Info/Logistic route attributes/Maximum weight" ), maxWeight_ );
    dico.Register( *this, tools::translate( "LogisticRouteAttributes", "Info/Logistic route attributes/Equipped" ), equipped_ );
}
