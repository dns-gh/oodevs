// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Profile.h"
#include "AvailableProfile.h"
#include "Services.h"
#include "Simulation.h"
#include "reports/Tools.h"
#include "clients_kernel/Controller.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/AuthenticationSenders.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::Profile( Controllers& controllers, Publisher_ABC& publisher, const std::string& profile, bool isLoginSet )
    : controller_ ( controllers.controller_ )
    , publisher_  ( publisher )
    , login_      ( profile )
    , loggedIn_   ( false )
    , supervision_( false )
    , simulation_ ( true )
{
    controller_.Register( *this );
    if( !isLoginSet )
        password_ += static_cast< char >( 0x7f ); // set as invalid password (fix for mantis 0003178)
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login() const
{
    authentication::AuthenticationRequest message;
    message().mutable_version()->set_value( sword::ProtocolVersion().value() );
    message().set_login( login_.c_str() );
    message().set_password( password_.c_str() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login( const std::string& login, const std::string& password ) const
{
    login_ = login;
    password_ = password;
    Login();
    controller_.Update( *static_cast< Profile_ABC* >( const_cast< Profile* >( this ) ) );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadList
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
template< typename T >
void Profile::ReadList( const T& idList, T_Ids& ids )
{
    ids.clear();
    ids.reserve( idList.elem_size() );
    for( int i = 0; i < idList.elem_size(); ++i )
        ids.push_back( idList.elem( i ).id() );
    std::sort( ids.begin(), ids.end() );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Update( const sword::AuthenticationResponse& message )
{
    if( message.error_code() == sword::AuthenticationResponse::too_many_connections )
        throw std::exception( tools::translate( "Profile", "Too many connections" ).ascii() );
    else
    {
        loggedIn_ = ( message.error_code() == sword::AuthenticationResponse::success );
        if( message.has_profile() )
        {
            Update( message.profile() );
            controller_.Update( *static_cast< Profile_ABC* >( this ) );
        }
        controller_.Update( *this );
        if( !loggedIn_ && message.has_profiles() )
            for( int i = 0; i < message.profiles().elem_size(); ++i )
                controller_.Update( AvailableProfile( message.profiles().elem( i ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: LGY 2011-11-21
// -----------------------------------------------------------------------------
void Profile::Update( const sword::ConnectedProfileList& message )
{
    profiles_.clear();
    for( int i = 0; i < message.elem_size(); ++i )
    {
        const std::string& login = message.elem( i ).login();
        IT_Profiles it = profiles_.find( login );
        if( it == profiles_.end() )
            profiles_[ login ] = 0u;
        profiles_[ login ] ++;
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: SBO 2007-01-23
// -----------------------------------------------------------------------------
void Profile::Update( const Model& model, const sword::ProfileUpdate& message )
{
    if( message.login() == login_ )
    {
        Update( message.profile() );
        RightsResolver::Update( model );
        controller_.Update( *this );
        controller_.Update( *static_cast< Profile_ABC* >( this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: SBO 2007-01-23
// -----------------------------------------------------------------------------
void Profile::Update( const sword::Profile& profile )
{
    login_ = profile.login();
    if( profile.has_password()  )
        password_ = profile.password();
    else
        password_ = "";
    supervision_ = profile.supervisor();

    if( profile.has_read_only_parties()  )
        ReadList( profile.read_only_parties(), readTeams_ );
    if( profile.has_read_write_parties()  )
        ReadList( profile.read_write_parties(), writeTeams_ );

    if( profile.has_read_only_automates()  )
        ReadList( profile.read_only_automates(), readAutomats_ );
    if( profile.has_read_write_automates()  )
        ReadList( profile.read_write_automates(), writeAutomats_ );

    if( profile.has_read_only_crowds()  )
        ReadList( profile.read_only_crowds(), readPopulations_ );
    if( profile.has_read_write_crowds()  )
        ReadList( profile.read_write_crowds(), writePopulations_ );

    if( profile.has_read_only_formations()  )
        ReadList( profile.read_only_formations(), readFormations_ );
    if( profile.has_read_write_formations()  )
        ReadList( profile.read_write_formations(), writeFormations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::GetLogin
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
QString Profile::GetLogin() const
{
    return login_.c_str();
}

// -----------------------------------------------------------------------------
// Name: Profile::IsLoggedIn
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::IsLoggedIn() const
{
    return loggedIn_;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsSupervision
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
bool Profile::IsSupervision() const
{
    return supervision_;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsVisible
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::IsVisible( const Entity_ABC& entity ) const
{
    if( const kernel::TacticalHierarchies* tacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        if( supervision_ && entity.GetTypeName() == Object_ABC::typeName_ && tacticalHierarchies->GetTop().GetId() == 0 )
            return true;
    return RightsResolver::IsVisible( entity );
}


// -----------------------------------------------------------------------------
// Name: Profile::CanBeOrdered
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::CanBeOrdered( const Entity_ABC& entity ) const
{
    return simulation_ && RightsResolver::CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: Profile::CanDoMagic
// Created: AGE 2006-10-13
// -----------------------------------------------------------------------------
bool Profile::CanDoMagic( const Entity_ABC& entity ) const
{
    return simulation_ && supervision_ && RightsResolver::CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: Profile::Clean
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void Profile::Clean()
{
    login_ = "";
    password_ = static_cast< char >( 0x7f ); // mantis 0003178
    supervision_ = false;
    loggedIn_ = false;
    readEntities_.clear();
    readWriteEntities_.clear();
    readTeams_.clear();
    writeTeams_.clear();
    readAutomats_.clear();
    writeAutomats_.clear();
    readPopulations_.clear();
    writePopulations_.clear();
    readFormations_.clear();
    writeFormations_.clear();
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyUpdated
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void Profile::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() )
        Clean();
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyUpdated
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Profile::NotifyUpdated( const Services& services )
{
    simulation_ = services.HasService< simulation::Service >();
    if( services.HasService< authentication::Service >() )
        Login();
}

// -----------------------------------------------------------------------------
// Name: Profile::GetProfileCount
// Created: LGY 2011-11-21
// -----------------------------------------------------------------------------
unsigned int Profile::GetProfileCount( const std::string& login ) const
{
    CIT_Profiles it = profiles_.find( login );
    if( it != profiles_.end() )
        return it->second;
    return 0u;
}
