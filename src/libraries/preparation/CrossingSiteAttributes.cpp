// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "CrossingSiteAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttributes::CrossingSiteAttributes( kernel::PropertiesDictionary& dico )
    : width_( 0, Units::meters )
    , depth_( 0, Units::meters )
    , speed_( 0, Units::metersPerSecond )
    , needsConstruction_( false )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
CrossingSiteAttributes::CrossingSiteAttributes( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : width_( 0, Units::meters )
    , depth_( 0, Units::meters )
    , speed_( 0, Units::metersPerSecond )
    , needsConstruction_( false )
{
    xis >> start( "specific-attributes" )
            >> content( "width", (int&)(width_.value_) )
            >> content( "depth", (int&)(depth_.value_) )
            >> content( "speed", (int&)(speed_.value_) )
            >> content( "construction-needed", needsConstruction_ )
        >> end();
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttributes::~CrossingSiteAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Crossing site" ) )
                .Display( tools::translate( "Object", "Width:" ), width_ )
                .Display( tools::translate( "Object", "Depth:" ), depth_ )
                .Display( tools::translate( "Object", "Stream speed:" ), speed_ )
                .Display( tools::translate( "Object", "Build river banks:" ), needsConstruction_ );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SetWidth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SetWidth( unsigned int value )
{
    width_.value_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SetDepth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SetDepth( unsigned int value )
{
    depth_.value_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SetFlowSpeed
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SetFlowSpeed( unsigned int value )
{
    speed_.value_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SetConstruction
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SetConstruction( bool need )
{
    needsConstruction_ = need;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "specific-attributes" )
            << content( "width", int( width_.value_ ) )
            << content( "depth", int( depth_.value_ ) )
            << content( "speed", int( speed_.value_ ) )
            << content( "construction-needed", needsConstruction_ )
        << end();
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "CrossingSiteAttributes", "Info/Crossing site attributes/Width" ), width_ );
    dico.Register( *this, tools::translate( "CrossingSiteAttributes", "Info/Crossing site attributes/Depth" ), depth_ );
    dico.Register( *this, tools::translate( "CrossingSiteAttributes", "Info/Crossing site attributes/Speed" ), speed_ );
    dico.Register( *this, tools::translate( "CrossingSiteAttributes", "Info/Crossing site attributes/Build river banks" ), needsConstruction_ );
}
