// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "UniteFormation.h"

#include "dispatcher/Formation.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: UniteFormation constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UniteFormation::UniteFormation( dispatcher::Formation& formation )
: Unite_ABC( formation.GetId(), formation.GetName().ascii() )
{}

// -----------------------------------------------------------------------------
// Name: UniteFormation constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UniteFormation::UniteFormation( xml::xistream& xis )
: Unite_ABC( xis )
{
  //@NOTHING
}