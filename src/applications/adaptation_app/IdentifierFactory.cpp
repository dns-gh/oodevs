// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "IdentifierFactory.h"

// -----------------------------------------------------------------------------
// Name: IdentifierFactory constructor
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
IdentifierFactory::IdentifierFactory()
    : last_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifierFactory destructor
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
IdentifierFactory::~IdentifierFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifierFactory::Create
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
unsigned long IdentifierFactory::Create()
{
    return ++last_;
}

// -----------------------------------------------------------------------------
// Name: IdentifierFactory::Reserve
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
void IdentifierFactory::Reserve( unsigned long id )
{
    if( id > last_ )
        last_ = id;
}

// -----------------------------------------------------------------------------
// Name: IdentifierFactory::Reset
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
void IdentifierFactory::Reset()
{
    last_ = 0;
}
