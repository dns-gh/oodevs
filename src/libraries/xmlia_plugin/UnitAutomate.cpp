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
{}