// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Sitrep.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>

#include "dispatcher/Side.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Model.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ClientPublisher_ABC.h"

#include "ReportManager.h"
#include "Unite_ABC.h"
#include "UniteAutomat.h"
#include "UniteFormation.h"
#include "UniteAgent.h"
#include "Point.h"
#include "xmlia_plugin/Point.h"
#include "xmlia_plugin/EtatOperationnel.h"
#include "game_asn/MessengerSenders.h"
#include "game_asn/Messenger.h"


using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: Sitrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Sitrep::Sitrep( ReportManager& manager, xml::xistream& xis )
:  Report_ABC( manager, xis )
{
   //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sitrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Sitrep::Sitrep( ReportManager& manager, dispatcher::Automat& author )
: Report_ABC( manager, author, "SITREP" )
{}

// -----------------------------------------------------------------------------
// Name: Sitrep destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Sitrep::~Sitrep()
{}

// -----------------------------------------------------------------------------
// Name: Sitrep::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::InsertOrUpdate( dispatcher::Agent& agent )
{
  std::map< unsigned, UniteAgent* >::iterator itFind = unites_.find( agent.GetId() );
  if( itFind == unites_.end() )
  {
    unites_.insert( std::pair< unsigned, UniteAgent* >( agent.GetId(), new UniteAgent( agent ) ) );
  }

  unites_[agent.GetId()]->Update( agent );
}

// -----------------------------------------------------------------------------
// Name: Sitrep::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::SerializeOtherEntities( xml::xostream& xos ) const
{
    std::string sQnameRapport = QName();

    for( std::map< unsigned, UniteAgent* >::const_iterator it = unites_.begin(); it != unites_.end(); it++ )
    {
      it->second->Serialize( xos, sQnameRapport );
    }
}

// -----------------------------------------------------------------------------
// Name: Sitrep::SerializeSide
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::SerializeSide( const dispatcher::Side& side, xml::xostream& xos, std::string sQnameRapport ) const
{
  std::ostringstream os;
  os << side.GetId();
  std::string sIdSide = os.str();
  std::string sQnameSide = "camp-" + sIdSide;

  xos << xml::start( "mpia:GroupeFonctionnel" )
      << xml::attribute( "id", sQnameSide )
      << xml::content( "mpia:Nom", side.GetName() )
      << xml::start( "mpia:EstRapporteePar_Rapport" )
      << xml::content( "mpia:refid", sQnameRapport )
      << xml::end();
  for( std::map< unsigned, UniteAgent* >::const_iterator it = unites_.begin(); it != unites_.end(); it++ )
  {
    if( it->second->IsSide( side.GetId() ) )
    {
      std::ostringstream os2;
      os2 << it->second->GetId();
      std::string sIdUnite = os2.str();
      xos << xml::start( "mpia:EstAffiliationDe_AssociationAffiliationInstanceObjet" )
          << xml::content( "mpia:refid", "association-" + sIdSide + "-" + sIdUnite)
          << xml::end();
    }
  }
  xos << xml::content( "Categorie", "NKN" )//@TODOFORCE
    << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Sitrep::UpdateSimulation
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::ReadEntities( xml::xistream& xis )
{
  xis >> xml::list( "mpia:PointGeographique", *this, &Sitrep::ReadPosition );
  xis >> xml::list( "mpia:EtatOperationnelEntiteOrganisationnelle", *this, &Sitrep::ReadEtatOps );
  //@TODO move in rapport if others report have same data
}

void Sitrep::ReadPosition( xml::xistream& xis )
{
  Point* pt = new Point( xis );
  unites_[pt->GetId()]->SetPosition( pt );
}

void Sitrep::ReadEtatOps( xml::xistream& xis )
{
  EtatOperationnel* etatOps = new EtatOperationnel( xis );
  unites_[etatOps->GetId()]->SetEtatOps( etatOps );
}

unsigned int Sitrep::GetAuthorID() const
{
	return author_->GetId();
}

// -----------------------------------------------------------------------------
// Name: Sitrep::UpdateSimulation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::UpdateSimulation()
{
  dispatcher::SimulationPublisher_ABC& simPublisher = reportManager_.GetSimulationPublisher();
  dispatcher::ClientPublisher_ABC& clientPublisher = *reportManager_.GetClientPublisher();
	unsigned int authorID = author_->GetId();
	unsigned long authorSideID = reportManager_.GetModel().automats_.Find( authorID )->team_.GetId();
	dispatcher::Agent* simAuthorAgent = reportManager_.GetModel().agents_.Find( authorID );
  for( std::map< unsigned, UniteAgent* >::const_iterator it = unites_.begin(); it != unites_.end(); it++ )
  {
    //@TODO link to magic action
    UniteAgent* reportAgent = it-> second;
    dispatcher::Agent* simAgent = reportManager_.GetModel().agents_.Find( it->first );
    if ( simAgent != 0 )
    {
      unsigned long agentSideID = reportManager_.GetModel().agents_.Find( reportAgent->GetId())->automat_->team_.GetId();
      {
        if ( agentSideID == authorSideID )
        {
          simulation::UnitMagicAction asnMsg;
          ASN1T_CoordLatLong utm;
          asnMsg().oid = reportAgent->GetId();
          asnMsg().action.t                        = T_MsgUnitMagicAction_action_move_to;
          reportAgent->GetLocalization()->FillLatLong( utm );
          asnMsg().action.u.move_to = &utm;
          asnMsg.Send( simPublisher );
        }
        else //create raw intelligence or knowledge on client for ENIs
        {
          ASN1T_MsgsMessengerToClient asnMsg;
          ASN1T_MsgIntelligenceCreation asnTmp;
          ASN1T_CoordLatLong utm;
          asnMsg.t              = T_MsgsMessengerToClient_msg_intelligence_creation;
          asnTmp.oid = reportAgent->GetId();
          asnMsg.u.msg_intelligence_creation = &asnTmp;
          reportAgent->GetLocalization()->FillLatLong( utm );
          asnTmp.intelligence.name = reportAgent->GetName().c_str();
          asnTmp.intelligence.nature = "ENI";
          asnTmp.intelligence.location = utm;
          clientPublisher.Send( asnMsg );
        }

        if ( reportAgent->GetOperationalState()->GetGeneralOperationalState() == "NOP" )
        {
          simulation::UnitMagicAction asnMsg;
          ASN1T_MagicActionPartialRecovery asnPartialRecovery;
          asnMsg().action.t                        = T_MsgUnitMagicAction_action_recompletement_partiel;
          asnMsg().oid = reportAgent->GetId();
          asnMsg().action.u.recompletement_partiel = &asnPartialRecovery;
          asnPartialRecovery.m.equipementsPresent = 0;
          asnPartialRecovery.m.personnelsPresent = 0;
          asnPartialRecovery.m.dotationsPresent = 0;
          asnPartialRecovery.m.munitionsPresent = 0;
          asnPartialRecovery.m.stocksPresent = 0;
          asnMsg.Send( simPublisher );
        }
      }
    }
  }
}