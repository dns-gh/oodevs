// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "Army.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Army::Army
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Army::Army( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< Team_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Army::Army
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Army::Army( const kernel::OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< kernel::Team_ABC >& resolver, kernel::Controller& controller )
    : Entity< Team_ABC >( parameter, &resolver.Get( attribute< unsigned long >( xis, "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Army::Army
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Army::Army( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::Team_ABC >& resolver, kernel::Controller& controller )
    : Entity< Team_ABC >( parameter, &resolver.Get( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Army::Army
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Army::Army( xml::xistream& xis, const tools::Resolver_ABC< kernel::Team_ABC >& resolver, kernel::Controller& controller )
    : Entity< Team_ABC >( OrderParameter( attribute< std::string >( xis, "name" ), "army", false ), &resolver.Get( attribute< unsigned long >( xis, "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Army constructor
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Army::Army( const kernel::OrderParameter& parameter, const kernel::Team_ABC& team, kernel::Controller& controller )
    : Entity< Team_ABC >( parameter, &team, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Army::~Army
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Army::~Army()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Army::CommitTo
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void Army::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() && GetValue())
        message.mutable_value()->set_army( GetValue()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: Army::Accept
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void Army::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
