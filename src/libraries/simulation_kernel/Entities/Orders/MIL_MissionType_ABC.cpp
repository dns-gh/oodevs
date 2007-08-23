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

#include "Decision/DEC_Tools.h"
#include "Network/NET_AsnException.h"
#include "xeumeuleu/xml.h"

using namespace xml;

//-----------------------------------------------------------------------------
// Name: MIL_MissionType_ABC constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_MissionType_ABC::MIL_MissionType_ABC( uint nID, xml::xistream& xis )
    : MIL_OrderType_ABC( nID, xis )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_MissionType_ABC destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_MissionType_ABC::~MIL_MissionType_ABC()
{
}
