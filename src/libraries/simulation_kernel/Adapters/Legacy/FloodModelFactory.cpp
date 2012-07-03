// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FloodModelFactory.h"
#include "flood/FloodModel.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "MIL_AgentServer.h"

using namespace sword::legacy;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::legacy::FloodModelFactory )

// -----------------------------------------------------------------------------
// Name: FloodModelFactory constructor
// Created: LGY 2012-06-19
// -----------------------------------------------------------------------------
FloodModelFactory::FloodModelFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodModelFactory destructor
// Created: LGY 2012-06-19
// -----------------------------------------------------------------------------
FloodModelFactory::~FloodModelFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodModelFactory::CreateFloodModel
// Created: LGY 2012-06-19
// -----------------------------------------------------------------------------
std::auto_ptr< flood::FloodModel_ABC > FloodModelFactory::CreateFloodModel() const
{
    return std::auto_ptr< flood::FloodModel_ABC >( new flood::FloodModel( *this ) );
}

// -----------------------------------------------------------------------------
// Name: FloodModelFactory::GetElevationAt
// Created: LGY 2012-06-19
// -----------------------------------------------------------------------------
short FloodModelFactory::GetElevationAt( const geometry::Point2f& point ) const
{
    return static_cast< short >( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( point.X(), point.Y() ) );
}
