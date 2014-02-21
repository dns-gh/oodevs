// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "BreakdownPart.h"
#include "tools/Resolver_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BreakdownPart constructor
// Created: ABR 2014-02-20
// -----------------------------------------------------------------------------
BreakdownPart::BreakdownPart()
    : quantity_( 0 )
    , resource_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownPart constructor
// Created: ABR 2014-02-20
// -----------------------------------------------------------------------------
BreakdownPart::BreakdownPart( xml::xistream& xis, const tools::Resolver_ABC< DotationType, std::string >& dotationResolver )
    : quantity_( xis.attribute< unsigned int >( "quantity" ) )
    , resource_( &dotationResolver.Get( xis.attribute< std::string >( "resource" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownPart destructor
// Created: ABR 2014-02-20
// -----------------------------------------------------------------------------
BreakdownPart::~BreakdownPart()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownPart::GetQuantity
// Created: ABR 2014-02-20
// -----------------------------------------------------------------------------
unsigned int BreakdownPart::GetQuantity() const
{
    return quantity_;
}

// -----------------------------------------------------------------------------
// Name: BreakdownPart::GetResource
// Created: ABR 2014-02-20
// -----------------------------------------------------------------------------
const DotationType& BreakdownPart::GetResource() const
{
    if( !resource_ )
        throw MASA_EXCEPTION( "Invalid resource in breakdown part" );
    return *resource_;
}
