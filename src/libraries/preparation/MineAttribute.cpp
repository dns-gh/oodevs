// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "MineAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : density_( 0, Units::percentage )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : density_( xis.attribute< float >( "density", 0. ) * 100., Units::percentage )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttribute::~MineAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void MineAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Improvable:" ), density_.value_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SerializeObjectAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "mine" ) 
            << xml::attribute( "density", ( density_.value_ != 0 ) ? density_.value_ / 100. : 0. ) 
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SetDensity
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::SetDensity( float density )
{
    density_.value_ = density;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "MineAttribute", "Info/Improvable/Value" ), density_ );
}
