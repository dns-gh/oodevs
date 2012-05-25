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
#include "clients_kernel/ModeController_ABC.h"
#include "ENT/ENT_Enums_Gen.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::Usages( xml::xistream& xis, kernel::PropertiesDictionary& dictionary,
                const kernel::AccommodationTypes& accommodationTypes, float livingSpace )
    : kernel::Usages( dictionary, accommodationTypes, livingSpace )
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
    if( usages_.empty() )
        return;

    xos << xml::start( "usages" );
    BOOST_FOREACH( const kernel::T_Usages::value_type& usage, usages_ )
    {
        if( usage.first == defaultStr_ )
            continue;
        xos << xml::start( "usage" )
            << xml::attribute( "type", usage.first )
            << xml::attribute( "proportion", usage.second / 100.f )
            << xml::end;
    }
    xos << xml::end;
}
