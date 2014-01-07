// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CommandLineTools.h"

uint16_t frontend::GetPort( unsigned idx, Ports port )
{
    return static_cast< uint16_t >( 10000 + (idx-1) * NUM_PORTS + port );
}
