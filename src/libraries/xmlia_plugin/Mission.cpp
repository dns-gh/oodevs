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

#include "clients_kernel/MissionType.h"

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
    >> xml::start( "LimiteGauche" )
        >> xml::list( "PointGeographique", *this, &Mission::ReadPoint, leftLimit_ )
    >> xml::end()
    >> xml::start( "LimiteDroite" )
        >> xml::list( "PointGeographique", *this, &Mission::ReadPoint, rightLimit_ )
    >> xml::end()
    >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Mission::Mission( kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2 )
: Entity_ABC()
, name_( mission.GetName() )
, leftLimit_( limit1 )
, rightLimit_( limit2 )
{}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Mission::~Mission()
{
}

// -----------------------------------------------------------------------------
// Name: Mission ReadPoint
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Mission::ReadPoint( xml::xistream& xis, std::vector< Point >& limite )
{
    limite.push_back( Point( xis ) );
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
  for( std::vector< Point >::const_iterator it = leftLimit_.begin(); it != leftLimit_.end(); it++ )
  {
      it->Serialize( xos );
  }
  xos << xml::end()
    << xml::start( "LimiteDroite" );
  for( std::vector< Point >::const_iterator it = rightLimit_.begin(); it != rightLimit_.end(); it++ )
  {
      it->Serialize( xos );
  }
  xos << xml::end()
    << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Mission::Update
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Mission::Update( kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2 )
{
  name_ = mission.GetName();
  leftLimit_ = limit1;
  rightLimit_ = limit2;
}

// -----------------------------------------------------------------------------
// Name: Mission::GetLeftLimit
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
const std::vector< Point >& Mission::GetLeftLimit() const
{
  return leftLimit_;
}

// -----------------------------------------------------------------------------
// Name: Mission::GetRightLimit
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
const std::vector< Point >& Mission::GetRightLimit() const
{
  return rightLimit_;
}

