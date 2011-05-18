// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AltitudeModifierAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( PropertiesDictionary& dico )
    : height_( 0, Units::meters)
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( xml::xistream& xis, PropertiesDictionary& dico )
    : height_( 0, Units::meters)
{
    xis >> xml::attribute( "height", height_.value_ );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::~AltitudeModifierAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Display
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Altitude modifier:" ), height_ * Units::meters );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SerializeAttributes
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "altitude-modifier" )
            << xml::attribute( "height", height_.value_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SetHeight
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SetHeight( unsigned int height )
{
    height_.value_ = height;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::CreateDictionary
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::CreateDictionary( PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "AltitudeModifierAttribute", "Info/Altitude modifier/Height" ), height_ );
}
