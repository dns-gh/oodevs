// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "NBCReport.h"
#include <xeumeuleu/xml.h>

#include "dispatcher/Side.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Model.h"
#include "dispatcher/Object.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ClientPublisher_ABC.h"

#include "ReportManager.h"
#include "Unit_ABC.h"
#include "UnitAutomate.h"
#include "UnitFormation.h"
#include "UnitAgent.h"
#include "xmlia_plugin/Point.h"
#include "xmlia_plugin/NBCObject.h"
#include "xmlia_plugin/XmliaOperationalState.h"
#include "game_asn/MessengerSenders.h"
#include "game_asn/Messenger.h"


using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: NBCReport constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
NBCReport::NBCReport( ReportManager& manager, xml::xistream& xis )
:  Report_ABC( manager, xis, "NBCReport" )
{
   //NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCReport constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
NBCReport::NBCReport( ReportManager& manager, dispatcher::Automat& author )
: Report_ABC( manager, author, "NBCReport" )
{}

// -----------------------------------------------------------------------------
// Name: NBCReport destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
NBCReport::~NBCReport()
{
  for( std::map< unsigned, NBCObject* >::iterator it = objects_.begin(); it != objects_.end(); it++ )
  {
    delete it->second;
  }
}

// -----------------------------------------------------------------------------
// Name: NBCReport::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void NBCReport::InsertOrUpdateNBC( dispatcher::Object& obj )
{
  std::map< unsigned, NBCObject* >::iterator itFind = objects_.find( obj.GetId() );
  if( itFind == objects_.end() )
  {
    objects_.insert( std::pair< unsigned, NBCObject* >( obj.GetId(), new NBCObject( obj ) ) );
  }

  objects_[obj.GetId()]->Update( obj );
}

// -----------------------------------------------------------------------------
// Name: NBCReport::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void NBCReport::SerializeContent( xml::xostream& xos ) const
{
    xos << xml::start( "Evenements" );
      for( std::map< unsigned, NBCObject* >::const_iterator it = objects_.begin(); it != objects_.end(); it++ )
      {
        it->second->Serialize( xos );
      }
    xos << xml::end();
}


// -----------------------------------------------------------------------------
// Name: NBCReport::UpdateSimulation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void NBCReport::UpdateSimulation()
{
  //@TODO
}