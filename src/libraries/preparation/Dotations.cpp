// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Dotations.h"
#include "Dotation.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Controller.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Dotations constructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotations::Dotations( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotations constructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotations::Dotations( xml::xistream& xis, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{
    xis >> optional() >> start( "dotations" )
            >> list( "dotation", *this, &Dotations::ReadDotation )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: Dotations destructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotations::~Dotations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotations::ReadDotation
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void Dotations::ReadDotation( xml::xistream& xis )
{
    Dotation* dotation = new Dotation( xis, resolver_ );
    Register( dotation->type_->GetId(), *dotation );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Dotations::DoSerialize
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void Dotations::SerializeAttributes( xml::xostream& xos ) const
{
    if( elements_.empty() )
        return;
    xos << start( "dotations" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "dotation" );
        it->second->SerializeAttributes( xos );
        xos << end();
    }
    xos << end();
}
