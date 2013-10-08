// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Supervisor.h"

using namespace dispatcher;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Supervisor constructor
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
Supervisor::Supervisor( const Model& model, ClientPublisher_ABC& clients )
    : Profile( model, clients, "MAsuperivisorSA", false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Supervisor destructor
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
Supervisor::~Supervisor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Supervisor::CheckRights
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
bool Supervisor::CheckRights( const sword::ClientToSim& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Supervisor::CheckRights
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
bool Supervisor::CheckRights( const sword::ClientToAuthentication& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Supervisor::CheckRights
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
bool Supervisor::CheckRights( const sword::ClientToReplay& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Supervisor::CheckRights
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
bool Supervisor::CheckRights( const sword::ChatTarget&, const sword::ChatTarget& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Supervisor::Send
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
void Supervisor::Send( sword::ConnectedProfileList& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Supervisor::Send
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
void Supervisor::Send( sword::AuthenticationResponse& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Supervisor::Send
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
void Supervisor::Send( sword::ProfileDescription& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Supervisor::SendCreation
// Created: LGY 2013-10-07
// -----------------------------------------------------------------------------
void Supervisor::SendCreation( ClientPublisher_ABC& ) const
{
    // NOTHING
}
