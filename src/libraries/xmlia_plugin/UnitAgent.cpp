// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "UnitAgent.h"

#include "dispatcher/Side.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"

#include "Point.h"
#include "XmliaOperationalState.h"
#include "xmlia_plugin/Mission.h"

#include "boost/bind.hpp"
#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: UnitAgent constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitAgent::UnitAgent( xml::xistream& xis )
: Unit_ABC( xis )
{
  if( xis.has_child( "mpia:PointGeographique" ) )
  {
    localisation_ = new Point( xis );
  }
  if( xis.has_child( "mpia:EtatOperationnelEntiteOrganisationnelle" ) )
  {
    etatOps_ = new XmliaOperationalState( xis );
  }
  if( xis.has_child( "Mission" ) )
  {
    mission_ = new Mission( xis );
  }
}

// -----------------------------------------------------------------------------
// Name: UnitAgent constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitAgent::UnitAgent( dispatcher::Agent& agent )
: Unit_ABC( agent.GetId(), agent.name_ )
, idSide_(agent.automat_->team_.GetId() )
{
  localisation_ = new Point( agent.position_.latitude, agent.position_.longitude );
  etatOps_      = new XmliaOperationalState( agent );
}

// -----------------------------------------------------------------------------
// Name: UnitAgent destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitAgent::~UnitAgent()
{
    if( localisation_ )
      delete localisation_;
    if( etatOps_ )
      delete etatOps_;
    if( mission_ )
      delete mission_;
}

// -----------------------------------------------------------------------------
// Name: UnitAgent::SerializePosition
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UnitAgent::SerializePosition( xml::xostream& xos ) const
{
  if( localisation_ )
    localisation_->Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UnitAgent::SerializeEtatOps
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UnitAgent::SerializeEtatOps( xml::xostream& xos ) const
{
  if( etatOps_ )
    etatOps_->Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UnitAgent::SerializeMission
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UnitAgent::SerializeMission( xml::xostream& xos ) const
{
  if( mission_ )
    mission_->Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UnitAgent::Update
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UnitAgent::Update( dispatcher::Agent& agent )
{
  id_        = agent.GetId();
  name_      = agent.name_;
  idSide_    = agent.automat_->team_.GetId();
  
  localisation_->Update( agent );
  etatOps_->Update( agent );
}

// -----------------------------------------------------------------------------
// Name: UnitAgent::UpdateMission
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UnitAgent::UpdateMission( kernel::MissionType& mission )
{
  mission_->Update( mission );
}

// -----------------------------------------------------------------------------
// Name: UnitAgent::IsSide
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
bool UnitAgent::IsSide( unsigned int idSide) const
{
  return idSide == idSide_;
}

// -----------------------------------------------------------------------------
// Name: UnitAgent::GetLocalization
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
Point* UnitAgent::GetLocalization() const
{
  return localisation_;
}

// -----------------------------------------------------------------------------
// Name: UnitAgent::GetOperationalState
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
XmliaOperationalState* UnitAgent::GetOperationalState() const
{
  return etatOps_;
}