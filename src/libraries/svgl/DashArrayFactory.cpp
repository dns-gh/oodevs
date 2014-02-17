// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "DashArrayFactory.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: DashArrayFactory constructor
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
DashArrayFactory::DashArrayFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DashArrayFactory destructor
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
DashArrayFactory::~DashArrayFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DashArrayFactory::CreateProperty
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
DashArray* DashArrayFactory::CreateProperty( const std::string& content ) const
{
    if( content.empty() || content == "inherit" )
        return 0;
    return new DashArray( content );
}
