// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "ReportManager.h"
#include "Publisher_ABC.h"

#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Model.h"
#include "dispatcher/Profile_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ClientPublisher_ABC.h"

#include "xmlia_plugin/Sitrep.h"

#include "boost/bind.hpp"
#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: ReportManager constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
ReportManager::ReportManager( dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulationPublisher )
: model_( model )
, simulationPublisher_( simulationPublisher )
, clientProfile_( 0 )
, clientPublisher_( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ReportManager destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
ReportManager::~ReportManager()
{
    for( std::map< unsigned, Sitrep* >::iterator it = reports_.begin(); it != reports_.end(); it++ )
    {
      delete it->second;
    }
    reports_.clear();
    for( std::vector< Sitrep* >::iterator it = receivedRapports_.begin(); it != receivedRapports_.end(); it++ )
    {
      delete (*it);
    }
    receivedRapports_.clear();
}

// -----------------------------------------------------------------------------
// Name: ReportManager Send
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::Send( Publisher_ABC& publisher)const
{
    for( std::map< unsigned, Sitrep* >::const_iterator it = reports_.begin(); it != reports_.end(); it++ )
    {
        xml::xostringstream xos;
        it->second->Serialize( xos );
        std::string xmliaMessage = xos.str();
        std::string urlId = publisher.GetUrlId();

        unsigned int index = urlId.find( '\r' );
        unsigned lengh = index - 11;
        std::string strPoe =  urlId.substr(11, lengh );
        publisher.PushReport( xmliaMessage, strPoe );
   
    }
}

// -----------------------------------------------------------------------------
// Name: ReportManager Receive
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::Receive( Publisher_ABC& publisher)
{
    const std::string UrlReportFromWebService = publisher.GetUrlReports();
    
    xml::xistringstream streamContent ( UrlReportFromWebService );
    streamContent   >> xml::start( "html" )
                    >> xml::start( "body" )
                    >> xml::start( "ul" )
                    >> xml::list( "li", *this, &ReportManager::ReadUrl, publisher )
                    >> xml::end()
                    >> xml::end()
                    >> xml::end();

}

// -----------------------------------------------------------------------------
// Name: ReportManager::ReadUrl
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::ReadUrl( xml::xistream& xis, Publisher_ABC& publisher )
{
    std::string url;
    xis >> xml::start( "a" )
        >> xml::attribute( "href", url );

    const std::string message = publisher.GetXmliaMessage( url );

    if( true /*type == "SITREP"*/ )
    {
      dispatcher::Profile_ABC* profile = GetClientProfile();
      dispatcher::ClientPublisher_ABC* target = GetClientPublisher();

      if ( profile != 0 && target != 0 )
      {
        ASN1T_MsgsMessengerToClient answer;
        answer.t = T_MsgsMessengerToClient_msg_text_message;
        ASN1T_MsgTextMessage asnMessage;
        answer.u.msg_text_message = &asnMessage;

        asnMessage.source.profile = ""; //profile->GetName().c_str();
        asnMessage.target.profile = ""; //profile->GetName().c_str();
        std::string xmlContent;
        std::string strTmp (message);
        xmlContent = strTmp.substr(0, 64) + " [...]";
        xmlContent = "\nXMLIA content: "+ xmlContent;
        strTmp =	"SITREP received from unit: ";
        strTmp += xmlContent;
        asnMessage.message = strTmp.c_str();
        target->Send( answer );
      }
    }

    xml::xistringstream streamXmlia( message );
    Read( streamXmlia );

}

// -----------------------------------------------------------------------------
// Name: ReportManager::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::DoUpdate( dispatcher::Agent& agent )
{
  std::map< unsigned, Sitrep* >::iterator itFind = reports_.find( agent.automat_->GetId() );
  if( itFind == reports_.end() )
  {
    reports_.insert( std::pair< unsigned, Sitrep*>( agent.automat_->GetId(), new  Sitrep( *this, *agent.automat_ ) ) );
  }
  reports_[ agent.automat_->GetId() ]->InsertOrUpdate( agent );
}
// -----------------------------------------------------------------------------
// Name: ReportManager::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::DoUpdate( dispatcher::Agent& agent, dispatcher::Agent& detected )
{
  std::map< unsigned, Sitrep* >::iterator itFind = reports_.find( agent.automat_->GetId() );
  if( itFind == reports_.end() )
  {
    reports_.insert( std::pair< unsigned, Sitrep*>( agent.automat_->GetId(), new  Sitrep( *this, *agent.automat_ ) ) );
  }
  reports_[ agent.automat_->GetId() ]->InsertOrUpdate( detected );
}

// -----------------------------------------------------------------------------
// Name: ReportManager::Read
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::Read( xml::xistream& xis )
{
  std::string type;
  std::string sQnameRapport;
  xis   >> xml::start( "mpia:MPIA_Message" )
        >> xml::start( "mpia:Header" )
        >> xml::content( "mpia:Name", type )
        >> xml::end()
        >> xml::start( "mpia:Entities" );

  if( type == "SITREP" )
  {
    Sitrep* newSitrep = new Sitrep( *this , xis );
    newSitrep->ReadEntities( xis );
    receivedRapports_.push_back( newSitrep );
  }
}


// -----------------------------------------------------------------------------
// Name: ReportManager::UpdateSimulation
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::UpdateSimulation() const
{
    //@TODO change to receivedRapports_
  for( std::map< unsigned, Sitrep* >::const_iterator it = reports_.begin(); it != reports_.end(); it++ )
  {
    it->second->UpdateSimulation();
  }
}

// -----------------------------------------------------------------------------
// Name: ReportManager::GetModel
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::Model& ReportManager::GetModel() const
{
  return model_;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::CleanReceivedRapport
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Name: ReportManager::GetSimulationPublisher
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::SimulationPublisher_ABC& ReportManager::GetSimulationPublisher() const
{
  return simulationPublisher_;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::GetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::Profile_ABC* ReportManager::GetClientProfile() const
{
  return clientProfile_;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::SetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::SetClientProfile( dispatcher::Profile_ABC& profile )
{
  clientProfile_ = &profile;
}


// -----------------------------------------------------------------------------
// Name: ReportManager::GetClientPublisher
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::ClientPublisher_ABC* ReportManager::GetClientPublisher() const
{
    return clientPublisher_;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::SetClientPublisher
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::SetClientPublisher( dispatcher::ClientPublisher_ABC& publisher )
{
    clientPublisher_ = &publisher;
}


// -----------------------------------------------------------------------------
// Name: ReportManager::CleanReceivedRapport
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------

void ReportManager::CleanReceivedRapport()
{
  for( std::vector< Sitrep* >::iterator it = receivedRapports_.begin(); it != receivedRapports_.end(); it++ )
  {
    delete (*it);
  }
  receivedRapports_.clear();
}
