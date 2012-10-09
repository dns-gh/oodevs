// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "IsPartOfStruct.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: IsPartOfStruct constructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
IsPartOfStruct::IsPartOfStruct()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IsPartOfStruct constructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
IsPartOfStruct::IsPartOfStruct( const rpr::EntityIdentifier& disId, const Omt13String& rtiId )
    : disId_( disId )
    , rtiId_( rtiId )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IsPartOfStruct destructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
IsPartOfStruct::~IsPartOfStruct()
{
    // NOTHING
}
