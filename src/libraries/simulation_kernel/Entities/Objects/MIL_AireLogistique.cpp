//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_AireLogistique.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:44 $
// $Revision: 3 $
// $Workfile: MIL_AireLogistique.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AireLogistique.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_AireLogistique, "MIL_AireLogistique" )

//-----------------------------------------------------------------------------
// Name: MIL_AireLogistique constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_AireLogistique::MIL_AireLogistique()
    : MIL_RealObject_ABC( MIL_RealObjectType::aireLogistique_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_AireLogistique destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_AireLogistique::~MIL_AireLogistique()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_AireLogistique::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_AireLogistique::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}