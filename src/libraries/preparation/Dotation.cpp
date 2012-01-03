// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Dotation.h"
#include "clients_kernel/DotationType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotation::Dotation( const DotationType& type, unsigned int quantity )
    : type_    ( type )
    , quantity_( quantity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotation::Dotation( xml::xistream& xis, const tools::Resolver_ABC< DotationType, std::string >& resolver )
    : type_    ( resolver.Get( xis.attribute< std::string >( "name" ) ) )
    , quantity_( static_cast< unsigned int >( xis.attribute< double >( "quantity" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation destructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotation::~Dotation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation::Serialize
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void Dotation::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", type_.GetName() )
        << xml::attribute( "quantity", quantity_ );
}
