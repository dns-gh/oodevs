// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Opord.h"
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
// Name: Opord constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Opord::Opord( ReportManager& manager, xml::xistream& xis )
:  Report_ABC( manager, xis, "Opord" )
, automat_( 0 )
{
  xis >> xml::start( "Missions" )
    >> xml::start( "Unite" );
    automat_ = new UnitAutomate( xis );
  xis >> xml::end()
    >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Opord constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Opord::Opord( ReportManager& manager, dispatcher::Automat& author )
: Report_ABC( manager, author, "Opord" )
, automat_( 0 )
{
  automat_ = new UnitAutomate( author );
}

// -----------------------------------------------------------------------------
// Name: Opord destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Opord::~Opord()
{
  if( automat_ )
    delete automat_;
}


// -----------------------------------------------------------------------------
// Name: Opord::SerializeContent
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Opord::SerializeContent( xml::xostream& xos ) const
{
  xos << xml::start( "Missions" )
        << xml::start( "Unite" );
          automat_->Serialize( xos );
          automat_->SerializeMission( xos );
   xos  << xml::end()
      << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Opord::UpdateMission
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Opord::UpdateMission( kernel::MissionType& mission,  std::vector< Point >& limit1, std::vector< Point >& limit2  )
{
  automat_->UpdateMission( mission, limit1, limit2 );
}


// -----------------------------------------------------------------------------
// Name: Opord::UpdateSimulation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Opord::UpdateSimulation()
{
  reportManager_.NewOrderUpdate( *automat_ );
}