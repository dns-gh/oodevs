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
#include "FolkUpdater.h"
#include "ReportFactory.h"
#include "OrderTypes.h"
#include "OrderListener.h"
#include "StatusListener.h"
#include "ObjectListener.h"
#include "ScopeEditor.h"
#include "ExtensionFactory.h"
#include "dispatcher/Model.h"
#include <xeumeuleu/xml.h>

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: DatabasePublisher constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabasePublisher::DatabasePublisher( const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher, xml::xistream& xis )
    : model_           ( model )
    , workspace_       ( new Workspace() )
    , reportFactory_   ( new ReportFactory( config, model ) )
    , orderTypes_      ( new OrderTypes( config ) )
    , modelLoaded_     ( false )
    , factory_         ( new ExtensionFactory() )
{
    model.RegisterFactory( *factory_ );

    Initialize( "geodatabase", config, xis );
    Initialize( "geodatabase-population", config, xis );
    Initialize( "geodatabase-shared", config, xis );
    
    databaseUpdater_.reset  ( new DatabaseUpdater( *databases_[ "geodatabase" ], model, *reportFactory_ ) );
    folkUpdater_.reset      ( new FolkUpdater( *databases_[ "geodatabase-population" ] ) );

    listeners_.push_back( T_SharedListener( new OrderListener ( *databases_[ "geodatabase-shared" ], model, *orderTypes_, publisher ) ) );
    listeners_.push_back( T_SharedListener( new StatusListener( *databases_[ "geodatabase-shared" ], publisher ) ) );
    listeners_.push_back( T_SharedListener( new ObjectListener( *databases_[ "geodatabase-shared" ], publisher ) ) );
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabasePublisher::~DatabasePublisher()
{
    model_.UnregisterFactory( *factory_ );
     // $$$$ JCR 2008-01-11: why release ?
    // databaseUpdater_.release();
    // folkUpdater_.release();
    reportFactory_.release();
    workspace_.release();
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::Initialize
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabasePublisher::Initialize( const std::string& name, const dispatcher::Config& config, xml::xistream& xis )
{
    databases_[ name ].reset( new Database( config, xml::attribute< std::string >( xis, name ) ) );
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
    case T_MsgsSimToClient_msg_msg_object_update:
    case T_MsgsSimToClient_msg_msg_object_destruction:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_update:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:
    case T_MsgsSimToClient_msg_msg_unit_destruction:
    case T_MsgsSimToClient_msg_msg_report:
    case T_MsgsSimToClient_msg_msg_folk_graph_update:
        return modelLoaded_;
    case T_MsgsSimToClient_msg_msg_control_send_current_state_begin:
    case T_MsgsSimToClient_msg_msg_control_send_current_state_end:
    case T_MsgsSimToClient_msg_msg_object_creation:
    case T_MsgsSimToClient_msg_msg_formation_creation:
    case T_MsgsSimToClient_msg_msg_automat_creation:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:
    case T_MsgsSimToClient_msg_msg_unit_creation:    
    case T_MsgsSimToClient_msg_msg_folk_creation:    
        return true;
    case T_MsgsSimToClient_msg_msg_automat_attributes:
        return asn.msg.u.msg_automat_attributes->m.etat_automatePresent == 1;        
    case T_MsgsSimToClient_msg_msg_unit_attributes:
        const ASN1T_MsgUnitAttributes* attributes = asn.msg.u.msg_unit_attributes;
        if( attributes->m.positionPresent || attributes->m.vitessePresent || attributes->m.etat_operationnelPresent )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::IsRelevant
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
bool DatabasePublisher::IsRelevant( const ASN1T_MsgsMessengerToClient& asn ) const
{
    switch ( asn.t )
    {
    case T_MsgsMessengerToClient_msg_lima_destruction:   
    case T_MsgsMessengerToClient_msg_limit_destruction:
        return modelLoaded_;
    case T_MsgsMessengerToClient_msg_lima_creation:   
    case T_MsgsMessengerToClient_msg_limit_creation:
        break;
    }
    return true;
}


// -----------------------------------------------------------------------------
// Name: DatabasePublisher::Receive
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabasePublisher::Receive( const ASN1T_MsgsSimToClient& asn )
{
    if( !IsRelevant( asn ) )
        return;

    UpdateOnTick( asn );
    UpdateDatabase( asn );
    UpdateFolkDatabase( asn );
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::Receive
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void DatabasePublisher::Receive( const ASN1T_MsgsMessengerToClient& asn )
{
    if( !IsRelevant( asn ) )
        return;

    UpdateDatabase( asn );
}


// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateOnTick
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateOnTick( const ASN1T_MsgsSimToClient& asn )
{
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_control_send_current_state_begin: modelLoaded_ = false; break;
    case T_MsgsSimToClient_msg_msg_control_send_current_state_end:   
        databaseUpdater_->UnLock();
        modelLoaded_ = true;  break;
    case T_MsgsSimToClient_msg_msg_control_begin_tick:         
        MT_LOG_INFO_MSG( "tick " << asn.msg.u.msg_control_begin_tick->current_tick );        
        {
            UpdateListeners();
        }
        databaseUpdater_->Lock();
        break;
    case T_MsgsSimToClient_msg_msg_control_end_tick:
        folkUpdater_->Drop();
        databaseUpdater_->UnLock();
        break;
    }
}
    
// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateDatabase
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateDatabase( const ASN1T_MsgsSimToClient& asn )
{
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_formation_creation:          databaseUpdater_->Update( *asn.msg.u.msg_formation_creation ); break;
    case T_MsgsSimToClient_msg_msg_automat_creation:            databaseUpdater_->Update( *asn.msg.u.msg_automat_creation ); break;
    case T_MsgsSimToClient_msg_msg_automat_attributes:          databaseUpdater_->Update( *asn.msg.u.msg_automat_attributes ); break;

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
    }
}
    
// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateDatabase
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateDatabase( const ASN1T_MsgsMessengerToClient& asn )
{
    switch ( asn.t )
    {
    case T_MsgsMessengerToClient_msg_lima_creation:               databaseUpdater_->Update( *asn.u.msg_lima_creation ); break;
//  case T_MsgsMessengerToClient_msg_lima_update:                 databaseUpdater_->Update( *asn.msg.u.msg_lima_update ); break;
//  case T_MsgsMessengerToClient_msg_lima_destruction:            databaseUpdater_->Update( asn.msg.u.msg_lima_destruction ); break;

    case T_MsgsMessengerToClient_msg_limit_creation:              databaseUpdater_->Update( *asn.u.msg_limit_creation ); break;
//  case T_MsgsMessengerToClient_msg_limit_destruction:           databaseUpdater_->Update( asn.msg.u.msg_lima_destruction ); break;
    }
}


// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateFolkDatabase
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateFolkDatabase( const ASN1T_MsgsSimToClient& asn )
{
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_folk_creation:               folkUpdater_->Update( *asn.msg.u.msg_folk_creation ); break;
    case T_MsgsSimToClient_msg_msg_folk_graph_update:           folkUpdater_->Update( *asn.msg.u.msg_folk_graph_update ); break;
    }
}

namespace 
{
    class LockedScopeEditor
    {
    public: 
        explicit LockedScopeEditor( Database_ABC& database ) 
            : database_ ( database )
        {
            database_.Lock();
        }
        ~LockedScopeEditor()
        {
            database_.UnLock();
        }
    private:
        Database_ABC& database_;
    };
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateListeners
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateListeners()
{
    if( modelLoaded_ )
    {
        // T_SharedDatabase    database( databases_[ "geodatabase-shared" ] );        
        // LockedScopeEditor   lock( *database );
        for( CIT_Listeners it = listeners_.begin(); it != listeners_.end(); ++it )
            (*it)->Listen();
    }
}
