// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "UnitAutomate.h"

#include "dispatcher/Automat.h"

#include "xmlia_plugin/Mission.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: UnitAutomate constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitAutomate::UnitAutomate( const dispatcher::Automat& automat )
: Unit_ABC( automat.GetId(), automat.name_ )
{}

// -----------------------------------------------------------------------------
// Name: UnitAutomate constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitAutomate::UnitAutomate( xml::xistream& xis )
: Unit_ABC( xis )
{
  //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitAutomate destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitAutomate::~UnitAutomate()
{
  if( mission_ )
    delete mission_;
}


// -----------------------------------------------------------------------------
// Name: UnitAutomate::UpdateMission
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UnitAutomate::UpdateMission( kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2 )
{
    if( mission_ )
        mission_->Update( mission );
    else
        mission_ = new Mission( mission, limit1, limit2 );
}

// -----------------------------------------------------------------------------
// Name: UnitAutomate::SerializeMission
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UnitAutomate::SerializeMission( xml::xostream& xos ) const
{
  if( mission_ )
    mission_->Serialize( xos );
}