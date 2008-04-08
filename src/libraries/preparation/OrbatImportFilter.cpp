// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "OrbatImportFilter.h"

// -----------------------------------------------------------------------------
// Name: OrbatImportFilter constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
OrbatImportFilter::OrbatImportFilter( bool objects, bool populations )
    : objects_( objects )
    , populations_( populations )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrbatImportFilter destructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
OrbatImportFilter::~OrbatImportFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrbatImportFilter::Filters
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
bool OrbatImportFilter::Filters( const std::string& field ) const
{
    return  ( ( !objects_ && field == "object" )
           || ( !populations_ && field == "population" ) );
}
