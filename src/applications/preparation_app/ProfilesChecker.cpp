// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ProfilesChecker.h"
#include "preparation/UserProfile.h"

// -----------------------------------------------------------------------------
// Name: ProfilesChecker constructor
// Created: LGY 2011-12-07
// -----------------------------------------------------------------------------
ProfilesChecker::ProfilesChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfilesChecker destructor
// Created: LGY 2011-12-07
// -----------------------------------------------------------------------------
ProfilesChecker::~ProfilesChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfilesChecker::Display
// Created: LGY 2011-12-07
// -----------------------------------------------------------------------------
void ProfilesChecker::Display( const T_ProfileEditors& editors )
{
    Clean();
    editors_ = editors;
}

// -----------------------------------------------------------------------------
// Name: ProfilesChecker::Clean
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
void ProfilesChecker::Clean()
{
    editors_.clear();
}

// -----------------------------------------------------------------------------
// Name: ProfilesChecker::Exists
// Created: LGY 2011-12-07
// -----------------------------------------------------------------------------
bool ProfilesChecker::Exists( const QString& oldLogin, const QString& newLogin ) const
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->first && it->first->GetLogin() != oldLogin && it->second && it->second->GetLogin() == newLogin )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesChecker::Exists
// Created: LGY 2011-12-07
// -----------------------------------------------------------------------------
bool ProfilesChecker::Exists( const QString& login ) const
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->first && it->first->GetLogin() == login && it->second && it->second->GetLogin() != login )
            return true;
    return false;
}