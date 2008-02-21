// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "EntityType.h"

// -----------------------------------------------------------------------------
// Name: EntityType constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
EntityType::EntityType()
    : entityKind_ ( platform )
    , domain_     ( land )
    , countryCode_( france )
    , category_   ( tank )
    , subcategory_( 0 )
    , specific_   ( 0 )
    , extra_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityType destructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
EntityType::~EntityType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityType::SetKind
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
void EntityType::SetKind( E_Kind kind )
{
    entityKind_ = (unsigned char)kind;
}

// -----------------------------------------------------------------------------
// Name: EntityType::SetDomain
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
void EntityType::SetDomain( E_Domain domain )
{
    domain_ = (unsigned char)domain;
}

// -----------------------------------------------------------------------------
// Name: EntityType::SetCountry
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
void EntityType::SetCountry( E_Country country )
{
    countryCode_ = (unsigned short)country;
}

// -----------------------------------------------------------------------------
// Name: EntityType::SetCategory
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
void EntityType::SetCategory( E_Category category )
{
    category_ = (unsigned char)category;
}
