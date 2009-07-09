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
#include "LdapClient.h"
#include "ReportManager.h"
#include "Simulation.h"
#include "dispatcher/Model.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

#include <xeumeuleu/xml.h>

#include "dispatcher/Agent.h"
#include "dispatcher/Object.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Model.h"
#include "dispatcher/Profile_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ClientPublisher_ABC.h"

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
    , clientProfile_ ( 0 )
    , clientPublisher_ ( 0 )
    , publisher_( new Publisher( xis ) ) 
    , simulation_( new Simulation() )
    , ldap_ ( new LdapClient ())
    , reportManager_( new ReportManager( model_, simulationPublisher_, *ldap_ ) )
    , extensionFactory_( new ExtensionFactory( *publisher_, *reportManager_, *simulation_, model_ ) )
    , nCptTick_(0)
{
    model_.RegisterFactory( *extensionFactory_ );
    
    xis >> xml::attribute( "export", bExportActivation_ )
        >> xml::attribute( "import", bImportActivation_ )
        >> xml::attribute( "nbTicks", nTick_ );

    if( bImportActivation_ && false )
    {
        try
        {
            ldap_->LdapConnection();
            ldap_->ReadLdapContent();
        }
        catch( std::exception& e )
        {
        MT_LOG_ERROR_MSG( "Ldap error : " << e.what() );
        }  
    }
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
 
          //HACK de TEST 
          if( true /*type == "SITREP"*/ )
          {
              dispatcher::Profile_ABC* profile = GetClientProfile();
              dispatcher::ClientPublisher_ABC* target = GetClientPublisher();

              if ( profile != 0 && target != 0 )
              {
                  ASN1T_MsgsSimToClient msgToClient;

                  msgToClient.msg.t = T_MsgsSimToClient_msg_msg_xmlia_text_message;
                  ASN1T_MsgXmliaTextMessage report;
                  msgToClient.msg.u.msg_xmlia_text_message = &report;

                  std::string content ("TOTO!");
                  std::string dest ("Supervisor");

                  report.message = content.c_str();
                  report.destinataire = dest.c_str();
                  target->Send( msgToClient );
              }
          }
          //$$ HACK de TEST 

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
    SetClientPublisher( client );
    SetClientProfile( profile );
}

// -----------------------------------------------------------------------------
// Name: XmliaPlugin::NotifyClientLeft
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void XmliaPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& client )
{
    // NOTHING
}



// Hack de test

// -----------------------------------------------------------------------------
// Name: ReportManager::GetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::Profile_ABC* XmliaPlugin::GetClientProfile() const
{
    return clientProfile_;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::SetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void XmliaPlugin::SetClientProfile( dispatcher::Profile_ABC& profile )
{
    clientProfile_ = &profile;
}


// -----------------------------------------------------------------------------
// Name: ReportManager::GetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::ClientPublisher_ABC* XmliaPlugin::GetClientPublisher() const
{
    return clientPublisher_;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::SetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void XmliaPlugin::SetClientPublisher( dispatcher::ClientPublisher_ABC& publisher )
{
    clientPublisher_ = &publisher;
}
