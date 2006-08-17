//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Eboulement.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 6 $
// $Workfile: MIL_Eboulement.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Eboulement.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_Eboulement, "MIL_Eboulement" )

//-----------------------------------------------------------------------------
// Name: MIL_Eboulement constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Eboulement::MIL_Eboulement()
    : MIL_Obstacle( MIL_RealObjectType::eboulement_ )
{
    nFullNbrDotationForConstruction_ = 200; // Explosifs
    nFullNbrDotationForMining_       = 17;   // Mines
}

//-----------------------------------------------------------------------------
// Name: MIL_Eboulement destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Eboulement::~MIL_Eboulement()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_Eboulement::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_Eboulement::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
