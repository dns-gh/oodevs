// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AvailableProfile.h"
#include "protocol/AuthenticationSenders.h"

// -----------------------------------------------------------------------------
// Name: AvailableProfile constructor
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
AvailableProfile::AvailableProfile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AvailableProfile constructor
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
AvailableProfile::AvailableProfile( const sword::ProfileDescription& profile )
    : login_( profile.login().c_str() )
    , password_( profile.password() ? true : false )
    , supervisor_( profile.supervisor() ? true : false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AvailableProfile destructor
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
AvailableProfile::~AvailableProfile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AvailableProfile::operator=
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
AvailableProfile& AvailableProfile::operator=( const AvailableProfile& rhs )
{
    login_ = rhs.login_;
    password_ = rhs.password_;
    supervisor_ = rhs.supervisor_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: AvailableProfile::GetLogin
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
const QString& AvailableProfile::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: AvailableProfile::IsPasswordProtected
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
bool AvailableProfile::IsPasswordProtected() const
{
    return password_;
}

// -----------------------------------------------------------------------------
// Name: AvailableProfile::IsSupervisor
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
bool AvailableProfile::IsSupervisor() const
{
    return supervisor_;
}
