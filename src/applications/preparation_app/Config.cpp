// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Config.h"

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
Config::Config( int argc, char** argv )
{
    Parse( argc, argv );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}
