// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Trace.h"
#include "clients_kernel/Time_ABC.h"
#include "protocol/Simulation.h"

//-----------------------------------------------------------------------------
// Name: Trace constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Trace::Trace( const kernel::Entity_ABC& agent, const kernel::Time_ABC& simulation, const sword::Trace& input )
    : Report( agent, Report::eTrace, QString( input.message().c_str() ), simulation.GetDateTime() )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Trace destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Trace::~Trace()
{
    // NOTHING
}
