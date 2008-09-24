// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameter_ABC.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameter_ABC::MissionParameter_ABC( const kernel::OrderParameter& type )
    : type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameter_ABC::~MissionParameter_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC::GetType
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
const kernel::OrderParameter& MissionParameter_ABC::GetType() const
{
    return type_;
}
