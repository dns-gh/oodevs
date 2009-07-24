// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Logassessrep.h"
#include <xeumeuleu/xml.h>

#include "dispatcher/Side.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Model.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ClientPublisher_ABC.h"

#include "ReportManager.h"
#include "Unit_ABC.h"
#include "UnitAutomate.h"
#include "UnitFormation.h"
#include "UnitAgent.h"
#include "Point.h"
#include "xmlia_plugin/Point.h"
#include "xmlia_plugin/XmliaOperationalState.h"
#include "game_asn/MessengerSenders.h"
#include "game_asn/Messenger.h"


using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: Logassessrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Logassessrep::Logassessrep( ReportManager& manager, xml::xistream& xis )
:  Report_ABC( manager, xis, "Logassessrep" )
{
  xis >> xml::start( "SituationAMI" )
      >> xml::start( "Unites" )
      >> xml::list( "Unite", *this, &Logassessrep::ReadUnite, unitesAMI_ )
      >> xml::end()
    >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Logassessrep ReadUnite
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Logassessrep::ReadUnite( xml::xistream& xis, std::map< unsigned, UnitAgent* >& map )
{
  UnitAgent* agent = new UnitAgent( xis );
  map[agent->GetId()] = agent;
}

// -----------------------------------------------------------------------------
// Name: Logassessrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Logassessrep::Logassessrep( ReportManager& manager, dispatcher::Automat& author )
: Report_ABC( manager, author, "Logassessrep" )
{}

// -----------------------------------------------------------------------------
// Name: Logassessrep destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Logassessrep::~Logassessrep()
{
  for( std::map< unsigned, UnitAgent* >::iterator it = unitesAMI_.begin(); it != unitesAMI_.end(); it++ )
  {
    delete it->second;
  }
  unitesAMI_.clear();
}

// -----------------------------------------------------------------------------
// Name: Sitrep::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Logassessrep::InsertOrUpdateFriendly( dispatcher::Agent& agent )
{
  std::map< unsigned, UnitAgent* >::iterator itFind = unitesAMI_.find( agent.GetId() );
  if( itFind == unitesAMI_.end() )
  {
    unitesAMI_.insert( std::pair< unsigned, UnitAgent* >( agent.GetId(), new UnitAgent( agent ) ) );
  }

  unitesAMI_[agent.GetId()]->Update( agent );
}
// -----------------------------------------------------------------------------
// Name: Logassessrep::SerializeContent
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Logassessrep::SerializeContent( xml::xostream& xos ) const
{ 
  float average = 0;
  xos << xml::start( "SituationAMI" )
        << xml::start( "Unites" );
        for( std::map< unsigned, UnitAgent* >::const_iterator it = unitesAMI_.begin(); it != unitesAMI_.end(); it++ )
        {
          xos << xml::start( "Unite" );
          it->second->Serialize( xos );
          it->second->SerializeEtatOps( xos );
          average += it->second->GetOperationalState()->GetAverageDotation();
          xos << xml::end();
        }
    xos << xml::end()
      << xml::end()
    << xml::content("SituationLogistiqueGenerale", average );
}



// -----------------------------------------------------------------------------
// Name: Logassessrep::UpdateSimulation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Logassessrep::UpdateSimulation()
{
  for( std::map< unsigned, UnitAgent* >::const_iterator it = unitesAMI_.begin(); it != unitesAMI_.end(); it++ )
  {
    reportManager_.UpdateOperationnalState( *(it->second) );
  }
}