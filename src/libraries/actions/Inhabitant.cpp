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
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"

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
Inhabitant::Inhabitant( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Inhabitant_ABC >( parameter, resolver.FindInhabitant( id ), controller )
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

// -----------------------------------------------------------------------------
// Name: Inhabitant::Accept
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void Inhabitant::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
