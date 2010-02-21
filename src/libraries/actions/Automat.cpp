// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Automat.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Automat::Automat( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< Automat_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Automat::Automat( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver, kernel::Controller& controller )
    : Entity< Automat_ABC >( parameter, &resolver.Get( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Automat::Automat( const OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< Automat_ABC >& resolver, kernel::Controller& controller )
    : Entity< Automat_ABC >( parameter, &resolver.Get( attribute< unsigned long >( xis, "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Automat::Automat( xml::xistream& xis, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver, kernel::Controller& controller )
    : Entity< Automat_ABC >( OrderParameter( attribute< std::string >( xis, "name" ), attribute< std::string >( xis, "type" ), false ), &resolver.Get( attribute< unsigned long >( xis, "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Automat::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    message.mutable_value()->mutable_automat();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        Entity< Automat_ABC >::CommitTo< Common::MsgAutomat >( *message.mutable_value()->mutable_automat() );
}

// -----------------------------------------------------------------------------
// Name: Automat::Accept
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void Automat::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
