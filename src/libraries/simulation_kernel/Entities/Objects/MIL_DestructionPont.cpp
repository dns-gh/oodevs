//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_DestructionPont.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 6 $
// $Workfile: MIL_DestructionPont.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_DestructionPont.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_DestructionPont, "MIL_DestructionPont" )

//-----------------------------------------------------------------------------
// Name: MIL_DestructionPont constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_DestructionPont::MIL_DestructionPont()
    : MIL_Obstacle( MIL_RealObjectType::destructionPont_ )
{
    nFullNbrDotationForConstruction_ = 300; // Explosifs
    nFullNbrDotationForMining_       = 7;   // Mines
}

//-----------------------------------------------------------------------------
// Name: MIL_DestructionPont destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_DestructionPont::~MIL_DestructionPont()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_DestructionPont::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_DestructionPont::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
