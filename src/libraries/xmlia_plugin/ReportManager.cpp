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
#include "LdapClient.h"

#include "clients_kernel/ObjectType.h"

#include "dispatcher/Agent.h"
#include "dispatcher/Object.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Model.h"
#include "dispatcher/Profile_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ClientPublisher_ABC.h"

#include "xmlia_plugin/Frago.h"
#include "xmlia_plugin/Intrep.h"
#include "xmlia_plugin/Logassessrep.h"
#include "xmlia_plugin/NBCReport.h"
#include "xmlia_plugin/Opord.h"
#include "xmlia_plugin/Sitrep.h"

#include "boost/bind.hpp"
#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: ReportManager constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
ReportManager::ReportManager( dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulationPublisher, LdapClient& Ldap )
: model_( model )
, simulationPublisher_( simulationPublisher )
, ldap_( Ldap )
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
    for( std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itMap = reports_.begin(); itMap != reports_.end(); itMap++ )
    {
      for( std::map< std::string, Report_ABC* >::iterator it = itMap->second.begin(); it != itMap->second.end(); itMap++ )
      {
        delete it->second;
      }
      itMap->second.clear();
    }
    reports_.clear();

    for( std::vector< Report_ABC* >::iterator it = receivedRapports_.begin(); it != receivedRapports_.end(); it++ )
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
  for( std::map< unsigned, std::map< std::string, Report_ABC* > >::const_iterator itMap = reports_.begin(); itMap != reports_.end(); itMap++ )
  {
    for( std::map< std::string, Report_ABC* >::const_iterator it = itMap->second.begin(); it != itMap->second.end(); itMap++ )
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
  std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itAutomat = reports_.find( agent.automat_->GetId() );
  if( itAutomat == reports_.end() )
  {
    itAutomat = reports_.insert( std::pair< unsigned, std::map< std::string, Report_ABC* > >( agent.automat_->GetId(), std::map< std::string, Report_ABC* > () ) ).first;
  }
  std::map< std::string, Report_ABC* >::iterator itReport = itAutomat->second.find( "Sitrep" );
  if( itReport == itAutomat->second.end() )
  {
    itAutomat->second.insert( std::pair< std::string, Sitrep*>( "Sitrep", new  Sitrep( *this, *agent.automat_ ) ) );
  }
  reports_[ agent.automat_->GetId() ]["Sitrep"]->InsertOrUpdateFriendly( agent );
}
// -----------------------------------------------------------------------------
// Name: ReportManager::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::DoUpdate( dispatcher::Agent& agent, dispatcher::Agent& detected )
{
  std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itAutomat = reports_.find( agent.automat_->GetId() );
  if( itAutomat == reports_.end() )
  {
      itAutomat = reports_.insert( std::pair< unsigned, std::map< std::string, Report_ABC* > >( agent.automat_->GetId(), std::map< std::string, Report_ABC* > () ) ).first;
  }
  std::map< std::string, Report_ABC* >::iterator itReport = itAutomat->second.find( "Sitrep" );
  if( itReport == itAutomat->second.end() )
  {
    itAutomat->second.insert( std::pair< std::string, Sitrep*>( "Sitrep", new  Sitrep( *this, *agent.automat_ ) ) );
  }
  reports_[ agent.automat_->GetId() ]["Sitrep"]->InsertOrUpdateEnemy( detected );
}

// -----------------------------------------------------------------------------
// Name: ReportManager::DoUpdate
// Created: MGD 2009-07-06
// -----------------------------------------------------------------------------
void ReportManager::DoUpdate( dispatcher::Agent& agent,  dispatcher::Object& detected )
{
  const kernel::ObjectType& type = detected.GetType();
  bool isNBC = false;
  for( kernel::ObjectType::CIT_Capacities it = type.CapacitiesBegin(); it != type.CapacitiesEnd() && !isNBC; it++ )
  {
    if( it->first == "contamination" )
    {
      isNBC = true;
    }
  }

  if( isNBC )
  {
    std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itAutomat = reports_.find( agent.automat_->GetId() );
    if( itAutomat == reports_.end() )
    {
        itAutomat = reports_.insert( std::pair< unsigned, std::map< std::string, Report_ABC* > >( agent.automat_->GetId(), std::map< std::string, Report_ABC* > () ) ).first;
    }
    std::map< std::string, Report_ABC* >::iterator itReport = itAutomat->second.find( "NBC" );
    if( itReport == itAutomat->second.end() )
    {
      itAutomat->second.insert( std::pair< std::string, NBCReport*>( "NBC", new  NBCReport( *this, *agent.automat_ ) ) );
    }
    reports_[ agent.automat_->GetId() ]["NBC"]->InsertOrUpdateNBC( detected );
  }
}
// -----------------------------------------------------------------------------
// Name: ReportManager::DoUpdate
// Created: MGD 2009-07-06
// -----------------------------------------------------------------------------
void ReportManager::DoUpdate( dispatcher::Agent& agent, kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2 )
{
  std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itAutomat = reports_.find( agent.automat_->GetId() );
  if( itAutomat == reports_.end() )
  {
      itAutomat = reports_.insert( std::pair< unsigned, std::map< std::string, Report_ABC* > >( agent.automat_->GetId(), std::map< std::string, Report_ABC* > () ) ).first;
  }
  std::map< std::string, Report_ABC* >::iterator itReport = itAutomat->second.find( "Frago" );
  if( itReport == itAutomat->second.end() )
  {
    itAutomat->second.insert( std::pair< std::string, Frago*>( "Frago", new  Frago( *this, *agent.automat_ ) ) );
  }

  reports_[ agent.automat_->GetId() ]["Frago"]->UpdateMission( mission, limit1, limit2  );
}
 // -----------------------------------------------------------------------------
 // Name: ReportManager::DoUpdate
 // Created: MGD 2009-07-06
 // -----------------------------------------------------------------------------
 void ReportManager::DoUpdate( dispatcher::Automat& agent, kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2 )
 {
     std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itAutomat = reports_.find( agent.GetId() );
     if( itAutomat == reports_.end() )
     {
         itAutomat = reports_.insert( std::pair< unsigned, std::map< std::string, Report_ABC* > >() ).first;
     }
     std::map< std::string, Report_ABC* >::iterator itReport = itAutomat->second.find( "Opord" );
     if( itReport == itAutomat->second.end() )
     {
         itAutomat->second.insert( std::pair< std::string, Opord*>( "Opord", new  Opord( *this, agent ) ) );
     }

     reports_[ agent.GetId() ]["Opord"]->UpdateMission( mission, limit1, limit2 );
 }

// -----------------------------------------------------------------------------
// Name: ReportManager::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::DoUpdateIntrep( dispatcher::Agent& agent,  dispatcher::Agent& fired )
{
  std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itAutomat = reports_.find( agent.automat_->GetId() );
  if( itAutomat == reports_.end() )
  {
      itAutomat = reports_.insert( std::pair< unsigned, std::map< std::string, Report_ABC* > >( agent.automat_->GetId(), std::map< std::string, Report_ABC* > () ) ).first;
  }
  std::map< std::string, Report_ABC* >::iterator itReport = itAutomat->second.find( "Intrep" );
  if( itReport == itAutomat->second.end() )
  {
    itAutomat->second.insert( std::pair< std::string, Sitrep*>( "Sitrep", new  Sitrep( *this, *agent.automat_ ) ) );
  }
  reports_[ agent.automat_->GetId() ]["Intrep"]->InsertOrUpdateEnemy( fired );
}

// -----------------------------------------------------------------------------
// Name: ReportManager::DoUpdateLog
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::DoUpdateLog( dispatcher::Agent& agent )
{
  std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itAutomat = reports_.find( agent.automat_->GetId() );
  if( itAutomat == reports_.end() )
  {
      itAutomat = reports_.insert( std::pair< unsigned, std::map< std::string, Report_ABC* > >( agent.automat_->GetId(), std::map< std::string, Report_ABC* > () ) ).first;
  }
  std::map< std::string, Report_ABC* >::iterator itReport = itAutomat->second.find( "Logassessrep" );
  if( itReport == itAutomat->second.end() )
  {
    itAutomat->second.insert( std::pair< std::string, Logassessrep*>( "Logassessrep", new  Logassessrep( *this, *agent.automat_ ) ) );
  }
  reports_[ agent.automat_->GetId() ]["Logassessrep"]->InsertOrUpdateEnemy( agent );
}
// -----------------------------------------------------------------------------
// Name: ReportManager::Read
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::Read( xml::xistream& xis )
{
  Report_ABC* newReport = 0;

  if( xis.has_child( "Frago") )
  {
    xis >> xml::start( "Frago");
    newReport = new Frago( *this, xis );
    xis >> xml::end();
  }
  else if( xis.has_child( "Intrep") )
  {
    xis >> xml::start( "Intrep");
    newReport = new Intrep( *this, xis ); 
    xis >> xml::end();
  }
  else if( xis.has_child( "Logassessrep") )
  {
    xis >> xml::start( "Logassessrep");
    newReport = new Logassessrep( *this, xis ); 
    xis >> xml::end();
  }
  else if( xis.has_child( "NBC") )
  {
    xis >> xml::start( "NBC");
    newReport = new NBCReport( *this, xis );
    xis >> xml::end();
  }
  else if( xis.has_child( "Opord") )
  {
    xis >> xml::start( "Opord");
    newReport = new Opord( *this, xis );
    xis >> xml::end();
  }
  else if( xis.has_child( "Sitrep") )
  {
    xis >> xml::start( "Sitrep");
    newReport = new Sitrep( *this, xis );
    xis >> xml::end();
  }
  
  receivedRapports_.push_back( newReport );
}


// -----------------------------------------------------------------------------
// Name: ReportManager::UpdateSimulation
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::UpdateSimulation() const
{
  for( std::map< unsigned, std::map< std::string, Report_ABC* > >::const_iterator itMap = reports_.begin(); itMap != reports_.end(); itMap++ )
  {
    for( std::map< std::string, Report_ABC* >::const_iterator it = itMap->second.begin(); it != itMap->second.end(); itMap++ )
    {
      it->second->UpdateSimulation();
    }
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
  for( std::vector< Report_ABC* >::iterator it = receivedRapports_.begin(); it != receivedRapports_.end(); it++ )
  {
    delete (*it);
  }
  receivedRapports_.clear();
}


// -----------------------------------------------------------------------------
// Name: ReportManager::IsTakenIntoAccount
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
bool ReportManager::IsTakenIntoAccount( const std::string messageType )
{
    return ldap_.IsTakenIntoAccount( messageType );
}