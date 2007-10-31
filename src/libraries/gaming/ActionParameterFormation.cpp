// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterFormation.h"
#include "clients_kernel/Formation_ABC.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterFormation constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterFormation::ActionParameterFormation( const OrderParameter& parameter, const Formation_ABC& formation )
    : ActionParameterEntity< Formation_ABC >( parameter, &formation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFormation constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterFormation::ActionParameterFormation( const OrderParameter& parameter, const ASN1T_OID& asn, const Resolver_ABC< Formation_ABC >& resolver )
    : ActionParameterEntity< Formation_ABC >( parameter, &resolver.Get( asn ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFormation constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterFormation::ActionParameterFormation( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Formation_ABC >& resolver )
    : ActionParameterEntity< Formation_ABC >( parameter, &resolver.Get( attribute< unsigned long >( xis, "value" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFormation destructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterFormation::~ActionParameterFormation()
{
    // NOTHING
}
