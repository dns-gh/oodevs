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
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( gui::PropertiesDictionary& dico, const kernel::Entity_ABC& entity )
    : rBypassConstructionPercentage_( 0, Units::percentage )
{
    CreateDictionary( dico, entity );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( xml::xistream& xis, gui::PropertiesDictionary& /*dictionary*/, const kernel::Entity_ABC& /*entity*/ )
    : rBypassConstructionPercentage_ ( 0, Units::percentage )
{
    float percentage = xis.attribute< float >( "value" );
    rBypassConstructionPercentage_.value_ = static_cast< int >( percentage * 100 + 0.5 );
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
             .Display( tools::translate( "Object", "Bypass:" ), rBypassConstructionPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void BypassAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Bypass:" ), rBypassConstructionPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::SerializeObjectAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "bypass" )
            << xml::attribute( "value", rBypassConstructionPercentage_.value_ * 0.01f )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::CreateDictionary( gui::PropertiesDictionary& dico, const kernel::Entity_ABC& entity )
{
    dico.RegisterExtension( entity, this, tools::translate( "Object", "Info/Breaching/Value" ), rBypassConstructionPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::SetBypassConstruction
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
void BypassAttribute::SetBypassConstruction( int value )
{
    rBypassConstructionPercentage_.value_ = value;
}