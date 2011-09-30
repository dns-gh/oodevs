// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SilentEntity.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: SilentEntity constructor
// Created: SLI 2011-09-30
// -----------------------------------------------------------------------------
SilentEntity::SilentEntity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SilentEntity constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
SilentEntity::SilentEntity( const rpr::EntityType& type, unsigned short number )
    : numberOfEntitiesOfThisType_( number )
    , numberOfAppearanceRecords_ ( 0 )
    , entityType_                ( type )
    , entityAppearance_          ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SilentEntity destructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
SilentEntity::~SilentEntity()
{
    // NOTHING
}
