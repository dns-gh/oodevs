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
    : rBypassConstructionPercentage_ ( 0., Units::percentage )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : rBypassConstructionPercentage_ ( 0., Units::percentage )
{
    xis >> xml::attribute( "value", rBypassConstructionPercentage_.value_ );
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
// Name: BypassAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "bypass" )
            << xml::attribute( "value", rBypassConstructionPercentage_.value_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void BypassAttribute::CreateDictionary( kernel::PropertiesDictionary& /*dico*/ )
{
    // NOTHING
}