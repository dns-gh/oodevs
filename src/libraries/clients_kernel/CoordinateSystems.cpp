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

#include <boost/assign/list_of.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CoordinateSystems constructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
CoordinateSystems::CoordinateSystems()
    : systems_( boost::assign::map_list_of
                ( E_Mgrs,     tools::translate( "CoordinateSystems", "UTM MGRS" ) )
                ( E_Wgs84Dd,  tools::translate( "CoordinateSystems", "LatLong WGS84 DD" ) )
                ( E_Wgs84Dms, tools::translate( "CoordinateSystems", "LatLong WGS84 DMS" ) )
                ( E_Local,    tools::translate( "CoordinateSystems", "Local coordinates" ) ) )
    , default_( E_Mgrs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystems destructor
// Created: AME 2010-03-12
// -----------------------------------------------------------------------------
CoordinateSystems::~CoordinateSystems()
{
    // NOTHING
}

