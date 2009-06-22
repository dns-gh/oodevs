// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "xmliaPlugin.h"
#include "ExtensionFactory.h"
#include "Publisher.h"
#include "ReportManager.h"
#include "Simulation.h"
#include "dispatcher/Model.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: XmliaPlugin constructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
XmliaPlugin::XmliaPlugin( dispatcher::Model& model,
                          xml::xistream& xis,
                          dispatcher::SimulationPublisher_ABC& simulationPublisher )
    : model_( model )
	, simulationPublisher_ ( simulationPublisher )
    , publisher_( new Publisher( xis ) ) 
    , simulation_( new Simulation() )
    , reportManager_( new ReportManager( model_, simulationPublisher_ ) )
    , extensionFactory_( new ExtensionFactory( *publisher_, *reportManager_, *simulation_, model_ ) )
    , nCptTick_(0)
{
    model_.RegisterFactory( *extensionFactory_ );
    
    xis >> xml::attribute( "export", bExportActivation_ )
        >> xml::attribute( "import", bImportActivation_ )
        >> xml::attribute( "nbTicks", nTick_ );
  
}

// -----------------------------------------------------------------------------
// Name: XmliaPlugin destructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
XmliaPlugin::~XmliaPlugin()
{
    model_.UnregisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: XmliaPlugin::Receive
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void XmliaPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_end_tick )
    {
        if( nCptTick_ == 0 )
        {
         
          simulation_->Update( *message.msg.u.msg_control_end_tick );//@NOTE, keep could be usefull for other message

          if( bExportActivation_ )
          {
            try
            {
              reportManager_->Send( *publisher_ );
            }
            catch( std::exception& e )
            {
              MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
            }  
          }

          if( bImportActivation_ )
          {
            try
            {
              reportManager_->CleanReceivedRapport();
              reportManager_->Receive(*publisher_);
              reportManager_->UpdateSimulation();
            }
            catch( std::exception& e )
            {
              MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
            }  
          }

          
        }
        nCptTick_ = ( nCptTick_ + 1 ) % nTick_ ;
    }
}

// -----------------------------------------------------------------------------
// Name: xmliaPlugin::NotifyClientAuthenticated
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void XmliaPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile )
{
    reportManager_->SetClientPublisher( client );
    reportManager_->SetClientProfile( profile );
}

// -----------------------------------------------------------------------------
// Name: XmliaPlugin::NotifyClientLeft
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void XmliaPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& client )
{
    // NOTHING
}
