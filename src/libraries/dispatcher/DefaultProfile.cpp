// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "DefaultProfile.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DefaultProfile constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
DefaultProfile::DefaultProfile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
DefaultProfile::~DefaultProfile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
bool DefaultProfile::CheckRights( const sword::ClientToSim& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
bool DefaultProfile::CheckRights( const sword::ClientToAuthentication& wrapper ) const
{
    return wrapper.message().has_authentication_request();
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
bool DefaultProfile::CheckRights( const sword::ClientToReplay& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
bool DefaultProfile::CheckRights( const sword::ChatTarget& , const sword::ChatTarget& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: SLI 2014-03-21
// -----------------------------------------------------------------------------
 bool DefaultProfile::CheckRights( const sword::ProfileUpdateRequest&, bool ) const
 {
     return false;
 }

// -----------------------------------------------------------------------------
// Name: DefaultProfile::GetName
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
std::string DefaultProfile::GetName() const
{
    return "";
}
