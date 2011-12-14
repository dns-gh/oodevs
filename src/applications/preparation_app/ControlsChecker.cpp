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
// Name: ControlsChecker::Find
// Created: LGY 2011-12-13
// -----------------------------------------------------------------------------
std::set< std::string > ControlsChecker::Find( const kernel::Entity_ABC& entity ) const
{
    std::set< std::string > results;
    std::set< std::string > editors;
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->first )
        {
            editors.insert( it->first->GetLogin().ascii() );
            if( it->second && IsWriteable( entity, *it->second ) )
                results.insert( it->second->GetLogin().ascii() );
        }
    ProfilesModel::T_Units units;
    model_.profiles_.Visit( units );
    Find( entity, units, results, editors );
    return results;
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::Find
// Created: LGY 2011-12-13
// -----------------------------------------------------------------------------
void ControlsChecker::Find( const kernel::Entity_ABC& entity, const ProfilesModel::T_Units& units,
                            std::set< std::string >& results, const std::set< std::string >& editors ) const
{
    ProfilesModel::CIT_Units it = units.find( entity.GetId() );
    if( it != units.end() )
        BOOST_FOREACH( const std::string& profile, it->second )
            if( editors.find( profile ) == editors.end() )
                results.insert( profile );
    if( const kernel::Entity_ABC* parent = entity.Get< ProfileHierarchies_ABC >().GetSuperior() )
        Find( *parent, units, results, editors );
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
        if( it->second && IsWriteable( entity, *it->second ) )
            return true;
    return model_.profiles_.IsWriteable( entity );
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::IsControlled
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
bool ControlsChecker::IsControlled( const std::string& exclude, const kernel::Entity_ABC& entity ) const
{
    std::set< std::string > editors;
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        if( it->first )
        {
            editors.insert( it->first->GetLogin().ascii() );
            if( it->second && it->second->GetLogin().ascii() != exclude && IsWriteable( entity, *it->second ) )
                return true;
        }
    editors.insert( exclude );
    return model_.profiles_.IsControlled( editors, entity );
}

// -----------------------------------------------------------------------------
// Name: ControlsChecker::IsWriteable
// Created: LGY 2011-12-13
// -----------------------------------------------------------------------------
bool ControlsChecker::IsWriteable( const kernel::Entity_ABC& entity, const UserProfile& profile ) const
{
    if( profile.IsWriteable( entity ) )
        return true;
    if( const kernel::Entity_ABC* parent = entity.Get< ProfileHierarchies_ABC >().GetSuperior() )
        return IsWriteable( *parent, profile );
    return false;
}
