// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "Trace.h"
#include "clients_kernel/Time_ABC.h"

//-----------------------------------------------------------------------------
// Name: Trace constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Trace::Trace( const kernel::Entity_ABC& agent, const kernel::Time_ABC& simulation, const MsgsSimToClient::MsgTrace& input )
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

