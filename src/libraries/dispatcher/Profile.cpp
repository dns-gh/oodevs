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

#include "Model.h"
#include "ClientPublisher_ABC.h"
#include "Automat.h"
#include "Side.h"
#include "Formation.h"
#include "Population.h"
#include "ClientPublisher_ABC.h"
#include <xeumeuleu/xml.h>
#include "MT/MT_Logger/MT_Logger_lib.h"

#include "protocol/authenticationsenders.h"
#include "protocol/clientsenders.h"
#include "protocol/simulationsenders.h"

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
{
    xis >> xml::attribute( "password", strPassword_ )
        >> xml::attribute( "supervision", bSupervision_ )
        >> xml::start( "rights" )
            >> xml::start( "readonly" )
                >> xml::list( "automat"   , *this, &Profile::ReadAutomatRights   , readOnlyAutomats_    )
                >> xml::list( "side"      , *this, &Profile::ReadSideRights      , readOnlySides_       )
                >> xml::list( "formation" , *this, &Profile::ReadFormationRights , readOnlyFormations_  )
                >> xml::list( "population", *this, &Profile::ReadPopulationRights, readOnlyPopulations_ )
            >> xml::end()
            >> xml::start( "readwrite" )
                >> xml::list( "automat"   , *this, &Profile::ReadAutomatRights   , readWriteAutomats_    )
                >> xml::list( "side"      , *this, &Profile::ReadSideRights      , readWriteSides_       )
                >> xml::list( "formation" , *this, &Profile::ReadFormationRights , readWriteFormations_  )
                >> xml::list( "population", *this, &Profile::ReadPopulationRights, readWritePopulations_ )
            >> xml::end()
        >> xml::end();
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
    authentication::ProfileDestruction asn;
    asn().set_login( strLogin_ );
    asn.Send( clients_ );
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Profile::ReadAutomatRights
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void Profile::ReadAutomatRights( xml::xistream& xis, T_AutomatSet& container )
{
    int nID;
    xis >> xml::attribute( "id", nID );

    const kernel::Automat_ABC* pAutomat = model_.automats_.Find( nID );
    if( pAutomat )
        container.insert( pAutomat );
    else
        MT_LOG_ERROR_MSG( "Invalid automat id ('" << nID << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadSideRights
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void Profile::ReadSideRights( xml::xistream& xis, T_SideSet& container )
{
    int nID;
    xis >> xml::attribute( "id", nID );

    const kernel::Team_ABC* pSide = model_.sides_.Find( nID );
    if( pSide )
        container.insert( pSide );
    else
        MT_LOG_ERROR_MSG( "Invalid side id ('" << nID << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadFormationRights
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void Profile::ReadFormationRights( xml::xistream& xis, T_FormationSet&  container )
{
    int nID;
    xis >> xml::attribute( "id", nID );

    const kernel::Formation_ABC* pFormation = model_.formations_.Find( nID );
    if( pFormation )
        container.insert( pFormation );
    else
        MT_LOG_ERROR_MSG( "Invalid formation id ('" << nID << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadPopulationRights
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
void Profile::ReadPopulationRights( xml::xistream& xis, T_PopulationSet& container )
{
    int nID;
    xis >> xml::attribute( "id", nID );

    const kernel::Population_ABC* pPopulation = model_.populations_.Find( nID );
    if( pPopulation )
        container.insert( pPopulation );
    else
        MT_LOG_ERROR_MSG( "Invalid population id ('" << nID << "') while reading profiles" );

}

namespace
{
    template< typename L, typename B, typename C >
    void SetRights( const L& source, std::set< const B* >& list, const tools::Resolver_ABC< C >& model )
    {
        list.clear();
        for( int i = 0; i < source.elem_size(); ++i )
            if( const B* entity = model.Find( source.elem(i).oid() ) )
                list.insert( entity );
    }
}
// $$$$ NLD 2007-01-30: Factoriser ASN & xml

// -----------------------------------------------------------------------------
// Name: Profile::ReadRights
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::ReadRights( const MsgsAuthenticationToClient::MsgProfile& message )
{
    if( message.has_read_only_automates()  )
        SetRights( message.read_only_automates(), readOnlyAutomats_, model_.automats_ );
    if( message.has_read_only_camps()  )
        SetRights( message.read_only_camps(), readOnlySides_, model_.sides_ );
    if( message.has_read_only_formations()  )
        SetRights( message.read_only_formations(), readOnlyFormations_, model_.formations_ );
    if( message.has_read_only_populations()  )
        SetRights( message.read_only_populations(), readOnlyPopulations_, model_.populations_ );
    if( message.has_read_write_automates()  )
        SetRights( message.read_write_automates(), readWriteAutomats_, model_.automats_ );
    if( message.has_read_write_camps()  )
        SetRights( message.read_write_camps(), readWriteSides_, model_.sides_ );
    if( message.has_read_write_formations()  )
        SetRights( message.read_write_formations(), readWriteFormations_, model_.formations_ );
    if( message.has_read_write_populations()  )
        SetRights( message.read_write_populations(), readWritePopulations_, model_.populations_ );
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
    if( message.has_control_global_meteo() )
        return bSupervision_;
    if( message.has_control_local_meteo() )
        return bSupervision_;
    if( message.has_control_checkpoint_save_now() )
        return bSupervision_;
    if( message.has_control_checkpoint_set_frequency() )
        return bSupervision_;
    if( wrapper.message().has_control_checkpoint_set_frequency() )
        return bSupervision_;
    if( wrapper.message().has_knowledge_group_creation_request() )
        return bSupervision_;
    if( wrapper.message().has_knowledge_group_update_request() )
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
    if( message.has_set_automat_mode() )
        return true;
    if( message.has_unit_creation_request() )
        return true;
    if( message.has_unit_magic_action() )
        return true;
    if( message.has_object_magic_action() )
        return true;
    if( message.has_population_magic_action() )
        return true;
    if( message.has_change_diplomacy() )
        return true;
    if( message.has_automat_change_knowledge_group() )
        return true;
    if( message.has_automat_change_logistic_links() )
        return true;
    if( message.has_automat_change_superior() )
        return true;
    if( message.has_unit_change_superior() )
        return true;
    if( message.has_log_supply_push_flow() )
        return true;
    if( message.has_log_supply_change_quotas() )
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
    void Serialize( List& asn, const std::set< const Entity* >& list )
    {
        for( std::set< const Entity* >::const_iterator it = list.begin(); it != list.end(); ++it )
            asn.add_elem()->set_oid( (*it)->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: NLD 2006-10-09
// -----------------------------------------------------------------------------
void Profile::Send( MsgsAuthenticationToClient::MsgProfile& asn ) const
{
    asn.set_login( strLogin_.c_str() );
    asn.set_superviseur( bSupervision_ );

    Serialize( *asn.mutable_read_only_automates(), readOnlyAutomats_ );
    Serialize( *asn.mutable_read_write_automates(), readWriteAutomats_ );
    Serialize( *asn.mutable_read_only_camps(), readOnlySides_ );
    Serialize( *asn.mutable_read_write_camps(), readWriteSides_ );
    Serialize( *asn.mutable_read_only_formations(), readOnlyFormations_ );
    Serialize( *asn.mutable_read_write_formations(), readWriteFormations_ );
    Serialize( *asn.mutable_read_only_populations(), readOnlyPopulations_ );
    Serialize( *asn.mutable_read_write_populations(), readWritePopulations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::Delete
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
void Profile::Delete( MsgsAuthenticationToClient::MsgProfile& asn )
{
    if( asn.has_read_only_automates() && asn.read_only_automates().elem_size() > 0 )
        asn.mutable_read_only_automates()->Clear();
    if( asn.has_read_write_automates() && asn.read_write_automates().elem_size() > 0 )
        asn.mutable_read_write_automates()->Clear();

    if( asn.has_read_only_camps() && asn.read_only_camps().elem_size() > 0 )
        asn.mutable_read_only_camps()->Clear();
    if( asn.has_read_write_camps() && asn.read_write_camps().elem_size() > 0 )
        asn.mutable_read_write_camps()->Clear();

    if( asn.has_read_only_formations() && asn.read_only_formations().elem_size() > 0 )
        asn.mutable_read_only_formations()->Clear();
    if( asn.has_read_write_formations() && asn.read_write_formations().elem_size() > 0 )
        asn.mutable_read_write_formations()->Clear();

    if( asn.has_read_only_populations() && asn.read_only_populations().elem_size() > 0 )
        asn.mutable_read_only_populations()->Clear();
    if( asn.has_read_write_populations() && asn.read_write_populations().elem_size() > 0 )
        asn.mutable_read_write_populations()->Clear();
}

// -----------------------------------------------------------------------------
// Name: Profile::SendCreation
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::SendCreation( ClientPublisher_ABC& publisher ) const
{
    authentication::ProfileCreation asn;
    //Send( *asn().mutable_profile() );
    Send( *asn().mutable_profile() );
    asn().mutable_profile()->set_password( strPassword_.c_str() );
    asn.Send( publisher );
    Profile::Delete( *asn().mutable_profile() );
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

    authentication::ProfileUpdate asn;
    asn().set_login( message.login() );
    if( asn().profile().has_password()  )
        asn().mutable_profile()->set_password( strPassword_.c_str() );
    Send( *asn().mutable_profile() );
    asn.Send( clients_ );
    Profile::Delete( *asn().mutable_profile() );
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

    authentication::ProfileUpdate asn;
    asn().set_login( strLogin_.c_str() );
    Send( *asn().mutable_profile() );
    asn.Send( clients_ );
    Profile::Delete( *asn().mutable_profile() );
}
