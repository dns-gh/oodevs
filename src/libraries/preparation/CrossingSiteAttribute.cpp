// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "CrossingSiteAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : width_            ( 0, Units::meters )
    , depth_            ( 0, Units::meters )
    , speed_            ( 0, Units::metersPerSecond )
    , needsConstruction_( false )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : width_            ( 0, Units::meters )
    , depth_            ( 0, Units::meters )
    , speed_            ( 0, Units::metersPerSecond )
    , needsConstruction_( false )
{
    xis >> xml::content( "width", (int&)(width_.value_) )
        >> xml::content( "depth", (int&)(depth_.value_) )
        >> xml::content( "speed", (int&)(speed_.value_) )
        >> xml::content( "construction-needed", needsConstruction_ );
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttribute::~CrossingSiteAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Crossing site" ) )
                .Display( tools::translate( "Object", "Width:" ), width_ )
                .Display( tools::translate( "Object", "Depth:" ), depth_ )
                .Display( tools::translate( "Object", "Stream speed:" ), speed_ )
                .Display( tools::translate( "Object", "Build river banks:" ), needsConstruction_ );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::SetWidth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SetWidth( unsigned int value )
{
    width_.value_ = value;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::SetDepth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SetDepth( unsigned int value )
{
    depth_.value_ = value;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::SetFlowSpeed
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SetFlowSpeed( unsigned int value )
{
    speed_.value_ = value;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::SetConstruction
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SetConstruction( bool need )
{
    needsConstruction_ = need;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::SerializeObjectAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "crossing-site" )
            << xml::content( "width", int( width_.value_ ) )
            << xml::content( "depth", int( depth_.value_ ) )
            << xml::content( "speed", int( speed_.value_ ) )
            << xml::content( "construction-needed", needsConstruction_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "CrossingSiteAttribute", "Info/Crossing site attributes/Width" ), width_ );
    dictionary.RegisterExtension( entity, this, tools::translate( "CrossingSiteAttribute", "Info/Crossing site attributes/Depth" ), depth_ );
    dictionary.RegisterExtension( entity, this, tools::translate( "CrossingSiteAttribute", "Info/Crossing site attributes/Speed" ), speed_ );
    dictionary.RegisterExtension( entity, this, tools::translate( "CrossingSiteAttribute", "Info/Crossing site attributes/Build river banks" ), needsConstruction_ );
}
