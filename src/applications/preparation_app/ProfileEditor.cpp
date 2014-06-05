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
#include "clients_kernel/UserProfile_ABC.h"

// -----------------------------------------------------------------------------
// Name: ProfileEditor constructor
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
ProfileEditor::ProfileEditor( kernel::UserProfile_ABC* profile, kernel::UserProfile_ABC* originalProfile )
    : profile_( profile )
    , originalProfile_( originalProfile )
    , deleted_( false )
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
// Name: ProfileEditor::GetProfile
// Created: JSR 2014-06-04
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC& ProfileEditor::GetProfile()
{
    return *profile_;
}

// -----------------------------------------------------------------------------
// Name: ProfileEditor::GetOriginalProfile
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC* ProfileEditor::GetOriginalProfile() const
{
    return originalProfile_;
}

// -----------------------------------------------------------------------------
// Name: ProfileEditor::Delete
// Created: JSR 2014-06-02
// -----------------------------------------------------------------------------
void ProfileEditor::Delete()
{
    deleted_ = true;
}

// -----------------------------------------------------------------------------
// Name: ProfileEditor::IsDeleted
// Created: JSR 2014-06-02
// -----------------------------------------------------------------------------
bool ProfileEditor::IsDeleted() const
{
    return deleted_;
}

// -----------------------------------------------------------------------------
// Name: ProfileEditor::NotifyOriginalProfileDeleted
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
void ProfileEditor::NotifyOriginalProfileDeleted()
{
    originalProfile_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ProfileEditor::NotifyOriginalProfileCreated
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
void ProfileEditor::NotifyOriginalProfileCreated( kernel::UserProfile_ABC* profile )
{
    originalProfile_ = profile;
}
