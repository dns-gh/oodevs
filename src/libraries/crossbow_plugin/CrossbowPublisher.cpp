// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "CrossbowPublisher.h"
#include "OGR_Workspace.h"
#include "Database_ABC.h"
#include "WorkingSession.h"
#include "DatabaseUpdater.h"
#include "ReportUpdater.h"
#include "FolkUpdater.h"
#include "ActionSerializer.h"
#include "OrderListener.h"
#include "StatusListener.h"
#include "ObjectListener.h"
#include "ExtensionFactory.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Config.h"
#include "clients_kernel/Controller.h"

#include <boost/bind.hpp>

using namespace plugins;
using namespace plugins::crossbow;


// -----------------------------------------------------------------------------
// Name: CrossbowPublisher constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
CrossbowPublisher::CrossbowPublisher( const dispatcher::Config& config, dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& publisher, xml::xistream& xis )
    : model_           ( model )
    , workspace_       ( new OGR_Workspace() )
    , serializer_      ( new ActionSerializer( config, model, staticModel, *workspace_ ) )
    , extensions_      ( new ExtensionFactory() )
    , modelLoaded_     ( false )
{
    model.RegisterFactory( *extensions_ );

    workspace_->Initialize( xis, config );

    session_.reset          ( new WorkingSession( *workspace_, config ) );
    databaseUpdater_.reset  ( new DatabaseUpdater( *workspace_, model, *session_ ) );
    reportUpdater_.reset    ( new ReportUpdater( *workspace_, config, model, *session_ ) );
    folkUpdater_.reset      ( new FolkUpdater( *workspace_, *session_ ) );

    // activate listeners
    listeners_.push_back( T_SharedListener( new OrderListener( model, *workspace_, *serializer_, publisher, *session_ ) ) );
    listeners_.push_back( T_SharedListener( new ObjectListener( *workspace_, *serializer_, publisher, *session_ ) ) );
    // listeners_.push_back( T_SharedListener( new StatusListener( *workspace_, publisher, *session_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
CrossbowPublisher::~CrossbowPublisher()
{
    model_.UnregisterFactory( *extensions_ );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::IsRelevant
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
bool CrossbowPublisher::IsRelevant( const sword::SimToClient& wrapper ) const
{
    if( wrapper.message().has_control_begin_tick() ||
        wrapper.message().has_control_end_tick() ||
        wrapper.message().has_object_update() ||
        wrapper.message().has_object_destruction() ||
        wrapper.message().has_unit_knowledge_update() ||
        wrapper.message().has_object_destruction() ||
        wrapper.message().has_unit_knowledge_update() ||
        wrapper.message().has_unit_knowledge_destruction() ||
        wrapper.message().has_object_knowledge_update() ||
        wrapper.message().has_object_knowledge_destruction() ||
        wrapper.message().has_unit_destruction() ||
        wrapper.message().has_report() ||
        wrapper.message().has_folk_graph_update() )
        return modelLoaded_;

    if( wrapper.message().has_control_send_current_state_begin() ||
        wrapper.message().has_control_send_current_state_end() ||
        wrapper.message().has_object_creation() ||
        wrapper.message().has_formation_creation() ||
        wrapper.message().has_automat_creation() ||
        wrapper.message().has_unit_knowledge_creation() ||
        wrapper.message().has_object_knowledge_creation() ||
        wrapper.message().has_unit_creation() ||
        wrapper.message().has_folk_creation() )
        return true;

    if( wrapper.message().has_automat_attributes() )
        return ( wrapper.message().automat_attributes().has_etat_automate() == 1 );

    if( wrapper.message().has_unit_attributes() )
    {
        const sword::UnitAttributes& attributes = wrapper.message().unit_attributes();
        if( attributes.has_position() || attributes.has_vitesse() || attributes.has_etat_operationnel() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::IsRelevant
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
bool CrossbowPublisher::IsRelevant( const sword::MessengerToClient& wrapper ) const
{
    if( wrapper.message().has_lima_destruction() ||
        wrapper.message().has_limit_destruction() )
        return modelLoaded_;

    if( wrapper.message().has_lima_creation() ||
        wrapper.message().has_limit_creation() )
        return true;

    return true;
}


// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::IsRelevantAcknowledge
// Created: MPT 2009-12-22
// -----------------------------------------------------------------------------
bool CrossbowPublisher::IsRelevantAcknowledge( const sword::SimToClient& wrapper ) const
{
    /* if( wrapper.message().has_unit_order_ack() ||
        wrapper.message().has_automat_order_ack() ||
        wrapper.message().has_population_order_ack() ||
        wrapper.message().has_frag_order_ack() ||
        wrapper.message().has_set_automat_mode_ack() ||
        wrapper.message().has_unit_creation_request_ack() ||
        wrapper.message().has_unit_magic_action_ack() ) */
    if( wrapper.message().has_object_magic_action_ack() )
        return true;

  /*case T_sword_msg_msg_population_magic_action_ack
    case T_sword_msg_msg_change_diplomacy_ack
    case T_sword_msg_msg_automat_change_knowledge_group_ack
    case T_sword_msg_msg_automat_change_logistic_links_ack
    case T_sword_msg_msg_automat_change_superior_ack
    case T_sword_msg_msg_unit_change_superior_ack
    case T_sword_msg_msg_log_supply_push_flow_ack
    case T_sword_msg_msg_log_supply_change_quotas_ack
    case T_sword_msg_msg_control_information
    case T_sword_msg_msg_control_profiling_information
    case T_sword_msg_msg_control_begin_tick
    case T_sword_msg_msg_control_end_tick
    case T_sword_msg_msg_control_stop_ack
    case T_sword_msg_msg_control_pause_ack
    case T_sword_msg_msg_control_resume_ack
    case T_sword_msg_msg_control_change_time_factor_ack
    case T_sword_msg_msg_control_date_time_change_ack
    case T_sword_msg_msg_control_global_meteo_ack
    case T_sword_msg_msg_control_local_meteo_ack*/
    return false;
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::Receive
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void CrossbowPublisher::Receive( const sword::SimToClient& asn )
{
    try
    {
        if( IsRelevantAcknowledge( asn ) )
            UpdateOnAcknowledge( asn );

        if( !IsRelevant( asn ) )
            return;

        UpdateOnTick( asn );
        UpdateDatabase( asn );
        UpdateFolkDatabase( asn );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "ERROR CATCHED (" << __FUNCTION__ << ") : " << std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::Receive
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::Receive( const sword::MessengerToClient& asn )
{
    try
    {
        if( !IsRelevant( asn ) )
            return;
        UpdateDatabase( asn );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "ERROR CATCHED :" << __FUNCTION__ << std::string( e.what() ) );
    }
}


// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateOnTick
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateOnTick( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_send_current_state_begin() )
        modelLoaded_ = false;
    else if( wrapper.message().has_control_send_current_state_end() )
    {
        databaseUpdater_->Flush();
        modelLoaded_ = true;
    }
    else if( wrapper.message().has_control_begin_tick() )
    {
        MT_LOG_INFO_MSG( "tick " << wrapper.message().control_begin_tick().current_tick() );
        // update simulation clock
        databaseUpdater_->Update( wrapper.message().control_begin_tick() );
        UpdateListeners();
    }
    else if( wrapper.message().has_control_end_tick() )
    {
        folkUpdater_->Flush();
        databaseUpdater_->Flush( /*false*/ );
    }
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateDatabase
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateDatabase( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_formation_creation() )
        databaseUpdater_->Update( wrapper.message().formation_creation() );
    else if( wrapper.message().has_automat_creation() )
        databaseUpdater_->Update( wrapper.message().automat_creation() );
    else if( wrapper.message().has_automat_attributes() )
        databaseUpdater_->Update( wrapper.message().automat_attributes() );

    else if( wrapper.message().has_unit_creation() )
        databaseUpdater_->Update( wrapper.message().unit_creation() );
    else if( wrapper.message().has_unit_attributes() )
        databaseUpdater_->Update( wrapper.message().unit_attributes() );
    else if( wrapper.message().has_unit_destruction() )
        databaseUpdater_->Update( wrapper.message().unit_destruction() );

    else if( wrapper.message().has_unit_knowledge_creation() )
        databaseUpdater_->Update( wrapper.message().unit_knowledge_creation() );
    else if( wrapper.message().has_unit_knowledge_update() )
        databaseUpdater_->Update( wrapper.message().unit_knowledge_update() );
    else if( wrapper.message().has_unit_knowledge_destruction() )
        databaseUpdater_->Update( wrapper.message().unit_knowledge_destruction() );

    else if( wrapper.message().has_object_knowledge_creation() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_creation() );
    else if( wrapper.message().has_object_knowledge_update() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_update() );
    else if( wrapper.message().has_object_knowledge_destruction() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_destruction() );

    else if( wrapper.message().has_object_creation() )
        databaseUpdater_->Update( wrapper.message().object_creation() );
    else if( wrapper.message().has_object_update() )
        databaseUpdater_->Update( wrapper.message().object_update() );
    else if( wrapper.message().has_object_destruction() )
        databaseUpdater_->Update( wrapper.message().object_destruction() );

    else if( wrapper.message().has_object_knowledge_creation() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_creation() );
    else if( wrapper.message().has_object_knowledge_update() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_update() );
    else if( wrapper.message().has_object_knowledge_destruction() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_destruction() );

    else if( wrapper.message().has_party_creation() )
        databaseUpdater_->Update( wrapper.message().party_creation() );

    else if( wrapper.message().has_report() )
        reportUpdater_->Update( wrapper.message().report() );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateDatabase
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateDatabase( const sword::MessengerToClient& wrapper )
{
    if( wrapper.message().has_lima_creation() )
        databaseUpdater_->Update( wrapper.message().lima_creation() );
//  else if( wrapper.message().has_lima_update() )
//      databaseUpdater_->Update( wrapper.message().lima_update() );
//  else if( wrapper.message().has_lima_destruction() )
//      databaseUpdater_->Update( wrapper.message().lima_destruction() );

    else if( wrapper.message().has_limit_creation() )
        databaseUpdater_->Update( wrapper.message().limit_creation() );
//  else if( wrapper.message().has_limit_destruction() )
//      databaseUpdater_->Update( wrapper.message().lima_destruction() );
}


// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateFolkDatabase
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateFolkDatabase( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_folk_creation() )
        folkUpdater_->Update( wrapper.message().folk_creation() );
    else if( wrapper.message().has_folk_graph_update() )
        folkUpdater_->Update( wrapper.message().folk_graph_update() );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::HandleAcknowledge
// Created: MPT 2009-12-22
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateOnAcknowledge( const sword::SimToClient& wrapper )
{
    if ( wrapper.message().has_object_magic_action_ack() )
        databaseUpdater_->Log( wrapper.message().object_magic_action_ack() );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateListeners
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateListeners()
{
    if( modelLoaded_ )
        std::for_each( listeners_.begin(), listeners_.end(), boost::bind( &Listener_ABC::Listen, _1 ) );
}
