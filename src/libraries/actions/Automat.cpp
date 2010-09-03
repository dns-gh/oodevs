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
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
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
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
Automat::Automat( const kernel::OrderParameter& parameter, const kernel::Automat_ABC& automat, kernel::Controller& controller )
    : Entity< Automat_ABC >( parameter, &automat, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Automat::Automat( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Automat_ABC >( parameter, &resolver.GetAutomat( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Automat::Automat( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Automat_ABC >( parameter, &resolver.GetAutomat( xis.attribute< unsigned long >( "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Automat::Automat( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Automat_ABC >( OrderParameter( xis.attribute< std::string >("name" ), xis.attribute< std::string >( "type" ), false ), &resolver.GetAutomat( xis.attribute< unsigned long >( "value" ) ), controller )
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
    message.set_null_value( !IsSet() );
    if( IsSet() )
        Entity< Automat_ABC >::CommitTo< Common::AutomatId >( *message.mutable_value()->mutable_automat() );
}

// -----------------------------------------------------------------------------
// Name: Automat::Accept
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void Automat::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
