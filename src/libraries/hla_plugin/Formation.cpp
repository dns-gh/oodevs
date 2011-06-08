// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Formation.h"
#include "SerializationTools.h"
#include "Dimension.h"
#include <hla/UpdateFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
Formation::Formation( bool isOnRoad )
    : formation_( isOnRoad ? 5 : 1 ) // column, assembly
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    // NOTHING
}
