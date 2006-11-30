// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "CommandPostAttributes.h"
#include "Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes constructor
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
CommandPostAttributes::CommandPostAttributes( const Agent_ABC& holder )
    : holder_( holder )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: CommandPostAttributes destructor
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
CommandPostAttributes::~CommandPostAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::IsCommandPost
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
bool CommandPostAttributes::IsCommandPost() const
{
    return holder_.IsCommandPost();
}
