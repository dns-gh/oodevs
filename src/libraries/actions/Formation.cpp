// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Formation.h"
#include "clients_kernel/Formation_ABC.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Formation::Formation( const OrderParameter& parameter, const Formation_ABC& formation, kernel::Controller& controller )
    : Entity< Formation_ABC >( parameter, &formation, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
Formation::Formation( const OrderParameter& parameter, const ASN1T_OID& asn, const Resolver_ABC< Formation_ABC >& resolver, kernel::Controller& controller )
    : Entity< Formation_ABC >( parameter, &resolver.Get( asn ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
Formation::Formation( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Formation_ABC >& resolver, kernel::Controller& controller )
    : Entity< Formation_ABC >( parameter, &resolver.Get( attribute< unsigned long >( xis, "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    // NOTHING
}
