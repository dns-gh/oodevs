// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "UniteAutomat.h"

#include "dispatcher/Automat.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: UniteAutomat constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UniteAutomat::UniteAutomat( const dispatcher::Automat& automat )
: Unite_ABC( automat.GetId(), automat.name_ )
{}

// -----------------------------------------------------------------------------
// Name: UniteAutomat constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UniteAutomat::UniteAutomat( xml::xistream& xis )
: Unite_ABC( xis )
{
  //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UniteAutomat destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UniteAutomat::~UniteAutomat()
{}