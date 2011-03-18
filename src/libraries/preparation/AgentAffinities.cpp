// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentAffinities.h"
#include "clients_kernel/Controllers.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: AgentAffinities constructor
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinities::AgentAffinities( kernel::Controllers& controllers )
    : Affinities()
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities constructor
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
AgentAffinities::AgentAffinities( xml::xistream& xis, kernel::Controllers& controllers )
    : Affinities( xis )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities destructor
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinities::~AgentAffinities()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::Clear
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
void AgentAffinities::Clear()
{
    affinities_.clear();
}
