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
#include "ClientManager.h"

#include "clients_kernel/ObjectType.h"

#include "dispatcher/Agent.h"
#include "dispatcher/Side.h"
#include "dispatcher/Object.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Formation.h"
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
#include "xmlia_plugin/Point.h"
#include "xmlia_plugin/UnitAgent.h"
#include "xmlia_plugin/UnitAutomate.h"
#include "xmlia_plugin/XmliaOperationalState.h"
#include "xmlia_plugin/Mission.h"

#include "boost/bind.hpp"
#include <xeumeuleu/xml.h>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using namespace plugins::xmlia;

namespace bpt = boost::posix_time;

namespace
{
  std::string CurrentTime()
  {
    std::string timeTemp = bpt::to_iso_extended_string( bpt::second_clock::local_time() );
    unsigned int separator = timeTemp.find( 'T' );
    std::string hour = timeTemp.substr( separator+1 );
    std::string date = timeTemp.substr ( 0, separator );
    unsigned int indexFirstTiret = timeTemp.find( '-' );
    std::string month = date.substr( indexFirstTiret+1, 2 ); 
    std::string day = timeTemp.substr( 8, 2 );
    std::string year = timeTemp.substr( 0, indexFirstTiret );
    return day+'/' + month + '/' + year + "%20" + hour;
  }
}

// -----------------------------------------------------------------------------
// Name: ReportManager constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
ReportManager::ReportManager( dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulationPublisher, LdapClient& Ldap, ClientManager& clientManager )
: model_( model )
, simulationPublisher_( simulationPublisher )
, ldap_( Ldap )
, clientManager_ ( clientManager )
{

  managedReports_["Sitrep"] = CurrentTime();
  managedReports_["Logassessrep"] = CurrentTime();
  managedReports_["Opord"] = CurrentTime();
  managedReports_["Frago"] = CurrentTime();
  managedReports_["Intrep"] = CurrentTime();
  managedReports_["NBC"] = CurrentTime();
}


// -----------------------------------------------------------------------------
// Name: ReportManager destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
ReportManager::~ReportManager()
{
    for( std::map< unsigned, std::map< std::string, Report_ABC* > >::iterator itMap = reports_.begin(); itMap != reports_.end(); itMap++ )
    {
      for( std::map< std::string, Report_ABC* >::iterator it = itMap->second.begin(); it != itMap->second.end(); it++ )
      {
        delete it->second;
      }
      itMap->second.clear();
    }
    reports_.clear();

    for( std::vector< Report_ABC* >::iterator it = receivedReports_.begin(); it != receivedReports_.end(); it++ )
    {
      delete (*it);
    }
    receivedReports_.clear();
}

// -----------------------------------------------------------------------------
// Name: ReportManager Send
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::Send( Publisher_ABC& publisher )const
{
  for( std::map< unsigned, std::map< std::string, Report_ABC* > >::const_iterator itMap = reports_.begin(); itMap != reports_.end(); itMap++ )
  {
    for( std::map< std::string, Report_ABC* >::const_iterator it = itMap->second.begin(); it != itMap->second.end(); it++ )
    {
      xml::xostringstream xos;
      it->second->Serialize( xos );
      std::string xmliaMessage = xos.str();
      xmliaMessage = xmliaMessage.substr(56,xmliaMessage.length());
      std::string urlId = publisher.GetUrlId( it->second->GetType() );

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
  for( std::map< std::string, std::string >::iterator it = managedReports_.begin(); it != managedReports_.end(); it++ )
  {
    std::string timeTemp = CurrentTime();
    try
    {
      const std::string UrlReportFromWebService = publisher.GetUrlReports( it->first, it->second );
      xml::xistringstream streamContent ( UrlReportFromWebService );
      streamContent   >> xml::start( "html" )
        >> xml::start( "body" )
        >> xml::start( "ul" )
        >> xml::list( "li", *this, &ReportManager::ReadUrl, publisher )
        >> xml::end()
        >> xml::end()
        >> xml::end();
      it->second = timeTemp;
    }
    catch (std::exception& e)
    {
      //NOTHING
    }   
  }

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

    std::string message = publisher.GetXmliaMessage( url );

    Read( message );

    xis >> xml::end();
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

  itReport = itAutomat->second.find( "Logassessrep" );
  if( itReport == itAutomat->second.end() )
  {
    itAutomat->second.insert( std::pair< std::string, Logassessrep*>( "Logassessrep", new  Logassessrep( *this, *agent.automat_ ) ) );
  }
  reports_[ agent.automat_->GetId() ]["Logassessrep"]->InsertOrUpdateFriendly( agent );
}
// -----------------------------------------------------------------------------
// Name: ReportManager::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::DoUpdate( dispatcher::Agent& agent, dispatcher::Agent& detected )
{
  if( agent.automat_->team_.GetId() == detected.automat_->team_.GetId() )
      return;

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
  const std::string& type = detected.strName_;

  if( type == "rota area" || type == "nbc zone" || type == "nbc cloud" )
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
void ReportManager::DoUpdate( dispatcher::Agent& agent, const ASN1T_MsgUnitOrder& message )
{
  if( message.parametres.n < 4 
    || message.parametres.elem[2].value.t != T_MissionParameter_value_line 
    || message.parametres.elem[3].value.t != T_MissionParameter_value_line )
    return;

  std::vector< Point > limit1, limit2;

  if( !ReadLine( *message.parametres.elem[2].value.u.line, limit1 ) )
      return;
  if( !ReadLine( *message.parametres.elem[3].value.u.line, limit2 ) )
      return;

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

  reports_[ agent.automat_->GetId() ]["Frago"]->UpdateMission( agent, model_.GetMissionTypes().Get( message.mission ), limit1, limit2 );
}
 // -----------------------------------------------------------------------------
 // Name: ReportManager::DoUpdate
 // Created: MGD 2009-07-06
 // -----------------------------------------------------------------------------
 void ReportManager::DoUpdate( dispatcher::Automat& agent, const ASN1T_MsgAutomatOrder& message )
 {
     if( message.parametres.n < 4 
         || message.parametres.elem[2].value.t != T_MissionParameter_value_line 
         || message.parametres.elem[3].value.t != T_MissionParameter_value_line )
         return;

     std::vector< Point > limit1, limit2;

     if( !ReadLine( *message.parametres.elem[2].value.u.line, limit1 ) )
         return;
     if( !ReadLine( *message.parametres.elem[3].value.u.line, limit2 ) )
        return;

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

     reports_[ agent.GetId() ]["Opord"]->UpdateMission( model_.GetMissionTypes().Get( message.mission ), limit1, limit2 );
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
    itAutomat->second.insert( std::pair< std::string, Intrep*>( "Intrep", new  Intrep( *this, *agent.automat_ ) ) );
  }
  reports_[ agent.automat_->GetId() ]["Intrep"]->InsertOrUpdateEnemy( fired );
}

// -----------------------------------------------------------------------------
// Name: ReportManager::Read
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::Read( std::string& message )
{
  xml::xistringstream xis( message );
  Report_ABC* newReport = 0;
  xis >> xml::start( "MPIA_Message" );
  if( xis.has_child( "Frago") && IsTakenIntoAccount( "Frago" ) )
  {
    xis >> xml::start( "Frago");
    newReport = new Frago( *this, xis );
    xis >> xml::end();
  }
  else if( xis.has_child( "Intrep" ) && IsTakenIntoAccount( "Intrep") )
  {
    xis >> xml::start( "Intrep");
    newReport = new Intrep( *this, xis ); 
    xis >> xml::end();
  }
  else if( xis.has_child( "Logassessrep" ) && IsTakenIntoAccount( "Logassessrep") )
  {
    xis >> xml::start( "Logassessrep");
    newReport = new Logassessrep( *this, xis ); 
    xis >> xml::end();
  }
  else if( xis.has_child( "NBC" ) && IsTakenIntoAccount( "NBC") )
  {
    xis >> xml::start( "NBC");
    newReport = new NBCReport( *this, xis );
    xis >> xml::end();
  }
  else if( xis.has_child( "Opord" ) && IsTakenIntoAccount( "Opord") )
  {
    xis >> xml::start( "Opord");
    newReport = new Opord( *this, xis );
    xis >> xml::end();
  }
  else if( xis.has_child( "Sitrep" ) && IsTakenIntoAccount( "Sitrep") )
  {
    xis >> xml::start( "Sitrep");
    newReport = new Sitrep( *this, xis );
    xis >> xml::end();
  }
  xis >> xml::end();
  if ( newReport )
  {
    receivedReports_.push_back( newReport );

    ASN1T_MsgsPluginToClient msgToClient;
    msgToClient.msg.t = T_MsgsPluginToClient_msg_plugin_text_message;
    ASN1T_MsgPluginTextMessage asnMessage;
    msgToClient.msg.u.plugin_text_message = &asnMessage;
    int temp = strlen( message.c_str() );
    if ( temp < 11000 )//hack de test
    {
      std::string command = "xmlia '" + message + "'";
      asnMessage.message = command.c_str();
      asnMessage.destinataire = "supervisor";

      SentToAllClients( msgToClient ); 
    }
    
  }
}


// -----------------------------------------------------------------------------
// Name: ReportManager::UpdateSimulation
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::UpdateSimulation() const
{
  for( std::vector< Report_ABC* >::const_iterator it = receivedReports_.begin(); it != receivedReports_.end(); it++ )
  {
      (*it)->UpdateSimulation();
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
// Name: ReportManager::GetSimulationPublisher
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::SimulationPublisher_ABC& ReportManager::GetSimulationPublisher() const
{
  return simulationPublisher_;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::CleanReceivedRapport
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::CleanReceivedRapport()
{
  for( std::vector< Report_ABC* >::iterator it = receivedReports_.begin(); it != receivedReports_.end(); it++ )
  {
    delete (*it);
  }
  receivedReports_.clear();
}

// -----------------------------------------------------------------------------
// Name: ReportManager::IsTakenIntoAccount
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
bool ReportManager::IsTakenIntoAccount( const std::string messageType )
{
    return clientManager_.IsTakenIntoAccount( messageType );
}

// -----------------------------------------------------------------------------
// Name: ReportManager::GetClientPublisher
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::ClientPublisher_ABC& ReportManager::GetClientPublisher(  const std::string& name ) const
{
    return *clientManager_.GetClientPublisher();
}

// -----------------------------------------------------------------------------
// Name: ReportManager::ReadLine
// Created: MGD 2009-07-09
// -----------------------------------------------------------------------------
bool ReportManager::ReadLine( const ASN1T_Line& asn, std::vector< Point >& points )
{
    if( asn.type != EnumLocationType::line || asn.coordinates.n < 2 )
        return false;

    points.clear();   
    points.reserve( asn.coordinates.n );
    for( unsigned int i = 0; i < asn.coordinates.n; ++i )
    {
        points.push_back( Point( asn.coordinates.elem[i].latitude, asn.coordinates.elem[i].longitude ) );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::AddIntelligence
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void ReportManager::AddIntelligence( const unsigned intelligenceID, std::string name )
{
    intelligences_.insert( std::pair<unsigned, std::string>( intelligenceID, name ) );
}

// -----------------------------------------------------------------------------
// Name: ReportManager::FindIntelligence
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
bool ReportManager::FindIntelligence( unsigned intelligenceID )
{
    std::map< unsigned, std::string >::iterator itIntelligence = intelligences_.find( intelligenceID );
    if( itIntelligence == intelligences_.end() )
        return false;
    return true;
}


// -----------------------------------------------------------------------------
// Name: ReportManager::MagicMove
// Created: MGD 2009-07-12
// -----------------------------------------------------------------------------
void ReportManager::MagicMove( UnitAgent& reportedUnit )
{
  dispatcher::Agent* simAgent = model_.agents_.Find( reportedUnit.GetId() );

  if ( simAgent != 0 )
  {
    simulation::UnitMagicAction asnMsg;
    asnMsg().oid      = reportedUnit.GetId();
    asnMsg().action.t = T_MsgUnitMagicAction_action_move_to;

    ASN1T_CoordLatLong utm;
    reportedUnit.GetLocalization()->FillLatLong( utm );
    asnMsg().action.u.move_to = &utm;
    if ( simAgent->position_.latitude != utm.latitude && simAgent->position_.longitude != utm.longitude )
        asnMsg.Send( simulationPublisher_ );
  }
}

// -----------------------------------------------------------------------------
// Name: ReportManager::UpdateOperationnalState
// Created: MGD 2009-07-12
// -----------------------------------------------------------------------------
void ReportManager::UpdateOperationnalState( UnitAgent& reportedUnit )
{
  dispatcher::Agent* simAgent = model_.agents_.Find( reportedUnit.GetId() );

  if ( simAgent != 0 )
  {
    if ( reportedUnit.GetOperationalState()->GetGeneralOperationalState() == "NOP" )
    {
      simulation::UnitMagicAction asnMsg;
      ASN1T_MagicActionPartialRecovery asnPartialRecovery;
      asnMsg().action.t = T_MsgUnitMagicAction_action_recompletement_partiel;
      asnMsg().oid      = reportedUnit.GetId();
      asnMsg().action.u.recompletement_partiel = &asnPartialRecovery;
      asnPartialRecovery.m.equipementsPresent = 0;//@TODO REFACTOR UPGRADE
      asnPartialRecovery.m.personnelsPresent = 0;
      asnPartialRecovery.m.dotationsPresent = 0;
      asnPartialRecovery.m.munitionsPresent = 0;
      asnPartialRecovery.m.stocksPresent = 0;
      asnMsg.Send( simulationPublisher_ );
    }
  }
}

// -----------------------------------------------------------------------------
// Name: ReportManager::UpdateOperationnalState
// Created: MGD 2009-07-12
// -----------------------------------------------------------------------------
void ReportManager::InsertOrUpdateIntelligence( UnitAgent& reportedUnit )
{
  /*dispatcher::Agent* simAgent = model_.agents_.Find( reportedUnit.GetId() );

  if ( simAgent != 0 )
  {
    ASN1T_MsgsPluginToClient asnMsg;
    ASN1T_MsgIntelligenceCreation creation;
    ASN1T_MsgIntelligenceUpdate update;
    ASN1T_CoordLatLong utm;
    reportedUnit.GetLocalization()->FillLatLong( utm );
    if ( FindIntelligence( reportedUnit.GetId() ) )
    {
      asnMsg.msg.t              = T_MsgsPluginToClient_msg_plugin_intelligence_update;
      asnMsg.msg.u.plugin_intelligence_update = &update;
      update.oid = reportedUnit.GetId();
      update.name = reportedUnit.GetName().c_str();
      update.level = ASN1T_EnumNatureLevel::ooo;
      update.diplomacy = ASN1T_EnumDiplomacy::ennemi ;
      update.nature = "symbols/shgpuci";
      update.formation = simAgent->automat_->parentFormation_->GetId();
      update.embarked = true;
      update.location = utm;
    }
    else
    {
      asnMsg.msg.t = T_MsgsPluginToClient_msg_plugin_intelligence_creation;
      creation.oid = reportedUnit.GetId();
      asnMsg.msg.u.plugin_intelligence_creation = &creation;
      creation.intelligence.name = reportedUnit.GetName().c_str();
      creation.intelligence.level = ASN1T_EnumNatureLevel::ooo;
      creation.intelligence.diplomacy = ASN1T_EnumDiplomacy::ennemi ;
      creation.intelligence.nature = "symbols/shgpuci";
      creation.intelligence.formation = simAgent->automat_->parentFormation_->GetId();
      creation.intelligence.embarked = true;
      creation.intelligence.location = utm;

      AddIntelligence( creation.oid, reportedUnit.GetName() );
    }

    SentToAllClients( asnMsg );
 }*/
}

// -----------------------------------------------------------------------------
// Name: ReportManager::NewOrderUpdate
// Created: MGD 2009-07-12
// -----------------------------------------------------------------------------
void ReportManager::NewOrderUpdate( UnitAutomate& reportedUnit )
{
  const unsigned int idAutomate = reportedUnit.GetId();
  dispatcher::Automat* simAutomate = model_.automats_.Find( idAutomate );
  Mission* mission = reportedUnit.GetMission();

  if( !simAutomate || ! mission )
    return;

  const std::vector< Point >& limit1 = mission->GetLeftLimit();
  const std::vector< Point >& limit2 = mission->GetRightLimit();

  std::set< unsigned int >::iterator itFind = oldLimits.find( idAutomate );
  if( itFind != oldLimits.end() )
  {
    DeleteLimit( idAutomate );
    DeleteLimit( idAutomate * 2 );
  }
  else
  {
    oldLimits.insert( idAutomate );
  }

  CreateLimit( idAutomate, *simAutomate, limit1 );
  CreateLimit( idAutomate * 2, *simAutomate, limit2 );

  //@TODO Add order name
}

// -----------------------------------------------------------------------------
// Name: ReportManager::CreateLimit
// Created: MGD 2009-07-12
// -----------------------------------------------------------------------------
void ReportManager::CreateLimit( const unsigned int idLimit, const dispatcher::Automat& automat, const std::vector< Point >& limit)
{
  if( limit.size() == 0 )
    return;

  ASN1T_MsgsPluginToClient asnMsg;
  ASN1T_MsgLimitCreation creation;

  asnMsg.msg.t = T_MsgsPluginToClient_msg_plugin_limit_creation;
  asnMsg.msg.u.plugin_limit_creation = &creation;
  creation.oid = idLimit;
  creation.tactical_line.name = "Limit";
  creation.tactical_line.diffusion.t = T_TacticalLinesDiffusion_automat;
  creation.tactical_line.diffusion.u.automat = automat.GetId();

  creation.tactical_line.geometry.type = EnumLocationType::line;
  creation.tactical_line.geometry.coordinates.n = limit.size();
  creation.tactical_line.geometry.coordinates.elem = new ASN1T_CoordLatLong[ limit.size() ];
  unsigned int i = 0;
  for( std::vector< Point >::const_iterator it = limit.begin(); it != limit.end(); ++it )
    it->FillLatLong( creation.tactical_line.geometry.coordinates.elem[i++] );

  SentToAllClients( asnMsg );

  delete[] creation.tactical_line.geometry.coordinates.elem;
}

// -----------------------------------------------------------------------------
// Name: ReportManager::DeleteLimit
// Created: MGD 2009-07-12
// -----------------------------------------------------------------------------
void ReportManager::DeleteLimit( unsigned int id )
{
  ASN1T_MsgsPluginToClient asnMsg;

  asnMsg.msg.t = T_MsgsPluginToClient_msg_plugin_limit_destruction;
  asnMsg.msg.u.plugin_limit_destruction = id;

  SentToAllClients( asnMsg );
}

void ReportManager::SentToAllClients( ASN1T_MsgsPluginToClient& asnMsg )
{
  clientManager_.GetClientPublisher()->Send( asnMsg );
}