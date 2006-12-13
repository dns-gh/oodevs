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

namespace
{
    QString ReadMessage( const TraceMessage& input )
    {
        std::string message;
        input >> message;
        return message.c_str();
    }
}

//-----------------------------------------------------------------------------
// Name: Trace constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Trace::Trace( const kernel::Entity_ABC& agent, const Simulation& simulation, const TraceMessage& input )
    : Report( agent, simulation, Report::eTrace, ReadMessage( input ) )
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

