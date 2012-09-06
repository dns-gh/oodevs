// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "BypassAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( kernel::PropertiesDictionary& dico )
    : rBypassConstructionPercentage_ ( 0, Units::percentage )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : rBypassConstructionPercentage_ ( 0, Units::percentage )
{
    float percentage = xis.attribute< float >( "value" );
    rBypassConstructionPercentage_.value_ = static_cast< int >( percentage * 100 + 0.5 );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::~BypassAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Breaching:" ), rBypassConstructionPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void BypassAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Breaching:" ), rBypassConstructionPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "bypass" )
            << xml::attribute( "value", rBypassConstructionPercentage_.value_ * 0.01f )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "Object", "Info/Breaching/Value" ), rBypassConstructionPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::SetBypassConstruction
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
void BypassAttribute::SetBypassConstruction( int value )
{
    rBypassConstructionPercentage_.value_ = value;
}
