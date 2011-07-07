// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "CoordinateSystems.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CoordinateSystems constructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
CoordinateSystems::CoordinateSystems()
{
    systems_[ E_Mgrs ]      = tools::translate( "CoordinateSystems", "UTM MGRS" );
    systems_[ E_Wgs84Dd ]   = tools::translate( "CoordinateSystems", "LatLong WGS84 DD" );
    systems_[ E_Wgs84Dms ]  = tools::translate( "CoordinateSystems", "LatLong WGS84 DMS" );
    systems_[ E_Local ]     = tools::translate( "CoordinateSystems", "Local coordinates" );

    defaultCoordinateSystem_ = E_Mgrs;
}
// -----------------------------------------------------------------------------
// Name: CoordinateSystems destructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
CoordinateSystems::~CoordinateSystems()
{
    // NOTHING
}

