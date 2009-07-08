// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Mission.h"

#include "dispatcher/Agent.h"

#include "xmlia_plugin/Point.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Mission::Mission( xml::xistream& xis )
{
  xis >> xml::start( "Mission" )
    >> xml::attribute( "nom", name_ )
    >> xml::start( "LimiteGauche" );
  gaucheDebut_ = new Point( xis );
  gaucheFin_ = new Point( xis );
  xis >> xml::end()
    >> xml::start( "LimiteDroite" );
  droiteDebut_ = new Point( xis );
  droiteFin_ = new Point( xis );
  xis >> xml::end()
    >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Mission::Mission( kernel::MissionType& mission )
: Entity_ABC()
{}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Mission::~Mission()
{//@TODO REMOVE PT!!!
  delete gaucheDebut_;
  delete gaucheFin_;
  delete droiteDebut_;
  delete droiteFin_;
}

// -----------------------------------------------------------------------------
// Name: Mission::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Mission::Serialize( xml::xostream& xos ) const
{
  xos << xml::start( "Mission" )
    << xml::attribute( "nom", name_ )
    << xml::start( "LimiteGauche" );
  gaucheDebut_->Serialize( xos );
  gaucheFin_->Serialize( xos );
  xos << xml::end()
    << xml::start( "LimiteDroite" );
  droiteDebut_->Serialize( xos );
  droiteFin_->Serialize( xos );
  xos << xml::end()
    << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Mission::Update
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Mission::Update( kernel::MissionType& agent )
{
}

