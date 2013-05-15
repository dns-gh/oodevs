// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionError.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionError constructor
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
ActionError::ActionError( const std::string& error )
    : error_( error )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionError destructor
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
ActionError::~ActionError()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionError::GetError
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
const std::string& ActionError::GetError() const
{
    return error_;
}
