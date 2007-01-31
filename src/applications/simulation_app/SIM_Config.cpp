// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_app_pch.h"
#include "SIM_Config.h"


// -----------------------------------------------------------------------------
// Name: SIM_Config constructor
// Created: NLD 2007-01-15
// -----------------------------------------------------------------------------
SIM_Config::SIM_Config( int argc, char** argv )
    : MIL_Config        ()
    , dispatcher::Config()
{
    Parse( argc, argv );
}

// -----------------------------------------------------------------------------
// Name: SIM_Config destructor
// Created: NLD 2007-01-15
// -----------------------------------------------------------------------------
SIM_Config::~SIM_Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SIM_Config::Parse
// Created: NLD 2007-01-30
// -----------------------------------------------------------------------------
void SIM_Config::Parse( int argc, char** argv )
{
    MIL_Config::Parse        ( argc, argv );
    dispatcher::Config::Parse( argc, argv );
}
