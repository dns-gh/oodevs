// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Trace.h"
#include "Simulation.h"

//-----------------------------------------------------------------------------
// Name: Trace constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Trace::Trace( const kernel::Entity_ABC& agent, const Simulation& simulation, const ASN1T_MsgTrace& input )
    : Report( agent, Report::eTrace, input.message, simulation.GetDateTime() )
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

