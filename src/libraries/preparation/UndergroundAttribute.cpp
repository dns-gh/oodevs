// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UndergroundAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( PropertiesDictionary& dico, kernel::Controller& controller )
    : controller_( controller )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( xml::xistream& xis, kernel::Controller& controller, PropertiesDictionary& dico )
    : controller_( controller )
{
    xis >> xml::attribute( "network", network_ );
    controller_.Update( *static_cast< UndergroundAttribute_ABC* >( this ) );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute destructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundAttribute::~UndergroundAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::GetNetwork
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
const std::string& UndergroundAttribute::GetNetwork() const
{
    return network_;
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::Display
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Underground network:" ), network_ );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::SerializeAttributes
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "underground" )
            << xml::attribute( "network", network_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::SetNetwork
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
void UndergroundAttribute::SetNetwork( const std::string& network )
{
    network_ = network;
    controller_.Update( *static_cast< UndergroundAttribute_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::CreateDictionary
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundAttribute::CreateDictionary( PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "UndergroundAttribute", "Info/Underground/Network" ), network_ );
}
