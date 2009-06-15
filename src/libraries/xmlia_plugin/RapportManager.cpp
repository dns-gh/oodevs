// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "RapportManager.h"

#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Model.h"

#include "xmlia_plugin/Sitrep.h"

#include "boost/bind.hpp"
#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: RapportManager constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
RapportManager::RapportManager( dispatcher::Model& model)
: model_( model )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: RapportManager destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
RapportManager::~RapportManager()
{
    for( std::map< unsigned, Sitrep* >::iterator it = rapports_.begin(); it != rapports_.end(); it++ )
    {
      delete it->second;
    }
    rapports_.clear();
    for( std::vector< Sitrep* >::iterator it = receivedRapports_.begin(); it != receivedRapports_.end(); it++ )
    {
      delete (*it);
    }
    receivedRapports_.clear();
}

// -----------------------------------------------------------------------------
// Name: RapportManager Send
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void RapportManager::Send()const
{
  for( std::map< unsigned, Sitrep* >::const_iterator it = rapports_.begin(); it != rapports_.end(); it++ )
  {
    xml::xostringstream xos;
    it->second->Serialize( xos );
    //@TODO link stream to webservice send, by call or return
  }
}

// -----------------------------------------------------------------------------
// Name: RapportManager::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void RapportManager::DoUpdate( dispatcher::Agent& agent )
{
  std::map< unsigned, Sitrep* >::iterator itFind = rapports_.find( agent.automat_->GetId() );
  if( itFind == rapports_.end() )
  {
    rapports_.insert( std::pair< unsigned, Sitrep*>( agent.automat_->GetId(), new  Sitrep( *this, *agent.automat_ ) ) );
  }

  rapports_[ agent.automat_->GetId() ]->InsertOrUpdate( agent );
}
// -----------------------------------------------------------------------------
// Name: RapportManager::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void RapportManager::DoUpdate( dispatcher::Agent& agent, dispatcher::Agent& detected )
{
  std::map< unsigned, Sitrep* >::iterator itFind = rapports_.find( agent.automat_->GetId() );
  if( itFind == rapports_.end() )
  {
    rapports_.insert( std::pair< unsigned, Sitrep*>( agent.automat_->GetId(), new  Sitrep( *this, *agent.automat_ ) ) );
  }

  rapports_[ agent.automat_->GetId() ]->InsertOrUpdate( detected );
}

// -----------------------------------------------------------------------------
// Name: RapportManager::Read
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void RapportManager::Read( xml::xistream& xis )
{
  std::string type;
  std::string sQnameRapport;

  xis >> xml::start( "mpia:MPIA_Message" )
        >> xml::start( "mpia:Header" )
          >> xml::content( "mpia:Name", type )
        >> xml::end()
        >> xml::start( "mpia:Entites" );

  if( type == "SITREP" )
  {
    Sitrep* newSitrep = new Sitrep( *this , xis );
    newSitrep->ReadEntities( xis );
    receivedRapports_.push_back( newSitrep );
  }
}


// -----------------------------------------------------------------------------
// Name: RapportManager::Read
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void RapportManager::UpdateSimulation() const
{
  for( std::map< unsigned, Sitrep* >::const_iterator it = rapports_.begin(); it != rapports_.end(); it++ )
  {
    it->second->UpdateSimulation();
  }
}

// -----------------------------------------------------------------------------
// Name: RapportManager::SerializeSides
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::Model& RapportManager::GetModel() const
{
  return model_;
}

void RapportManager::CleanReceivedRapport()
{
  for( std::vector< Sitrep* >::iterator it = receivedRapports_.begin(); it != receivedRapports_.end(); it++ )
  {
    delete (*it);
  }
  receivedRapports_.clear();
}