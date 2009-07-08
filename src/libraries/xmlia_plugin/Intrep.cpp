// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Intrep.h"
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
// Name: Intrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Intrep::Intrep( ReportManager& manager, xml::xistream& xis )
:  Report_ABC( manager, xis, "Intrep" )
{
  xis >> xml::start( "Cible" )
    >> xml::start( "Unite" );
  fired = new UnitAgent( xis );
  xis >> xml::end()
    >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Intrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Intrep::Intrep( ReportManager& manager, dispatcher::Automat& author )
: Report_ABC( manager, author, "Intrep" )
{}

// -----------------------------------------------------------------------------
// Name: Intrep destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Intrep::~Intrep()
{
  if( fired )
    delete fired;
}

// -----------------------------------------------------------------------------
// Name: Intrep::SerializeContent
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Intrep::SerializeContent( xml::xostream& xos ) const
{
  xos << xml::start( "Cible" )
      << xml::start( "Unite" );
    fired->Serialize( xos );
    fired->SerializePosition( xos );
    fired->SerializeEtatOps( xos );
    xos << xml::end()
      << xml::end();
}


// -----------------------------------------------------------------------------
// Name: Intrep::UpdateSimulation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Intrep::UpdateSimulation()
{
  //@TODO
}

// -----------------------------------------------------------------------------
// Name: Intrep::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Intrep::InsertOrUpdateEnemy( dispatcher::Agent& agent )
{
  if( !fired )
    fired = new UnitAgent( agent );
  else
    fired->Update( agent );
}