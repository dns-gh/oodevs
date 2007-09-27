// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "DatabasePublisher.h"
#include "Workspace.h"
#include "Database.h"
#include "DatabaseUpdater.h"
#include "ReportFactory.h"
#include "OrderTypes.h"
#include "OrderListener.h"
#include "StatusListener.h"
#include "ObjectListener.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: DatabasePublisher constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabasePublisher::DatabasePublisher( const dispatcher::Config& config, const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher )
    : workspace_      ( new Workspace() )
    , database_       ( new Database( config ) )
    , reportFactory_  ( new ReportFactory( config, model ) )
    , orderTypes_     ( new kernel::OrderTypes( config ) )
    , databaseUpdater_( new DatabaseUpdater( *database_, model, *reportFactory_ ) )
    , modelLoaded_    ( false )
{
    listeners_.push_back( T_SharedListener( new OrderListener ( *database_, model, *orderTypes_, publisher ) ) );
    listeners_.push_back( T_SharedListener( new StatusListener( *database_, publisher ) ) );
    listeners_.push_back( T_SharedListener( new ObjectListener( *database_, publisher ) ) );
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabasePublisher::~DatabasePublisher()
{
    databaseUpdater_.release();
    reportFactory_.release();
    database_.release();
    workspace_.release();
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::IsRelevant
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
bool DatabasePublisher::IsRelevant( const ASN1T_MsgsSimToClient& asn ) const
{
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_control_begin_tick:
    case T_MsgsSimToClient_msg_msg_control_end_tick:
    case T_MsgsSimToClient_msg_msg_control_send_current_state_begin:
    case T_MsgsSimToClient_msg_msg_control_send_current_state_end:
    case T_MsgsSimToClient_msg_msg_lima_creation:
    case T_MsgsSimToClient_msg_msg_lima_destruction:
    case T_MsgsSimToClient_msg_msg_limit_creation:
    case T_MsgsSimToClient_msg_msg_limit_destruction:
    case T_MsgsSimToClient_msg_msg_object_creation:
    case T_MsgsSimToClient_msg_msg_object_update:
    case T_MsgsSimToClient_msg_msg_object_destruction:
    case T_MsgsSimToClient_msg_msg_formation_creation:
    case T_MsgsSimToClient_msg_msg_automat_creation:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_update:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:
    case T_MsgsSimToClient_msg_msg_unit_creation:
    case T_MsgsSimToClient_msg_msg_unit_destruction:
    case T_MsgsSimToClient_msg_msg_report:
    case T_MsgsSimToClient_msg_msg_folk_creation:
    case T_MsgsSimToClient_msg_msg_folk_graph_update:
        return true;
    case T_MsgsSimToClient_msg_msg_unit_attributes:
        const ASN1T_MsgUnitAttributes* attributes = asn.msg.u.msg_unit_attributes;
        if( attributes->m.positionPresent || attributes->m.vitessePresent || attributes->m.etat_operationnelPresent )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::Receive
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabasePublisher::Receive( const ASN1T_MsgsSimToClient& asn )
{
    if( !IsRelevant( asn ) )
        return;

    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_control_send_current_state_begin: modelLoaded_ = false; break;
    case T_MsgsSimToClient_msg_msg_control_send_current_state_end:   modelLoaded_ = true;  break;
    case T_MsgsSimToClient_msg_msg_control_begin_tick: 
        MT_LOG_INFO_MSG( "tick " << asn.msg.u.msg_control_begin_tick );
        database_->Lock();
        UpdateListeners();
        break;
    case T_MsgsSimToClient_msg_msg_control_end_tick:
        database_->UnLock();
        break;

    case T_MsgsSimToClient_msg_msg_formation_creation:          databaseUpdater_->Update( *asn.msg.u.msg_formation_creation ); break;
    case T_MsgsSimToClient_msg_msg_automat_creation:            databaseUpdater_->Update( *asn.msg.u.msg_automat_creation ); break;

    case T_MsgsSimToClient_msg_msg_lima_creation:               databaseUpdater_->Update( *asn.msg.u.msg_lima_creation ); break;
//    case T_MsgsSimToClient_msg_msg_lima_update:                 databaseUpdater_->Update( *asn.msg.u.msg_lima_update ); break;
//    case T_MsgsSimToClient_msg_msg_lima_destruction:            databaseUpdater_->Update( asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsSimToClient_msg_msg_limit_creation:              databaseUpdater_->Update( *asn.msg.u.msg_limit_creation ); break;
//    case T_MsgsSimToClient_msg_msg_limit_destruction:           databaseUpdater_->Update( asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsSimToClient_msg_msg_unit_creation:               databaseUpdater_->Update( *asn.msg.u.msg_unit_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_attributes:             databaseUpdater_->Update( *asn.msg.u.msg_unit_attributes ); break;
    case T_MsgsSimToClient_msg_msg_unit_destruction:            databaseUpdater_->DestroyUnit( asn.msg.u.msg_unit_destruction ); break;

    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:     databaseUpdater_->Update( *asn.msg.u.msg_unit_knowledge_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_update:       databaseUpdater_->Update( *asn.msg.u.msg_unit_knowledge_update ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:  databaseUpdater_->Update( *asn.msg.u.msg_unit_knowledge_destruction );  break;

    case T_MsgsSimToClient_msg_msg_object_creation:             databaseUpdater_->Update( *asn.msg.u.msg_object_creation ); break;
//    case T_MsgsSimToClient_msg_msg_object_update:               databaseUpdater_->Update( *asn.msg.u.msg_object_update ); break;
    case T_MsgsSimToClient_msg_msg_object_destruction:          databaseUpdater_->DestroyObject( asn.msg.u.msg_object_destruction ); break;
    case T_MsgsSimToClient_msg_msg_report:                      databaseUpdater_->Update( *asn.msg.u.msg_report ); break;

    case T_MsgsSimToClient_msg_msg_folk_creation:               databaseUpdater_->Update( *asn.msg.u.msg_folk_creation ); break;
    case T_MsgsSimToClient_msg_msg_folk_graph_update:           databaseUpdater_->Update( *asn.msg.u.msg_folk_graph_update ); break;
    }
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateListeners
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateListeners()
{
    if( modelLoaded_ )
        for( CIT_Listeners it = listeners_.begin(); it != listeners_.end(); ++it )
            (*it)->Listen();
}
