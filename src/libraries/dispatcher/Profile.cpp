// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
#include "protocol/AuthenticationSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include <xeumeuleu/xml.hpp>

using namespace dispatcher;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile::Profile( const Model& model, ClientPublisher_ABC& clients, const std::string& strLogin, xml::xistream& xis )
    : model_       ( model )
    , clients_     ( clients )
    , strLogin_    ( strLogin )
    , bSupervision_( false )
    , bTimeControl_( false )
{
    std::string role;
    xis >> xml::attribute( "password", strPassword_ )
        >> xml::attribute( "supervision", bSupervision_ )
        >> xml::attribute( "time-control", bTimeControl_ )
        >> xml::start( "rights" )
            >> xml::start( "readonly" )
                >> xml::list( "automat"   , *this, &Profile::ReadAutomatRights   , readOnlyAutomats_    )
                >> xml::list( "side"      , *this, &Profile::ReadSideRights      , readOnlySides_       )
                >> xml::list( "formation" , *this, &Profile::ReadFormationRights , readOnlyFormations_  )
                >> xml::list( "crowd", *this, &Profile::ReadPopulationRights, readOnlyPopulations_ )
            >> xml::end
            >> xml::start( "readwrite" )
                >> xml::list( "automat"   , *this, &Profile::ReadAutomatRights   , readWriteAutomats_    )
                >> xml::list( "side"      , *this, &Profile::ReadSideRights      , readWriteSides_       )
                >> xml::list( "formation" , *this, &Profile::ReadFormationRights , readWriteFormations_  )
                >> xml::list( "crowd", *this, &Profile::ReadPopulationRights, readWritePopulations_ )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
Profile::Profile( const Model& model, ClientPublisher_ABC& clients, const sword::ProfileCreationRequest& message )
    : model_        ( model )
    , clients_      ( clients )
    , strLogin_     ( message.profile().login() )
    , strPassword_  ( message.profile().has_password() ? message.profile().password() : "" )
    , bSupervision_ ( message.profile().supervisor() )
    , bTimeControl_ ( message.profile().has_time_control() ? message.profile().time_control() : false )
{
    ReadRights( message.profile() );
    SendCreation( clients_ );
}

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: LGY 2013-07-10
// -----------------------------------------------------------------------------
Profile::Profile( const Model& model, ClientPublisher_ABC& clients, const std::string& strLogin )
    : model_   ( model )
    , clients_ ( clients )
    , strLogin_( strLogin )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    // NOTHING
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
    template< typename L, typename B, typename R >
    void SetRights( const L& source, B& list, const R& resolver )
    {
        list.clear();
        for( int i = 0; i < source.elem_size(); ++i )
            if( const B::value_type entity = resolver.Find( source.elem(i).id() ) )
                list.insert( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadRights
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::ReadRights( const sword::Profile& message )
{
    if( message.has_read_only_automates() )
        SetRights( message.read_only_automates(), readOnlyAutomats_, model_.Automats() );
    if( message.has_read_only_parties() )
        SetRights( message.read_only_parties(), readOnlySides_, model_.Sides() );
    if( message.has_read_only_formations() )
        SetRights( message.read_only_formations(), readOnlyFormations_, model_.Formations() );
    if( message.has_read_only_crowds() )
        SetRights( message.read_only_crowds(), readOnlyPopulations_, model_.Populations() );
    if( message.has_read_write_automates() )
        SetRights( message.read_write_automates(), readWriteAutomats_, model_.Automats() );
    if( message.has_read_write_parties() )
        SetRights( message.read_write_parties(), readWriteSides_, model_.Sides() );
    if( message.has_read_write_formations() )
        SetRights( message.read_write_formations(), readWriteFormations_, model_.Formations() );
    if( message.has_read_write_crowds() )
        SetRights( message.read_write_crowds(), readWritePopulations_, model_.Populations() );
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
bool Profile::CheckRights( const sword::ClientToSim& wrapper ) const
{
    const ::sword::ClientToSim_Content& message = wrapper.message();
    if( message.has_control_pause() )
        return bTimeControl_;
    if( message.has_control_resume() )
        return bTimeControl_;
    if( message.has_control_stop() )
        return bTimeControl_;
    if( message.has_control_change_time_factor() )
        return bTimeControl_;
    if( message.has_control_date_time_change() )
        return bTimeControl_;
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
    if( message.has_crowd_order() )
        return true;
    if( message.has_frag_order() )
        return true;
    if( message.has_unit_creation_request() )
        return true;
    if( message.has_set_automat_mode() )
        return true;
    if( message.has_unit_magic_action() )
    {
        if( message.unit_magic_action().type()== sword::UnitMagicAction::create_fire_order )
            return bSupervision_;
        return true;
    }
    if( message.has_magic_action() )
    {
        if( message.magic_action().type()== sword::MagicAction::global_weather
            || message.magic_action().type()== sword::MagicAction::local_weather
            || message.magic_action().type()== sword::MagicAction::create_knowledge_group
            || message.magic_action().type()== sword::MagicAction::change_resource_network_properties
            || message.magic_action().type()== sword::MagicAction::create_fire_order_on_location )
            return bSupervision_;
        return true;
    }
    if( message.has_knowledge_magic_action() )
        return bSupervision_;
    if( message.has_object_magic_action() )
        return true;
    if( message.has_burning_cell_request() )
        return true;
    if( message.has_order_stream() )
        return true;
    if( message.has_list_enabled_vision_cones() )
        return true;
    if( message.has_pathfind_request() )
        return true;
    if( message.has_segment_request() )
        return true;
    if( message.has_logistic_history_request() ||
        message.has_list_logistic_requests() )
        // This message is handled by the logistic plugin, we allow it here only
        // to avoid the dispatcher to send "forbidden" errors to clients. A better
        // fix is to make DispatcherPlugin use HandleClientToSim() so the
        // message is filtered by the handling plugin.
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const sword::ClientToAuthentication& wrapper ) const
{
    if( wrapper.message().has_authentication_request() )
        return true;
    if( wrapper.message().has_connected_profiles_request() )
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
bool Profile::CheckRights( const sword::ClientToReplay& ) const
{
    return bSupervision_;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const sword::ChatTarget& source, const sword::ChatTarget& target ) const
{
    const std::string t( target.profile() );
    const std::string s( source.profile() );
    return t.empty() || strLogin_ == t || strLogin_ == s;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: SLI 2014-03-21
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const sword::ProfileUpdateRequest& msg, bool currentTimeControl ) const
{
    if( !msg.has_profile() || !msg.profile().has_time_control() )
        return true;
    if( msg.profile().time_control() != currentTimeControl )
        return bTimeControl_;
    return true;
}

namespace
{
    template< typename List, typename Container >
    void Serialize( List& message, const Container& c )
    {
        for( auto it = c.begin(); it != c.end(); ++it )
            message.add_elem()->set_id( (*it)->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: LGY 2013-10-08
// -----------------------------------------------------------------------------
void Profile::Send( sword::ConnectedProfileList& msg ) const
{
    Send( *msg.add_elem() );
}

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: LGY 2013-10-08
// -----------------------------------------------------------------------------
void Profile::Send( sword::AuthenticationResponse& msg ) const
{
    Send( *msg.mutable_profile() );
}

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: LGY 2013-10-08
// -----------------------------------------------------------------------------
void Profile::Send( sword::Profile& profile ) const
{
    profile.set_login( strLogin_ );
    profile.set_supervisor( bSupervision_ );
    profile.set_time_control( bTimeControl_ );
    Serialize( *profile.mutable_read_only_automates(), readOnlyAutomats_ );
    Serialize( *profile.mutable_read_write_automates(), readWriteAutomats_ );
    Serialize( *profile.mutable_read_only_parties(), readOnlySides_ );
    Serialize( *profile.mutable_read_write_parties(), readWriteSides_ );
    Serialize( *profile.mutable_read_only_formations(), readOnlyFormations_ );
    Serialize( *profile.mutable_read_write_formations(), readWriteFormations_ );
    Serialize( *profile.mutable_read_only_crowds(), readOnlyPopulations_ );
    Serialize( *profile.mutable_read_write_crowds(), readWritePopulations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
void Profile::Send( sword::ProfileDescription& message ) const
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
bool Profile::Update( const sword::ProfileUpdateRequest& message, const Profile_ABC& requester )
{
    if( message.profile().has_time_control() )
    {
        if( !requester.CheckRights( message, bTimeControl_ ) )
            return false;
        bTimeControl_ = message.profile().time_control();
    }
    strLogin_ = message.profile().login();
    if( message.profile().has_password() )
        strPassword_ = message.profile().password();
    bSupervision_ = message.profile().supervisor() != 0;
    ReadRights( message.profile() );
    authentication::ProfileUpdate updatemessage;
    updatemessage().set_login( message.login() );
    if( !strPassword_.empty() )
        updatemessage().mutable_profile()->set_password( strPassword_ );
    Send( *updatemessage().mutable_profile() );
    updatemessage.Send( clients_ );
    return true;
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

// -----------------------------------------------------------------------------
// Name: Profile::SerializeProfile
// Created: LGY 2011-03-25
// -----------------------------------------------------------------------------
void Profile::SerializeProfile( xml::xostream& xos ) const
{
    xos << xml::start( "profile" )
            << xml::attribute( "name", strLogin_ )
            << xml::attribute( "password", strPassword_ )
            << xml::attribute( "supervision", bSupervision_ )
            << xml::attribute( "time-control", bTimeControl_ )
            << xml::start( "rights" )
                << xml::start( "readonly" );
    SerializeRights( xos, "side", readOnlySides_ );
    SerializeRights( xos, "formation", readOnlyFormations_ );
    SerializeRights( xos, "automat", readOnlyAutomats_ );
    SerializeRights( xos, "crowd", readOnlyPopulations_ );
    xos         << xml::end
                << xml::start( "readwrite" );
    SerializeRights( xos, "side", readWriteSides_ );
    SerializeRights( xos, "formation", readWriteFormations_ );
    SerializeRights( xos, "automat", readWriteAutomats_ );
    SerializeRights( xos, "crowd", readWritePopulations_ );
    xos         << xml::end
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Profile::SerializeRights
// Created: LGY 2011-03-25
// -----------------------------------------------------------------------------
template< typename T >
void Profile::SerializeRights( xml::xostream& xos, const std::string& tag, const T& list ) const
{
    for( T::const_iterator it = list.begin(); it != list.end(); ++it )
        xos << xml::start( tag )
            << xml::attribute( "id", (*it)->GetId() )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Profile::SendDestruction
// Created: LGY 2013-10-09
// -----------------------------------------------------------------------------
void Profile::SendDestruction() const
{
    authentication::ProfileDestruction message;
    message().set_login( strLogin_ );
    message.Send( clients_ );
}
