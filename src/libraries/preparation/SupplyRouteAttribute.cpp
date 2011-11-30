// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SupplyRouteAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyRouteAttribute::SupplyRouteAttribute( kernel::PropertiesDictionary& dico )
    : flow_( 0, Units::vehiclesPerHour )
    , width_( 0, Units::meters )
    , length_( 0, Units::meters )
    , maxWeight_( 0, Units::tons )
    , equipped_( false )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
SupplyRouteAttribute::SupplyRouteAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : flow_( 0, Units::vehiclesPerHour )
    , width_( 0, Units::meters )
    , length_( 0, Units::meters )
    , maxWeight_( 0, Units::tons )
    , equipped_( false )
{
    xis >> xml::content( "flow", (int&)(flow_.value_) )
        >> xml::content( "width", (int&)(width_.value_) )
        >> xml::content( "length", (int&)(length_.value_) )
        >> xml::content( "max-weight", (int&)(maxWeight_.value_) )
        >> xml::content( "equipped", equipped_ );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyRouteAttribute::~SupplyRouteAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Supply route" ) )
                .Display( tools::translate( "Object", "Equipped:" ), equipped_ )
                .Display( tools::translate( "Object", "Flow:" ), flow_ )
                .Display( tools::translate( "Object", "Width:" ), width_ )
                .Display( tools::translate( "Object", "Length:" ), length_ )
                .Display( tools::translate( "Object", "Maximum weight:" ), maxWeight_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::SetFlow
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::SetFlow( unsigned int value )
{
    flow_.value_ = value;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::SetWidth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::SetWidth( unsigned int value )
{
    width_.value_ = value;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::SetLength
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::SetLength( unsigned int value )
{
    length_.value_ = value;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::SetMaxWeight
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::SetMaxWeight( unsigned int value )
{
    maxWeight_.value_ = value;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::SetEquipped
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::SetEquipped( bool value )
{
    equipped_ = value;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "supply-route" )
            << xml::content( "flow", int( flow_.value_ ) )
            << xml::content( "width", int( width_.value_ ) )
            << xml::content( "length", int( length_.value_ ) )
            << xml::content( "max-weight", int( maxWeight_.value_ ) )
            << xml::content( "equipped", equipped_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "SupplyRouteAttribute", "Info/Logistic route attributes/Flow" ), flow_ );
    dico.Register( *this, tools::translate( "SupplyRouteAttribute", "Info/Logistic route attributes/Width" ), width_ );
    dico.Register( *this, tools::translate( "SupplyRouteAttribute", "Info/Logistic route attributes/Length" ), length_ );
    dico.Register( *this, tools::translate( "SupplyRouteAttribute", "Info/Logistic route attributes/Maximum weight" ), maxWeight_ );
    dico.Register( *this, tools::translate( "SupplyRouteAttribute", "Info/Logistic route attributes/Equipped" ), equipped_ );
}
