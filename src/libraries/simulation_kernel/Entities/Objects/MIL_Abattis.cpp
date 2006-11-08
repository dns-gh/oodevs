//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Abattis.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:44 $
// $Revision: 4 $
// $Workfile: MIL_Abattis.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Abattis.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_Abattis, "MIL_Abattis" )

//-----------------------------------------------------------------------------
// Name: MIL_Abattis constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Abattis::MIL_Abattis( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_Obstacle( type, nID, army )
{    
}

// -----------------------------------------------------------------------------
// Name: MIL_Abattis constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Abattis::MIL_Abattis()
    : MIL_Obstacle()
{

}

//-----------------------------------------------------------------------------
// Name: MIL_Abattis destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Abattis::~MIL_Abattis()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_Abattis::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_Abattis::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

