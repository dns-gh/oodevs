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
#include "clients_kernel/Controller.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/Dispatcher.h"
#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::Profile( Controllers& controllers, Publisher_ABC& publisher, const std::string& profile, bool isLoginSet )
    : controller_ ( controllers.controller_ )
    , publisher_  ( publisher )
    , login_      ( profile.c_str() )
    , loggedIn_   ( false )
    , supervision_( false )
    , timeControl_( false )
    , simulation_ ( true )
    , clientId_   ( 0 )
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
    message().set_login( login_.toStdString() );
    message().set_password( password_.c_str() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::SetCredentials
// Created: BAX 2013-10-25
// -----------------------------------------------------------------------------
void Profile::SetCredentials( const std::string& login, const std::string& password )
{
    login_ = login.c_str();
    password_ = password;
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login( const std::string& login, const std::string& password ) const
{
    login_ = login.c_str();
    password_ = password;
    Login();
    controller_.Update( *static_cast< Profile_ABC* >( const_cast< Profile* >( this ) ) );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: ABR 2013-06-05
// -----------------------------------------------------------------------------
void Profile::Update()
{
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Update( const sword::AuthenticationToClient& wrapper )
{
    const sword::AuthenticationResponse& message = wrapper.message().authentication_response();
    if( message.error_code() == sword::AuthenticationResponse::too_many_connections )
        throw MASA_EXCEPTION( tools::translate( "Profile", "Too many connections" ).toStdString().c_str() );
    else
    {
        loggedIn_ = ( message.error_code() == sword::AuthenticationResponse::success );
        // Register client id
        if( loggedIn_ && wrapper.has_client_id() )
            clientId_ = wrapper.client_id();
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
    if( message.elem_size() == 0 )
        return;

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
    if( message.login() == login_.toStdString() )
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
    login_ = profile.login().c_str();
    if( profile.has_password()  )
        password_ = profile.password();
    else
        password_ = "";
    supervision_ = profile.supervisor();
    if( profile.has_time_control() )
        timeControl_ = profile.time_control();

    RightsResolver::Update( profile );
}

// -----------------------------------------------------------------------------
// Name: Profile::GetLogin
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
const QString& Profile::GetLogin() const
{
    return login_;
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
// Name: Profile::DisplayMessage
// Created: LGY 2013-05-06
// -----------------------------------------------------------------------------
bool Profile::DisplayMessage( unsigned int messageClientId ) const
{
    return loggedIn_ && clientId_ == messageClientId;
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
// Name: Profile::HasTimeControl
// Created: BAX 2014-01-30
// -----------------------------------------------------------------------------
bool Profile::HasTimeControl() const
{
    return timeControl_;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsVisible
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::IsVisible( const Entity_ABC& entity ) const
{
    if( const kernel::TacticalHierarchies* tacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        if( supervision_ && ( ( entity.GetTypeName() == Object_ABC::typeName_ && tacticalHierarchies->GetTop().GetId() == 0 ) || ( entity.GetTypeName() == Team_ABC::typeName_ && entity.GetId() == 0 ) ) )
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
bool Profile::CanDoMagic( const Entity_ABC& ) const
{
    if( !simulation_ || !supervision_ )
        return false;
    return true;
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
    timeControl_ = false;
    loggedIn_ = false;
    readEntities_.clear();
    readWriteEntities_.clear();
    rights_.Clear();
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
void Profile::NotifyUpdated( const ::Services& services )
{
    simulation_ = services.HasService( sword::service_simulation );
    if( services.HasService( sword::service_authentication ) )
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
