// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LodgingAttribute.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    xis >> xml::attribute( "capacity", lodgingCapacity_ );
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute destructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingAttribute::~LodgingAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::DisplayInTooltip
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
void LodgingAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Capacity:" ), lodgingCapacity_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::SerializeObjectAttributes
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
void LodgingAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "lodging" )
        << xml::attribute( "capacity", lodgingCapacity_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::SetLodgingCapacity
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
void LodgingAttribute::SetLodgingCapacity( unsigned int capacity )
{
    lodgingCapacity_ = capacity;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::CreateDictionary
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
void LodgingAttribute::CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "LodgingAttribute", "Info/Lodging attributes/Capacity" ), lodgingCapacity_ );
}
