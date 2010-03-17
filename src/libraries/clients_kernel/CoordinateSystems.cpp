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
    systems_[ E_Mgrs ]      = tools::translate( "UTM MGRS", "UTM MGRS" );
	systems_[ E_Wgs84Dd ]   = tools::translate( "LatLong WGS84 DD", "LatLong WGS84 DD" );
	systems_[ E_Wgs84Dms ]  = tools::translate( "LatLong WGS84 DMS", "LatLong WGS84 DMS" );
	systems_[ E_Local ]     = tools::translate( "Local Coordinates", "Coordonn�es locales" );

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



