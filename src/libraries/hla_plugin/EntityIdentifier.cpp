// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "EntityIdentifier.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: EntityIdentifier constructor
// Created: AGE 2008-04-30
// -----------------------------------------------------------------------------
EntityIdentifier::EntityIdentifier()
    : federateIdentifier_( 0, 0 )
    , entityNumber_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifier constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
EntityIdentifier::EntityIdentifier( unsigned short site, unsigned short application, unsigned short number )
    : federateIdentifier_( site, application )
    , entityNumber_( number )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifier destructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
EntityIdentifier::~EntityIdentifier()
{
    // NOTHING
}
