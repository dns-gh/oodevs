//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_DestructionRoute.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:13 $
// $Revision: 6 $
// $Workfile: MIL_DestructionRoute.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_DestructionRoute.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_DestructionRoute, "MIL_DestructionRoute" )

//-----------------------------------------------------------------------------
// Name: MIL_DestructionRoute constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_DestructionRoute::MIL_DestructionRoute()
    : MIL_Obstacle( MIL_RealObjectType::destructionRoute_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_DestructionRoute destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_DestructionRoute::~MIL_DestructionRoute()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_DestructionRoute::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_DestructionRoute::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
