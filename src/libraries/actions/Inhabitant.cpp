// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "Inhabitant.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< Inhabitant_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Inhabitant_ABC >( parameter, &resolver.GetInhabitant( xis.attribute< unsigned long >( "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Inhabitant_ABC >( parameter, &resolver.GetInhabitant( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Inhabitant_ABC >( OrderParameter( xis.attribute< std::string >( "name" ), "crowd", false ), &resolver.GetInhabitant( xis.attribute< unsigned long >( "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant destructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Inhabitant::~Inhabitant()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SerializeType
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
std::string Inhabitant::SerializeType() const
{
    return std::string( "inhabitant" );
}
