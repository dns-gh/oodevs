// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Profile.h"
#include "ProfileManager.h"
#include "Automat.h"
#include "Formation.h"
#include "Model.h"
#include "Population.h"
#include "Side.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/authenticationsenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include <xeumeuleu/xml.hpp>
#include "MT_Tools/MT_Scipio_enum.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile::Profile( const Model& model, ClientPublisher_ABC& clients, const std::string& strLogin, xml::xistream& xis )
    : model_       ( model )
    , clients_     ( clients )
    , strLogin_    ( strLogin )
    , bSupervision_( false )
    , role_        ( eRoleUndefined )
{
    std::string role;
    xis >> xml::attribute( "password", strPassword_ )
        >> xml::attribute( "supervision", bSupervision_ )
        >> xml::optional >> xml::attribute( "scipio-role", role )
        >> xml::start( "rights" )
            >> xml::start( "readonly" )
                >> xml::list( "automat"   , *this, &Profile::ReadAutomatRights   , readOnlyAutomats_    )
                >> xml::list( "side"      , *this, &Profile::ReadSideRights      , readOnlySides_       )
                >> xml::list( "formation" , *this, &Profile::ReadFormationRights , readOnlyFormations_  )
                >> xml::list( "population", *this, &Profile::ReadPopulationRights, readOnlyPopulations_ )
            >> xml::end
            >> xml::start( "readwrite" )
                >> xml::list( "automat"   , *this, &Profile::ReadAutomatRights   , readWriteAutomats_    )
                >> xml::list( "side"      , *this, &Profile::ReadSideRights      , readWriteSides_       )
                >> xml::list( "formation" , *this, &Profile::ReadFormationRights , readWriteFormations_  )
                >> xml::list( "population", *this, &Profile::ReadPopulationRights, readWritePopulations_ )
            >> xml::end
        >> xml::end;
    if( !role.empty() )
        role_ = ProfileManager::FindRole( role );
}

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
Profile::Profile( const Model& model, ClientPublisher_ABC& clients, const MsgsClientToAuthentication::MsgProfileCreationRequest& message )
    : model_       ( model )
    , clients_     ( clients )
    , strLogin_    ( message.profile().login() )
    , strPassword_ ( message.profile().has_password()  ? message.profile().password() : "" )
    , bSupervision_( message.profile().superviseur() != 0 )
    , role_        ( message.profile().has_role()   ? static_cast< E_ScipioRole > ( message.profile().role() ) : eRoleUndefined )
{
    ReadRights( message.profile() );
    SendCreation( clients_ );
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    authentication::ProfileDestruction message;
    message().set_login( strLogin_ );
    message.Send( clients_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadAutomatRights
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void Profile::ReadAutomatRights( xml::xistream& xis, T_AutomatSet& container )
{
    const int id = xis.attribute< int >( "id" );
    if( const kernel::Automat_ABC* pAutomat = model_.Automats().Find( id ) )
        container.insert( pAutomat );
    else
        MT_LOG_ERROR_MSG( "Invalid automat id ('" << id << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadSideRights
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void Profile::ReadSideRights( xml::xistream& xis, T_SideSet& container )
{
    const int id = xis.attribute< int >( "id" );
    if( const kernel::Team_ABC* pSide = model_.Sides().Find( id ) )
        container.insert( pSide );
    else
        MT_LOG_ERROR_MSG( "Invalid side id ('" << id << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadFormationRights
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void Profile::ReadFormationRights( xml::xistream& xis, T_FormationSet&  container )
{
    const int id = xis.attribute< int >( "id" );
    if( const kernel::Formation_ABC* pFormation = model_.Formations().Find( id ) )
        container.insert( pFormation );
    else
        MT_LOG_ERROR_MSG( "Invalid formation id ('" << id << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadPopulationRights
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
void Profile::ReadPopulationRights( xml::xistream& xis, T_PopulationSet& container )
{
    const int id = xis.attribute< int >( "id" );
    if( const kernel::Population_ABC* pPopulation = model_.Populations().Find( id ) )
        container.insert( pPopulation );
    else
        MT_LOG_ERROR_MSG( "Invalid population id ('" << id << "') while reading profiles" );

}

namespace
{
    template< typename L, typename B, typename C >
    void SetRights( const L& source, std::set< const B* >& list, const tools::Resolver_ABC< C >& model )
    {
        list.clear();
        for( int i = 0; i < source.elem_size(); ++i )
            if( const B* entity = model.Find( source.elem(i).id() ) )
                list.insert( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadRights
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::ReadRights( const MsgsAuthenticationToClient::MsgProfile& message )
{
    if( message.has_read_only_automates()  )
        SetRights( message.read_only_automates(), readOnlyAutomats_, model_.Automats() );
    if( message.has_read_only_camps()  )
        SetRights( message.read_only_camps(), readOnlySides_, model_.Sides() );
    if( message.has_read_only_formations()  )
        SetRights( message.read_only_formations(), readOnlyFormations_, model_.Formations() );
    if( message.has_read_only_populations()  )
        SetRights( message.read_only_populations(), readOnlyPopulations_, model_.Populations() );
    if( message.has_read_write_automates()  )
        SetRights( message.read_write_automates(), readWriteAutomats_, model_.Automats() );
    if( message.has_read_write_camps()  )
        SetRights( message.read_write_camps(), readWriteSides_, model_.Sides() );
    if( message.has_read_write_formations()  )
        SetRights( message.read_write_formations(), readWriteFormations_, model_.Formations() );
    if( message.has_read_write_populations()  )
        SetRights( message.read_write_populations(), readWritePopulations_, model_.Populations() );
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckPassword
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
bool Profile::CheckPassword( const std::string& strPassword ) const
{
    return strPassword == strPassword_;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const MsgsClientToSim::MsgClientToSim& wrapper ) const
{
    const ::MsgsClientToSim::MsgClientToSim_Content& message = wrapper.message();
    if( message.has_control_pause() )
        return bSupervision_;
    if( message.has_control_resume() )
        return bSupervision_;
    if( message.has_control_change_time_factor() )
        return bSupervision_;
    if( message.has_control_date_time_change() )
        return bSupervision_;
    if( message.has_control_checkpoint_save_now() )
        return bSupervision_;
    if( message.has_control_checkpoint_set_frequency() )
        return bSupervision_;
    if( wrapper.message().has_control_checkpoint_set_frequency() )
        return bSupervision_;
    if( wrapper.message().has_control_toggle_vision_cones() )
        return true;
    if( message.has_unit_order() )
        return true;
    if( message.has_automat_order() )
        return true;
    if( message.has_population_order() )
        return true;
    if( message.has_frag_order() )
        return true;
    if( message.has_unit_creation_request() )
        return true;
    if( message.has_set_automat_mode() )
        return true;
    if( message.has_unit_magic_action() )
    {
        if( message.unit_magic_action().type()== MsgsClientToSim::MsgUnitMagicAction::create_fire_order )
            return bSupervision_;
        return true;
    }
    if( message.has_magic_action() )
    {
        if( message.magic_action().type()== MsgsClientToSim::MsgMagicAction::global_meteo
            || message.magic_action().type()== MsgsClientToSim::MsgMagicAction::local_meteo
            || message.magic_action().type()== MsgsClientToSim::MsgMagicAction::create_knowledge_group
            || message.magic_action().type()== MsgsClientToSim::MsgMagicAction::change_resource_links )
            return bSupervision_;
        return true;
    }
    if( message.has_knowledge_magic_action() )
        return bSupervision_;
    if( message.has_object_magic_action() )
        return true;

    return false;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const MsgsClientToAuthentication::MsgClientToAuthentication& wrapper ) const
{
    if( wrapper.message().has_authentication_request() )
        return true;
    else if( wrapper.message().has_profile_creation_request() )
        return bSupervision_; //$$$ Administration
    else if( wrapper.message().has_profile_update_request() )
        return bSupervision_; //$$$ Administration
    else if( wrapper.message().has_profile_destruction_request() )
        return bSupervision_; //$$$ Administration
    else
        return false;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const MsgsClientToReplay::MsgClientToReplay& ) const
{
    return bSupervision_;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const Common::MsgChatTarget& source, const Common::MsgChatTarget& target ) const
{
    const std::string t( target.profile() );
    const std::string s( source.profile() );
    return t.empty() || strLogin_ == t || strLogin_ == s;
}

namespace
{
    template< typename List, typename Entity >
    void Serialize( List& message, const std::set< const Entity* >& list )
    {
        for( std::set< const Entity* >::const_iterator it = list.begin(); it != list.end(); ++it )
            message.add_elem()->set_id( (*it)->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: NLD 2006-10-09
// -----------------------------------------------------------------------------
void Profile::Send( MsgsAuthenticationToClient::MsgProfile& message ) const
{
    message.set_login( strLogin_ );
    message.set_superviseur( bSupervision_ );
    if( role_ != eRoleUndefined )
        message.set_role( static_cast< MsgsAuthenticationToClient::Role >( role_ ) );

    Serialize( *message.mutable_read_only_automates(), readOnlyAutomats_ );
    Serialize( *message.mutable_read_write_automates(), readWriteAutomats_ );
    Serialize( *message.mutable_read_only_camps(), readOnlySides_ );
    Serialize( *message.mutable_read_write_camps(), readWriteSides_ );
    Serialize( *message.mutable_read_only_formations(), readOnlyFormations_ );
    Serialize( *message.mutable_read_write_formations(), readWriteFormations_ );
    Serialize( *message.mutable_read_only_populations(), readOnlyPopulations_ );
    Serialize( *message.mutable_read_write_populations(), readWritePopulations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
void Profile::Send( MsgsAuthenticationToClient::MsgProfileDescription& message ) const
{
    message.set_login( strLogin_ );
    message.set_password( !strPassword_.empty() );
    message.set_supervisor( bSupervision_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::SendCreation
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::SendCreation( ClientPublisher_ABC& publisher ) const
{
    authentication::ProfileCreation message;
    Send( *message().mutable_profile() );
    message().mutable_profile()->set_password( strPassword_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::Update( const MsgsClientToAuthentication::MsgProfileUpdateRequest& message )
{
    strLogin_ = message.profile().login();
    if( message.profile().has_password()  )
        strPassword_ = message.profile().password();
    bSupervision_ = message.profile().superviseur() != 0;
    ReadRights( message.profile() );

    authentication::ProfileUpdate updatemessage;
    updatemessage().set_login( message.login() );
    if( updatemessage().profile().has_password()  )
        updatemessage().mutable_profile()->set_password( strPassword_ );
    if( updatemessage().profile().has_role()  )
        updatemessage().mutable_profile()->set_role( static_cast< MsgsAuthenticationToClient::Role >( role_ ) );
    Send( *updatemessage().mutable_profile() );
    updatemessage.Send( clients_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::GetName
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
std::string Profile::GetName() const
{
    return strLogin_;
}

// -----------------------------------------------------------------------------
// Name: Profile::SetRight
// Created: SBO 2008-07-24
// -----------------------------------------------------------------------------
void Profile::SetRight( const kernel::Automat_ABC& entity, bool readonly, bool readwrite )
{
    if( readonly )
        readOnlyAutomats_.insert( &entity );
    else
        readOnlyAutomats_.erase( &entity );
    if( readwrite )
        readWriteAutomats_.insert( &entity );
    else
        readWriteAutomats_.erase( &entity );

    authentication::ProfileUpdate updatemessage;
    updatemessage().set_login( strLogin_ );
    Send( *updatemessage().mutable_profile() );
    updatemessage.Send( clients_ );
}
