// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "UnitFormation.h"

#include "dispatcher/Formation.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: UnitFormation constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitFormation::UnitFormation( dispatcher::Formation& formation )
: Unit_ABC( formation.GetId(), formation.GetName().ascii() )
{}

// -----------------------------------------------------------------------------
// Name: UnitFormation constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitFormation::UnitFormation( xml::xistream& xis )
: Unit_ABC( xis )
{
  //@NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitFormation destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UnitFormation::~UnitFormation()
{}
