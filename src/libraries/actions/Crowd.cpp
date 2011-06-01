// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "Crowd.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Crowd constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Crowd::Crowd( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< Population_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Crowd constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Crowd::Crowd( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Population_ABC >( parameter, &resolver.GetPopulation( xis.attribute< unsigned long >( "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Crowd constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Crowd::Crowd( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Population_ABC >( parameter, &resolver.GetPopulation( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Crowd constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Crowd::Crowd( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Population_ABC >( OrderParameter( xis.attribute< std::string >( "name" ), "crowd", false ), &resolver.GetPopulation( xis.attribute< unsigned long >( "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Crowd destructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
Crowd::~Crowd()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Crowd::SerializeType
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
std::string Crowd::SerializeType() const
{
    return std::string( "crowd" );
}
