//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_TerrainLargage.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:23 $
// $Revision: 3 $
// $Workfile: MIL_TerrainLargage.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_TerrainLargage.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_TerrainLargage, "MIL_TerrainLargage" )

//-----------------------------------------------------------------------------
// Name: MIL_TerrainLargage constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_TerrainLargage::MIL_TerrainLargage()
    : MIL_RealObject_ABC( MIL_RealObjectType::terrainLargage_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_TerrainLargage destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_TerrainLargage::~MIL_TerrainLargage()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_TerrainLargage::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_TerrainLargage::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
