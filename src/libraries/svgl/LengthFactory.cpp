// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "LengthFactory.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: LengthFactory constructor
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
LengthFactory::LengthFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LengthFactory destructor
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
LengthFactory::~LengthFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LengthFactory::CreateProperty
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
Length* LengthFactory::CreateProperty( const std::string& content ) const
{
    if( content.empty() || content == "inherit" )
        return 0;
    // $$$$ AGE 2006-09-06: sortir la gestion de l'unité
    return new Length( content );
}
