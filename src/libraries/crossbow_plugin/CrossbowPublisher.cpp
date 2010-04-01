// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
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
#include "OrderTypes.h"
#include "OrderListener.h"
#include "StatusListener.h"
#include "ObjectListener.h"
#include "ExtensionFactory.h"
#include "dispatcher/Model.h"
#include "dispatcher/Config.h"

using namespace plugins;
using namespace plugins::crossbow;


// -----------------------------------------------------------------------------
// Name: CrossbowPublisher constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
CrossbowPublisher::CrossbowPublisher( const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher, xml::xistream& xis )
    : model_           ( model )
    , workspace_       ( new OGR_Workspace() )
    , orderTypes_      ( new OrderTypes( config ) )
    , modelLoaded_     ( false )
    , factory_         ( new ExtensionFactory() )
{
    model.RegisterFactory( *factory_ );

    workspace_->Initialize( xis, config );

    session_.reset( new WorkingSession( *workspace_, config ) );
    databaseUpdater_.reset( new DatabaseUpdater( *workspace_, model, *session_ ) );
    reportUpdater_.reset( new ReportUpdater( *workspace_, config, model, *session_ ) );
	folkUpdater_.reset( new FolkUpdater( *workspace_, *session_ ) );

	// activate listeners
    listeners_.push_back( T_SharedListener( new OrderListener( *workspace_, model, *orderTypes_, publisher, *session_ ) ) );
    listeners_.push_back( T_SharedListener( new ObjectListener( *workspace_, publisher, *session_ ) ) );
    // listeners_.push_back( T_SharedListener( new StatusListener( workspace_->GetDatabase( "geodatabase" ), publisher ) ) );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
CrossbowPublisher::~CrossbowPublisher()
{
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::IsRelevant
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
bool CrossbowPublisher::IsRelevant( const MsgsSimToClient::MsgSimToClient& wrapper ) const
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
        const MsgsSimToClient::MsgUnitAttributes& attributes = wrapper.message().unit_attributes();
        if( attributes.has_position() || attributes.has_vitesse() || attributes.has_etat_operationnel() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::IsRelevant
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
bool CrossbowPublisher::IsRelevant( const MsgsMessengerToClient::MsgMessengerToClient& wrapper ) const
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
// Name: CrossbowPublisher::Receive
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void CrossbowPublisher::Receive( const MsgsSimToClient::MsgSimToClient& asn )
{
    if( !IsRelevant( asn ) )
        return;

    UpdateOnTick( asn );
    UpdateDatabase( asn );
    UpdateFolkDatabase( asn );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::Receive
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::Receive( const MsgsMessengerToClient::MsgMessengerToClient& asn )
{
    if( !IsRelevant( asn ) )
        return;

    UpdateDatabase( asn );
}


// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateOnTick
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateOnTick( const MsgsSimToClient::MsgSimToClient& wrapper )
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
        {
            UpdateListeners();
        }
//        databaseUpdater_->Lock();
    }
    else if( wrapper.message().has_control_end_tick() )
    {
        // folkUpdater_->Drop();
        databaseUpdater_->Flush();
    }
}
    
// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateDatabase
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateDatabase( const MsgsSimToClient::MsgSimToClient& wrapper )
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
        databaseUpdater_->DestroyUnit( wrapper.message().unit_destruction() );

    else if( wrapper.message().has_unit_knowledge_creation() )
        databaseUpdater_->Update( wrapper.message().unit_knowledge_creation() );
    else if( wrapper.message().has_unit_knowledge_update() )
        databaseUpdater_->Update( wrapper.message().unit_knowledge_update() );
    else if( wrapper.message().has_unit_knowledge_destruction() )
        databaseUpdater_->DestroyUnitKnowledge( wrapper.message().unit_knowledge_destruction() );

    else if( wrapper.message().has_object_knowledge_creation() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_creation() );
    else if( wrapper.message().has_object_knowledge_update() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_update() );
	else if( wrapper.message().has_object_knowledge_destruction() )
        databaseUpdater_->DestroyObjectKnowledge( wrapper.message().object_knowledge_destruction() );

	else if( wrapper.message().has_object_creation() )
        databaseUpdater_->Update( wrapper.message().object_creation() );
    // else if( wrapper.message().has__object_update() )
    //    databaseUpdater_->Update( wrapper.message().object_update() );
    else if( wrapper.message().has_object_destruction() )
        databaseUpdater_->DestroyObject( wrapper.message().object_destruction() );
    
    else if( wrapper.message().has_report() )
        reportUpdater_->Update( wrapper.message().report() );
}
    
// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateDatabase
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateDatabase( const MsgsMessengerToClient::MsgMessengerToClient& wrapper )
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
void CrossbowPublisher::UpdateFolkDatabase( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_folk_creation() )
        folkUpdater_->Update( wrapper.message().folk_creation() );
    else if( wrapper.message().has_folk_graph_update() )
        folkUpdater_->Update( wrapper.message().folk_graph_update() );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPublisher::UpdateListeners
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void CrossbowPublisher::UpdateListeners()
{
    if( modelLoaded_ )
    {
        for( CIT_Listeners it = listeners_.begin(); it != listeners_.end(); ++it )
            (*it)->Listen();
    }
}
