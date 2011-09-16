// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ControlsChecker.h"
#include "preparation/ProfileHierarchies_ABC.h"
#include "preparation/UserProfile.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ControlsChecker constructor
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
ControlsChecker::ControlsChecker( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker destructor
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
ControlsChecker::~ControlsChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::Display
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
void ControlsChecker::Display( const T_ProfileEditors& editors )
{
    editors_.clear();
    editors_ = editors;
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::CheckControl
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
std::string ControlsChecker::CheckControl( const UserProfile& profile, const kernel::Entity_ABC& entity ) const
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->second )
        {
            std::string login = it->second->GetLogin();
            const kernel::Entity_ABC* parent = entity.Get< ProfileHierarchies_ABC >().GetSuperior();
            if( profile.GetLogin().ascii() != login )
                if( it->second->IsWriteable( entity ) || ( parent && it->second->IsWriteable( *parent ) ) )
                    return login;
        }
    return "";
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::Update
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
void ControlsChecker::Update( const UserProfile& profile, const kernel::Entity_ABC& entity )
{
    for( IT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->second && profile.GetLogin().ascii() != it->second->GetLogin() )
        {
            UserProfile* userProfile = it->second;
            const kernel::Entity_ABC* parent = entity.Get< ProfileHierarchies_ABC >().GetSuperior();
            if( parent && userProfile->IsWriteable( *parent ) )
            {
                tools::Iterator< const kernel::Entity_ABC& > it = parent->Get< ProfileHierarchies_ABC >().CreateSubordinateIterator();
                while( it.HasMoreElements() )
                    UpdateProfile( *userProfile, it.NextElement(), true );
                UpdateProfile( *userProfile, *parent, false );
            }
            UpdateProfile( *userProfile, entity, false );
            controllers_.controller_.Update( userProfile );
        }
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::UpdateProfile
// Created: LGY 2011-09-16
// -----------------------------------------------------------------------------
void ControlsChecker::UpdateProfile( UserProfile& profile, const kernel::Entity_ABC& entity, bool control )
{
    profile.SetWriteable( entity, control );
    profile.SetReadable( entity, control );
}
