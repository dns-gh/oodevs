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
#include "Tools.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( kernel::PropertiesDictionary& dico )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
{
    xis >> xml::attribute( "capacity", lodgingCapacity_ );
    CreateDictionary( dico );
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
// Name: LogisticAttribute::SerializeAttributes
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
void LodgingAttribute::SerializeAttributes( xml::xostream& xos ) const
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
void LodgingAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "LodgingAttribute", "Info/Lodging attributes/Capacity" ), lodgingCapacity_ );
}
