// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanColor.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "ENT/ENT_Enums.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: UrbanColor constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanColor::UrbanColor( const kernel::Entity_ABC* parent, kernel::UrbanObject_ABC& object, gui::PropertiesDictionary& dictionary )
{
    if( parent )
    {
        const kernel::UrbanColor_ABC& parentColor = parent->Get< kernel::UrbanColor_ABC >();
        SetColor( parentColor.Red(), parentColor.Green(), parentColor.Blue(), static_cast< int >( parentColor.Alpha() * 255 + 0.5 ) );
        initial_ = current_;
    }
    dictionary.Register( object, tools::translate( "UrbanColor", "Info/Color" ), current_, false, gui::eUrbanTemplate );
}

// -----------------------------------------------------------------------------
// Name: UrbanColor constructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::UrbanColor( xml::xistream& xis, kernel::UrbanObject_ABC& object, gui::PropertiesDictionary& dictionary )
{
    if( xis.has_child( "color" ) )
    {
        xis >> xml::start( "color" );
        SetColor( xis.attribute< int >( "red" ), xis.attribute< int >( "green" ),
                  xis.attribute< int >( "blue" ), static_cast< int >( xis.attribute< float >( "alpha" ) * 255 ) );
        initial_ = current_;
        xis >> xml::end;
    }
    dictionary.Register( object, tools::translate( "UrbanColor", "Info/Color" ), current_, false, gui::eUrbanTemplate );
}

// -----------------------------------------------------------------------------
// Name: UrbanColor destructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::~UrbanColor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::SerializeAttributes
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanColor::SerializeAttributes( xml::xostream& xos ) const
{
    if( current_.red_ != 200 || current_.green_ != 200 || current_.blue_ != 200 || current_.alpha_ != 180 )
    {
        xos << xml::start( "color" )
            << xml::attribute< int >( "red", current_.red_ )
            << xml::attribute< int >( "green", current_.green_ )
            << xml::attribute< int >( "blue", current_.blue_ )
            << xml::attribute< float >( "alpha", Alpha() )
        << xml::end;
    }
}
