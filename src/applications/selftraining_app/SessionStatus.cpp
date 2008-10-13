// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "SessionStatus.h"

// -----------------------------------------------------------------------------
// Name: SessionStatus constructor
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
SessionStatus::SessionStatus( Status status, Session& session )
    : session_( session )
    , status_( status )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionStatus destructor
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
SessionStatus::~SessionStatus()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::GetSession
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
Session& SessionStatus::GetSession() const
{
    return session_;
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::GetStatus
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
SessionStatus::Status SessionStatus::GetStatus() const
{
    return status_;
}
