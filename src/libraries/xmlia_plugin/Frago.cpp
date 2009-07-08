// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Frago.h"
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
// Name: Frago constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Frago::Frago( ReportManager& manager, xml::xistream& xis )
:  Report_ABC( manager, xis, "Frago" )
{
  xis >> xml::start( "Missions" )
    >> xml::list( "Unite", *this, &Frago::ReadUnite, unitesAMI_ )
    >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Frago ReadUnite
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Frago::ReadUnite( xml::xistream& xis, std::map< unsigned, UnitAgent* >& map )
{
  UnitAgent* agent = new UnitAgent( xis );
  map[agent->GetId()] = agent;
}

// -----------------------------------------------------------------------------
// Name: Frago constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Frago::Frago( ReportManager& manager, dispatcher::Automat& author )
: Report_ABC( manager, author, "Frago" )
{}

// -----------------------------------------------------------------------------
// Name: Frago destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Frago::~Frago()
{}

// -----------------------------------------------------------------------------
// Name: Frago::SerializeContent
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Frago::SerializeContent( xml::xostream& xos ) const
{
  xos << xml::start( "Missions" );
  for( std::map< unsigned, UnitAgent* >::const_iterator it = unitesAMI_.begin(); it != unitesAMI_.end(); it++ )
  {
    xos << xml::start( "Unite" );
    it->second->Serialize( xos );
    it->second->SerializeMission( xos );
    xos << xml::end();
  }
  xos << xml::end()
    << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Frago::UpdateMission
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Frago::UpdateMission( dispatcher::Agent agent, kernel::MissionType& mission )
{
  std::map< unsigned, UnitAgent* >::iterator itFind = unitesAMI_.find( agent.GetId() );
  if( itFind == unitesAMI_.end() )
  {
    unitesAMI_.insert( std::pair< unsigned, UnitAgent* >( agent.GetId(), new UnitAgent( agent ) ) );
  }

  unitesAMI_[agent.GetId()]->UpdateMission( mission );
}


// -----------------------------------------------------------------------------
// Name: Frago::UpdateSimulation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Frago::UpdateSimulation()
{
  //@TODO
}