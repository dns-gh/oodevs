//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Barricade.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:10 $
// $Revision: 6 $
// $Workfile: MIL_Barricade.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Barricade.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_Barricade, "MIL_Barricade" )

//-----------------------------------------------------------------------------
// Name: MIL_Barricade constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Barricade::MIL_Barricade( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_Obstacle( type, nID, army )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Barricade constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Barricade::MIL_Barricade()
    : MIL_Obstacle()
{

}

//-----------------------------------------------------------------------------
// Name: MIL_Barricade destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Barricade::~MIL_Barricade()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_Barricade::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_Barricade::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}


