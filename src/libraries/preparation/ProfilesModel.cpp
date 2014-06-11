// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ProfilesModel.h"
#include "Model.h"
#include "AgentsModel.h"
#include "UserProfile.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/ProfileFactory_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/UserProfile_ABC.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter_ABC.h"
#include "clients_kernel/ProfileEditor.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <tools/Iterator.h>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ProfilesModel constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfilesModel::ProfilesModel( kernel::Controllers& controllers, const kernel::ProfileFactory_ABC& factory )
    : controllers_( controllers )
    , factory_( factory )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfilesModel::~ProfilesModel()
{
    Purge();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Purge
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::Purge()
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        delete *it;
    userProfiles_.clear();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Load
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::Load( const tools::Loader_ABC& fileLoader, const tools::Path& file )
{
    fileLoader.LoadFile( file, boost::bind( &ProfilesModel::Read, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Read
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::Read( xml::xistream& xis )
{
    xis >> xml::start( "profiles" )
        >> xml::list( "profile", *this, &ProfilesModel::LoadProfile );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Serialize
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void ProfilesModel::Serialize( const tools::Path& file, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    tools::Xofstream xos( file );
    xos << xml::start( "profiles" );
    schemaWriter.WriteExerciseSchema( xos, "profiles" );
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        static_cast< UserProfile* >(*it)->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::LoadProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::LoadProfile( xml::xistream& xis )
{
    std::unique_ptr< kernel::UserProfile_ABC > profile( factory_.Create( xis ) );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC* ProfilesModel::CreateProfile( const QString& name )
{
    std::unique_ptr< kernel::UserProfile_ABC > profile( factory_.Create( name ) );
    userProfiles_.push_back( profile.release() );
    return userProfiles_.back();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfile
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
void ProfilesModel::CreateProfile( const QString& name, const kernel::Entity_ABC& entity, bool readonly )
{
    std::unique_ptr< kernel::UserProfile_ABC > profile( factory_.Create( name ) );
    if( readonly )
        profile->SetReadable( entity, true );
    else
        profile->SetWriteable( entity, true );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::DeleteProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::DeleteProfile( const kernel::UserProfile_ABC& profile )
{
    auto it = std::find( userProfiles_.begin(), userProfiles_.end(), &profile );
    if( it != userProfiles_.end() )
    {
        const kernel::UserProfile_ABC* element = *it;
        userProfiles_.erase( it );
        delete element;
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfileEditor
// Created: JSR 2014-06-04
// -----------------------------------------------------------------------------
kernel::ProfileEditor* ProfilesModel::CreateProfileEditor() const
{
    return new kernel::ProfileEditor( factory_.Create(), 0 );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfileEditor
// Created: JSR 2014-06-04
// -----------------------------------------------------------------------------
kernel::ProfileEditor* ProfilesModel::CreateProfileEditor( kernel::UserProfile_ABC& profile ) const
{
    return new kernel::ProfileEditor( factory_.Create( profile ), &profile );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CommitFromEditor
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
void ProfilesModel::CommitFromEditor( kernel::ProfileEditor& editor )
{
    kernel::UserProfile_ABC* originalProfile = editor.GetOriginalProfile();
    if( editor.IsDeleted() )
    {
        if( originalProfile )
            DeleteProfile( *originalProfile );
        return;
    }
    if( originalProfile == 0 )
        originalProfile = CreateProfile( editor.GetProfile().GetLogin() );
    if( originalProfile && editor.GetProfile() != *originalProfile )
    {
        *originalProfile = editor.GetProfile();
        controllers_.controller_.Update( *originalProfile );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Exists
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
bool ProfilesModel::Exists( const QString& login ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( login == (*it)->GetLogin() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::IsReadable
// Created: SBO 2007-11-06
// -----------------------------------------------------------------------------
bool ProfilesModel::IsReadable( const kernel::Entity_ABC& entity ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( (*it)->IsReadable( entity ) )
            return true;
    const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior ? IsReadable( *superior ) : false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::IsWriteable
// Created: SBO 2007-11-06
// -----------------------------------------------------------------------------
bool ProfilesModel::IsWriteable( const kernel::Entity_ABC& entity ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( (*it)->IsWriteable( entity ) )
            return true;
    const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior ? IsWriteable( *superior ) : false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::IsReadable
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
bool ProfilesModel::IsReadable( const kernel::Entity_ABC& entity, const std::string& profile ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( (*it)->GetLogin().toStdString() == profile && (*it)->IsReadable( entity ) )
                return true;
    const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior ? IsReadable( *superior, profile ) : false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::IsWriteable
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
bool ProfilesModel::IsWriteable( const kernel::Entity_ABC& entity, const std::string& profile ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( (*it)->GetLogin().toStdString() == profile && (*it)->IsWriteable( entity ) )
            return true;
    const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior ? IsWriteable( *superior, profile ) : false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Find
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
const kernel::UserProfile_ABC* ProfilesModel::Find( const QString& name ) const
{
    BOOST_FOREACH( const T_UserProfiles::value_type profile, userProfiles_ )
        if( profile->GetLogin() == name )
            return profile;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Find
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC* ProfilesModel::Find( const std::string& name ) const
{
    BOOST_FOREACH( const T_UserProfiles::value_type profile, userProfiles_ )
        if( profile->GetLogin() == name.c_str() )
            return profile;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Team_ABC& team)
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        static_cast< UserProfile* >(*it)->NotifyTeamDeleted( team.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Formation_ABC& formation)
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        static_cast< UserProfile* >(*it)->NotifyFormationDeleted( formation.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Automat_ABC& automat)
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        static_cast< UserProfile* >(*it)->NotifyAutomatDeleted( automat.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Population_ABC& population)
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        static_cast< UserProfile* >(*it)->NotifyPopulationDeleted( population.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Ghost_ABC& ghost )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        static_cast< UserProfile* >(*it)->NotifyGhostDeleted( ghost.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Apply
// Created: JSR 2014-06-03
// -----------------------------------------------------------------------------
void ProfilesModel::Apply( boost::function< void( kernel::UserProfile_ABC& ) > functor )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        functor( **it );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::profilesCount
// Created: MMC 2012-03-06
// -----------------------------------------------------------------------------
unsigned int ProfilesModel::GetProfilesCount() const
{
    return static_cast< unsigned int >( userProfiles_.size() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::GetProfilesWhoCanRead
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
std::vector< std::string > ProfilesModel::GetProfilesWhoCanRead( const kernel::Entity_ABC& entity ) const
{
    std::vector< std::string > result;
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( ( *it )->IsReadable( entity ) )
            result.push_back( ( *it )->GetLogin().toStdString() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::GetProfilesWhoCanWrite
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
std::vector< std::string > ProfilesModel::GetProfilesWhoCanWrite( const kernel::Entity_ABC& entity ) const
{
    std::vector< std::string > result;
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( ( *it )->IsWriteable( entity ) )
            result.push_back( ( *it )->GetLogin().toStdString() );
    return result;
}
