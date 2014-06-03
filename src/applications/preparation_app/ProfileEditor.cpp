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
#include "preparation/ProfilesModel.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: ProfileEditor constructor
// Created: JSR 2014-05-27
// -----------------------------------------------------------------------------
ProfileEditor::ProfileEditor( const QString& login, kernel::Controller& controller, const Model& model )
    : UserProfile( login, controller, model )
    , originalProfile_( 0 )
    , deleted_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileEditor constructor
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
ProfileEditor::ProfileEditor( UserProfile& profile )
    : UserProfile( profile )
    , originalProfile_( &profile )
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
// Name: ProfileEditor::Commit
// Created: JSR 2014-05-27
// -----------------------------------------------------------------------------
void ProfileEditor::Commit( ProfilesModel& model, kernel::Controller& controller )
{
    if( deleted_ )
    {
        if( originalProfile_ )
            model.DeleteProfile( *originalProfile_ );
        originalProfile_ = 0;
        return;
    }
    if( originalProfile_ == 0 )
        originalProfile_ = model.CreateProfile( GetLogin() );
    if( *this != *originalProfile_ )
    {
        *originalProfile_ = *this;
        controller.Update( *originalProfile_ );
    }
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
