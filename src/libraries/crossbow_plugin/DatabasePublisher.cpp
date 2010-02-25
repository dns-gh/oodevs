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
#include "DatabaseFactory.h"
#include "Database_ABC.h"
#include "WorkingSession.h"
#include "QueryDatabaseUpdater.h"
#include "FolkUpdater.h"
#include "ReportFactory.h"
#include "OrderTypes.h"
#include "OrderListener.h"
#include "StatusListener.h"
#include "ObjectListener.h"
#include "ExtensionFactory.h"
#include "dispatcher/Model.h"
#include "dispatcher/Config.h"
#include "protocol/simulationsenders.h"

using namespace plugins;
using namespace plugins::crossbow;

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

    // retrieve exercise and session name
    // init databases
    Initialize( "geodatabase", config, xis );
    Initialize( "geodatabase-population", config, xis );
    databases_[ "geodatabase-shared" ] = databases_[ "geodatabase" ];
    
    session_.reset( new WorkingSession( config, *databases_[ "geodatabase" ] ) );

    databaseUpdater_.reset( new QueryDatabaseUpdater( *databases_[ "geodatabase" ], model, *reportFactory_, *session_ ) );
    databaseUpdater_->Clean();

    folkUpdater_.reset( new FolkUpdater( *databases_[ "geodatabase-population" ], *session_ ) );

    // activate listeners
    listeners_.push_back( T_SharedListener( new OrderListener( *databases_[ "geodatabase-shared" ], model, *orderTypes_, publisher, *session_ ) ) );
    // listeners_.push_back( T_SharedListener( new StatusListener( *databases_[ "geodatabase-shared" ], publisher ) ) );
    listeners_.push_back( T_SharedListener( new ObjectListener( *databases_[ "geodatabase-shared" ], publisher, *session_ ) ) );
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
    DatabaseFactory factory;
    databases_[ name ].reset( &factory.Create( config, xml::attribute< std::string >( xis, name ) ) );
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::IsRelevant
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
bool DatabasePublisher::IsRelevant( const MsgsSimToClient::MsgSimToClient& wrapper ) const
{
    if( wrapper.message().has_control_begin_tick() ||
        wrapper.message().has_control_end_tick() ||        
        wrapper.message().has_object_update() ||
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
        return wrapper.message().automat_attributes().has_etat_automate();        
    if (wrapper.message().has_unit_attributes() )
    {
        const MsgsSimToClient::MsgUnitAttributes& attributes = wrapper.message().unit_attributes();
        if( attributes.has_position() || attributes.has_vitesse() || attributes.has_etat_operationnel() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::IsRelevant
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
bool DatabasePublisher::IsRelevant( const MsgsMessengerToClient::MsgMessengerToClient& wrapper ) const
{
    if (wrapper.message().has_lima_destruction() ||   
        wrapper.message().has_limit_destruction() )
        return modelLoaded_;
    if (wrapper.message().has_lima_creation() ||   
        wrapper.message().has_limit_creation())
        return true;
    return true;
}


// -----------------------------------------------------------------------------
// Name: DatabasePublisher::Receive
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabasePublisher::Receive( const MsgsSimToClient::MsgSimToClient& message )
{
    if( !IsRelevant( message ) )
        return;

    UpdateOnTick( message );
    UpdateDatabase( message );
    UpdateFolkDatabase( message );
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::Receive
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void DatabasePublisher::Receive( const MsgsMessengerToClient::MsgMessengerToClient& message )
{
    if( !IsRelevant( message ) )
        return;

    UpdateDatabase( message );
}


// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateOnTick
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateOnTick( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_control_send_current_state_begin())
        modelLoaded_ = false; 
    if( wrapper.message().has_control_send_current_state_end() )   
    {
        databaseUpdater_->Flush();
        modelLoaded_ = true;  
    }
    if( wrapper.message().has_control_begin_tick() )
    {
        MT_LOG_INFO_MSG( "tick " << wrapper.message().control_begin_tick().current_tick() );        
        {
            UpdateListeners();
        }
//        databaseUpdater_->Lock();
    }
    if( wrapper.message().has_control_end_tick() )
    {
        // folkUpdater_->Drop();
        databaseUpdater_->Flush();
        
    }
}
    
// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateDatabase
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateDatabase( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_formation_creation() )
        databaseUpdater_->Update( wrapper.message().formation_creation() ); 
    if( wrapper.message().has_automat_creation() )
        databaseUpdater_->Update( wrapper.message().automat_creation() ); 
    if( wrapper.message().has_automat_attributes() )
        databaseUpdater_->Update( wrapper.message().automat_attributes() ); 

    if( wrapper.message().has_unit_creation() )
        databaseUpdater_->Update( wrapper.message().unit_creation() ); 
    if( wrapper.message().has_unit_attributes() )
        databaseUpdater_->Update( wrapper.message().unit_attributes() ); 
    if( wrapper.message().has_unit_destruction() )
        databaseUpdater_->DestroyUnit( wrapper.message().unit_destruction() ); 

    if( wrapper.message().has_unit_knowledge_creation() )
        databaseUpdater_->Update( wrapper.message().unit_knowledge_creation() ); 
    if( wrapper.message().has_unit_knowledge_update() )
        databaseUpdater_->Update( wrapper.message().unit_knowledge_update() ); 
    if( wrapper.message().has_unit_knowledge_destruction() )
        databaseUpdater_->DestroyUnitKnowledge( wrapper.message().unit_knowledge_destruction() );  

    if( wrapper.message().has_object_knowledge_creation() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_creation() ); 
    if( wrapper.message().has_object_knowledge_update() )
        databaseUpdater_->Update( wrapper.message().object_knowledge_update() ); 
    if( wrapper.message().has_object_knowledge_destruction() )
        databaseUpdater_->DestroyObjectKnowledge( wrapper.message().object_knowledge_destruction() );  

    if( wrapper.message().has_object_creation() )
        databaseUpdater_->Update( wrapper.message().object_creation() ); 
//    if( wrapper.message().has_object_update() )
//               databaseUpdater_->Update( *wrapper.message().object_update ); 
    if( wrapper.message().has_object_destruction() )
        databaseUpdater_->DestroyObject( wrapper.message().object_destruction() ); 
    if( wrapper.message().has_report() )
        databaseUpdater_->Update( wrapper.message().report() ); 
}
    
// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateDatabase
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateDatabase( const MsgsMessengerToClient::MsgMessengerToClient& wrapper )
{
    if (wrapper.message().has_lima_creation() )
        databaseUpdater_->Update( wrapper.message().lima_creation() ); 
//  wrapper.message().has_lima_update() ||                 databaseUpdater_->Update( *wrapper.message().lima_update ); break;
//  wrapper.message().has_lima_destruction() ||            databaseUpdater_->Update( wrapper.message().lima_destruction ); break;

    if (wrapper.message().has_limit_creation() )
        databaseUpdater_->Update( wrapper.message().limit_creation() );
//  case T_MsgsMessengerToClient_limit_destruction:           databaseUpdater_->Update( wrapper.message().lima_destruction ); break;

}


// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateFolkDatabase
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateFolkDatabase( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_folk_creation() )
        folkUpdater_->Update( wrapper.message().folk_creation() ); 
    if( wrapper.message().has_folk_graph_update() )
        folkUpdater_->Update( wrapper.message().folk_graph_update() ); 
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::UpdateListeners
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabasePublisher::UpdateListeners()
{
    if( modelLoaded_ )
    {
        for( CIT_Listeners it = listeners_.begin(); it != listeners_.end(); ++it )
            (*it)->Listen();
    }
}
