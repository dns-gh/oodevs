// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorProfiles.h"
#include "ProfilesModel.h"
#include "UserProfile.h"
#include "clients_kernel/Controllers.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfiles constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorProfiles::SuccessFactorProfiles( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfiles constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorProfiles::SuccessFactorProfiles( kernel::Controllers& controllers, xml::xistream& xis, const ProfilesModel& model )
    : controllers_( controllers )
{
    xis >> xml::start( "profiles" )
            >> xml::list( "profile", *this, &SuccessFactorProfiles::ReadProfile, model )
        >> xml::end();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfiles destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorProfiles::~SuccessFactorProfiles()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfiles::ReadProfiles
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorProfiles::ReadProfile( xml::xistream& xis, const ProfilesModel& model )
{
    const std::string name = xml::attribute< std::string >( xis, "name" );
    if( const UserProfile* profile = model.Find( name.c_str() ) )
        Register( profile->GetLogin(), *profile );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfiles::Serialize
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorProfiles::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "profiles" );
    BOOST_FOREACH( const T_Elements::value_type profile, elements_ )
        xos << xml::start( "profile" )
                << xml::attribute( "name", profile.second->GetLogin() )
            << xml::end();
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfiles::NotifyUpdated
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorProfiles::NotifyUpdated( const UserProfile& profile )
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
// Name: SuccessFactorProfiles::NotifyDeleted
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorProfiles::NotifyDeleted( const UserProfile& profile )
{
    Remove( profile.GetLogin() );
}
