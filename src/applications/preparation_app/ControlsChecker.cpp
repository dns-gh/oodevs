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
#include "preparation/TacticalHierarchies.h"
#include "preparation/Model.h"
#include "preparation/UserProfile.h"
#include "preparation/ProfilesModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: ControlsChecker constructor
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
ControlsChecker::ControlsChecker( kernel::Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_      ( model )
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
    Clean();
    editors_ = editors;
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::Clean
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void ControlsChecker::Clean()
{
    editors_.clear();
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::GetProfileControl
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
QString ControlsChecker::GetProfileControl( const UserProfile& profile, const kernel::Entity_ABC& entity ) const
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->second )
        {
            QString login = it->second->GetLogin();
            const kernel::Entity_ABC* parent = 0;
            if( const ProfileHierarchies_ABC* pProfileHierarchy = entity.Retrieve< ProfileHierarchies_ABC >() )
                parent = pProfileHierarchy->GetSuperior();
            else if( const TacticalHierarchies* pTacticalHierarchy = entity.Retrieve< TacticalHierarchies >() )
                parent = &pTacticalHierarchy->GetTop();
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

// -----------------------------------------------------------------------------
// Name: ControlsChecker::Exists
// Created: LGY 2011-09-26
// -----------------------------------------------------------------------------
bool ControlsChecker::Exists( const QString& oldLogin, const QString& newLogin ) const
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->first && it->first->GetLogin() != oldLogin && it->second && it->second->GetLogin() == newLogin )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::Exists
// Created: LGY 2011-09-27
// -----------------------------------------------------------------------------
bool ControlsChecker::Exists( const QString& login ) const
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->first && it->first->GetLogin() == login && it->second && it->second->GetLogin() != login )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::IsControlled
// Created: LGY 2011-12-13
// -----------------------------------------------------------------------------
bool ControlsChecker::IsControlled( const kernel::Entity_ABC& entity ) const
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->second && IsIsWriteable( entity, *it->second ) )
            return true;
    return model_.profiles_.IsWriteable( entity );
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::IsIsWriteable
// Created: LGY 2011-12-13
// -----------------------------------------------------------------------------
bool ControlsChecker::IsIsWriteable( const kernel::Entity_ABC& entity, const UserProfile& profile ) const
{
    if( profile.IsWriteable( entity ) )
        return true;
    if( const kernel::Entity_ABC* parent = entity.Get< ProfileHierarchies_ABC >().GetSuperior() )
        return IsIsWriteable( *parent, profile );
    return false;
}
