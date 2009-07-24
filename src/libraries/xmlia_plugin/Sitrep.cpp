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
// Name: Sitrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Sitrep::Sitrep( ReportManager& manager, xml::xistream& xis )
:  Report_ABC( manager, xis, "Sitrep" )
{
  xis >> xml::start( "SituationAMI" )
        >> xml::start( "Unites" )
          >> xml::list( "Unite", *this, &Sitrep::ReadUnite, unitesAMI_ )
        >> xml::end()
      >> xml::end()
      >> xml::start( "SituationENI" )
        >> xml::start( "Unites" )
          >> xml::list( "Unite", *this, &Sitrep::ReadUnite, unitesENI_ )
        >> xml::end()
      >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Sitrep ReadUnite
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::ReadUnite( xml::xistream& xis, std::map< unsigned, UnitAgent* >& map )
{
  UnitAgent* agent = new UnitAgent( xis );
  map[agent->GetId()] = agent;
}

// -----------------------------------------------------------------------------
// Name: Sitrep constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Sitrep::Sitrep( ReportManager& manager, dispatcher::Automat& author )
: Report_ABC( manager, author, "Sitrep" )
{}

// -----------------------------------------------------------------------------
// Name: Sitrep destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Sitrep::~Sitrep()
{
  for( std::map< unsigned, UnitAgent* >::iterator it = unitesAMI_.begin(); it != unitesAMI_.end(); it++ )
  {
    delete it->second;
  }
  unitesAMI_.clear();
  for( std::map< unsigned, UnitAgent* >::iterator it = unitesENI_.begin(); it != unitesENI_.end(); it++ )
  {
    delete it->second;
  }
  unitesENI_.clear();
}

// -----------------------------------------------------------------------------
// Name: Sitrep::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::InsertOrUpdateFriendly( dispatcher::Agent& agent )
{
  std::map< unsigned, UnitAgent* >::iterator itFind = unitesAMI_.find( agent.GetId() );
  if( itFind == unitesAMI_.end() )
  {
    unitesAMI_.insert( std::pair< unsigned, UnitAgent* >( agent.GetId(), new UnitAgent( agent ) ) );
  }

  unitesAMI_[agent.GetId()]->Update( agent );
}

// -----------------------------------------------------------------------------
// Name: Sitrep::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::InsertOrUpdateEnemy( dispatcher::Agent& agent )
{
  std::map< unsigned, UnitAgent* >::iterator itFind = unitesENI_.find( agent.GetId() );
  if( itFind == unitesENI_.end() )
  {
    unitesENI_.insert( std::pair< unsigned, UnitAgent* >( agent.GetId(), new UnitAgent( agent ) ) );
  }

  unitesENI_[agent.GetId()]->Update( agent );
}

// -----------------------------------------------------------------------------
// Name: Sitrep::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::SerializeContent( xml::xostream& xos ) const
{
  xos << xml::start( "SituationAMI" )
        << xml::start( "Unites" );
        for( std::map< unsigned, UnitAgent* >::const_iterator it = unitesAMI_.begin(); it != unitesAMI_.end(); it++ )
        {
          xos << xml::start( "Unite" );
          it->second->Serialize( xos );
          it->second->SerializePosition( xos );
          it->second->SerializeEtatOps( xos );
          xos << xml::end();
        }
    xos << xml::end()
      << xml::end();

  xos << xml::start( "SituationENI" )
    << xml::start( "Unites" );
  for( std::map< unsigned, UnitAgent* >::const_iterator it = unitesENI_.begin(); it != unitesENI_.end(); it++ )
  {
    xos << xml::start( "Unite" );
    it->second->Serialize( xos );
    it->second->SerializePosition( xos );
    it->second->SerializeEtatOps( xos );
    xos << xml::end();
  }
  xos << xml::end()
    << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Sitrep::UpdateSimulation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Sitrep::UpdateSimulation()
{
  for( std::map< unsigned, UnitAgent* >::const_iterator it = unitesAMI_.begin(); it != unitesAMI_.end(); it++ )
  {
    reportManager_.MagicMove( *(it->second) );
    reportManager_.UpdateOperationnalState( *(it->second) );
  }

  for( std::map< unsigned, UnitAgent* >::const_iterator it = unitesENI_.begin(); it != unitesENI_.end(); it++ )
  {
    reportManager_.InsertOrUpdateIntelligence( *(it->second) );
    reportManager_.UpdateOperationnalState( *(it->second) ); //@TODO WHY magic action if we work with intelligence??? 
  }
}