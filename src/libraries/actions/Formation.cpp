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
#include "protocol/Protocol.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
Formation::Formation( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< Formation_ABC >( parameter, controller )
{
    // NOTHING
}

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
Formation::Formation( const OrderParameter& parameter, const int& message, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Formation_ABC >( parameter, resolver.FindFormation( message ), controller )
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

// -----------------------------------------------------------------------------
// Name: Formation::CommitTo
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void Formation::CommitTo( sword::MissionParameter& message ) const
{
    auto* value = message.add_value()->mutable_formation();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        Entity< Formation_ABC >::CommitTo( *value );
}
// -----------------------------------------------------------------------------
// Name: Formation::CommitTo
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void Formation::CommitTo( sword::MissionParameter_Value& message ) const
{
    message.mutable_formation();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        Entity< Formation_ABC >::CommitTo( *message.mutable_formation() );
}

// -----------------------------------------------------------------------------
// Name: Formation::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Formation::SerializeType() const
{
    return "formation";
}
