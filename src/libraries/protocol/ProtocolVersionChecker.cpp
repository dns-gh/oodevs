// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ProtocolVersionChecker.h"
#include "Protocol.h"

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker constructor
// Created: RPD 2010-05-26
// -----------------------------------------------------------------------------
ProtocolVersionChecker::ProtocolVersionChecker( const Version::ProtocolVersion& version )
    : protocolVersion_ ( version.value() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker destructor
// Created: RPD 2010-05-26
// -----------------------------------------------------------------------------
ProtocolVersionChecker::~ProtocolVersionChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker::CheckCompatibility
// Created: RPD 2010-05-26
// -----------------------------------------------------------------------------
bool ProtocolVersionChecker::CheckCompatibility()
{
    //return GetCurrentProtocolVersion() == protocolVersion_;
    return true;
}

std::string ProtocolVersionChecker::GetCurrentProtocolVersion()
{
    Version::ProtocolVersion currentVersion;
    return currentVersion.value();
}