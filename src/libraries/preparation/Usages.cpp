// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Usages.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "ENT/ENT_Enums.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
Usages::Usages( const kernel::Entity_ABC* parent, gui::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes, float livingSpace, kernel::Entity_ABC& owner )
    : gui::Usages( dictionary, accommodationTypes, livingSpace, owner )
{
    if( parent )
    {
        const kernel::PhysicalAttribute_ABC& parentPhysicalAttribute = parent->Get< kernel::PhysicalAttribute_ABC >();
        const kernel::Usages_ABC& parentUsage = parentPhysicalAttribute.GetUsages();
        const kernel::T_Usages& usages = parentUsage.GetUsages();
        for( auto it = usages.begin(); it != usages.end(); ++it )
            if( it->first != gui::Usages::defaultStr_ )
                Add( it->first, it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::Usages( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes, float livingSpace, kernel::Entity_ABC& owner  )
    : gui::Usages( dictionary, accommodationTypes, livingSpace, owner )
{
    xis >> xml::start( "usages" )
            >> xml::list( "usage", *this, &Usages::ReadUsages )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Usages destructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::~Usages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Usages::ReadUsages
// Created: LGY 2012-04-11
// -----------------------------------------------------------------------------
void Usages::ReadUsages( xml::xistream& xis )
{
    Add( xis.attribute< std::string >( "type" ), static_cast< unsigned int >( ( xis.attribute< float >( "proportion" ) + 0.001 ) * 100u ) );
}

// -----------------------------------------------------------------------------
// Name: Usages::SerializeAttributes
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void Usages::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "usages" );
    for( auto it = usages_.begin(); it != usages_.end(); ++it )
    {
        if( it->first == defaultStr_ )
            continue;
        xos << xml::start( "usage" )
                << xml::attribute( "type", it->first )
                << xml::attribute( "proportion", it->second / 100.f )
            << xml::end;
    }
    xos << xml::end;
}
