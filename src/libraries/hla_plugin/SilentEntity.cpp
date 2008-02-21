// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SilentEntity.h"

// -----------------------------------------------------------------------------
// Name: SilentEntity constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
SilentEntity::SilentEntity( const EntityType& type, unsigned short number )
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
