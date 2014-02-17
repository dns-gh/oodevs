// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "OpacityFactory.h"
#include "Opacity.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: OpacityFactory constructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
OpacityFactory::OpacityFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OpacityFactory destructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
OpacityFactory::~OpacityFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OpacityFactory::CreateProperty
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
Opacity* OpacityFactory::CreateProperty( const std::string& content ) const
{
    if( content.empty() || content == "inherit" )
        return 0;
    return new Opacity( content );
}
