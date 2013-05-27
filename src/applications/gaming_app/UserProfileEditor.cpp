// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UserProfileEditor.h"
#include "gaming/UserProfile.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: UserProfileEditor constructor
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
UserProfileEditor::UserProfileEditor( const UserProfile& profile, kernel::Controller& controller )
    : UserProfile( profile )
    , controller_( controller )
    , profile_   ( profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileEditor destructor
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
UserProfileEditor::~UserProfileEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileEditor::SetLogin
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileEditor::SetLogin( const QString& value )
{
    UserProfile::SetLogin( value );
    controller_.Update( profile_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileEditor::SetSupervisor
// Created: NPT 2013-05-24
// -----------------------------------------------------------------------------
void UserProfileEditor::SetSupervisor( bool supervisor )
{
    UserProfile::SetSupervisor( supervisor );
    controller_.Update( profile_ );
}
