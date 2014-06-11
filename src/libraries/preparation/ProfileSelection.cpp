// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ProfileSelection.h"
#include "ProfilesModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/UserProfile_ABC.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ProfileSelection constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
ProfileSelection::ProfileSelection( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileSelection constructor
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
ProfileSelection::ProfileSelection( const ProfileSelection& selection )
    : controllers_( selection.controllers_ )
{
    auto it( selection.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const kernel::UserProfile_ABC& profile = it.NextElement();
        Register( profile.GetLogin(), profile );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileSelection constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
ProfileSelection::ProfileSelection( kernel::Controllers& controllers, xml::xistream& xis, const ProfilesModel& model )
    : controllers_( controllers )
{
    xis >> xml::optional >> xml::start( "profiles" )
            >> xml::list( "profile", *this, &ProfileSelection::ReadProfile, model )
        >> xml::end;
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileSelection destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
ProfileSelection::~ProfileSelection()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileSelection::ReadProfiles
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void ProfileSelection::ReadProfile( xml::xistream& xis, const ProfilesModel& model )
{
    const std::string name = xis.attribute< std::string >( "name" );
    if( const kernel::UserProfile_ABC* profile = model.Find( QString( name.c_str() ) ) )
        Register( profile->GetLogin(), *profile );
}

// -----------------------------------------------------------------------------
// Name: ProfileSelection::Serialize
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void ProfileSelection::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "profiles" );
    BOOST_FOREACH( const T_Elements::value_type profile, elements_ )
        xos << xml::start( "profile" )
                << xml::attribute( "name", profile.second->GetLogin() )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ProfileSelection::NotifyUpdated
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void ProfileSelection::NotifyUpdated( const kernel::UserProfile_ABC& profile )
{
    BOOST_FOREACH( T_Elements::value_type element, elements_ )
        if( element.second == &profile && element.first != profile.GetLogin() )
        {
            Remove( element.first );
            Register( profile.GetLogin(), profile );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: ProfileSelection::NotifyDeleted
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void ProfileSelection::NotifyDeleted( const kernel::UserProfile_ABC& profile )
{
    Remove( profile.GetLogin() );
}
