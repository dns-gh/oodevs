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
#include "UserProfile.h"
#include "ProfileFactory_ABC.h"
#include "Model.h"
#include "ProfileHierarchies_ABC.h"
#include "clients_kernel/Tools.h"
#include "AgentsModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "tools/Iterator.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Controller.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ProfilesModel constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfilesModel::ProfilesModel( kernel::Controllers& controllers, const ProfileFactory_ABC& factory )
    : controllers_( controllers )
    , factory_   ( factory )
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
void ProfilesModel::Load( const tools::Loader_ABC& fileLoader, const std::string& file )
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
void ProfilesModel::Serialize( const std::string& file, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xml::xofstream xos( file );
    xos << xml::start( "profiles" );
    schemaWriter.WriteExerciseSchema( xos, "profiles" );
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        (*it)->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::LoadProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::LoadProfile( xml::xistream& xis )
{
    std::auto_ptr< UserProfile > profile( factory_.Create( xis ) );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::CreateProfile( const QString& name )
{
    std::auto_ptr< UserProfile > profile( factory_.Create( name ) );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfile
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void ProfilesModel::CreateProfile( const QString& name, const std::string& userRole )
{
    std::auto_ptr< UserProfile > profile( factory_.Create( name, userRole ) );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfile
// Created: JSR 2011-12-08
// -----------------------------------------------------------------------------
void ProfilesModel::CreateProfile( const QString& name, const std::string& userRole, const kernel::Entity_ABC& entity, bool readonly )
{
    std::auto_ptr< UserProfile > profile( factory_.Create( name, userRole ) );
    if( readonly )
        profile->SetReadable( entity, true );
    else
        profile->SetWriteable( entity, true );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CreateProfile
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
void ProfilesModel::CreateProfile( const QString& name, const std::string& userRole, const std::vector< const kernel::Entity_ABC* >& entities, bool readonly, bool timeControl /* = false */ )
{
    std::auto_ptr< UserProfile > profile( factory_.Create( name, userRole ) );
    profile->SetTimeControl( timeControl );
    if( readonly )
        for( std::vector< const kernel::Entity_ABC* >::const_iterator it = entities.begin(); it != entities.end(); ++it )
            profile->SetReadable( **it, true );
    else
        for( std::vector< const kernel::Entity_ABC* >::const_iterator it = entities.begin(); it != entities.end(); ++it )
            profile->SetWriteable( **it, true );
    userProfiles_.push_back( profile.release() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::DeleteProfile
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfilesModel::DeleteProfile( const UserProfile& profile )
{
    T_UserProfiles::iterator it = std::find( userProfiles_.begin(), userProfiles_.end(), &profile );
    if( it != userProfiles_.end() )
    {
        const UserProfile* element = *it;
        userProfiles_.erase( it );
        delete element;
    }
}

namespace
{
    bool Find( const QString& name, const UserProfile* value )
    {
        return name == value->GetLogin();
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::DeleteProfile
// Created: LGY 2012-02-07
// -----------------------------------------------------------------------------
void ProfilesModel::DeleteProfile( const QString& name )
{
    T_UserProfiles::iterator it = std::find_if( userProfiles_.begin(), userProfiles_.end(), boost::bind( &::Find, name, _1 ) );
    if( it != userProfiles_.end() )
    {
        const UserProfile* element = *it;
        userProfiles_.erase( it );
        delete element;
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::CheckUnicityAndRename
// Created: JSR 2011-12-12
// -----------------------------------------------------------------------------
bool ProfilesModel::CheckUnicityAndRename()
{
    bool ret = false;
    std::map< const QString, unsigned int > logins;
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
    {
        const QString login = ( *it )->GetLogin();
        ++logins[ login ];
        if( logins[ login ] > 1 )
        {
            ret = true;
            ( *it )->SetLogin( login + '(' + QString::number( logins[ login ] - 1 ) + ')' );
        }
    }
    return ret;
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
// Name: ProfilesModel::IsWriteableByLowLevel
// Created: SBO 2007-11-06
// -----------------------------------------------------------------------------
bool ProfilesModel::IsWriteableByLowLevel( const kernel::Entity_ABC& entity ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( (*it)->IsLowLevel() && (*it)->IsWriteable( entity ) )
            return true;
    const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior ? IsWriteableByLowLevel( *superior ) : false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::IsReadable
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
bool ProfilesModel::IsReadable( const kernel::Entity_ABC& entity, const std::string& profile ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( (*it)->GetLogin().toAscii().constData() == profile && (*it)->IsReadable( entity ) )
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
        if( (*it)->GetLogin().toAscii().constData() == profile && (*it)->IsWriteable( entity ) )
            return true;
    const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior ? IsWriteable( *superior, profile ) : false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::IsControlledByLowLevel
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
bool ProfilesModel::IsControlledByLowLevel( const std::set< std::string >& editors, const kernel::Entity_ABC& entity )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( editors.find( (*it)->GetLogin().toAscii().constData() ) == editors.end() && (*it)->IsLowLevel() && (*it)->IsWriteable( entity ) )
            return true;
    const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
    return superior ? IsControlledByLowLevel( editors, *superior ) : false;
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Find
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
const UserProfile* ProfilesModel::Find( const QString& name ) const
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
UserProfile* ProfilesModel::Find( const std::string& name ) const
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
void ProfilesModel::NotifyDeleted( const kernel::Team_ABC& team )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        (*it)->NotifyTeamDeleted( team.GetId() );
    RemoveEmptyProfile();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Formation_ABC& formation )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        (*it)->NotifyFormationDeleted( formation.GetId() );
    RemoveEmptyProfile();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Automat_ABC& automat )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        (*it)->NotifyAutomatDeleted( automat.GetId() );
    RemoveEmptyProfile();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Population_ABC& population )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        (*it)->NotifyPopulationDeleted( population.GetId() );
    RemoveEmptyProfile();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::NotifyDeleted
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
void ProfilesModel::NotifyDeleted( const kernel::Ghost_ABC& ghost )
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        (*it)->NotifyGhostDeleted( ghost.GetId() );
    RemoveEmptyProfile();
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Visit
// Created: LGY 2011-09-16
// -----------------------------------------------------------------------------
void ProfilesModel::Visit( T_Units& units ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( (*it)->IsLowLevel() )
        {
            std::vector< unsigned long > ids;
            (*it)->Visit( ids );
            BOOST_FOREACH( unsigned long id, ids )
                units[ id ].insert( (*it)->GetLogin().toAscii().constData() );
        }
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::Visit
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
void ProfilesModel::Visit( T_Profiles& profiles ) const
{
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        profiles.insert( (*it)->GetLogin() );
}

// -----------------------------------------------------------------------------
// Name: ProfilesModel::RemoveEmptyProfile
// Created: LGY 2012-02-07
// -----------------------------------------------------------------------------
void ProfilesModel::RemoveEmptyProfile()
{
    std::vector< QString > emptyProfiles;
    for( auto it = userProfiles_.begin(); it != userProfiles_.end(); ++it )
        if( (*it)->GetProfilesCount() == 0 )
            emptyProfiles.push_back( (*it)->GetLogin() );
    BOOST_FOREACH( const QString& profile, emptyProfiles )
        DeleteProfile( profile );
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
            result.push_back( ( *it )->GetLogin().toAscii().constData() );
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
            result.push_back( ( *it )->GetLogin().toAscii().constData() );
    return result;
}
