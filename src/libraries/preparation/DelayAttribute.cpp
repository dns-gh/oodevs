// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "DelayAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : delay_( 0, Units::hours )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : delay_( 0, Units::hours )
{
    xis >> xml::attribute( "value", delay_.value_ );
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute destructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::~DelayAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::Display
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Delay time:" ), delay_ );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::SerializeObjectAttributes
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "delay" )
            << xml::attribute( "value", delay_.value_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::SetDelay
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::SetDelay( unsigned int time )
{
    delay_.value_ = time;
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::CreateDictionary
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "DelayAttribute", "Info/Delay parameters/Delay" ), delay_ );
}
