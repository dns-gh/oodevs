// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_MissionType_ABC.h"

//-----------------------------------------------------------------------------
// Name: MIL_MissionType_ABC constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_MissionType_ABC::MIL_MissionType_ABC( uint nID, xml::xistream& xis )
    : MIL_OrderType_ABC( nID, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionType_ABC constructor
// Created: LDC 2009-04-27
// -----------------------------------------------------------------------------
MIL_MissionType_ABC::MIL_MissionType_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_MissionType_ABC destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_MissionType_ABC::~MIL_MissionType_ABC()
{
    // NOTHING
}
