// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AutomatChecker.h"
#include "tools/Resolver_ABC.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AutomatChecker constructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
AutomatChecker::AutomatChecker( const tools::Resolver_ABC< dispatcher::Agent_ABC, unsigned long >& resolver )
    : resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatChecker destructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
AutomatChecker::~AutomatChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatChecker::IsAutomatDisengaged
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
bool AutomatChecker::IsAutomatDisengaged( unsigned int unitIdentifier ) const
{
    return !resolver_.Get( unitIdentifier ).GetSuperior().IsEngaged();
}
