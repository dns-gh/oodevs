// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Trace.h"
#include "protocol/Simulation.h"

Trace::Trace( const kernel::Entity_ABC& agent, const QDateTime& time, const sword::Trace& input )
    : Report( agent, 0, Report::eTrace, QString( input.message().c_str() ), time )
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
