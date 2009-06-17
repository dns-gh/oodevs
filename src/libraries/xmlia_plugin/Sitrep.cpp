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

#include "RapportManager.h"
#include "Unite_ABC.h"
#include "UniteAutomat.h"
#include "UniteFormation.h"
#include "UniteAgent.h"
#include "Point.h"
#include "xmlia_plugin/Point.h"
#include "xmlia_plugin/EtatOperationnel.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: Sitrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Sitrep::Sitrep( RapportManager& manager, xml::xistream& xis )
:  Rapport( manager, xis )
{
   //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sitrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Sitrep::Sitrep( RapportManager& manager, dispatcher::Automat& author )
: Rapport( manager, author, "SITREP" )
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

// -----------------------------------------------------------------------------
// Name: Sitrep::UpdateSimulation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::UpdateSimulation()
{
    dispatcher::SimulationPublisher_ABC& simPublisher = rapportManager_.GetSimulationPublisher();
    for( std::map< unsigned, UniteAgent* >::const_iterator it = unites_.begin(); it != unites_.end(); it++ )
    {
    //@TODO link to magic action
      UniteAgent* reportAgent = it-> second;
      dispatcher::Agent* simAgent = rapportManager_.GetModel().agents_.Find( it->first );
      if ( simAgent != 0 )
      {
          {
              simulation::UnitMagicAction asnMsg;

              {
                  ASN1T_CoordLatLong utm;
                  asnMsg().action.t                        = T_MsgUnitMagicAction_action_move_to;
                  asnMsg().oid = reportAgent->GetId();
                  reportAgent->GetLocalization()->FillLatLong( utm );
                  asnMsg().action.u.move_to = &utm;
                  asnMsg.Send( simPublisher );
              }

              {
              //    ASN1T_MagicActionPartialRecovery asnMagicAction;
              //    asnMsg().action.t                        = T_MsgUnitMagicAction_action_recompletement_partiel;
              //    asnMsg().action.u.recompletement_partiel = &asnMagicAction;
              //    //@TODO: fill msg
              //    asnMsg.Send( simPublisher );
              }
          }
      }
    }
}