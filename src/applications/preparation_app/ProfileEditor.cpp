// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ProfileEditor.h"
#include "preparation/UserProfile.h"
#include "preparation/ProfilesModel.h"
#include "clients_kernel/Controller.h"
#include "preparation/Tools.h"

// -----------------------------------------------------------------------------
// Name: ProfileEditor constructor
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
ProfileEditor::ProfileEditor( const UserProfile& profile, kernel::Controller& controller, const ProfilesModel& profiles )
    : UserProfile( profile )
    , controller_( controller )
    , profiles_  ( profiles )
    , profile_   ( profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileEditor destructor
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
ProfileEditor::~ProfileEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileEditor::SetLogin
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void ProfileEditor::SetLogin( const QString& value )
{
    if( profile_.GetLogin() != value && profiles_.Exists( value ) )
        throw std::exception( tools::translate( "ProfileEditor", "Duplicate login: '%1'." ).arg( value ).ascii() );
    UserProfile::SetLogin( value );
    controller_.Update( profile_ );
}
